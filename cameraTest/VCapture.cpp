//
//  VCapture.cpp
//  cameraTest
//
//  Created by Mark Nijboer on 22/02/2017.
//  Copyright © 2017 Mark Nijboer. All rights reserved.
//

#include "VCapture.hpp"
#include "socketserver.hpp"
#include "faceidentifier.hpp"


// Constructor
//
// Setup the current capture
//
VCapture::VCapture(std::string casc_path)
{
    this->cap = cv::VideoCapture(1);
    this->casc_path = casc_path;
}


// Initialize the facial recognition
// Load the HAAR Cascade file and instantiate the cascade
//
// @return facial_cascade   The cascade containing the haarstructure for facial recognition
cv::CascadeClassifier VCapture::init_facial_recognition()
{
    cv::CascadeClassifier facial_cascade;
    facial_cascade.load(this->casc_path);
    return facial_cascade;
}


// Method to run in thread
//
void VCapture::run()
{
    cv::CascadeClassifier cascade = this->init_facial_recognition();
    
    cv::Mat img;
    std::vector<struct FaceIdentifier::NamedFace> prevFaces;
    FaceIdentifier faceIdentifier;
    
    
    while (this->now_running)
    {
        this->cap >> img;
        if (img.empty())
            continue;
        
        img = this->resize_frame(img);
        
        std::vector<struct VCapture::Face> faces = this->find_faces(img, cascade);
        std::vector<struct FaceIdentifier::NamedFace> namedFaces = faceIdentifier.identifyFaces(faces);
        if (namedFaces.size() == prevFaces.size())
        {
            std::string output = "{\"frame\":{\"faces\": {";
            
            for (int i = 0; i < namedFaces.size(); i++)
            {
                if (i > 0)
                {
                    output.append(",");
                }
                
                output.append("\"");
                output.append(namedFaces[i].name);
                output.append("\"");
                output.append(":{\"x\":");
                output.append(std::to_string(namedFaces[i].face.x));
                output.append(",\"y\":");
                output.append(std::to_string(namedFaces[i].face.y));
                output.append(",\"w\":");
                output.append(std::to_string(namedFaces[i].face.width));
                output.append(",\"h\":");
                output.append(std::to_string(namedFaces[i].face.height));
                output.append("}");
            }
                
            output.append("},\"sizes\":{\"x\":");
            output.append(std::to_string(img.cols));
            output.append(",\"y\":");
            output.append(std::to_string(img.rows));
            output.append("}}}");
            
            CamSocketServer::broadcast(output);
        }
        prevFaces = namedFaces;
        
        /*imshow("opencv", img);*/
        if (cv::waitKey(10)>=0)
            continue;
    }
}

// Method to stop the thread
//
void VCapture::stop()
{
    this->now_running = false;
}


// Method to resize the frame
//
// @param frame     The frame to resize
// @return frame    The resized frame
//
cv::Mat VCapture::resize_frame(cv::Mat frame)
{
    cv::Size size(480, 270);
    cv::resize(frame, frame, size);
    return frame;
}


// Method to get all the faces from an image
//
// @param frame     The frame in question
// @param cascade   The haarcascade to use while looking for faces
//
std::vector<struct VCapture::Face> VCapture::find_faces(cv::Mat &frame, cv::CascadeClassifier cascade)
{
    cv::Mat gray;
    std::vector<cv::Rect> faces;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cascade.detectMultiScale(gray, faces, 1.2, 5);
    
    std::vector<struct VCapture::Face> faces_filtered;
    
    for (int i = 0; i < faces.size(); i++)
    {
        int j;
        cv::Rect r = faces[i];
        for (j = 0; j < faces.size(); j++)
        {
            if (j != i && (r & faces[j]) == r)
            {
                break;
            }
        }
        if (j == faces.size())
        {
            struct VCapture::Face face;
            face.x = r.x + cvRound(r.width*0.1);
            face.y = r.y + cvRound(r.height*0.07);
            face.height = cvRound(r.height*0.8);
            face.width = cvRound(r.width*0.8);
            
            faces_filtered.push_back(face);
            
        }
    }
    
    /*
    
    // Code for displaying faces as rectangles on the image
    for (int i = 0; i <faces_filtered.size(); i++)
    {
        cv::Rect r = faces_filtered[i];
        rectangle(frame, r.tl(), r.br(), cv::Scalar(0,255,0), 1);
    }
    */
    
    
    return faces_filtered;
}
