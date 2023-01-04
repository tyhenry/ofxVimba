#include "ofxVimbaSystem.h"

bool ofxVimba::Core::ofxVimbaSystem::startup()
{
	if (!m_bSysStart) 
	{ 
		VmbErrorType status = m_vmbSystem.Startup();
		m_bSysStart = (status == VmbErrorSuccess);
	}
	return m_bSysStart;
}

void ofxVimba::Core::ofxVimbaSystem::shutdown()
{
	m_vmbSystem.Shutdown();
	ofLogNotice("ofxVimba") << "Vimba API system has been shut down";
	m_bSysStart = false;
}

vector<ofVideoDevice> ofxVimba::Core::ofxVimbaSystem::listDevices(bool log)
{
	vector<ofVideoDevice> devices;
	VmbAPI::CameraPtrVector cams;
	VmbErrorType status = m_vmbSystem.GetCameras(cams);

	// get device list from Vimba API
	if (status == VmbErrorSuccess)
	{
		if (log) ofLogNotice(__FUNCTION__) << "Available Cameras:\n------------";
		for (int i = 0; i < cams.size(); i++) {
			ofVideoDevice device;
			device.id = i;
			if (cams[i])
			{
				cams[i]->GetID(device.deviceName);
				cams[i]->GetSerialNumber( device.serialID );
				cams[i]->GetModel(device.hardwareName);
				devices.push_back(device);
				if (log) ofLogNotice(__FUNCTION__) << "deviceName: " << device.deviceName
					<< " serialID: " << device.serialID
					<< " hardwareName: " << device.hardwareName;
			}
		}
		if (log) ofLogNotice(__FUNCTION__) << "------------";
	}
	else {
		ofLogError(__FUNCTION__) << "Unable to access Vimba device list!";
	}
	return devices;
}

bool ofxVimba::Core::ofxVimbaSystem::openDevice(string deviceID, VmbAPI::CameraPtr& pCamera)
{
	VmbErrorType status = m_vmbSystem.OpenCameraByID(deviceID.c_str(), VmbAccessModeFull, pCamera);
	if (status == VmbErrorSuccess) {
		return true;
	}
	ofLogError(__FUNCTION__) << "Unable to open device [" << deviceID << "]";
	return false;
}



