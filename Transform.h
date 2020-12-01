//
// Created by GOFORBROKE on 30.11.2020.
//

#ifndef RENDERSYSTEM_TRANSFORM_H
#define RENDERSYSTEM_TRANSFORM_H

#include "domain.h"

namespace GEngTmp {
    class Transform {
    public:
         Vector3 &position();

         Vector4 &rotation();

        Vector3 &scale();

    private:
        Vector3 mPosition;
        Vector4 mRotation;
        Vector3 mScale;
    };
}


#endif //RENDERSYSTEM_TRANSFORM_H
