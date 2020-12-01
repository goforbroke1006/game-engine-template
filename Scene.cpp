//
// Created by GOFORBROKE on 30.11.2020.
//

#include "Scene.h"


GEngTmp::Scene::~Scene() {
    for (auto &pNode : mNodes) {
        delete pNode.second;
    }
}

GEngTmp::Node *GEngTmp::Scene::createNode(const std::string &name) {
    Node *pNode = new Node();
    mNodes[name] = pNode;
    return pNode;
}

std::map<std::string, GEngTmp::Node *> &GEngTmp::Scene::nodes() {
    return mNodes;
}

std::map<GEngTmp::Node *, std::vector<NodeBehavior>> & GEngTmp::Scene::behaviors() {
    return mBehaviors;
}

GEngTmp::Camera *GEngTmp::Scene::camera() {
    if (nullptr == mCamera) {
        mCamera = new Camera;
        mNodes["camera"] = mCamera;
    }

    return mCamera;
}

void GEngTmp::Scene::createBehavior(GEngTmp::Node *pNode, void (*pFunction)(Node *const)) {
    mBehaviors[pNode].push_back(pFunction);
}
