#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetEscapeQuitsApp(true);
	ofSetWindowTitle("ofxVimba GUI test");
	ofSetVerticalSync(false);			// Don't limit framerate to screen refresh rate
	ofDisableArbTex();					// Use only normalized texture coords
	ofSetFrameRate(90);					// Set it super high just to see how performance is doing.
	//ofDisableDataPath();

	vector<string> devices = cam.listDevices();
	for (auto& d : devices) {
		ofLogNotice(__FUNCTION__) << d;
	}
	if (devices.size() < 1) {
		ofExit();
	}

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	gui.setup();

	cam.open(devices[0]);
	cam.listFeatures();
	//cam.load();
	cam.start();
	bPlaying = true;
	//cam.setFeatureValue<VmbInt64_t>("BinningHorizontal", 2);
	//cam.setFeatureValue<VmbInt64_t>("BinningVertical", 2);

	//tex.allocate(cam.getCamWidth(), cam.getCamHeight(), GL_RGB);
	//

	hue = cam.getFeatureValue<double>("Hue");

}

//--------------------------------------------------------------
void ofApp::exit() {
	cam.stop();
	cam.close();
	ofxVimba::exit();
}


//--------------------------------------------------------------
void ofApp::update(){
	cam.update();
	if (cam.isFrameNew()) {
		uint64_t now = ofGetElapsedTimeMillis();
		uint64_t elapsed = now - lastCamFrame;
		camFps.push_back(1000 / (float)elapsed);
		lastCamFrame = now;
		while (camFps.size() > 500) camFps.erase(camFps.begin());
	}

	appFps.push_back(ofGetFrameRate());
	while (appFps.size() > 500) appFps.erase(appFps.begin());
}

//--------------------------------------------------------------
void ofApp::draw(){
	//cam.draw(10, 20, cam.getCamWidth() * 0.5, cam.getCamHeight() * 0.5);

	gui.begin();
	ImGui::Begin("Application");
	char msg[255];
	if (appFps.size() > 0) {
		float avg = accumulate(appFps.begin(), appFps.end(), 0.0) / appFps.size();
		sprintf(msg, "avg: %02f", avg);
		ImGui::PlotLines("App FPS", &appFps[0], appFps.size(), 0, msg, 0, 100, ImVec2(0, 80));
	}
	if (camFps.size() > 0) {
		float avg = accumulate(camFps.begin(), camFps.end(), 0.0) / camFps.size();
		sprintf(msg, "avg: %02f", avg);
		ImGui::PlotLines("Cam FPS", &camFps[0], camFps.size(), 0, msg, 0, 100, ImVec2(0, 80));
	}
	ImGui::End();

	ImGui::Begin("Output Video");
	ImGui::Image(GetImTextureID(cam.getFrame().getTexture()), ImVec2(cam.getCamWidth() * 0.5, cam.getCamHeight()*0.5));
	ImGui::End();

	ImGui::Begin("Camera Settings");
	if (ImGui::SliderFloat("Hue", &hue, -40, 40)) {
		cam.setFeatureValue<double>("Hue", hue);
	}
	ImGui::End();
	gui.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ' ') {
		if (bPlaying) cam.stop();
		else cam.start();
		bPlaying = !bPlaying;
	}
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
