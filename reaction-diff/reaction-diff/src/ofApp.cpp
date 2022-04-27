#include "ofApp.h"

class Cell {
public:
	float a;
	float b;

	Cell(float a_, float b_) {
		a = a_;
		b = b_;
	}

	virtual Cell* Clone()
	{
		return new Cell(*this);
	}
};

std::vector<std::vector<Cell*>> grid;
std::vector<std::vector<Cell*>> prevGrid;

ofImage img;

float appWidth;
float appHeight;
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	
	appWidth = ofGetWidth();
	appHeight = ofGetHeight();
	
	//img.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);

	grid.resize(ofGetWidth());
	prevGrid.resize(ofGetWidth());
	for (int i = 0; i < ofGetWidth(); i++) {
		grid[i].resize(ofGetHeight());
		prevGrid[i].resize(ofGetHeight());
		for (int j = 0; j < ofGetHeight(); j++) {
			float a = 1;
			float b = 0;
			grid[i][j] = new Cell(a, b);
			prevGrid[i][j] = new Cell(a, b);
		}
	}

	for (int n = 0; n < 10; n++) {
		int startx = (int)ofRandom(20, ofGetWidth() - 20);
		int starty = (int)ofRandom(20, ofGetHeight() - 20);

		for (int i = startx; i < startx + 10; i++) {
			for (int j = starty; j < starty + 10; j++) {
				float a = 1;
				float b = 1;
				grid[i][j] = new Cell(a, b);
				prevGrid[i][j] = new Cell(a, b);
			}
		}
	}
}

float dA = 1.0;
float dB = 0.5;
float feed = 0.055;
float k = 0.062;

//--------------------------------------------------------------
void updateGrid() {
	for (int i = 1; i < appWidth - 1; i++) {
		for (int j = 1; j < appHeight - 1; j++) {
			Cell* spot = prevGrid[i][j];
			Cell* newspot = grid[i][j];

			float a = spot->a;
			float b = spot->b;

			float laplaceA = 0;
			laplaceA += a * -1;
			laplaceA += prevGrid[i + 1][j]->a*0.2;
			laplaceA += prevGrid[i - 1][j]->a*0.2;
			laplaceA += prevGrid[i][j + 1]->a*0.2;
			laplaceA += prevGrid[i][j - 1]->a*0.2;
			laplaceA += prevGrid[i - 1][j - 1]->a*0.05;
			laplaceA += prevGrid[i + 1][j - 1]->a*0.05;
			laplaceA += prevGrid[i - 1][j + 1]->a*0.05;
			laplaceA += prevGrid[i + 1][j + 1]->a*0.05;

			float laplaceB = 0;
			laplaceB += b * -1;
			laplaceB += prevGrid[i + 1][j]->b*0.2;
			laplaceB += prevGrid[i - 1][j]->b*0.2;
			laplaceB += prevGrid[i][j + 1]->b*0.2;
			laplaceB += prevGrid[i][j - 1]->b*0.2;
			laplaceB += prevGrid[i - 1][j - 1]->b*0.05;
			laplaceB += prevGrid[i + 1][j - 1]->b*0.05;
			laplaceB += prevGrid[i - 1][j + 1]->b*0.05;
			laplaceB += prevGrid[i + 1][j + 1]->b*0.05;

			newspot->a = a + (dA*laplaceA - a * b*b + feed * (1 - a)) * 1;
			newspot->b = b + (dB*laplaceB + a * b*b - (k + feed)*b) * 1;

			newspot->a = ofClamp(newspot->a, 0, 1);
			newspot->b = ofClamp(newspot->b, 0, 1);
		}
	}
}

void swap() {
	std::vector<std::vector<Cell*>> temp = prevGrid;
	prevGrid = grid;
	grid = temp;
}

//--------------------------------------------------------------
void ofApp::update() {
	for (int i = 0; i < 1; i++) {
		updateGrid();
		swap();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	img.grabScreen(0, 0, appWidth, appHeight);
	ofPixels& pixels = img.getPixels();
	for (int i = 0; i < appWidth; i++) {
		for (int j = 0; j < appHeight; j++) {
			Cell* spot = grid[i][j];
			float a = spot->a;
			float b = spot->b;
			pixels.setColor(i,j, (a - b) * 255);
		}
	}
	img.update();
	img.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}