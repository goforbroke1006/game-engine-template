//
// Created by GOFORBROKE on 01.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_OPENGLRENDERTARGET_H
#define GAME_ENGINE_TEMPLATE_OPENGLRENDERTARGET_H


#include "../BaseRenderTarget.h"

class OpenGLRenderTarget : public BaseRenderTarget {
public:
    void Release() override;

    void loop(GEngTmp::Scene *scene) override;

    static BaseRenderTarget *create(int w, int h, int argc, char **argv);

private:
    OpenGLRenderTarget(int w, int h);

    static void display();
};


#endif //GAME_ENGINE_TEMPLATE_OPENGLRENDERTARGET_H
