#include "ofApp.h"

int numMasks;
int maskOffset;

//--------------------------------------------------------------
void ofApp::setup(){
    
    // native res of camera = 1280 x 720
    
    camFrameRate = 60;
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
    
    numMasks = (int)dir.size();
    maskOffset = rand() % numMasks;
    
    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < numMasks; i++){
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
    }
}

const int maxFaces = 5;
const int smoothingMax = 3;
bool sampleExists[maxFaces][smoothingMax];
ofRectangle smoothingSamples[maxFaces][smoothingMax];
int smoothingIndex = 0;

//--------------------------------------------------------------
void ofApp::draw(){
    
    finder.findHaarObjects(img, 50, 50);
    
    // draw image to fit the whole screen
    img.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    for (int faceIndex = 0; faceIndex < maxFaces; faceIndex++) {
        
        // create samples
        if (faceIndex < finder.blobs.size() ) {
            smoothingSamples[faceIndex][smoothingIndex] = finder.blobs[faceIndex].boundingRect;
            sampleExists[faceIndex][smoothingIndex] = true;
        } else {
            sampleExists[faceIndex][smoothingIndex] = false;
        }
        
        // create average of samples
        int x = 0, y = 0, w = 0, h = 0, n = 0;
        
        for (int i = 0; i < smoothingMax; i++) {
            if (sampleExists[faceIndex][i]) {
                ofRectangle cur = smoothingSamples[faceIndex][i];
                x = x + cur.x;
                y = y + cur.y;
                w = w + cur.width;
                h = h + cur.height;
                n++;
            }
        }
        
        ofImage mask =  masks[(maskOffset + faceIndex) % numMasks];
        
        if (n > smoothingMax - 1) {
            x = x / n;
            y = y / n;
            w = w / n;
            h = h / n;
            
            mask.draw(ofMap(x, 0, camWidth, 0, ofGetWidth()), ofMap(y, 0, camHeight, 0, ofGetHeight()),
                  ofMap(w, 0, camWidth, 0, ofGetWidth()), ofMap(h, 0, camHeight, 0, ofGetHeight()));
        } else {
            mask.draw(0,0,0,0);
        }
    }
    
    smoothingIndex = (smoothingIndex + 1) % smoothingMax;
    
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
