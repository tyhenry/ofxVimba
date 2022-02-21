#pragma once

#include "VimbaCPP.h"
#include "VmbTransform.h"
#include "ofMain.h"

using namespace AVT;

namespace ofxVimba {
namespace Utils {


string ErrorToString(VmbError_t err);
string PixelFormatToString(VmbPixelFormat_t fmt);

#define CHECK_ERR(F)		CheckError(F,true,#F)	// checks for and logs any error using function name
#define CHECK_ERR_Q(F)		CheckError(F,false)		// quiet version, doesn't log
#define CHECK_ERR_MSG(F,M)	CheckError(F,true,M)	// checks for error and logs with custom message

//
//	@brief Checks VmbErrorType for error or success and logs errors
//
//	@param[in]	err		The Vimba API error code to check
//	@param[in]	logErr	(Optional) whether to console log an error
//	@param[in]	msg		(Optional) extra message to log, e.g. function name
//	@return				True if VmbErrorType is VmbErrorSuccess, false if other
//
inline bool CheckError(VmbErrorType err, bool logErr = true, string msg = "")
{
	if (err != VmbErrorSuccess) {
		if (logErr) ofLogError("ofxVimba") << msg + " : Vimba Error Code : " << ErrorToString(err);
		return false;
	}
	return true;
}

inline string IPv4ToString(const VmbInt64_t nIPAddress)
{
    stringstream sIPv4;
    sIPv4 << ofToString(0xFF & (nIPAddress >> 24)) << "."
        << ofToString(0xFF & (nIPAddress >> 16)) << "."
        << ofToString(0xFF & (nIPAddress >> 8)) << "."
        << ofToString(0xFF & nIPAddress);
    return sIPv4.str();
}

VmbErrorType GetFeatureIntValue(const VmbAPI::CameraPtr& camera, const std::string& featureName, VmbInt64_t& value);
VmbErrorType SetFeatureIntValue(const VmbAPI::CameraPtr& camera, const std::string& featureName, VmbInt64_t value);



VmbErrorType TransformImage(const VmbAPI::FramePtr& SourceFrame, std::vector<VmbUchar_t>& DestinationData, const std::string& DestinationFormat);

}} // namespace ofxVimba::Utils