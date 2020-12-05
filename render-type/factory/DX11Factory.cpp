//
// Created by GOFORBROKE on 02.12.2020.
//

#include "DX11Factory.h"

#include <vector>
#include <stdexcept>
#include "../../def.h"
#include <d3dcompiler.h>

const std::string shaderSourceStr = R"(

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VOut {
    float4 position: SV_POSITION;
    float4 color: COLOR;
};

VOut vs_shader(float4 position : POSITION, float4 color : COLOR) {
    VOut output;
    //output.position = position;
    output.position = mul(position, World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);
    output.color = color;
    return output;
}

float4 ps_shader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET {
    return color;
}

)";

/*const std::string shader2SourceStr = R"(

struct VOut { float4 position: SV_POSITION; float4 color: COLOR; };

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

VOut vs_shader(float4 position : POSITION, float4 color : COLOR) {
    VOut output;

    // Change the position vector to be 4 units for proper matrix calculations.
    position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.color = color;

    return output;
}

float4 ps_shader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET { return color; }

)";*/

const std::string lightShaderVertexSourceStr = R"(

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.color = input.color;

    return output;
}

)";

const std::string lightShaderPixelSourceStr = R"(
    struct PixelInputType
    {
        float4 position : SV_POSITION;
        float4 color : COLOR;
        float3 normal : NORMAL;
    };

    float4 LightPixelShader(PixelInputType input) : SV_TARGET
    {
        return input.color;
    }
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
        ID3D11SamplerState *&samplerState,
        DirectX::XMMATRIX &mWorldMatrix,
        DirectX::XMMATRIX &mViewMatrix,
        DirectX::XMMATRIX &mProjectionMatrix,
        ID3D11InputLayout *&inputLayout,
        ID3D11Buffer *&constantBuffer,
        ID3D11VertexShader *&pVS,
        ID3D11PixelShader *&pPS
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

        if (FAILED(D3DCompile(
                shaderSourceStr.c_str(), shaderSourceStr.length(), nullptr, nullptr, nullptr,
                "vs_shader", "vs_4_0", 0, 0, &VS, nullptr
        ))) {
            throw std::runtime_error("can't compile vertex shader");
        }
        if (FAILED(D3DCompile(
                shaderSourceStr.c_str(), shaderSourceStr.length(), nullptr, nullptr, nullptr,
                "ps_shader", "ps_4_0", 0, 0, &PS, nullptr
        ))) {
            throw std::runtime_error("can't compile pixel shader");
        }

//        ID3D11VertexShader *vertexShader;
//        ID3D11PixelShader *pixelShader;

        // encapsulate both shaders into shader objects
        device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &pVS);
        device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pPS);

        // set the shader objects
        deviceContext->VSSetShader(pVS, 0, 0);
        deviceContext->PSSetShader(pPS, 0, 0);

        // create the input layout object
        D3D11_INPUT_ELEMENT_DESC ied[] =
                {
                        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
                        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
                };

        device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &inputLayout);
        deviceContext->IASetInputLayout(inputLayout);

        {
            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));

            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(ConstantBuffer);
            bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bd.CPUAccessFlags = 0;

            if (FAILED(device->CreateBuffer(&bd, nullptr, &constantBuffer))) {
                throw std::runtime_error("can't create constant buffer");
            }

            D3D11_SAMPLER_DESC sampDesc;
            ZeroMemory(&sampDesc, sizeof(sampDesc));
            sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            sampDesc.MinLOD = 0;
            sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

            if (FAILED(device->CreateSamplerState(&sampDesc, &samplerState))) {
                throw std::runtime_error("can't create sampler state");
            }

//            DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 5.0f, -300.0f, 0.0f);
//            DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 100.0f, 0.0f, 0.0f);
//            DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//            mViewMatrix = DirectX::XMMatrixLookAtLH(Eye, At, Up);
//
//            mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
//                    DirectX::XM_PIDIV4,
//                    (float) screenWidth / (float) screenHeight, 0.01f, 1000.0f);
        }


    }


//    float fieldOfView, screenAspect;
//    fieldOfView = (float) 3.14 / 4.0f;
//    screenAspect = (float) screenWidth / (float) screenHeight;

//    projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
//    worldMatrix = DirectX::XMMatrixIdentity();
//    orthoMatrix = DirectX::XMMatrixOrthographicLH((float) screenWidth, (float) screenHeight, screenNear, screenDepth);
}
