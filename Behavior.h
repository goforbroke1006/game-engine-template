//
// Created by GOFORBROKE on 02.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_BEHAVIOR_H
#define GAME_ENGINE_TEMPLATE_BEHAVIOR_H


#include "Node.h"

namespace GEngTmp {
    class Behavior {
    public:
        void update(Node *node);

        void OnUpdate(void (*pFunction)());

    private:
        void (*mOnUpdateCB)();
    };
}

#endif //GAME_ENGINE_TEMPLATE_BEHAVIOR_H
