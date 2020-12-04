//
// Created by GOFORBROKE on 02.12.2020.
//

#include "Camera.h"

GEngTmp::Mesh *GEngTmp::Camera::mesh() const {
    return nullptr;
}

Vector3 &GEngTmp::Camera::lookAt() {
    return mLookAt;
}
