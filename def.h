//
// Created by GOFORBROKE on 01.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_DEF_H
#define GAME_ENGINE_TEMPLATE_DEF_H

#include <iostream>

#define Logger_Info(message)  { std::cout << "[I] " << message << std::endl; }
#define Logger_Warn(message)  { std::cerr << "[W] " << message << std::endl; }
#define Logger_Error(message) { std::cerr << "[E] " << message << std::endl; }

// ==========

#include <windows.h>
#include <string>
#include <DxErr.h>

#define HRES_LOG_ERR_D(HrFuncCall) { \
    HRESULT hres = HrFuncCall; \
    if(FAILED(hres)) { \
        Logger_Error(std::string(DXGetErrorString(hres)) + " : " +DXGetErrorDescription(hres)); \
    } \
}

#define HRES_LOG_ERR(HrFuncCall, message) { \
    HRESULT hres = HrFuncCall; \
    if(FAILED(hres)) { \
        Logger_Error(std::string(message) + " : " + DXGetErrorString(hres) + " : " +DXGetErrorDescription(hres)); \
    } \
}

#define HRES_LOG_WARN(HrFuncCall, message) { \
    HRESULT hres = HrFuncCall; \
    if(FAILED(hres)) { \
        Logger_Warn(std::string(message) + " : " + DXGetErrorString(hres) + " : " +DXGetErrorDescription(hres)); \
    } \
}

#endif //GAME_ENGINE_TEMPLATE_DEF_H
