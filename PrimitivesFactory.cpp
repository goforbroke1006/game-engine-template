//
// Created by GOFORBROKE on 01.12.2020.
//

#include "PrimitivesFactory.h"
#include "domain.h"

#include <vector>

GEngTmp::Mesh *GEngTmp::PrimitivesFactory::triangle() {
    Mesh *mesh
            = new Mesh(
                    {
                            {0.0f,   0.5f,  0.5f, {255, 0, 0,   255}},
                            {0.45f,  -0.5,  0.5f, {0,   255,  0,   255}},
                            {-0.45f, -0.5f, 0.5f, {0,   0,    255, 255}}
                    }
            );
    return mesh;
}

GEngTmp::Mesh *GEngTmp::PrimitivesFactory::cube(const double size) {
    std::vector<Vertex> vertices = {
            {-1.0f, -1.0f, -1.0f, {191, 114, 114, 255}},
            {-1.0f, -1.0f, 1.0f,  {114, 191, 165, 255}},
            {-1.0f, 1.0f,  1.0f,  {165, 165, 191, 255}},
            {1.0f,  1.0f,  -1.0f, {191, 114, 114, 255}},
            {-1.0f, -1.0f, -1.0f, {114, 191, 165, 255}},
            {-1.0f, 1.0f,  -1.0f, {165, 165, 191, 255}},
            {1.0f,  -1.0f, 1.0f,  {191, 114, 114, 255}},
            {-1.0f, -1.0f, -1.0f, {114, 191, 165, 255}},
            {1.0f,  -1.0f, -1.0f, {165, 165, 191, 255}},
            {1.0f,  1.0f,  -1.0f, {191, 114, 114, 255}},
            {1.0f,  -1.0f, -1.0f, {114, 191, 165, 255}},
            {-1.0f, -1.0f, -1.0f, {165, 165, 191, 255}},
            {-1.0f, -1.0f, -1.0f, {191, 114, 114, 255}},
            {-1.0f, 1.0f,  1.0f,  {114, 191, 165, 255}},
            {-1.0f, 1.0f,  -1.0f, {165, 165, 191, 255}},
            {1.0f,  -1.0f, 1.0f,  {191, 114, 114, 255}},
            {-1.0f, -1.0f, 1.0f,  {114, 191, 165, 255}},
            {-1.0f, -1.0f, -1.0f, {165, 165, 191, 255}},
            {-1.0f, 1.0f,  1.0f,  {191, 114, 114, 255}},
            {-1.0f, -1.0f, 1.0f,  {114, 191, 165, 255}},
            {1.0f,  -1.0f, 1.0f,  {165, 165, 191, 255}},
            {1.0f,  1.0f,  1.0f,  {191, 114, 114, 255}},
            {1.0f,  -1.0f, -1.0f, {114, 191, 165, 255}},
            {1.0f,  1.0f,  -1.0f, {165, 165, 191, 255}},
            {1.0f,  -1.0f, -1.0f, {191, 114, 114, 255}},
            {1.0f,  1.0f,  1.0f,  {114, 191, 165, 255}},
            {1.0f,  -1.0f, 1.0f,  {165, 165, 191, 255}},
            {1.0f,  1.0f,  1.0f,  {191, 114, 114, 255}},
            {1.0f,  1.0f,  -1.0f, {114, 191, 165, 255}},
            {-1.0f, 1.0f,  -1.0f, {165, 165, 191, 255}},
            {1.0f,  1.0f,  1.0f,  {191, 114, 114, 255}},
            {-1.0f, 1.0f,  -1.0f, {114, 191, 165, 255}},
            {-1.0f, 1.0f,  1.0f,  {165, 165, 191, 255}},
            {1.0f,  1.0f,  1.0f,  {191, 114, 114, 255}},
            {-1.0f, 1.0f,  1.0f,  {114, 191, 165, 255}},
            {1.0f,  -1.0f, 1.0f,  {165, 165, 191, 255}},
    };

    double modifier = size / 2;
    for (auto &v : vertices) {
        v.position.x *= modifier;
        v.position.y *= modifier;
        v.position.z *= modifier;
    }

    return new Mesh(vertices);
}
