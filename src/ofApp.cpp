#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // native res of camera = 1280 x 720
    
    camFrameRate = 30;
    camWidth  = 640;
    camHeight = 360;
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
    vidGrabber.setDesiredFrameRate(camFrameRate);
    vidGrabber.initGrabber(camWidth,camHeight);
    ofSetVerticalSync(true);
    
    // setup face finder with settings from xml file
    finder.setup("haarcascade_frontalface_default.xml");
    
    // grab first frame
    img.setFromPixels(vidGrabber.getPixelsRef());
    img.mirror(flipVert, flipHoriz);
    finder.findHaarObjects(img);
    
    // load mask images
    dir.allowExt("png");
    dir.listDir("masks");
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    
    //allocate the vector to have as many ofImages as masks
    if( dir.size() ){
        masks.assign(dir.size(), ofImage());
    }
    
    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < (int)dir.size(); i++){
        masks[i].loadImage(dir.getPath(i));
    }
    
    
    mainOutputSyphonServer1.setName("Party Animals Main Output 1");
    mainOutputSyphonServer2.setName("Party Animals Main Output 2");
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
        masks[0].draw(ofMap(cur.x, 0, camWidth, 0, ofGetWidth()), ofMap(cur.y, 0, camHeight, 0, ofGetHeight()),
                  ofMap(cur.width, 0, camWidth, 0, ofGetWidth()), ofMap(cur.height, 0, camHeight, 0, ofGetHeight()));
    }
    
    mainOutputSyphonServer1.publishScreen();
    mainOutputSyphonServer2.publishScreen();
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
