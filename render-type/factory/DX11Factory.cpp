//
// Created by GOFORBROKE on 02.12.2020.
//

#include "DX11Factory.h"

#include <vector>
#include <stdexcept>
#include "../../def.h"
#include <d3dx9math.h>
#include <d3dcompiler.h>

const std::string shaderSourceStr = R"(

struct VOut { float4 position: SV_POSITION; float4 color: COLOR; };

VOut vs_shader(float4 position : POSITION, float4 color : COLOR) { VOut output; output.position = position; output.color = color; return output; }

float4 ps_shader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET { return color; }

)";

void DX11Factory::make(
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
) {


    std::vector<D3D_FEATURE_LEVEL> levels = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1,
    };

    D3D_FEATURE_LEVEL obtained_feature_level;
    D3D_DRIVER_TYPE obtained_driverType;

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = screenWidth;                    // set the back buffer width
    scd.BufferDesc.Height = screenHeight;                  // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = windowDescriptor;                   // the window to be used
    scd.SampleDesc.Count = 1;                              // how many multisamples
    scd.SampleDesc.Quality = 0;
    scd.Windowed = windowed;                               // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;

//    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//    scd.Flags = 0;


    const D3D_DRIVER_TYPE driverTypes[] =
            {
                    D3D_DRIVER_TYPE_HARDWARE,
                    D3D_DRIVER_TYPE_WARP,
                    D3D_DRIVER_TYPE_REFERENCE,
                    D3D_DRIVER_TYPE_SOFTWARE,
            };

    const D3D_FEATURE_LEVEL *featureLevelsArr = &levels[0];
    const int featureLevelsLen = levels.size();

    // create a device, device context and swap chain using the information in the scd struct

    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    bool createDeviceAndSwapChain = false;
    for (auto m_driverType : driverTypes) {
        HRESULT initResult = D3D11CreateDeviceAndSwapChain(
                nullptr,
                m_driverType,
                nullptr,
                createDeviceFlags,
                featureLevelsArr,
                featureLevelsLen,
                D3D11_SDK_VERSION,
                &scd,
                &swapChain,
                &device,
                &obtained_feature_level,
                &deviceContext
        );

        if (SUCCEEDED(initResult)) {
            obtained_driverType = m_driverType;
            createDeviceAndSwapChain = true;
            break;
        }
    }

    if (!createDeviceAndSwapChain) {
        throw std::logic_error("Initialize device failed");
    }

    Logger_Info("Initialize device: DirectX 11")
//    Logger_Info("Obtain driver type: " + driverTypeLabel(obtained_driverType))
//    Logger_Info("Obtain feature level: " + featureLevelLabel(obtained_feature_level));

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    HRES_LOG_ERR(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &pBackBuffer),
                 "get buffer failed");

    // use the back buffer address to create the render target
    HRES_LOG_ERR(device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView),
                 "create render target view failed");
    pBackBuffer->Release();
    pBackBuffer = nullptr;


    D3D11_TEXTURE2D_DESC depthBufferDesc;
    // Initialize the description of the depth buffer.
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = screenWidth;
    depthBufferDesc.Height = screenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    if (FAILED(device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer))) {
        throw std::runtime_error("can't init depth stencil buffer");
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    // Initialize the description of the stencil state.
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState))) {
        throw std::runtime_error("can't create depth stencil state");
    }
    // Set the depth stencil state.
    deviceContext->OMSetDepthStencilState(depthStencilState, 1);


    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    // Initialize the depth stencil view.
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    if (FAILED(device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView))) {
        throw std::runtime_error("can't create depth stencil view");
    }

    // set the render target as the back buffer
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);


    D3D11_RASTERIZER_DESC rasterDesc;
    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterizer state from the description we just filled out.
    if (FAILED(device->CreateRasterizerState(&rasterDesc, &rasterState))) {
        throw std::runtime_error("can't create rasterizer state");
    }

    // Now set the rasterizer state.
    deviceContext->RSSetState(rasterState);




    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float) screenWidth;
    viewport.Height = (float) screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    deviceContext->RSSetViewports(1, &viewport);

    {
        ID3D10Blob *VS, *PS;

        D3DCompile(
                shaderSourceStr.c_str(), shaderSourceStr.length(), nullptr, nullptr, nullptr,
                "vs_shader", "vs_4_0", 0, 0, &VS, nullptr
        );
        D3DCompile(
                shaderSourceStr.c_str(), shaderSourceStr.length(), nullptr, nullptr, nullptr,
                "ps_shader", "ps_4_0", 0, 0, &PS, nullptr
        );

        ID3D11VertexShader *vertexShader;
        ID3D11PixelShader *pixelShader;

        // encapsulate both shaders into shader objects
        device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &vertexShader);
        device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pixelShader);

        // set the shader objects
        deviceContext->VSSetShader(vertexShader, 0, 0);
        deviceContext->PSSetShader(pixelShader, 0, 0);

        // create the input layout object
        D3D11_INPUT_ELEMENT_DESC ied[] =
                {
                        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
                        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
                };

        device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &inputLayout);
        deviceContext->IASetInputLayout(inputLayout);
    }


    float fieldOfView, screenAspect;
    // Setup the projection matrix.
    fieldOfView = (float) D3DX_PI / 4.0f;
    screenAspect = (float) screenWidth / (float) screenHeight;

    // Create the projection matrix for 3D rendering.
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

    // Initialize the world matrix to the identity matrix.
    D3DXMatrixIdentity(&worldMatrix);

    // Create an orthographic projection matrix for 2D rendering.
    D3DXMatrixOrthoLH(&orthoMatrix, (float) screenWidth, (float) screenHeight, screenNear, screenDepth);
}
