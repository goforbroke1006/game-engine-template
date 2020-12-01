//
// Created by GOFORBROKE on 30.11.2020.
//

#include "utils.h"
#include <d3dx9math.h>
#include <iostream>
#include "Application.h"
#include "PrimitivesFactory.h"

using namespace GEngTmp;

int main(int argc, char **argv) {
    static const int screenW = 800;
    static const int screenH = 600;

    SupportedRenderTypes type = detectRenderType(argc, argv);

    Application *app = Application::create(
            argc, argv,
            screenW, screenH,
            type, false);

    Node *pNode = app->getScene()->createNode("obj1");
    pNode->setMesh(PrimitivesFactory::triangle());
//    pNode->setMesh(PrimitivesFactory::cube(0.5));

    app->getScene()->createBehavior(pNode, [](Node *const self) {
        static float rotation = 0.0f;
        rotation += (float) D3DX_PI * 0.01f;
        if (rotation > 360.0f) {
            rotation -= 360.0f;
        }
        self->transform().rotation().y = rotation;

        std::cout << "Rotation Y = " << rotation << std::endl;
    });

    app->loop();
    return 0;
}
