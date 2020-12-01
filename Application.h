//
// Created by GOFORBROKE on 30.11.2020.
//

#ifndef RENDERSYSTEM_APPLICATION_H
#define RENDERSYSTEM_APPLICATION_H

#include "BaseRenderTarget.h"
#include "Scene.h"
#include "domain.h"

namespace GEngTmp {
    class Application {
    public:
        static Application *
        create(int argc, char **argv, int w, int h, SupportedRenderTypes type, bool fullScreen);

        ~Application();

        void loop();

        Scene *getScene();

    private:
        explicit Application();

        BaseRenderTarget *mRenderTarget = nullptr;
        Scene *mScene = new Scene();

        void setRenderTarget(BaseRenderTarget *renderTarget);
    };
}


#endif //RENDERSYSTEM_APPLICATION_H
