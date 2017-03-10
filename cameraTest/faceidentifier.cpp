//
//  faceidentifier.cpp
//  cameraTest
//
//  Created by Mark Nijboer on 09/03/2017.
//  Copyright © 2017 Mark Nijboer. All rights reserved.
//

#include "faceidentifier.hpp"


std::vector<FaceIdentifier::NamedFace> FaceIdentifier::identifyFaces(std::vector<VCapture::Face> face)
{
    std::vector<FaceIdentifier::NamedFace> namedFaces;
    this->shiftFaces();
    
    std::cout << this->storedFaces.size() << std::endl;
    
    // Check if face storage is empty
    if (this->storedFaces.size() == 0)
    {
        for (size_t i = 0; i < face.size(); i++)
        {
            NamedFace nf = this->storeNewFace(face[i]);
            namedFaces.push_back(nf);
        }
    }
    else
    {
        std::vector<FaceIdentifier::NamedFace> meanFaces;
        for (size_t i = 0; i < this->storedFaces.size(); i++)
        {
            VCapture::Face *tempFace;
            for (size_t j = 0; j < this->FACE_HISTORY_LENGTH; j++)
            {
                if (!this->faceIsEmpty(this->storedFaces.at(i).face[j])) {
                    tempFace = &this->storedFaces.at(i).face[j];
                    break;
                }
            }
            
            if (tempFace == NULL)
            {
                std::cout << "Code failed. The storage cannot be empty here" << std::endl;
            }
            else
            {
                NamedFace tfs;
                tfs.face = *tempFace;
                tfs.name = this->storedFaces.at(i).name;
                meanFaces.push_back(tfs);
            }
        }
        
        for (size_t i = 0; i < face.size(); i++)
        {
            for (size_t j = 0; j < meanFaces.size(); j++)
            {
                if (face[i].x < meanFaces.at(j).face.x + this->THRESHOLD_PIXELS && face[i].x > meanFaces.at(j).face.x - this->THRESHOLD_PIXELS
                    && face[i].y < meanFaces.at(j).face.y + this->THRESHOLD_PIXELS && face[i].y > meanFaces.at(j).face.y - this->THRESHOLD_PIXELS
                    && face[i].width < meanFaces.at(j).face.width + this->THRESHOLD_PIXELS && face[i].width > meanFaces.at(j).face.width - this->THRESHOLD_PIXELS
                    && face[i].height < meanFaces.at(j).face.height + this->THRESHOLD_PIXELS && face[i].height > meanFaces.at(j).face.height - this->THRESHOLD_PIXELS)
                {
                    // Append this face to the right storage track.
                    for (size_t k = 0; k < this->storedFaces.size(); k++)
                    {
                        // Find right track
                        if (this->storedFaces.at(k).name == meanFaces.at(j).name)
                        {
                            // Add storage to the track
                            this->storedFaces.at(k).face[0] = face[i];
                            
                            NamedFace retnamedface;
                            retnamedface.name = meanFaces.at(j).name;
                            retnamedface.face = face[i];
                            namedFaces.push_back(retnamedface);
                            
                            break;
                        }
                    }
                    break;
                }
                else
                {
                    NamedFace nf = this->storeNewFace(face[i]);
                    namedFaces.push_back(nf);
                }
                
            }
        }
    }
    
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

FaceIdentifier::NamedFace FaceIdentifier::storeNewFace(VCapture::Face face)
{
    std::string id = this->getRandomIdentifier(20);
    
    FaceStorage st;
    st.name = id;
    st.face[0] = face;
    this->storedFaces.push_back(st);
    
    NamedFace nf;
    nf.name = id;
    nf.face = face;
    return nf;
}


void FaceIdentifier::shiftFaces()
{
    
    for (size_t i = 0; i < this->storedFaces.size(); i++)
    {
        FaceStorage *face = &this->storedFaces.at(i);
        
        // Shift the array one position to the right
        // 255 simontanious heads is plenty enough
        ushort newPosition;
        for(newPosition = this->FACE_HISTORY_LENGTH-1; newPosition > 0;newPosition--)
        {
            face->face[newPosition]=face->face[newPosition - 1];
        }
        
        VCapture::Face newFace;
        face->face[newPosition] = newFace; // fill the gap.
        
        // Set first element as empty face
        VCapture::Face t;
        face->face[0] = t;
        
        
        // Remove face if it didn't occur in the last x frames
        if (this->identifiedFaceIsEmpty(*face))
        {
            
            this->storedFaces.erase(this->storedFaces.begin() + i);
        }
    }
}


bool FaceIdentifier::identifiedFaceIsEmpty(FaceStorage face)
{
    for (size_t i = 0; i < this->FACE_HISTORY_LENGTH; i++)
    {
        if (!this->faceIsEmpty(face.face[i]))
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
