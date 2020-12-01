//
// Created by GOFORBROKE on 02.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_DX11FACTORY_H
#define GAME_ENGINE_TEMPLATE_DX11FACTORY_H

#include <D3D11.h>
#include <DirectXMath.h>
#include <d3dx9math.h>
#include "../DX11RenderTarget.h"

class DX11Factory {
public:
    static void make(
            HWND windowDescriptor,
            UINT screenWidth, UINT screenHeight, bool windowed,
            float screenDepth, float screenNear,
            //
            IDXGISwapChain *&swapChain,
            ID3D11Device *&device,
            ID3D11DeviceContext *&deviceContext,
            ID3D11RenderTargetView *&renderTargetView,
            ID3D11Texture2D *&depthStencilBuffer,
            ID3D11DepthStencilState *&depthStencilState,
            ID3D11DepthStencilView *&depthStencilView,
            ID3D11RasterizerState *&rasterState,
            D3DXMATRIX &projectionMatrix,
            D3DXMATRIX &worldMatrix,
            D3DXMATRIX &orthoMatrix,

            ID3D11InputLayout *&inputLayout
    );
};


#endif //GAME_ENGINE_TEMPLATE_DX11FACTORY_H
