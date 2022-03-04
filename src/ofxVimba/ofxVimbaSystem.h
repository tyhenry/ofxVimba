#pragma once

#include "VimbaCPP.h"
#include "VmbTransform.h"

#include "ofMain.h"

#include "ofxVimbaUtils.h"

using namespace AVT; // Allied Vision Technology
using namespace ofxVimba::Utils;

namespace ofxVimba {

class ofxVimbaCam;

namespace Core {

class ofxVimbaSystem {

public:

	/// return singleton instance
	static ofxVimbaSystem& getInstance()
	{
		static ofxVimbaSystem    instance;
		return instance;
	}

	/// init Vimba API
	bool startup();

	/// close Vimba API
	void shutdown();

	/// get list of devices as string IDs
	vector<ofVideoDevice> listDevices(bool log = true);

	/// open camera
	/// \param[in]		deviceID	device ID as string, from listDevices()
	///	\param[out]		pCamera		shared_ptr to Vimba camera
	/// \return			true if successful
	bool openDevice(string deviceID, VmbAPI::CameraPtr& pCamera);


private:
	/// \section	Singleton C'tor/D'tor

	/// private c'tor
	ofxVimbaSystem()
		: m_vmbSystem(VmbAPI::VimbaSystem::GetInstance())
		, m_bSysStart(false)	
	{}

	/// private d'tor
	~ofxVimbaSystem()
	{
		shutdown();
	}

	/// deleted for Singleton behavior
	ofxVimbaSystem(ofxVimbaSystem const&) = delete; // Don't Implement.
	void operator=(ofxVimbaSystem const&) = delete; // Don't implement.

	VmbAPI::VimbaSystem& m_vmbSystem; // singleton access to Vimba API
	bool m_bSysStart; // is Vimba API started?


	/// \section Camera Detection

	// NOTE: following is not implemented, will be used for external callbacks

	class CameraObserver;
	CameraObserver* m_pCameraObserver; // registered callback for camera plugged/unplugged

	////////////////////////////////////////////////////////////
	/// \class	CameraObserver watches for changes in devices
	////////////////////////////////////////////////////////////
	class CameraObserver : virtual public VmbAPI::ICameraListObserver
	{
	public:
		CameraObserver() : m_parent(ofxVimbaSystem::getInstance()) {}

		/// \brief Callback executed whenenever camera is plugged in/unplugged.
		virtual void CameraListChanged(VmbAPI::CameraPtr pCam, VmbAPI::UpdateTriggerType reason)
		{
			if (reason == VmbAPI::UpdateTriggerPluggedIn || reason == VmbAPI::UpdateTriggerPluggedOut)
			{	// do nothing yet
			}
		}

	private:
		ofxVimbaSystem& m_parent;
	};
};

}} // namespace ofxVimba::Core