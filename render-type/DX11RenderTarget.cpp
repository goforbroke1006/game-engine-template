//
// Created by GOFORBROKE on 01.12.2020.
//

#include "DX11RenderTarget.h"

#include "../def.h"
#include "../PrimitivesFactory.h"
#include "factory/DX11Factory.h"

#include <string>
#include <vector>
#include <stdexcept>

#include <Windows.h>
#include <D3D11.h>
#include <D3Dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

BaseRenderTarget *
DX11RenderTarget::create(UINT screenWidth, UINT screenHeight, bool windowed) {
    HWND hwnd = createWin32Window(screenWidth, screenHeight);


    IDXGISwapChain *swapChain;
    ID3D11Device *device;
    ID3D11DeviceContext *deviceContext;
    ID3D11RenderTargetView *renderTargetView;
    ID3D11Texture2D *depthStencilBuffer;
    ID3D11DepthStencilState *depthStencilState;
    ID3D11DepthStencilView *depthStencilView;
    ID3D11RasterizerState *rasterState;
    ID3D11SamplerState *samplerState;
    ID3D11InputLayout *inputLayout;

    DirectX::XMMATRIX mWorldMatrix = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX mViewMatrix = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX mProjectionMatrix = DirectX::XMMatrixIdentity();

    ID3D11Buffer *constantBuffer;
    ID3D11VertexShader *pVS;
    ID3D11PixelShader *pPS;

    DX11Factory::make(
            hwnd,
            screenWidth, screenHeight, windowed,
            1000.0f, 0.1f,
            //
            swapChain,
            device,
            deviceContext,
            renderTargetView,
            depthStencilBuffer,
            depthStencilState,
            depthStencilView,
            rasterState,
            samplerState,
            mWorldMatrix,
            mViewMatrix,
            mProjectionMatrix,
            inputLayout,
            constantBuffer,
            pVS,
            pPS
    );

    return new DX11RenderTarget(
            screenWidth, screenHeight,
            swapChain, device, deviceContext, renderTargetView,
            depthStencilBuffer, depthStencilState, depthStencilView,
            rasterState,
            samplerState,
            mWorldMatrix, mViewMatrix, mProjectionMatrix,
            constantBuffer,
            pVS, pPS
    );
}

HWND DX11RenderTarget::createWin32Window(int w, int h) {
    // Register the window class.
    const char *CLASS_NAME = "Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            CLASS_NAME,                     // Window class
            "Learn to Program Windows",    // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, w, h,

            NULL,         // Parent window
            NULL,         // Menu
            wc.hInstance, // Instance handle
            NULL          // Additional application data
    );

    if (hwnd == NULL) {
        throw std::runtime_error("can't create window");
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    return hwnd;
}

LRESULT DX11RenderTarget::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: ImGUI integration

    switch (message) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

DX11RenderTarget::DX11RenderTarget(
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
        ID3D11Buffer *constantBuffer,
        ID3D11VertexShader *pVS,
        ID3D11PixelShader *pPS
) : BaseRenderTarget(screenWidth, screenHeight),
    mSwapChain(swapChain),
    mDevice(device),
    mDeviceContext(deviceContext),
    mRenderTargetView(renderTargetView),
    mDepthStencilBuffer(depthStencilBuffer),
    mDepthStencilState(depthStencilState),
    mDepthStencilView(depthStencilView),
    mRasterState(rasterState),
    mSamplerState(samplerState),
    mWorld(m_World),
    mView(m_View),
    mProjection(m_Projection),
    mConstantBuffer(constantBuffer),
    pVertexShader(pVS), pPixelShader(pPS) {}

void DX11RenderTarget::Release() {
    // switch to windowed mode
    mSwapChain->SetFullscreenState(false, nullptr);

    // close and release all existing COM objects
//    mInputLayout->Release();
//    mInputLayout = nullptr;

    mSwapChain->Release();
    mSwapChain = nullptr;

    mRenderTargetView->Release();
    mRenderTargetView = nullptr;

    mDevice->Release();
    mDevice = nullptr;

    mDeviceContext->Release();
    mDeviceContext = nullptr;

    mConstantBuffer->Release();
    mConstantBuffer = nullptr;
}

