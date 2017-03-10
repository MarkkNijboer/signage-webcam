//
//  VCapture.hpp
//  cameraTest
//
//  Created by Mark Nijboer on 22/02/2017.
//  Copyright © 2017 Mark Nijboer. All rights reserved.
//

#ifndef VCapture_hpp
#define VCapture_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

class VCapture
{
    
private:
    cv::VideoCapture cap;
    
    bool now_running = true;
    
    std::string casc_path = "";
    
    
public:
    
    struct Face {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
    };
    
    
    // Constructor
    VCapture(std::string casc_path);
    
    // Method to run in a thread
    void run();
    
    // Method to stop the running thread
    void stop();
    
    // Find the faces in an image
    std::vector<struct VCapture::Face> find_faces(cv::Mat &frame, cv::CascadeClassifier cascade);
    
    // Initialize the facial recognition
    cv::CascadeClassifier init_facial_recognition();
    
    // Resize the image to optimize performance
    cv::Mat resize_frame(cv::Mat frame);
    
};

#endif /* VCapture_hpp */
