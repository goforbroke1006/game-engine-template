//
// Created by GOFORBROKE on 30.11.2020.
//

#include "utils.h"
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

    {
        Node *pNode = app->getScene()->createNode("cube-1");
        pNode->setMesh(PrimitivesFactory::cube(0.8));
        app->getScene()->createBehavior(pNode, [](Node *const self) {
            static float rotation = 0.0f;
            rotation += (float) 3.14f * 0.01f;
            if (rotation > 360.0f) {
                rotation -= 360.0f;
            }
            self->transform().rotation().y = rotation;

            std::cout << "Rotation Y = " << rotation << std::endl;
        });
    }
    {
        Node *pNode = app->getScene()->createNode("cube-2");
        pNode->setMesh(PrimitivesFactory::cube(0.5));
        pNode->transform().position().x = 1;
        app->getScene()->createBehavior(pNode, [](Node *const self) {
            static float rotation = 0.0f;
            rotation -= (float) 3.14f * 0.03f;
            if (rotation < 0) {
                rotation += 360.0f;
            }
            self->transform().rotation().y = rotation;

            std::cout << "Rotation Y = " << rotation << std::endl;
        });
    }

    app->getScene()->camera()->transform().position() = Vector3{3.0, 3.0, 3.0};
    app->getScene()->camera()->lookAt() = Vector3::zero();

    app->loop();
    return 0;
}
