//
// Created by GOFORBROKE on 30.11.2020.
//

#ifndef RENDERSYSTEM_DOMAIN_H
#define RENDERSYSTEM_DOMAIN_H

enum SupportedRenderTypes {
    ANY,
    DIRECTX11,
    OPENGL,
};

struct Vector2 {
    double x, y;
};

struct Vector3 {
    double x, y, z;

    static Vector3 zero() {
        return Vector3{0.0f, 0.0f, 0.0f};
    }
};

struct Vector4 {
    double x, y, z, w;
};

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

struct Vertex {
    Vector3 position;
    Color color;
};

#endif //RENDERSYSTEM_DOMAIN_H
