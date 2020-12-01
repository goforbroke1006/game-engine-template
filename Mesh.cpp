//
// Created by GOFORBROKE on 30.11.2020.
//

#include "Mesh.h"

GEngTmp::Mesh::Mesh(std::vector<Vertex> vertices)
        : mVertices(std::move(vertices)) {}

size_t GEngTmp::Mesh::size() const {
    return mVertices.size();
}

std::vector<Vertex> &GEngTmp::Mesh::vertices() {
    return mVertices;
}
