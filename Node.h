//
// Created by GOFORBROKE on 30.11.2020.
//

#ifndef RENDERSYSTEM_NODE_H
#define RENDERSYSTEM_NODE_H


#include "Mesh.h"
#include "Transform.h"

namespace GEngTmp {
    class Node {
    public:
        ~Node();

        void setMesh(GEngTmp::Mesh *pMesh);

        virtual Mesh *mesh() const;

        Transform &transform();

//        void OnUpdate(void (*pFunction)(Node *));

    protected:
        Transform mTransform;
        Mesh *mMesh;
    };
}


#endif //RENDERSYSTEM_NODE_H
