#pragma once

#include "VimbaCPP.h"
#include "VmbTransform.h"
#include "ofMain.h"

#include "ofxVimbaSystem.h"

using namespace AVT;
using namespace ofxVimba::Core;


namespace ofxVimba {

enum class VmbGainAutoMode {
	Off = 1,
	Once = 2,
	Continuous = 3
};

enum class VmbSensorShutterMode {
	Global = 1,
	Rolling = 2,
	GlobalReset = 3
};

enum class GevIPConfigurationMode {
	LLA = 1,
	DHCP = 2,
	Persistent = 3
};

enum class BandwidthControlMode {
	StreamBytesPerSecond = 1,
	SCPD = 2,
	Both = 3
};

class ofxVimbaCam :	public ofBaseHasPixels { //TODO : public ofBaseVideoGrabber, public ofBaseVideoDraws {

public:

	struct ofxVimbaCamAttributes {
		string deviceID = "";
		unsigned int width = 0;
		unsigned int height = 0;
		VmbPixelFormat_t pixelFormat = 0;
		float maxFrameRate = 0;
	};

	struct ofxVimbaCamStatus {
		VmbInt64_t bps;
		double limit;
		double exposure;
		double tempC;
		double tempF;
		double gain;
		double fps;
		VmbInt64_t payloadSize;
		VmbInt64_t width;
		VmbInt64_t height;
		VmbInt64_t imageSize;
		VmbInt64_t pixelFmt;
	};

	ofxVimbaCam() 
	: m_System(ofxVimbaSystem::getInstance()) 
	{
		m_System.startup(); // inits API if not already
	}

	virtual ~ofxVimbaCam()
	{

	}

	/// \brief Get a list of available video grabber devices.
	/// \returns a std::vector of ofVideoDevice objects.
	vector<ofVideoDevice> listDevices();

	/// \brief	Open a connection to a camera
	/// \param[in]	deviceID	(Optional) Specify device ID as string, as provided by listDevices()
	///							If left blank, will try to open first available device
	bool open(string deviceName);
	bool open(int n=0);
	

	bool set(ofJson& settings);

	/// \brief	StartContinuousImageAcquisition
	bool start();

	/// \brief	StopContinuousImageAcquisition
	bool stop();

	/// \brief	Closes the camera connection, freeing up resources
	void close();

	//bool save(string filename="");

	//bool load(string filename="");

	/// \brief	Grabs new frame(s) from the async queue (FrameObserver), if any ready
	bool update();
	
	/// \return True if new frame(s)
	bool isFrameNew() const	{ return m_bNewFrame; }

	int getCamWidth() const	{ return m_attributes.width; }
	int getCamHeight() const	{ return m_attributes.height; }
	string ofxVimbaCam::getID() const { return m_attributes.deviceID; }
	ofxVimbaCamStatus getStatus() {
		ofxVimbaCamStatus status;
		status.bps = getFeatureValue<VmbInt64_t>("StreamBytesPerSecond");
		status.limit =  getFeatureValue<double>("AcquisitionFrameRateLimit");
		status.exposure =  getFeatureValue<double>("ExposureTimeAbs");
		status.tempC =  getFeatureValue<double>("DeviceTemperature");
		status.tempF = (status.tempC * 1.8) + 32;
		status.payloadSize =  getFeatureValue<VmbInt64_t>("PayloadSize");
		status.imageSize =  getFeatureValue<VmbInt64_t>("ImageSize");
		status.pixelFmt =  getFeatureValue<VmbInt64_t>("PixelFormat");
		status.gain =  getFeatureValue<double>("Gain");
		status.fps =  getFeatureValue<double>("StatFrameRate");
		return status;
	}
	string getCamPixelFormatAsString() 	{ return PixelFormatToString(m_attributes.pixelFormat); }

	void draw(float x, float y, float w, float h) const		{ if (m_frame.isAllocated()) m_frame.draw(x,y,w,h); }
	void draw(float x, float y) const						{ if(m_frame.isAllocated()) m_frame.draw(x,y);  }

	inline int getNumFramesReceived() const { return m_NumFramesReceived; }

	ofImage& getFrame()		{ return m_frame; }
	ofTexture& getTexture() { return m_frame.getTexture(); }

	void listFeatures();
	bool runCommand(string featureName);

	template <typename T>
	bool setFeatureValue(const std::string& featureName, T value) {
		if (SP_ISNULL(m_pCamera))
		{
			ofLogWarning(__FUNCTION__) << "Camera has not been initialized";
			return false;
		}
		VmbAPI::FeaturePtr pFeature;
		VmbErrorType result;
		result = SP_ACCESS(m_pCamera)->GetFeatureByName(featureName.c_str(), pFeature);
		if (VmbErrorSuccess == result)
		{
			result = SP_ACCESS(pFeature)->SetValue(value);
			if (VmbErrorSuccess != result)
			{
				ofLogWarning(__FUNCTION__) << featureName << ": " << ErrorToString(result);
				return false;
			}
			return true;
		}
		return false;
	}

	template <typename T>
	T getFeatureValue(const std::string& featureName) const {
		if (SP_ISNULL(m_pCamera))
		{
			ofLogWarning(__FUNCTION__) << "Camera has not been initialized";
			return -1;
		}
		VmbAPI::FeaturePtr pFeature;
		VmbErrorType result;
		result = SP_ACCESS(m_pCamera)->GetFeatureByName(featureName.c_str(), pFeature);
		if (VmbErrorSuccess == result)
		{
			T value;
			result = SP_ACCESS(pFeature)->GetValue(value);
			if (VmbErrorSuccess != result)
			{
				ofLogWarning(__FUNCTION__) << ErrorToString(result);
				return -1;
			}
			return value;
		}
		return -1;
	}

	const ofPixels& getPixels() const;
	ofPixels& getPixels();

protected:
	bool updateAttributes();

	ofImage m_frame;
	bool m_bNewFrame; // new frame flagged during update()
	VmbAPI::CameraPtr m_pCamera;
	ofxVimbaCamAttributes m_attributes;

	unsigned long m_NumFramesReceived = 0;

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
		/// the frame observer stores FramePtrs before processing
		std::queue<VmbAPI::FramePtr> m_Frames;
		AVT::VmbAPI::Mutex m_FramesMutex;

	};

	FrameObserver* m_pFrameObserver;

private:

	Core::ofxVimbaSystem& m_System; // ofxVimbaSystem singleton

};

}
