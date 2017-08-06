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

					// Create a frame observer for the camera
					m_pFrameObserver = new FrameObserver(m_pCamera);
					success = CHECK_ERR_MSG( m_pCamera->StartContinuousImageAcquisition(3, IFrameObserverPtr(m_pFrameObserver)), "Unable to start camera!");
					if (success) {
						m_attributes.deviceID = deviceID; // store ID
						return true;
					}
				}
			}
		}
	}
	// failed
	close();
	m_attributes = ofxVimbaCamAttributes(); // reset
	return false;

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
	if (m_pFrameObserver != nullptr && m_pFrameObserver->HasNewFrame())
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
						//cout
						//	<< "w x h, size in bytes - pxl fmt: "
						//	<< width << " x " << height << ", " << nSize << " - "
						//	<< PixelFormatToString(pixelFmt)
						//	<< endl;
						m_frame.setFromPixels(pBuffer, width, height, OF_IMAGE_COLOR);
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



