//
// Created by GOFORBROKE on 30.11.2020.
//

#include "Node.h"

GEngTmp::Node::~Node() {
    delete mMesh;
}

void GEngTmp::Node::setMesh(GEngTmp::Mesh *pMesh) {
    mMesh = pMesh;
}

GEngTmp::Mesh *GEngTmp::Node::mesh() const {
    return mMesh;
}

GEngTmp::Transform GEngTmp::Node::transform() {
    return mTransform;
}


