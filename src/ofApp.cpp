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
    
    // grab first frame
    img.setFromPixels(vidGrabber.getPixelsRef());
    img.mirror(flipVert, flipHoriz);
    finder.findHaarObjects(img);
    
    // load mask image
    mask.loadImage("mask2.png");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    if (vidGrabber.isFrameNew()) {
        // if video grabber has a new frame
        // grab the image and run face finder
        img.setFromPixels(vidGrabber.getPixelsRef());
        img.mirror(flipVert, flipHoriz);
        finder.findHaarObjects(img);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw image to fit the whole screen
    img.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    for(unsigned int i = 0; i < finder.blobs.size(); i++) {

        ofRectangle cur = finder.blobs[i].boundingRect;
        mask.draw(ofMap(cur.x, 0, camWidth, 0, ofGetWidth()), ofMap(cur.y, 0, camHeight, 0, ofGetHeight()),
                  ofMap(cur.width, 0, camWidth, 0, ofGetWidth()), ofMap(cur.height, 0, camHeight, 0, ofGetHeight()));
    }
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
