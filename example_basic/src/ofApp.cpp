#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cam.open();
}

//--------------------------------------------------------------
void ofApp::exit() {
	cam.close();
	ofxVimba::exit();
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.draw(10, 20, cam.getCamWidth()*0.5, cam.getCamHeight()*0.5);

	stringstream ss;
	ss << ofGetFrameRate() << "fps";
	ofDrawBitmapString(ss.str(), 10, 20);
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