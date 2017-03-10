//
//  faceidentifier.cpp
//  cameraTest
//
//  Created by Mark Nijboer on 09/03/2017.
//  Copyright © 2017 Mark Nijboer. All rights reserved.
//

#include "faceidentifier.hpp"

// TODO: make this class not static

std::vector<FaceIdentifier::NamedFace> FaceIdentifier::identifyFaces(std::vector<VCapture::Face> face)
{
    std::vector<FaceIdentifier::NamedFace> namedFaces;
    FaceIdentifier::shiftFaces();
    
    // Check if face storage is empty
    if (FaceIdentifier::storedFaces.size() == 0)
    {
        for (size_t i = 0; i < face.size(); i++)
        {
            std::string id = FaceIdentifier::getRandomIdentifier(10);
            
            FaceStorage st;
            st.name = id;
            st.face[0] = face[i];
            FaceIdentifier::storedFaces.push_back(st);
            
            FaceIdentifier::NamedFace nf;
            nf.name = id;
            nf.face = face[i];
            namedFaces.push_back(nf);
        }
    }
    else
    {
        std::vector<FaceIdentifier::NamedFace> meanFaces;
        for (size_t i = 0; i < FaceIdentifier::storedFaces.size(); i++)
        {
            VCapture::Face *tempFace;
            for (size_t j = 0; j < FaceIdentifier::FACE_HISTORY_LENGTH; j++)
            {
                if (!FaceIdentifier::faceIsEmpty(FaceIdentifier::storedFaces.at(i).face[j])) {
                    tempFace = &FaceIdentifier::storedFaces.at(i).face[j];
                    break;
                }
            }
            
            if (tempFace == NULL)
            {
                std::cout << "Code failed. The storage cannot be empty here" << std::endl;
            }
            else
            {
                FaceIdentifier::NamedFace tfs;
                
                tfs.face = *tempFace;
                tfs.name = FaceIdentifier::storedFaces.at(i).name;
                meanFaces.push_back(tfs);
            }
        }
    }
    // Get mean of last stored faces;
    
    // Define a threshold
    
    // Get stored face identification that was within the threshold and closest to the given value
    
    // Save the given value in the storage on position 0 of the given identification
    
    // Return the faces with their given names
    
    return namedFaces;
}

std::string FaceIdentifier::getRandomIdentifier(int size) {
    
    std::string s = "";
    static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
    
    for (int i = 0; i < size; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return s;
}


void FaceIdentifier::shiftFaces()
{
    for (size_t i = 0; i < FaceIdentifier::storedFaces.size(); i++)
    {
        FaceStorage face = FaceIdentifier::storedFaces.at(i);
        
        // Shift the array one position to the right
        std::copy(face.face + 0, face.face + (FaceIdentifier::FACE_HISTORY_LENGTH - 2), face.face + 1);
        
        // Set first element as empty face
        VCapture::Face t;
        face.face[0] = t;
        
        
        // Remove face if it didn't occur in the last x frames
        if (identifiedFaceIsEmpty(face))
        {
            FaceIdentifier::storedFaces.erase(FaceIdentifier::storedFaces.begin() + i);
        }
    }
}


bool FaceIdentifier::identifiedFaceIsEmpty(FaceStorage face)
{
    for (size_t i = 0; i < FaceIdentifier::FACE_HISTORY_LENGTH; i++)
    {
        if (!FaceIdentifier::faceIsEmpty(face.face[i]))
        {
            return false;
        }
    }
    return true;
}



bool FaceIdentifier::faceIsEmpty(VCapture::Face face)
{
    if (face.x == 0 && face.y == 0 && face.height == 0 && face.width == 0)
    {
        return true;
    }
    return false;
}
