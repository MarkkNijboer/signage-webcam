//
//  socketserver.hpp
//  cameraTest
//
//  Created by Mark Nijboer on 01/03/2017.
//  Copyright © 2017 Mark Nijboer. All rights reserved.
//

#ifndef socketserver_hpp
#define socketserver_hpp

#include <stdio.h>
#include <uWS/uWS.h>

class CamSocketServer
{
    
private:
    static std::vector<uWS::WebSocket<uWS::SERVER>> * connections;
    
public:
    CamSocketServer();
    ~CamSocketServer();
    
    void run();
    
    static void broadcast(std::string);
    static void disconnectSocket(uS::Socket::Address address);
};

#endif /* socketserver_hpp */
