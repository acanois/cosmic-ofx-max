//
//  OscReceiver.cpp
//  cosmic
//
//  Created by David Richter on 2/13/24.
//

#include "OscReceiver.hpp"

OscReceiver::OscReceiver()
: mReceiver(std::make_unique<ofxOscReceiver>())
{
    mReceiver->setup(RECEIVE_PORT);
}

void OscReceiver::receiveMessages() {
    while (mReceiver->hasWaitingMessages()) {
        ofxOscMessage m;
        mReceiver->getNextMessage(m);
        
        ofLog() << m.getAddress();
    }
}
