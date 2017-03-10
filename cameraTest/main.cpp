//
//  main.cpp
//  cameraTest
//
//  Created by Mark Nijboer on 05/01/2017.
//  Copyright © 2017 Mark Nijboer. All rights reserved.
//

#include <thread>

#include "capture.hpp"
#include "socketserver.hpp"

void doRunCap(VCapture * capture)
{
    capture->run();
}

void doRunServ(CamSocketServer * server)
{
    server->run();
}


int main (int argc, const char * argv[])
{
    VCapture cap("/Users/mark/Documents/Development/C++/cameraTest/assets/haarcascade_frontalface_alt.xml");
    std::thread thrCam(doRunCap, &cap);
    
    CamSocketServer server;
    std::thread thrServ(doRunServ, &server);
    
    thrCam.join();
    thrServ.join();
    
    return 0;
}
