//
// Created by GOFORBROKE on 02.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_UTILS_H
#define GAME_ENGINE_TEMPLATE_UTILS_H

#include <cstring>
#include "domain.h"

inline
SupportedRenderTypes detectRenderType(int argc, char *const *argv) {
    SupportedRenderTypes type = ANY;
    if (argc >= 2) {
        if (strcmp(argv[1], "dx11") == 0) type = DIRECTX11;
        if (strcmp(argv[1], "opengl") == 0) type = OPENGL;
    }

    if (ANY == type) {
#if defined(WIN32)
        type = DIRECTX11;
#elif defined(__LINUX__) || defined(__GNUC__)
        type = SupportedRenderTypes::OPENGL;
#endif
    }
    return type;
}

#endif //GAME_ENGINE_TEMPLATE_UTILS_H
