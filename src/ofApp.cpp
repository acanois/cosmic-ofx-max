#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowTitle("oscSenderExample");
    ofSetFrameRate(60); // run at 60 fps
    ofSetVerticalSync(true);

    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, SEND_PORT);

    // preload image to send into a buffer
    imgAsBuffer = ofBufferFromFile("of-logo.png", true);
    
    // Receiver
    ofLog() << "listening for osc messages on port " << RECEIVE_PORT;
    receiver.setup(RECEIVE_PORT);
}

//--------------------------------------------------------------
void ofApp::update() {
    ofxOscMessage m;
    m.setAddress("/mouse/position");
    m.addFloatArg(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.f, 1.f, true));
    m.addFloatArg(ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.f, 1.f, true));
    sender.sendMessage(m, false);
    
    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msgStrings[i] = "";
        }
    }

    // check for waiting messages
    while(receiver.hasWaitingMessages()){

        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        // check for mouse moved message
        if(m.getAddress() == "/mouse/position"){

            // both the arguments are floats
            mouseXf = m.getArgAsFloat(0);
            mouseYf = m.getArgAsFloat(1);
        }
        // check for mouse button message
        else if(m.getAddress() == "/mouse/button"){

            // first argument is int32, second is a string
            mouseButtonInt = m.getArgAsInt32(0);
            mouseButtonState = m.getArgAsString(1);
        }
        // check for an image being sent
        // note: the size of the image depends greatly on your network buffer
        // sizes, if an image is too big the message won't come through
        else if(m.getAddress() == "/image"){
            ofBuffer buffer = m.getArgAsBlob(0);
            receivedImage.load(buffer);
        }
        else{

            // unrecognized message: display on the bottom of the screen
            string msgString;
            msgString = m.getAddress();
            msgString += ":";
            for(size_t i = 0; i < m.getNumArgs(); i++){

                // get the argument type
                msgString += " ";
                msgString += m.getArgTypeName(i);
                msgString += ":";

                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msgString += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msgString += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msgString += m.getArgAsString(i);
                }
                else{
                    msgString += "unhandled argument type " + m.getArgTypeName(i);
                }
            }

            // add to the list of strings to display
            msgStrings[currentMsgString] = msgString;
            timers[currentMsgString] = ofGetElapsedTimef() + 5.0f;
            currentMsgString = (currentMsgString + 1) % NUM_MSG_STRINGS;

            // clear the next line
            msgStrings[currentMsgString] = "";
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(255, 100);

    // draw image if it's loaded
    if(img.isAllocated()) {
        ofSetColor(255);
        img.draw(ofGetWidth()/2 - img.getWidth()/2,
                 ofGetHeight()/2 - img.getHeight()/2);
    }

    // display instructions
    string sendBuf = "sending osc messages to: " + string(HOST);
    sendBuf += " " + ofToString(SEND_PORT);
    sendBuf += "\npress A to send osc message [/test 1 3.5 hello <time>]\n";
    sendBuf += "press I to send a (small) image as a osc blob to [/image]";
    ofDrawBitmapStringHighlight(sendBuf, 10, 20);
    
    // if image exists, draw it
    if(receivedImage.getWidth() > 0){
        ofSetColor(255);
        receivedImage.draw(ofGetWidth()/2 - receivedImage.getWidth()/2,
                           ofGetHeight()/2 - receivedImage.getHeight()/2);
    }

    // draw recent unrecognized messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        ofDrawBitmapStringHighlight(msgStrings[i], 10, 40 + 15 * i);
    }

    string receiveBuf = "listening for osc messages on port " + ofToString(RECEIVE_PORT);
    ofDrawBitmapStringHighlight(receiveBuf, 10, 100);

    // draw mouse state
    glm::vec3 mouseIn(mouseXf*ofGetWidth(), mouseYf*ofGetHeight(),0);
    if(mouseButtonInt == 0){
        ofSetColor(255);
    } else{
        ofSetColor(ofColor::salmon);
    }
    ofDrawCircle(mouseIn, 20);
    ofDrawBitmapStringHighlight(mouseButtonState, mouseIn);
}

//--------------------------------------------------------------
void ofApp::exit() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    // send a test message
    if(key == 'a' || key == 'A'){
        ofxOscMessage m;
        m.setAddress("/test");
        m.addIntArg(1);
        m.addFloatArg(3.5f);
        m.addStringArg("hello");
        m.addFloatArg(ofGetElapsedTimef());
        sender.sendMessage(m, false);
    }

    // send an image
    // note: the size of the image depends greatly on your network buffer sizes,
    // if an image is too big the message won't come through and you may need
    // to break it up into multiple blobs
    if(key == 'i' || key == 'I'){

        // load image from buffer
        img.load(imgAsBuffer);

        // send as a binary blob
        ofxOscMessage m;
        m.setAddress("/image");
        m.addBlobArg(imgAsBuffer);
        sender.sendMessage(m);
        ofLog() << "sending image with size: " << imgAsBuffer.size();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    ofxOscMessage m;
    m.setAddress("/mouse/button");
    m.addIntArg(1);
    m.addStringArg("down");
    sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    ofxOscMessage m;
    m.setAddress("/mouse/button");
    m.addIntArg(0);
    m.addStringArg("up");
    sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
