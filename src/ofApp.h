#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "OscReceiver.hpp"

// send host (aka ip address)
#define HOST "localhost"

// send port
#define SEND_PORT 12345
#define RECEIVE_PORT 7400

// max number of strings to display
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp {

public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
        
    ofTrueTypeFont font;
    ofBuffer imgAsBuffer;
    ofImage img;
    
    int currentMsgString;
    string msgStrings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];

    float mouseXf = 0;
    float mouseYf = 0;
    int mouseButtonInt = 0;
    string mouseButtonState = "";

    ofImage receivedImage;
};
