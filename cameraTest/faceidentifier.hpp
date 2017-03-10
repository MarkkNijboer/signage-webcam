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
#include "VCapture.hpp"

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
    
    static std::vector<FaceStorage> storedFaces;
    
    static void shiftFaces();
    static bool faceIsEmpty(VCapture::Face);
    static bool identifiedFaceIsEmpty(FaceStorage face);
    static std::string getRandomIdentifier(int size);
    
public:
    static void init();
    static std::vector<NamedFace> identifyFaces(std::vector<VCapture::Face> faces);
    
};

#endif /* faceidentifier_hpp */
