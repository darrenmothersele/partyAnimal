#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    camWidth  = 320;
    camHeight = 240;
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
    
    flipVert = false;
    flipHoriz = true;
    
    // list video devices to debug console
    // use this to find correct ID for video grabber
    /*
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    */
    
    // setup grabber on device 0
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth,camHeight);
    ofSetVerticalSync(true);
    
    // setup face finder with settings from xml file
    finder.setup("haarcascade_frontalface_default.xml");
    
    // load mask image
    mask.loadImage("mask0.png");
   
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    if (vidGrabber.isFrameNew()) {
        
        // if video grabber has a new frame
        // grab the image and run face finder
        img.setFromPixels(vidGrabber.getPixelsRef());
        img.mirror(flipVert, flipHoriz);
    }
}

const int maxFaces = 5;
const int smoothingMax = 4;
bool sampleExists[maxFaces][smoothingMax];
ofRectangle smoothingSamples[maxFaces][smoothingMax];
int smoothingIndex = 0;

//--------------------------------------------------------------
void ofApp::draw(){
    
    finder.findHaarObjects(img, 50, 50);
    
    // draw image to fit the whole screen
    img.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    for(unsigned int faceIndex = 0; faceIndex < finder.blobs.size() && faceIndex < maxFaces; faceIndex++) {
        
        smoothingSamples[faceIndex][smoothingIndex] = finder.blobs[faceIndex].boundingRect;
        sampleExists[faceIndex][smoothingIndex] = true;
        
        int x = 0, y = 0, w = 0, h = 0, n = 0;
        
        for (int i = 0; i < smoothingMax; i++) {
            if (sampleExists[i]) {
                ofRectangle cur = smoothingSamples[faceIndex][i];
                x = x + cur.x;
                y = y + cur.y;
                w = w + cur.width;
                h = h + cur.height;
                n++;
            }
        }
        
        if (n > 0) {
            x = x / n;
            y = y / n;
            w = w / n;
            h = h / n;
            
            mask.draw(ofMap(x, 0, camWidth, 0, ofGetWidth()), ofMap(y, 0, camHeight, 0, ofGetHeight()),
                  ofMap(w, 0, camWidth, 0, ofGetWidth()), ofMap(h, 0, camHeight, 0, ofGetHeight()));
        }
    }
    
    smoothingIndex = (smoothingIndex + 1) % smoothingMax;
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
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
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
