//
//  socketserver.cpp
//  cameraTest
//
//  Created by Mark Nijboer on 01/03/2017.
//  Copyright © 2017 Mark Nijboer. All rights reserved.
//

#include "socketserver.hpp"

std::vector<uWS::WebSocket<uWS::SERVER>> *CamSocketServer::connections;


CamSocketServer::CamSocketServer()
{
    CamSocketServer::connections = new std::vector<uWS::WebSocket<uWS::SERVER>>();
}


CamSocketServer::~CamSocketServer()
{
    delete CamSocketServer::connections;
    CamSocketServer::connections = NULL;
}


void CamSocketServer::run()
{
    uWS::Hub h;
    
    h.onMessage([](uWS::WebSocket<uWS::SERVER> ws, char *message, size_t length, uWS::OpCode opCode)
    {
        std::string str_message = message;
        std::cout << str_message.substr(0,length) << std::endl;
    });
    
    h.onConnection([&](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req)
    {
        // Save connection
        CamSocketServer::connections->push_back(ws);
    });
    
    h.onDisconnection([&](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length)
    {
        // Disconnect saved server
        CamSocketServer::disconnectSocket(ws.getAddress());
    });
    
    h.listen(8000);
    h.run();
}


void CamSocketServer::broadcast(std::string message)
{
    for (size_t i = 0; i < CamSocketServer::connections->size(); i++)
    {
            CamSocketServer::connections->at(i).send(message.c_str());
    }
}


void CamSocketServer::disconnectSocket(uS::Socket::Address address)
{
    for (size_t i = 0; i < CamSocketServer::connections->size(); i++)
    {
        uS::Socket::Address tempAddress = CamSocketServer::connections->at(i).getAddress();
        if (tempAddress.address == address.address && tempAddress.port == address.port)
        {
            CamSocketServer::connections->erase(CamSocketServer::connections->begin() + i);
        }
    }
}
