#include "ofApp.h"

using namespace ofxVimba;



//--------------------------------------------------------------
void ofApp::setup(){

	// we need to start up the Vimba system to list devices.
	ofxVimba::Core::ofxVimbaSystem& system = ofxVimbaSystem::getInstance();
	system.startup();

	vector<ofVideoDevice> devices = system.listDevices();
	cameras.resize(devices.size());


	// assumes a fully dedicated gigabit switch and NIC (no overhead)
	int availableBandwidth = 124000000;
	int bpsPerCamera = availableBandwidth / devices.size();
	//bpsPerCamera *= 0.75;
	ofJson settings = {
		{"StreamBytesPerSecond", bpsPerCamera },
		{"PixelFormat", VmbPixelFormatMono8},
		{"BinningHorizontal", 2 },
		{"BinningVertical", 1 },
		{"DecimationHorizontal", 1},
		{"DecimationVertical", 2},
		{"Height", 768},
		{"Width", 1024},
		{"OffsetX", 4},
		{"OffsetY", 4},
		{"ChunkModeActive", false},
		{"GainAuto", Off},
		//{"GevIPConfigurationMode", Persistent },
		{"BandwidthControlMode", StreamBytesPerSecond },
		{"StreamFrameRateConstrain", false}
	};


	for (int i = 0; i < cameras.size(); i++) {
		ofxVimbaCam& cam = cameras[i];
		char userID[10];
		sprintf(userID, "Cam%03d", i);
		//char ipAddress[24];
		//sprintf(ipAddress, "169.254.123.%i", 45+i);

		settings["DeviceUserID"] = userID;
		//settings["GevPersistentIPAddress"] = StringToIPv4(ipAddress);

		cam.open(i);
		cam.set(settings);
		cameras[i].start();
 
		VmbInt64_t bps = cameras[i].getFeatureValue<VmbInt64_t>("StreamBytesPerSecond");
		VmbInt64_t ip = cameras[i].getFeatureValue<VmbInt64_t>("GevCurrentIPAddress");
		//cameras[i].listFeatures();
		ofLogNotice(__FUNCTION__) << "opening " << devices[i].deviceName << " (" << Utils::IPv4ToString(ip) << ") " << bps << " bytes per second ";
	}
}


//--------------------------------------------------------------
void ofApp::exit() {
	for (ofxVimba::ofxVimbaCam& cam : cameras) {
		cam.close();
	}
	ofxVimba::exit();
}

//--------------------------------------------------------------
void ofApp::update(){
	for (ofxVimba::ofxVimbaCam& cam : cameras) {
		cam.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(ofColor::white);
	ofPushMatrix();
	//ofScale(0.25f, 0.25f);
	float x = 0;
	for (const ofxVimba::ofxVimbaCam& cam : cameras) {
		cam.draw(x, 10);

		stringstream info;
		info << cam.getNumFramesReceived() << " frames";
		ofDrawBitmapString(info.str(), x, cam.getCamHeight() + 14);

		x += cam.getCamWidth();
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
