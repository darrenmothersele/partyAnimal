#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxSyphon.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofVideoGrabber 		vidGrabber;
    
    int camFrameRate;
    int camWidth;
    int camHeight;
    int screenWidth;
    int screenHeight;
    
    ofImage img;
    ofxCvHaarFinder finder;
    
    ofImage mask;
    
    bool flipVert;
    bool flipHoriz;

    ofxSyphonServer mainOutputSyphonServer;
};
