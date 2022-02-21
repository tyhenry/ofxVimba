#include "ofApp.h"

using namespace ofxVimba;

//--------------------------------------------------------------
void ofApp::setup(){

	// we need to start up the Vimba system to list devices.
	ofxVimba::Core::ofxVimbaSystem& system = ofxVimbaSystem::getInstance();
	system.startup();

	vector<string> ids = system.listDevices(true);
	cameras.resize(ids.size());


	// assumes a fully dedicated gigabit switch and NIC (no overhead)
	int availableBandwidth = 124000000;
	int bpsPerCamera = availableBandwidth / ids.size();
	//bpsPerCamera *= 0.75;

	for (int i = 0; i < cameras.size(); i++) {
		cameras[i].open(ids[i]);
		cameras[i].start();
		//cameras[i].setFeatureValue<VmbInt64_t>("StreamBytesPerSecond", bpsPerCamera);
		VmbInt64_t bps = cameras[i].getFeatureValue<VmbInt64_t>("StreamBytesPerSecond");
		VmbInt64_t ip = cameras[i].getFeatureValue<VmbInt64_t>("GevCurrentIPAddress");
		//cameras[i].listFeatures();
		ofLogNotice(__FUNCTION__) << "opening " << ids[i] << " (" << Utils::IPv4ToString(ip) << ") " << bps << " bytes per second ";
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
	ofScale(0.25f, 0.25f);
	float x = 0;
	for (const ofxVimba::ofxVimbaCam& cam : cameras) {
		cam.draw(x, 10);

		stringstream info;
		info << cam.getNumFramesReceived() << " frames | " << setprecision(3) << cam.getFPS() << "fps";
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
