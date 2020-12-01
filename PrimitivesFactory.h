//
// Created by GOFORBROKE on 01.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_PRIMITIVESFACTORY_H
#define GAME_ENGINE_TEMPLATE_PRIMITIVESFACTORY_H


#include "Mesh.h"

namespace GEngTmp {
    class PrimitivesFactory {
    public:
        static GEngTmp::Mesh *triangle();

        static GEngTmp::Mesh *cube(const double size);
    };
}

#endif //GAME_ENGINE_TEMPLATE_PRIMITIVESFACTORY_H
