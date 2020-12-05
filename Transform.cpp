//
// Created by GOFORBROKE on 30.11.2020.
//

#include "Transform.h"

GEngTmp::Transform::Transform() :
        mPosition({0.0, 0.0, 0.0}),
        mRotation({0.0, 0.0, 0.0, 1.0}),
        mScale({1.0, 1.0, 1.0}) {}

Vector3 &GEngTmp::Transform::position() {
    return mPosition;
}

Vector4 &GEngTmp::Transform::rotation() {
    return mRotation;
}

Vector3 &GEngTmp::Transform::scale() {
    return mScale;
}
