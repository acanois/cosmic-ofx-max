//
//  OscReceiver.hpp
//  cosmic
//
//  Created by David Richter on 2/13/24.
//

#pragma once

#include <memory>

#include "ofxOsc.h"

class OscReceiver {
public:
    OscReceiver();
    void receiveMessages();
    
public:
    static constexpr int RECEIVE_PORT { 7400 };
    
private:
    std::unique_ptr<ofxOscReceiver> mReceiver { nullptr };
};

