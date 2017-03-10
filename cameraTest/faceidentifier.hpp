//
//  faceidentifier.hpp
//  cameraTest
//
//  Created by Mark Nijboer on 09/03/2017.
//  Copyright © 2017 Mark Nijboer. All rights reserved.
//

#ifndef faceidentifier_hpp
#define faceidentifier_hpp

#include <stdio.h>
#include "capture.hpp"

class FaceIdentifier
{
public:
    struct NamedFace {
        std::string name;
        struct VCapture::Face face;
    };
    
private:
    static const int FACE_HISTORY_LENGTH = 6;
    static const int THRESHOLD_PIXELS = 30;
    struct FaceStorage {
        std::string name;
        struct VCapture::Face face[FACE_HISTORY_LENGTH];
    };
    
    std::vector<FaceStorage> storedFaces;
    
    void shiftFaces();
    bool faceIsEmpty(VCapture::Face);
    bool identifiedFaceIsEmpty(FaceStorage face);
    std::string getRandomIdentifier(int size);
    FaceIdentifier::NamedFace storeNewFace(VCapture::Face face);
    
public:
    std::vector<NamedFace> identifyFaces(std::vector<VCapture::Face> faces);
    
};

#endif /* faceidentifier_hpp */
