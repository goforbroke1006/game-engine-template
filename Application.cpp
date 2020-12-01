//
// Created by GOFORBROKE on 30.11.2020.
//

#include "Application.h"
#include "render-type/DX11RenderTarget.h"
#include "render-type/OpenGLRenderTarget.h"

GEngTmp::Application *
GEngTmp::Application::create(int argc, char **argv, int w, int h, SupportedRenderTypes type, bool fullScreen) {
    auto *app = new Application();

    BaseRenderTarget *renderTarget;
    if (SupportedRenderTypes::DIRECTX11 == type) {
        renderTarget = DX11RenderTarget::create(w, h, !fullScreen);
    } else if (SupportedRenderTypes::OPENGL == type) {
        renderTarget = OpenGLRenderTarget::create(w, h, argc, argv);
    }

    app->setRenderTarget(renderTarget);
    return app;
}

GEngTmp::Application::Application() {}

GEngTmp::Application::~Application() {
    delete mRenderTarget;
    delete mScene;
}

void GEngTmp::Application::setRenderTarget(BaseRenderTarget *renderTarget) {
    mRenderTarget = renderTarget;
}

void GEngTmp::Application::loop() {
    mRenderTarget->loop(mScene);
}

GEngTmp::Scene *GEngTmp::Application::getScene() {
    return mScene;
}
