#pragma once

#include "VimbaCPP.h"
#include "VmbTransform.h"
#include "ofMain.h"

#include "ofxVimbaSystem.h"

using namespace AVT;
using namespace ofxVimba::Core;

namespace ofxVimba {

class ofxVimbaCam {

public:

	struct ofxVimbaCamAttributes {
		string deviceID = "";
		unsigned int width = 0;
		unsigned int height = 0;
		VmbPixelFormat_t pixelFormat = 0;
		float maxFrameRate = 0;
	};

	ofxVimbaCam() 
	: m_System(ofxVimbaSystem::getInstance()) 
	{
		m_System.startup(); // inits API if not already
	}

	virtual ~ofxVimbaCam()
	{

	}

	vector<string> listDevices(bool log =true);

	/// \brief	Begin streaming from a camera
	/// \param[in]	deviceID	(Optional) Specific device ID as string, provided by listDevices()
	///							If left blank, will try to open first available device
	bool open(string deviceID="");

	/// \brief	Closes the camera connection, freeing up resources
	void close();

	/// \return	True if new frame(s)
	bool update();
	
	/// \return True if new frame(s)
	bool isFrameNew()	{ return m_bNewFrame; }

	int getCamWidth()	{ return m_attributes.width; }
	int getCamHeight()	{ return m_attributes.height; }
	string getCamPixelFormatAsString() 	{ return PixelFormatToString(m_attributes.pixelFormat); }

	void draw(float x, float y, float w, float h)	{ m_frame.draw(x,y,w,h); }
	void draw(float x, float y)						{ m_frame.draw(x,y); }

protected:

	ofImage m_frame;
	bool m_bNewFrame; // new frame flagged during update()
	VmbAPI::CameraPtr m_pCamera;
	ofxVimbaCamAttributes m_attributes;

	VmbErrorType ofxVimbaCam::GetFeatureIntValue( const VmbAPI::CameraPtr &camera, const std::string &featureName, VmbInt64_t & value );
	VmbErrorType ofxVimbaCam::SetFeatureIntValue( const VmbAPI::CameraPtr &camera, const std::string &featureName, VmbInt64_t value );

	
	/// \class	Asynchronous listener for new frames from device
	///	\brief	keeps new frames in queue for ofxVimbaCam to grab
	class FrameObserver : public VmbAPI::IFrameObserver 
	{

	public:
		FrameObserver(VmbAPI::CameraPtr pCamera) : IFrameObserver(pCamera) {}
		virtual void FrameReceived(const VmbAPI::FramePtr pFrame);

		///	\brief		Grabs and removes frame from internal queue
		/// \return		A shared pointer to the latest frame, null if no new frames
		VmbAPI::FramePtr GetFrame();

		/// \brief		Clears the internal (double buffering) frame queue
		void ClearNewFrames();

		/// \return		True if internal queue has any frames in it
		bool HasNewFrame();

		/// \return		Number of frames in internal queue
		int GetNumNewFrames();

		/// \brief		Sends frame to be re-filled by Vimba API
		/// \return		Vimba error code
		VmbErrorType QueueFrame(const VmbAPI::FramePtr pFrame);

	private:
	  // the frame observer stores all FramePtrs
		std::queue<VmbAPI::FramePtr> m_Frames;
		AVT::VmbAPI::Mutex m_FramesMutex;

	};

	FrameObserver* m_pFrameObserver;

private:

	Core::ofxVimbaSystem& m_System; // ofxVimbaSystem singleton

};

}
