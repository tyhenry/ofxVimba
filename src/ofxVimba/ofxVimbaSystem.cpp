#include "ofxVimbaSystem.h"

bool ofxVimba::Core::ofxVimbaSystem::startup()
{
	if (!m_bSysStart) 
	{ 
		m_bSysStart = CHECK_ERR(m_vmbSystem.Startup());
	}
	return m_bSysStart;
}

void ofxVimba::Core::ofxVimbaSystem::shutdown()
{
	m_vmbSystem.Shutdown();
	ofLogNotice("ofxVimba") << "Vimba API system has been shut down";
	m_bSysStart = false;
}

vector<string> ofxVimba::Core::ofxVimbaSystem::listDevices(bool log)
{
	vector<string> devices;
	VmbAPI::CameraPtrVector cams;
	// get device list from Vimba API
	if (CHECK_ERR_MSG(m_vmbSystem.GetCameras(cams), "Unable to access Vimba device list!"))
	{
		if (log) ofLogNotice("ofxVimba") << "Available Cameras:\n------------";
		for (auto& cam : cams) {
			// grab the device ID as string
			string id;
			if (cam && CHECK_ERR(cam->GetID(id)))
			{
				if (log) cout << "ID: " << id << endl;
				devices.push_back(id);
			}
		}
		if (log) cout << "------------" << endl;
	}
	return devices;
}

bool ofxVimba::Core::ofxVimbaSystem::openDevice(string deviceID, VmbAPI::CameraPtr& pCamera)
{
	return CHECK_ERR_MSG(m_vmbSystem.OpenCameraByID(deviceID.c_str(), VmbAccessModeFull, pCamera), "Unable to open device [" + deviceID +"]");
}



