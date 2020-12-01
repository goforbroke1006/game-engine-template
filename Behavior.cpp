//
// Created by GOFORBROKE on 02.12.2020.
//

#include "Behavior.h"

void GEngTmp::Behavior::OnUpdate(void (*pFunction)()) {
    mOnUpdateCB = pFunction;
}

void GEngTmp::Behavior::update(Node * node) {
    mOnUpdateCB();
}