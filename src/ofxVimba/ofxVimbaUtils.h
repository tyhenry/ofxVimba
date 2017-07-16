#pragma once

#include "VimbaCPP.h"
#include "VmbTransform.h"
#include "ofMain.h"


namespace ofxVimba {
namespace Utils {

/**
 * @brief Converts VmbErrorType code to string description
 */
inline string ErrorToString(VmbError_t err)
{
	switch (err)
	{

	// general function errors
	case VmbErrorSuccess:           return  "Success.";
	case VmbErrorInternalFault:     return  "Unexpected fault in VmbApi or driver.";
	case VmbErrorApiNotStarted:     return  "API not started.";
	case VmbErrorNotFound:          return  "Not found.";
	case VmbErrorBadHandle:         return  "Invalid handle ";
	case VmbErrorDeviceNotOpen:     return  "Device not open.";
	case VmbErrorInvalidAccess:     return  "Invalid access.";
	case VmbErrorBadParameter:      return  "Bad parameter.";
	case VmbErrorStructSize:        return  "Wrong DLL version.";
	case VmbErrorMoreData:          return  "More data returned than memory provided.";
	case VmbErrorWrongType:         return  "Wrong type.";
	case VmbErrorInvalidValue:      return  "Invalid value.";
	case VmbErrorTimeout:           return  "Timeout.";
	case VmbErrorOther:             return  "TL error.";
	case VmbErrorResources:         return  "Resource not available.";
	case VmbErrorInvalidCall:       return  "Invalid call.";
	case VmbErrorNoTL:              return  "TL not loaded.";
	case VmbErrorNotImplemented:    return  "Not implemented.";
	case VmbErrorNotSupported:      return  "Not supported.";

	default:                        return  "Unknown error.";
	}
}

inline string PixelFormatToString(VmbPixelFormat_t fmt) 
{
	switch (fmt)
	{
		// monochrome
		case VmbPixelFormatMono8:				return "Monochrome, 8 bits (PFNC:Mono8)";
        case VmbPixelFormatMono10:				return "Monochrome, 10 bits in 16 bits (PFNC:Mono10)";
        case VmbPixelFormatMono10p:				return "Monochrome, 10 bits in 16 bits (PFNC:Mono10p)";
        case VmbPixelFormatMono12:				return "Monochrome, 12 bits in 16 bits (PFNC:Mono12)";
        case VmbPixelFormatMono12Packed:		return "Monochrome, 2x12 bits in 24 bits (GEV:Mono12Packed)";
        case VmbPixelFormatMono12p:				return "Monochrome, 2x12 bits in 24 bits (PFNC:MonoPacked)";
        case VmbPixelFormatMono14:				return "Monochrome, 14 bits in 16 bits (PFNC:Mono14)";
        case VmbPixelFormatMono16:				return "Monochrome, 16 bits (PFNC:Mono16)";

        // bayer formats
        case VmbPixelFormatBayerGR8:			return "Bayer-color, 8 bits, starting with GR line (PFNC:BayerGR8)";
        case VmbPixelFormatBayerRG8:			return "Bayer-color, 8 bits, starting with RG line (PFNC:BayerRG8)";        
		case VmbPixelFormatBayerGB8:			return "Bayer-color, 8 bits, starting with GB line (PFNC:BayerGB8)";
        case VmbPixelFormatBayerBG8:			return "Bayer-color, 8 bits, starting with BG line (PFNC:BayerBG8)";
        case VmbPixelFormatBayerGR10:			return "Bayer-color, 10 bits in 16 bits, starting with GR line (PFNC:BayerGR10)";
        case VmbPixelFormatBayerRG10:			return "Bayer-color, 10 bits in 16 bits, starting with RG line (PFNC:BayerRG10)";
        case VmbPixelFormatBayerGB10:			return "Bayer-color, 10 bits in 16 bits, starting with GB line (PFNC:BayerGB10)";
        case VmbPixelFormatBayerBG10:			return "Bayer-color, 10 bits in 16 bits, starting with BG line (PFNC:BayerBG10)";
        case VmbPixelFormatBayerGR12:			return "Bayer-color, 12 bits in 16 bits, starting with GR line (PFNC:BayerGR12)";
        case VmbPixelFormatBayerRG12:			return "Bayer-color, 12 bits in 16 bits, starting with RG line (PFNC:BayerRG12)";
        case VmbPixelFormatBayerGB12:			return "Bayer-color, 12 bits in 16 bits, starting with GB line (PFNC:BayerGB12)";
        case VmbPixelFormatBayerBG12:			return "Bayer-color, 12 bits in 16 bits, starting with BG line (PFNC:BayerBG12)";
        case VmbPixelFormatBayerGR12Packed:		return "Bayer-color, 2x12 bits in 24 bits, starting with GR line (GEV:BayerGR12Packed)";
        case VmbPixelFormatBayerRG12Packed:		return "Bayer-color, 2x12 bits in 24 bits, starting with RG line (GEV:BayerRG12Packed)";
        case VmbPixelFormatBayerGB12Packed:		return "Bayer-color, 2x12 bits in 24 bits, starting with GB line (GEV:BayerGB12Packed)";
        case VmbPixelFormatBayerBG12Packed:		return "Bayer-color, 2x12 bits in 24 bits, starting with BG line (GEV:BayerBG12Packed)";
        case VmbPixelFormatBayerGR10p:			return "Bayer-color, 12 bits continuous packed, starting with GR line (PFNC:BayerGR10p)";
        case VmbPixelFormatBayerRG10p:			return "Bayer-color, 12 bits continuous packed, starting with RG line (PFNC:BayerRG10p)";
        case VmbPixelFormatBayerGB10p:			return "Bayer-color, 12 bits continuous packed, starting with GB line (PFNC:BayerGB10p)";
        case VmbPixelFormatBayerBG10p:			return "Bayer-color, 12 bits continuous packed, starting with BG line (PFNC:BayerBG10p)";
        case VmbPixelFormatBayerGR12p:			return "Bayer-color, 12 bits continuous packed, starting with GR line (PFNC:BayerGR12p)";
        case VmbPixelFormatBayerRG12p:			return "Bayer-color, 12 bits continuous packed, starting with RG line (PFNC:BayerRG12p)";
        case VmbPixelFormatBayerGB12p:			return "Bayer-color, 12 bits continuous packed, starting with GB line (PFNC:BayerGB12p)";
        case VmbPixelFormatBayerBG12p:			return "Bayer-color, 12 bits continuous packed, starting with BG line (PFNC:BayerBG12p)";
        case VmbPixelFormatBayerGR16:			return "Bayer-color, 16 bits, starting with GR line (PFNC:BayerGR16)";
        case VmbPixelFormatBayerRG16:			return "Bayer-color, 16 bits, starting with RG line (PFNC:BayerRG16)";
        case VmbPixelFormatBayerGB16:			return "Bayer-color, 16 bits, starting with GB line (PFNC:BayerGB16)";
        case VmbPixelFormatBayerBG16:			return "Bayer-color, 16 bits, starting with BG line (PFNC:BayerBG16)";

        // rgb formats
        case VmbPixelFormatRgb8:				return "RGB, 8 bits x 3 (PFNC:RGB8)";
        case VmbPixelFormatBgr8:				return "BGR, 8 bits x 3 (PFNC:BGR8)";
        case VmbPixelFormatRgb10:				return "RGB, 12 bits in 16 bits x 3 (PFNC:RGB12)";
        case VmbPixelFormatBgr10:				return "RGB, 12 bits in 16 bits x 3 (PFNC:RGB12)";
        case VmbPixelFormatRgb12:				return "RGB, 12 bits in 16 bits x 3 (PFNC:RGB12)";
        case VmbPixelFormatBgr12:				return "RGB, 12 bits in 16 bits x 3 (PFNC:RGB12)";

        case VmbPixelFormatRgb14:				return "RGB, 14 bits in 16 bits x 3 (PFNC:RGB12)";
        case VmbPixelFormatBgr14:				return "RGB, 14 bits in 16 bits x 3 (PFNC:RGB12)";

        case VmbPixelFormatRgb16:				return "RGB, 16 bits x 3 (PFNC:RGB16)";
        case VmbPixelFormatBgr16:				return "RGB, 16 bits x 3 (PFNC:RGB16)";

        // rgba formats
        case VmbPixelFormatArgb8:				return "ARGB, 8 bits x 4 (PFNC:RGBa8)";
        // duplicate enum value as above! -- case VmbPixelFormatRgba8:				return "RGBA, 8 bits x 4, legacy name";
        case VmbPixelFormatBgra8:				return "BGRA, 8 bits x 4 (PFNC:BGRa8)";
        case VmbPixelFormatRgba10:				return "RGBA, 8 bits x 4, legacy name";
        case VmbPixelFormatBgra10:				return "RGBA, 8 bits x 4, legacy name";
        case VmbPixelFormatRgba12:				return "RGBA, 8 bits x 4, legacy name";
        case VmbPixelFormatBgra12:				return "RGBA, 8 bits x 4, legacy name";

        case VmbPixelFormatRgba14:				return "RGBA, 8 bits x 4, legacy name";
        case VmbPixelFormatBgra14:				return "RGBA, 8 bits x 4, legacy name";

        case VmbPixelFormatRgba16:				return "RGBA, 8 bits x 4, legacy name";
		case VmbPixelFormatBgra16:				return "RGBA, 8 bits x 4, legacy name";

        // yuv/ycbcr formats
        case VmbPixelFormatYuv411:				return "YUV 411 with 8 bits (GEV:YUV411Packed)";
        case VmbPixelFormatYuv422:				return "YUV 422 with 8 bits (GEV:YUV422Packed)";
        case VmbPixelFormatYuv444:				return "YUV 444 with 8 bits (GEV:YUV444Packed)";
        case VmbPixelFormatYCbCr411_8_CbYYCrYY:	return "Y´CbCr 411 with 8 bits (PFNC:YCbCr411_8_CbYYCrYY) - identical to VmbPixelFormatYuv411";
        case VmbPixelFormatYCbCr422_8_CbYCrY:	return "Y´CbCr 422 with 8 bits (PFNC:YCbCr422_8_CbYCrY) - identical to VmbPixelFormatYuv422";
        case VmbPixelFormatYCbCr8_CbYCr:		return "Y´CbCr 444 with 8 bits (PFNC:YCbCr8_CbYCr) - identical to VmbPixelFormatYuv444";

		default:								return "Unknown pixel format";
	}
}

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

}} // namespace ofxVimba::Utils