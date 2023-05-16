#include "Render.h"
#include "Window.h"
#include "Logger.h"
using namespace sg::graphics;
using namespace sg::core;
using namespace sg::utility;

sg::graphics::Render::Render() : pWindow_{nullptr}, d3dDevice_{ nullptr }, d3dContext_{ nullptr }, swapChain_{ nullptr },
backBufferTarget_{ nullptr }, featureLevel_{ D3D_FEATURE_LEVEL_11_0 }
{
    this->EnumerateAdapters();
}

sg::graphics::Render::~Render()
{
    Logger<wchar_t>::infoLogger.Print(L"Render delete...");
    _RELEASE(this->d3dDevice_);
    _RELEASE(this->d3dContext_);
    _RELEASE(this->swapChain_);
    _RELEASE(this->backBufferTarget_);
}

bool sg::graphics::Render::Init()
{
    if (!pWindow_ && d3dDevice_ && d3dContext_ && swapChain_ && backBufferTarget_)
    {
        return false;
    }
    Logger<wchar_t>::infoLogger.Print(L"Render init start...");
    D3D_DRIVER_TYPE driverType_ = D3D_DRIVER_TYPE_HARDWARE;
    D3D_DRIVER_TYPE driverTypes[] =
    {
    D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
    };
    unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
    
    D3D_FEATURE_LEVEL featureLevels[] =
    {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0
    };
    unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);
    Logger<wchar_t>::infoLogger.Print(L"Render devices create...");
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = this->pWindow_->windowSetting.Width();
    swapChainDesc.BufferDesc.Height = this->pWindow_->windowSetting.Height();
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = this->pWindow_->windowSetting.Handle();
    swapChainDesc.Windowed = true;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    
        unsigned int creationFlags = 0;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    HRESULT result;
    unsigned int driver = 0;
    for (driver = 0; driver < totalDriverTypes; ++driver)
    {
        result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
            creationFlags, featureLevels, totalFeatureLevels,
            D3D11_SDK_VERSION, &swapChainDesc, &this->swapChain_,
            &this->d3dDevice_, &this->featureLevel_, &this->d3dContext_);
        if (SUCCEEDED(result))
        {
            driverType_ = driverTypes[driver];
            break;
        }
    }
    if (FAILED(result))
    {
        return false;
    }
    Logger<wchar_t>::infoLogger.Print(L"Render devices created.");
    
    ID3D11Texture2D* backBufferTexture;
    result = this->swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
        (LPVOID*)&backBufferTexture);
    if (FAILED(result))
    {
        return false;
    }
    result = this->d3dDevice_->CreateRenderTargetView(backBufferTexture, 0,
        &this->backBufferTarget_);
    _RELEASE(backBufferTexture);
    if (FAILED(result))
    {
        return false;
    }
    d3dContext_->OMSetRenderTargets(1, &this->backBufferTarget_, 0);

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<float>(this->pWindow_->windowSetting.Width());
    viewport.Height = static_cast<float>(this->pWindow_->windowSetting.Height());
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    this->d3dContext_->RSSetViewports(1, &viewport);
    
    this->ClearBuffers();
    Logger<wchar_t>::infoLogger.Print(L"Render init end.");
    return true;
}

void sg::graphics::Render::ClearBuffers(const fColorRGBA color)
{
    if (this->IsInit())
    {
        float clearColor[4] = { color.r, color.g, color.b, color.a };
        this->d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
        this->swapChain_->Present(0, 0);
    }
    
}

bool sg::graphics::Render::IsInit() const
{
    return pWindow_ && d3dDevice_ && d3dContext_ && swapChain_ && backBufferTarget_;
}

void sg::graphics::Render::BindWindow(core::Window* pWindow)
{
    this->pWindow_ = pWindow;
    Logger<wchar_t>::infoLogger.Print(L"Window binded to render.");
    if (!this->pWindow_)
        Logger<wchar_t>::warnLogger.Print(L"Window pointer is nullptr");
}

bool sg::graphics::Render::EnumerateAdapters()
{
    IDXGIAdapter1* pAdapter;
    IDXGIFactory1* pFactory = NULL;


    // Create a DXGIFactory object.
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&pFactory)))
    {
        return false;
    }


    for (UINT i = 0;
        pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
        ++i)
    {
        this->vAdapters_.push_back(std::shared_ptr<IDXGIAdapter1>(pAdapter, [](IDXGIAdapter1* pAdapter) 
            { 
                if (pAdapter)
                {
                    DXGI_ADAPTER_DESC1 a {};
            
                    pAdapter->GetDesc1(&a);
                    std::wstring adapterDescription(a.Description);
                    Logger<wchar_t>::infoLogger.Print(adapterDescription + L" adapter desc destroyed.");
                    pAdapter->Release();
                }
                
            }));
    }


    _RELEASE(pFactory);
    return true;
}

