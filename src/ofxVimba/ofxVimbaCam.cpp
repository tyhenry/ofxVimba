#pragma once
#include "ofxVimbaCam.h"

using namespace ofxVimba;

vector<string> ofxVimbaCam::listDevices(bool log)
{
	return m_System.listDevices(log);
}

bool ofxVimbaCam::open(string deviceID)
{

	using namespace VmbAPI;
	if (deviceID == "") {
		auto devices = listDevices(false);
		if (devices.size()) deviceID = devices[0];
		else {
			ofLogError("ofxVimbaCam") << "Couldn't open camera, no devices detected";
			return false;
		}
	}

	if (m_System.openDevice(deviceID, m_pCamera))
	{
		m_attributes.deviceID = deviceID; // store ID

		// setup some stuff before we register the frame observer

		// Set the GeV packet size to the highest possible value
		// (In this example we do not test whether this cam actually is a GigE cam)
		FeaturePtr pCommandFeature;
		if (CHECK_ERR_Q(m_pCamera->GetFeatureByName("GVSPAdjustPacketSize", pCommandFeature)))
		{
			if (CHECK_ERR_Q(pCommandFeature->RunCommand()))
			{
				bool bIsCommandDone = false;
				while (!bIsCommandDone) {
					if (pCommandFeature->IsCommandDone(bIsCommandDone) != VmbErrorSuccess) break;
				}
			}
		}

		// save the image width, height, and set/get pixel format
		VmbInt64_t width, height, pixelFmt;
		bool success = false; 

		// get width
		if (success = CHECK_ERR_Q(GetFeatureIntValue(m_pCamera, "Width", width)))
		{
			m_attributes.width = (unsigned int)width;

			// get height
			if (success = CHECK_ERR_Q(GetFeatureIntValue(m_pCamera, "Height", height)))
			{
				m_attributes.height = (unsigned int)height;

				// set pixel format to RGB24
				if (success = !CHECK_ERR_Q(SetFeatureIntValue(m_pCamera, "PixelFormat", VmbPixelFormatRgb8)))
				{
					// try YUV422
					SetFeatureIntValue(m_pCamera, "PixelFormat", VmbPixelFormatYuv422);
				}
				// get current pixel format
				if (success = CHECK_ERR_Q(GetFeatureIntValue(m_pCamera, "PixelFormat", pixelFmt)))
				{
					m_attributes.pixelFormat = (VmbPixelFormat_t)pixelFmt;
					return true;
				}
			}
		}
	}
	// failed
	close();
	m_attributes = ofxVimbaCamAttributes(); // reset
	return false;
}


bool ofxVimbaCam::start()
{
	using namespace VmbAPI;
	if (m_pCamera == nullptr) {
		ofLogError(__FUNCTION__) << "camera no yet constructed.";
		return false;
	}

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

//bool ofxVimbaCam::save()
//{
//	string path = ofToDataPath(m_attributes.deviceID + "_settings.xml", true);
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
//bool ofxVimbaCam::load()
//{
//	string path = ofToDataPath(m_attributes.deviceID + "_settings.xml", true);
//	auto err = m_pCamera->LoadCameraSettings(path);
//	if (err == VmbErrorSuccess) {
//		ofLogNotice(__FUNCTION__) << "Loaded settings from " << path;
//		return true;
//	}
//
//	ofLogError(__FUNCTION__) << "Couldn't load settings " << ErrorToString(err);
//	return false;
//}


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

void ofxVimba::ofxVimbaCam::listFeatures()
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

void ofxVimba::ofxVimbaCam::close()
{
	if (m_pCamera != nullptr)
	{
		m_pCamera->Close();
		ofLogNotice("ofxVimbaCam") << "Closed camera ID: " << m_attributes.deviceID;
		m_attributes = ofxVimbaCamAttributes(); // reset
	}
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
				
				VmbUchar_t *pBuffer;
				VmbErrorType err = pFrame->GetImage( pBuffer );
				if( VmbErrorSuccess == err )
				{
					VmbUint32_t nSize, width, height; 
					VmbPixelFormatType pixelFmt;
					bool good = true;
					good = good && CHECK_ERR_Q( pFrame->GetImageSize( nSize ) );
					good = good && CHECK_ERR_Q( pFrame->GetWidth( width ) );
					good = good && CHECK_ERR_Q( pFrame->GetHeight( height ) );
					good = good && CHECK_ERR_Q( pFrame->GetPixelFormat( pixelFmt ) ); 

					if(good)
					{
						//ofLogNotice(__FUNCTION__)
						//	<< "w x h, size in bytes - pxl fmt: "
						//	<< width << " x " << height << ", " << nSize << " - "
						//	<< PixelFormatToString(pixelFmt)
						//	<< endl;
						m_frame.setFromPixels(pBuffer, width, height, OF_IMAGE_COLOR);
						m_NumFramesReceived++;
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


/** read an integer feature from camera.
*/
VmbErrorType ofxVimbaCam::GetFeatureIntValue( const VmbAPI::CameraPtr &camera, const std::string &featureName, VmbInt64_t & value )
{
	if( SP_ISNULL( camera ) )
	{
		return VmbErrorBadParameter;
	}
	VmbAPI::FeaturePtr      pFeature;
	VmbErrorType    result;
	result = SP_ACCESS( camera )->GetFeatureByName( featureName.c_str(), pFeature );
	if( VmbErrorSuccess == result )
	{
		result = SP_ACCESS( pFeature )->GetValue( value );
	}
	return result;
}
/** write an integer feature from camera.
*/
VmbErrorType ofxVimbaCam::SetFeatureIntValue( const VmbAPI::CameraPtr &camera, const std::string &featureName, VmbInt64_t value )
{
	if( SP_ISNULL( camera ) )
	{
		return VmbErrorBadParameter;
	}
	VmbAPI::FeaturePtr      pFeature;
	VmbErrorType    result;
	result = SP_ACCESS( camera )->GetFeatureByName( featureName.c_str(), pFeature );
	if( VmbErrorSuccess == result )
	{
		result = SP_ACCESS( pFeature )->SetValue( value );
	}
	return result;
}


