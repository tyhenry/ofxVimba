#pragma once
#include "ofxVimbaCam.h"

using namespace ofxVimba;
using namespace VmbAPI;

vector<ofVideoDevice> ofxVimbaCam::listDevices()
{
	return m_System.listDevices(true);
}

bool ofxVimbaCam::open(int id)
{
	auto devices = m_System.listDevices(false);

	if (devices.size() == 0) {
		ofLogError("ofxVimbaCam") << "Couldn't open camera, no devices detected";
		return false;
	}
	if (id >= devices.size()) {
		ofLogError("ofxVimbaCam") << "Device " << id << " doesn't exist.";
		return false;
	}

	string deviceName = devices[id].deviceName;
	if (m_System.openDevice(deviceName, m_pCamera))
	{
		m_attributes.deviceID = deviceName; // store ID

		// Set the GeV packet size to the highest possible value
		// (In this example we do not test whether this cam actually is a GigE cam)
		runCommand("GVSPAdjustPacketSize");
		return updateAttributes();
	}

	// failed
	close();
	m_attributes = ofxVimbaCamAttributes(); // reset
	return false;
}

// -------------------------------------------------
bool ofxVimbaCam::updateAttributes()
{
	m_attributes.width = getFeatureValue<VmbInt64_t>("Width");
	m_attributes.height = getFeatureValue<VmbInt64_t>("Height");
	m_attributes.pixelFormat = (VmbPixelFormat_t)getFeatureValue<VmbInt64_t>("PixelFormat");
}

bool ofxVimbaCam::set(ofJson& settings) {
	bool success = false;
	for (auto& el : settings.items()) {
		string name = el.key();
		ofJson value = el.value();
		
		if (value.is_boolean()) {
			success |= setFeatureValue(name, value.get<bool>());
		}
		if (value.is_number_integer()) {
			success |= setFeatureValue(name, value.get<VmbInt64_t>());
		}
		if (value.is_string()) {
			success |= setFeatureValue(name, value.get<string>().c_str());
		}
	}
	return success && updateAttributes();
}


bool ofxVimbaCam::start()
{
	using namespace VmbAPI;
	if (m_pCamera == nullptr) {
		ofLogError(__FUNCTION__) << "camera no yet constructed.";
		return false;
	}

	//lastFrame = Clock::now();

	// Create a frame observer for the camera
	m_pFrameObserver = new FrameObserver(m_pCamera);
	auto error = m_pCamera->StartContinuousImageAcquisition(3, IFrameObserverPtr(m_pFrameObserver));
	if (error == VmbErrorSuccess) 
	{
		return true;
	}
	ofLogError(__FUNCTION__) << ErrorToString(error);

	return false;
}

bool ofxVimbaCam::stop()
{
	if (m_pCamera != nullptr)
	{
		auto error = m_pCamera->StopContinuousImageAcquisition();
		if (error == VmbErrorSuccess)
		{
			m_pFrameObserver = nullptr;
			return true;
		}
		ofLogError(__FUNCTION__) << ErrorToString(error);
	}
	return false;
}


//bool ofxVimbaCam::save(string filename)
//{
//	if (filename.length() == 0) {
//		filename = m_attributes.deviceID + "_settings.xml";
//	}
//	string path = ofToDataPath(filename, true);
//	auto err = m_pCamera->SaveCameraSettings(path);
//	if (err == VmbErrorSuccess) {
//		ofLogNotice(__FUNCTION__) << "Saved settings to " << path;
//		return true;
//	}
//
//	ofLogError(__FUNCTION__) << "Couldn't save settings " << ErrorToString(err);
//	return false;
//}
//
//
//bool ofxVimbaCam::load(string filename)
//{
//	if (filename.length() == 0) {
//		filename = m_attributes.deviceID + "_settings.xml";
//	}
//	string path = ofToDataPath(filename, true);
//	ofLogNotice(__FUNCTION__) << "Loading " << path;
//	auto err = m_pCamera->LoadCameraSettings(path);
//	if (err == VmbErrorSuccess) {
//		ofLogNotice(__FUNCTION__) << "Loaded settings from " << path;
//		return true;
//	}
//
//	ofLogError(__FUNCTION__) << "Couldn't load settings from " << filename << " " << ErrorToString(err);
//	return false;
//}



void ofxVimbaCam::listFeatures()
{
	map<VmbFeatureDataType,string> types = {
		{ VmbFeatureDataUnknown, "Unknown feature type" },
		{ VmbFeatureDataInt, "64 bit integer feature" },
		{ VmbFeatureDataFloat, "64 bit floating point feature" },
		{ VmbFeatureDataEnum, "Enumeration feature" },
		{ VmbFeatureDataString, "String feature" },
		{ VmbFeatureDataBool, "Boolean feature" },
		{ VmbFeatureDataCommand, "Command feature" },
		{ VmbFeatureDataRaw, "Raw (direct register access) feature" },
		{ VmbFeatureDataNone, "Feature with no data" },
	};

	AVT::VmbAPI::FeaturePtrVector features;
	auto error = m_pCamera->GetFeatures(features);
	if (error != VmbErrorSuccess) return;

	std::string featureName;
	std::string featureCategory;
	VmbFeatureDataType featureType;
	for (const auto& feature : features) {
		error = feature->GetName(featureName);
		if (error == VmbErrorSuccess) error = feature->GetCategory(featureCategory);
		if (error == VmbErrorSuccess) error = feature->GetDataType(featureType);
		if (error == VmbErrorSuccess) {
			ofLogNotice(__FUNCTION__) << featureName << "\t\t" << featureCategory << "\t\t" << types[featureType];
		}
	}
}

