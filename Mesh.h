//
// Created by GOFORBROKE on 30.11.2020.
//

#ifndef RENDERSYSTEM_MESH_H
#define RENDERSYSTEM_MESH_H

#include <utility>
#include <vector>
#include "domain.h"

namespace GEngTmp {
    class Mesh {
    public:
        explicit Mesh(std::vector<Vertex> vertices);

        size_t size() const;

        std::vector<Vertex> &vertices();

    private:
        std::vector<Vertex> mVertices;
    };
}


#endif //RENDERSYSTEM_MESH_H
