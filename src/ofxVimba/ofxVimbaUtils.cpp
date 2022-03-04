#include "ofxVimbaUtils.h"

namespace ofxVimba {
namespace Utils {



/**
* @brief Converts VmbErrorType code to string description
*/
string ErrorToString(VmbError_t err)
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

string PixelFormatToString(VmbPixelFormat_t fmt)
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

//
///** read an integer feature from camera.
//*/
//VmbErrorType  GetFeatureIntValue(const VmbAPI::CameraPtr& camera, const std::string& featureName, VmbInt64_t& value)
//{
//	if (SP_ISNULL(camera))
//	{
//		return VmbErrorBadParameter;
//	}
//	VmbAPI::FeaturePtr      pFeature;
//	VmbErrorType    result;
//	result = SP_ACCESS(camera)->GetFeatureByName(featureName.c_str(), pFeature);
//	if (VmbErrorSuccess == result)
//	{
//		result = SP_ACCESS(pFeature)->GetValue(value);
//	}
//	return result;
//}
//
//
///** write an integer feature to camera.
//*/
//VmbErrorType SetFeatureIntValue(const VmbAPI::CameraPtr& camera, const std::string& featureName, VmbInt64_t value)
//{
//	if (SP_ISNULL(camera))
//	{
//		return VmbErrorBadParameter;
//	}
//	VmbAPI::FeaturePtr      pFeature;
//	VmbErrorType    result;
//	result = SP_ACCESS(camera)->GetFeatureByName(featureName.c_str(), pFeature);
//	if (VmbErrorSuccess == result)
//	{
//		result = SP_ACCESS(pFeature)->SetValue(value);
//	}
//	return result;
//}


VmbErrorType TransformImage(const VmbAPI::FramePtr& SourceFrame, std::vector<VmbUchar_t>& DestinationData, const std::string& DestinationFormat)
{
	if (SP_ISNULL(SourceFrame))
	{
		return VmbErrorBadParameter;
	}
	VmbErrorType        Result;
	VmbPixelFormatType  InputFormat;
	VmbUint32_t         InputWidth, InputHeight;
	Result = SP_ACCESS(SourceFrame)->GetPixelFormat(InputFormat);
	if (VmbErrorSuccess != Result)
	{
		return Result;
	}
	Result = SP_ACCESS(SourceFrame)->GetWidth(InputWidth);
	if (VmbErrorSuccess != Result)
	{
		return Result;
	}
	Result = SP_ACCESS(SourceFrame)->GetHeight(InputHeight);
	if (VmbErrorSuccess != Result)
	{
		return Result;
	}
	// Prepare source image
	VmbImage SourceImage;
	SourceImage.Size = sizeof(SourceImage);
	Result = static_cast<VmbErrorType>(VmbSetImageInfoFromPixelFormat(InputFormat, InputWidth, InputHeight, &SourceImage));
	if (VmbErrorSuccess != Result)
	{
		return Result;
	}
	VmbUchar_t* DataBegin = NULL;
	Result = SP_ACCESS(SourceFrame)->GetBuffer(DataBegin);
	if (VmbErrorSuccess != Result)
	{
		return Result;
	}
	SourceImage.Data = DataBegin;
	// Prepare destination image
	VmbImage DestinationImage;
	DestinationImage.Size = sizeof(DestinationImage);
	Result = static_cast<VmbErrorType>(VmbSetImageInfoFromString(DestinationFormat.c_str(), static_cast<VmbUint32_t>(DestinationFormat.size()), InputWidth, InputHeight, &DestinationImage));
	if (VmbErrorSuccess != Result)
	{
		return Result;
	}
	const size_t ByteCount = (DestinationImage.ImageInfo.PixelInfo.BitsPerPixel * InputWidth * InputHeight) / 8;
	DestinationData.resize(ByteCount);
	DestinationImage.Data = &*DestinationData.begin();
	// Transform data
	Result = static_cast<VmbErrorType>(VmbImageTransform(&SourceImage, &DestinationImage, NULL, 0));
	return Result;
}

string IPv4ToString(const VmbInt64_t nIPAddress)
{
	stringstream sIPv4;
	sIPv4 << ofToString(0xFF & (nIPAddress >> 24)) << "."
		<< ofToString(0xFF & (nIPAddress >> 16)) << "."
		<< ofToString(0xFF & (nIPAddress >> 8)) << "."
		<< ofToString(0xFF & nIPAddress);
	return sIPv4.str();
}

VmbInt64_t StringToIPv4(const string& sIPString)
{
	VmbInt64_t ipAddress = 0;
	vector<string> ipParts = ofSplitString(sIPString, ".", true, true);

	if (ipParts.size() == 4)
	{
		for (string& part : ipParts) {
			VmbInt64_t temp = stoi(part);
			if (!ofInRange(temp, 0, 255)) {
				ofLogWarning(__FUNCTION__) << "Illegal stuff in IP address";
				return -1;
			}
			ipAddress = (ipAddress << 8) + temp;
		}
		return ipAddress;
	}
	return -1;
}



}} // namespace ofxVimba::Utils