void ofxVimbaCam::close()
{
	if (m_pCamera != nullptr)
	{
		m_pCamera->Close();
		ofLogNotice("ofxVimbaCam") << "Closed camera ID: " << m_attributes.deviceID;
		m_attributes = ofxVimbaCamAttributes(); // reset
	}
}


bool ofxVimbaCam::runCommand(string featureName) {
	if (SP_ISNULL(m_pCamera))
	{
		ofLogWarning(__FUNCTION__) << "Camera has not been initialized";
		return false;
	}


	//FeaturePtr pCommandFeature;
	//if (CHECK_ERR_Q(m_pCamera->GetFeatureByName("GVSPAdjustPacketSize", pCommandFeature)))
	//{
	//	if (CHECK_ERR_Q(pCommandFeature->RunCommand()))
	//	{
	//		bool bIsCommandDone = false;
	//		while (!bIsCommandDone) {
	//			if (pCommandFeature->IsCommandDone(bIsCommandDone) != VmbErrorSuccess) break;
	//		}
	//	}
	//}

	FeaturePtr pCommandFeature;
	VmbErrorType result = SP_ACCESS(m_pCamera)->GetFeatureByName(featureName.c_str(), pCommandFeature);
	if (VmbErrorSuccess == result) {
		result = SP_ACCESS(pCommandFeature)->RunCommand();
		if (VmbErrorSuccess != result)
		{
			ofLogWarning(__FUNCTION__) << featureName << ": " << ErrorToString(result);
			return false;
		}
		return true;
	}
	else
	{
		ofLogWarning(__FUNCTION__) << featureName << ": " << ErrorToString(result);
		return false;
	}
	return false;
}


bool ofxVimbaCam::update()
{
	m_bNewFrame = false;
	if (m_pFrameObserver == nullptr) {
		ofLogWarning(__FUNCTION__) << "Frame Observer not constructed. You will not receive new frames. Perhaps you didn't call start()?";
		return false;
	}

	if (m_pFrameObserver->HasNewFrame())
	{
		VmbAPI::FramePtr pFrame = m_pFrameObserver->GetFrame();
		if (pFrame != nullptr) {

			VmbFrameStatusType status;
			pFrame->GetReceiveStatus(status);
			if (status == VmbFrameStatusComplete)
			{
				m_bNewFrame = true;

				// copy to image here

				VmbUchar_t* pBuffer;
				VmbErrorType err = pFrame->GetImage(pBuffer);
				if (VmbErrorSuccess == err)
				{
					VmbUint32_t nSize, width, height;
					VmbPixelFormatType pixelFmt;
		

					VmbErrorType err = pFrame->GetImageSize(nSize);
					if(err == VmbErrorSuccess) err = pFrame->GetWidth(width);
					if(err == VmbErrorSuccess) err = pFrame->GetHeight(height);
					if(err == VmbErrorSuccess) err = pFrame->GetPixelFormat(pixelFmt);
					if(err == VmbErrorSuccess)
					{
						//ofLogNotice(__FUNCTION__)
						//	<< "w x h, size in bytes - pxl fmt: "
						//	<< width << " x " << height << ", " << nSize << " - "
						//	<< PixelFormatToString(pixelFmt)
						//	<< endl;

						if (pixelFmt == VmbPixelFormatYuv444) {
							std::vector<VmbUchar_t> TransformedData;
							TransformImage(pFrame, TransformedData, "RGB24");
							pBuffer = &TransformedData[0];
						}
			
						ofImageType type = OF_IMAGE_COLOR;
						if (pixelFmt == VmbPixelFormatMono8) {
							type = OF_IMAGE_GRAYSCALE;
						}

						m_frame.setFromPixels(pBuffer, width, height, type); 
						m_NumFramesReceived++;

						//TimePoint now = Clock::now();
						//Duration diff = now - lastFrame;
						//if (diff.count() < 1000)
						//	times.push_back(diff.count());
						//while (times.size() > 100)
						//	times.erase(times.begin());
						//lastFrame = Clock::now();
					}
					else
					{
						ofLogWarning(__FUNCTION__) << "bad frame received";
					}
				}
			}
			else
			{
				ofLogWarning("ofxVimbaCam")
					<< "Camera ID: " << m_attributes.deviceID << " received an incomplete frame! Probably due to lack of bandwidth.";
			}

			// queue frame to receive more
			m_pFrameObserver->QueueFrame(pFrame);

		}
		else {
			//ofLogNotice(__FUNCTION__) << "m_pFrameObserver returned nullptr.  weird";
		}
	}
	else {
		//ofLogNotice(__FUNCTION__) << "no new frame";
	}
	return m_bNewFrame;
}




