#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);

	this->mesh.setMode(OF_PRIMITIVE_LINES);

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofClear(0);
	ofSetColor(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 320, true, true, true);

	font.drawString("NE", font.stringWidth("NE") * -0.5, font.stringHeight("NE") * -0.05);
	font.drawString("AR", font.stringWidth("AR") * -0.5, font.stringHeight("AR") * 1.05);

	fbo.end();

	auto span = 15;

	ofPixels pixels;
	fbo.readToPixels(pixels);
	for (int x = 0; x < fbo.getWidth(); x += span) {

		for (int y = 0; y < fbo.getHeight(); y += span) {

			ofColor color = pixels.getColor(x, y);
			if (color != ofColor(0, 0)) {

				this->mesh.addVertex(glm::vec3(x, y, 0));
			}
		}
	}

	ofSetColor(39);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->draw_mesh = this->mesh;

	for (auto& vertex : this->draw_mesh.getVertices()) {

		vertex.x += ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -5, 5);
		vertex.y += ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -5, 5);
	}

	for (int i = 0; i < this->draw_mesh.getVertices().size(); i++) {

		auto location = this->draw_mesh.getVertex(i);
		vector<int> near_index_list;
		for (int k = 0; k < this->draw_mesh.getVertices().size(); k++) {

			if (i == k) { continue; }

			auto other = this->draw_mesh.getVertex(k);
			auto distance = glm::distance(location, other);
			if (distance < 16.5) {

				this->draw_mesh.addIndex(i);
				this->draw_mesh.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->draw_mesh.draw();

	for (int i = 0; i < this->draw_mesh.getVertices().size(); i++) {

		ofDrawCircle(this->draw_mesh.getVertex(i), 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}