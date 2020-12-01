//
// Created by GOFORBROKE on 01.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_BASERENDERTARGET_H
#define GAME_ENGINE_TEMPLATE_BASERENDERTARGET_H


#include "Scene.h"

class BaseRenderTarget {
public:
    BaseRenderTarget(int w, int h);

    virtual void Release();

    virtual void loop(GEngTmp::Scene *scene);

protected:
    int mScreenWidth;
    int mScreenHeight;
};


#endif //GAME_ENGINE_TEMPLATE_BASERENDERTARGET_H
