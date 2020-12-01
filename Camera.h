//
// Created by GOFORBROKE on 02.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_CAMERA_H
#define GAME_ENGINE_TEMPLATE_CAMERA_H

#include "Node.h"

namespace GEngTmp {
    class Camera : public Node {
    public:
        Mesh *mesh() const override;
    };
}

#endif //GAME_ENGINE_TEMPLATE_CAMERA_H
