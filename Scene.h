//
// Created by GOFORBROKE on 30.11.2020.
//

#ifndef RENDERSYSTEM_SCENE_H
#define RENDERSYSTEM_SCENE_H


#include <string>
#include <map>
#include "Node.h"
#include "Camera.h"

typedef void (*NodeBehavior)(GEngTmp::Node *const);

namespace GEngTmp {
    class Scene {

    public:
        ~Scene();

        Node *createNode(const std::string &name);

        std::map<std::string, Node *> &nodes() ;

        std::map<Node *, std::vector<NodeBehavior>> &behaviors() ;

        Camera *camera();

        void createBehavior(Node *pNode, NodeBehavior);

    private:
        std::map<std::string, Node *> mNodes;
        Camera *mCamera = nullptr;
        std::map<Node *, std::vector<NodeBehavior>> mBehaviors;
    };
}


#endif //RENDERSYSTEM_SCENE_H
