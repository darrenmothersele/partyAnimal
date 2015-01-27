#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    camWidth 		= 320;	// try to grab at this size.
    camHeight 		= 240;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    videoTexture.allocate(camWidth,camHeight, GL_RGB);
    ofSetVerticalSync(true);
    
    
    finder.setup("haarcascade_frontalface_default.xml");
    
    
    mask.loadImage("mask.jpg");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    vidGrabber.update();
    if (vidGrabber.isFrameNew()) {
        
        img.setFromPixels(vidGrabber.getPixelsRef());
        
        //img.setFromPixels(vidGrabber.getPixels(), 320,240);

        
        finder.findHaarObjects(img);
        
        
    }
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    //vidGrabber.draw(20,20);
    img.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    for(unsigned int i = 0; i < finder.blobs.size(); i++) {
        ofRectangle cur = finder.blobs[i].boundingRect;
        
       // ofRect(ofMap(cur.x, 0, 320, 0, ofGetWidth()), ofMap(cur.y, 0, 240, 0, ofGetHeight()),
       //        ofMap(cur.width, 0, 320, 0, ofGetWidth()), ofMap(cur.height, 0, 240, 0, ofGetHeight()));
        
        mask.draw(ofMap(cur.x, 0, 320, 0, ofGetWidth()), ofMap(cur.y, 0, 240, 0, ofGetHeight()),
                  ofMap(cur.width, 0, 320, 0, ofGetWidth()), ofMap(cur.height, 0, 240, 0, ofGetHeight()));
    }
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
