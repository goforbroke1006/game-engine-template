//
// Created by GOFORBROKE on 01.12.2020.
//

#ifndef GAME_ENGINE_TEMPLATE_DX11RENDERTARGET_H
#define GAME_ENGINE_TEMPLATE_DX11RENDERTARGET_H


#include "../BaseRenderTarget.h"

#include <D3D11.h>
#include <DirectXMath.h>
#include <map>
#include "../Mesh.h"

struct ConstantBuffer {
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;
};

struct DxVERTEX {
    DirectX::XMVECTOR pos;
    DirectX::XMVECTOR color;
//    DirectX::XMFLOAT4 pos;
//    DirectX::XMFLOAT4 color;
//    float pos[3];
//    float color[4];
};

class DX11RenderTarget : public BaseRenderTarget {
public:
    static BaseRenderTarget *
    create(UINT screenWidth, UINT screenHeight, bool windowed);

    void Release() override;

    void loop(GEngTmp::Scene *scene) override;

private:
    static HWND createWin32Window(int w, int h);

    static LRESULT CALLBACK
    WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    DX11RenderTarget(
            int screenWidth, int screenHeight,
            IDXGISwapChain *swapChain,
            ID3D11Device *device,
            ID3D11DeviceContext *deviceContext,
            ID3D11RenderTargetView *renderTargetView,
            ID3D11Texture2D *depthStencilBuffer,
            ID3D11DepthStencilState *depthStencilState,
            ID3D11DepthStencilView *depthStencilView,
            ID3D11RasterizerState *rasterState,
            ID3D11SamplerState *samplerState,
            DirectX::XMMATRIX m_World,
            DirectX::XMMATRIX m_View,
            DirectX::XMMATRIX m_Projection,
            ID3D11Buffer *mConstantBuffer,
            ID3D11VertexShader *pVS,
            ID3D11PixelShader *pPS
    );

    IDXGISwapChain *mSwapChain;
    ID3D11Device *mDevice;
    ID3D11DeviceContext *mDeviceContext;
    ID3D11RenderTargetView *mRenderTargetView;
    ID3D11Texture2D *mDepthStencilBuffer;
    ID3D11DepthStencilState *mDepthStencilState;
    ID3D11DepthStencilView *mDepthStencilView;
    ID3D11RasterizerState *mRasterState;
    ID3D11SamplerState *mSamplerState;

    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;

    ID3D11Buffer *mConstantBuffer = nullptr;

    ID3D11VertexShader *pVertexShader = nullptr;
    ID3D11PixelShader *pPixelShader = nullptr;

//    std::map<const GEngTmp::Mesh *const, std::vector<DxVERTEX>> mVertexCache;
    std::map<const GEngTmp::Mesh *const, ID3D11Buffer *> mMeshBuffers;
};


#endif //GAME_ENGINE_TEMPLATE_DX11RENDERTARGET_H