void DX11RenderTarget::loop(GEngTmp::Scene *scene) {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // begin scene
        FLOAT c[] = {0.2f, 0.5f, 0.7f, 1.0f};
        mDeviceContext->ClearRenderTargetView(mRenderTargetView, c);
        mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        // set view matrix with camera
        GEngTmp::Camera *pCamera = scene->camera();
        {
            DirectX::XMVECTOR Eye = DirectX::XMVectorSet(
                    (float) pCamera->transform().position().x,
                    (float) pCamera->transform().position().y,
                    (float) pCamera->transform().position().z,
                    0.0f);
            DirectX::XMVECTOR At = DirectX::XMVectorSet(
                    (float) pCamera->lookAt().x,
                    (float) pCamera->lookAt().y,
                    (float) pCamera->lookAt().z,
                    0.0f);
            DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            mView = DirectX::XMMatrixLookAtLH(Eye, At, Up);

            mProjection = DirectX::XMMatrixPerspectiveFovLH(
                    DirectX::XM_PIDIV4,
                    (float) mScreenWidth / (float) mScreenHeight, 0.01f, 1000.0f);
        }
        /*{
            DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
            DirectX::XMVECTOR position = DirectX::XMVectorSet(
                    (float) pCamera->transform().position().x,
                    (float) pCamera->transform().position().y,
                    (float) pCamera->transform().position().z,
                    1.0f
            );
            DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(
                    (float) pCamera->lookAt().x,
                    (float) pCamera->lookAt().y,
                    (float) pCamera->lookAt().z,
                    1.0f
            );

            float yaw, pitch, roll;
            DirectX::XMMATRIX rotationMatrix;

            // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
            pitch = (float) pCamera->transform().rotation().x * 0.0174532925f;
            yaw = (float) pCamera->transform().rotation().y * 0.0174532925f;
            roll = (float) pCamera->transform().rotation().z * 0.0174532925f;

            // Create the rotation matrix from the yaw, pitch, and roll values.
            rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(yaw, pitch, roll);

            // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
            lookAt = DirectX::XMVector3TransformCoord(lookAt, rotationMatrix);
            up = DirectX::XMVector3TransformCoord(up, rotationMatrix);

            // Translate the rotated camera position to the location of the viewer.
//            lookAt = position + lookAt;// TODO

            // Finally create the view matrix from the three updated vectors.
            mViewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
        }*/


        {
            for (auto &nn : scene->nodes()) { // TODO: draw frame here
                GEngTmp::Node *node = nn.second;
                for (auto &b : scene->behaviors()[node]) {
                    b(node);
                }

                GEngTmp::Mesh *mesh = node->mesh();

                if (nullptr == mesh) {
                    // empty node
                    continue;
                }

                if (nullptr == mMeshBuffers[mesh]) {
                    size_t bytesWidth = sizeof(DxVERTEX) * mesh->size();

                    D3D11_BUFFER_DESC bd;
                    ZeroMemory(&bd, sizeof(bd));
                    bd.Usage = D3D11_USAGE_DYNAMIC;
                    bd.ByteWidth = bytesWidth;
                    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

                    ID3D11Buffer *verticesBuffer = nullptr;
                    if (FAILED(mDevice->CreateBuffer(&bd, nullptr, &verticesBuffer))) {
                        throw std::runtime_error("device can't create buffer");
                    }

                    auto *dxVertices = new DxVERTEX[mesh->size()];
                    for (size_t vi = 0; vi < mesh->size(); ++vi) {
                        dxVertices[vi].pos = DirectX::XMVectorSet(
                                (float) mesh->vertices()[vi].position.x,
                                (float) mesh->vertices()[vi].position.y,
                                (float) mesh->vertices()[vi].position.z,
                                1.0f
                        );
                        dxVertices[vi].color = DirectX::XMVectorSet(
                                (float) mesh->vertices()[vi].color.green / 255.0f,
                                (float) mesh->vertices()[vi].color.blue / 255.0f,
                                (float) mesh->vertices()[vi].color.alpha / 255.0f,
                                (float) mesh->vertices()[vi].color.red / 255.0f
                        );
                    }

//                    DxVERTEX *dxVertices = &mVertexCache[mesh][0];

                    // copy the vertices into the buffer
                    D3D11_MAPPED_SUBRESOURCE ms;
                    if (FAILED(mDeviceContext->Map(verticesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms))) {
                        throw std::runtime_error("device context can't map vertex buffer");
                    }
                    memcpy(ms.pData, dxVertices, bytesWidth);
                    mDeviceContext->Unmap(verticesBuffer, 0);

                    mMeshBuffers[mesh] = verticesBuffer;
                }

                mWorld = DirectX::XMMatrixIdentity();

                mWorld *= DirectX::XMMatrixRotationX((float) node->transform().rotation().x);
                mWorld *= DirectX::XMMatrixRotationY((float) node->transform().rotation().y);
                mWorld *= DirectX::XMMatrixRotationZ((float) node->transform().rotation().z);

                mWorld *= DirectX::XMMatrixScaling(
                        (float) node->transform().scale().x,
                        (float) node->transform().scale().y,
                        (float) node->transform().scale().z
                );

                mWorld *= DirectX::XMMatrixTranslation(
                        (float) node->transform().position().x,
                        (float) node->transform().position().y,
                        (float) node->transform().position().z
                );

                ConstantBuffer cb{};
                cb.mWorld = XMMatrixTranspose(mWorld);
                cb.mView = XMMatrixTranspose(mView);
                cb.mProjection = XMMatrixTranspose(mProjection);
                mDeviceContext->UpdateSubresource(mConstantBuffer, 0, nullptr, &cb, 0, 0);

                mDeviceContext->VSSetShader(pVertexShader, 0, 0);
                mDeviceContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);
                mDeviceContext->PSSetShader(pPixelShader, 0, 0);
                mDeviceContext->PSSetSamplers(0, 1, &mSamplerState);

                ID3D11Buffer *verticesBuffer = mMeshBuffers[mesh];
                UINT stride = sizeof(DxVERTEX);
                UINT offset = 0;
                mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                mDeviceContext->IASetVertexBuffers(0, 1, &verticesBuffer, &stride, &offset);
                mDeviceContext->Draw(mesh->size(), 0);
                mMeshBuffers[mesh] = verticesBuffer;
            }

            //Set the WVP matrix and send it to the constant buffer in effect file
//            WVP = cube1World * camView * camProjection;
//            cbPerObj.WVP = XMMatrixTranspose(WVP);
//            mDeviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//            mDeviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

            HRES_LOG_WARN(mSwapChain->Present(1, 0), "can't render frame");
        }
    }
}
