#include "Engine.hpp"

#include <algorithm>

#include "CameraComponent.hpp"
#include "Component.hpp"
#include "Helpers.hpp"
#include "Timer.hpp"

using namespace std;

// singleton
static Engine* s_singleInstance = nullptr;
/*static*/ Engine* Engine::Get() { return s_singleInstance; };

Engine::Engine(const wstring& windowClass, const wstring& windowName)
    : mWindowClass(windowClass)
    , mWindowName(windowName)
{
    assert(s_singleInstance == nullptr); 
    s_singleInstance = this;
}

Engine::~Engine()
{
    assert(!mDevice);
    assert(!mHwnd);
    assert(s_singleInstance);
    s_singleInstance = nullptr;
}

void Engine::Initialise()
{
	InitialiseWindow();
	CreateDevice();
	CreateSizeDependentResources();
}

void Engine::Run()
{
	mRootComponent->Initialise();
    Loop();
    ShutdownDirectX();
    DestroyWindow();
}

void Engine::SetRootComponent(Component* component) 
{ 
	mRootComponent = unique_ptr<Component>(component);
}

void Engine::Update(const TimerTime& time)
{
    if (mRootComponent)
		mRootComponent->Update(time);
}

void Engine::Draw()
{
	// setup render state
	mRenderState.deviceContext = mDeviceContext.Get();
	mRenderState.viewProjectionMatrix = mCurrentCamera->GetViewProjectionMatrix();
	mRenderState.cameraTranslation = mCurrentCamera->GetGlobalTranslation();

    if (mRootComponent)
		mRootComponent->Draw(mRenderState);
}

LRESULT WINAPI Engine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    Engine* engine = (Engine*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (engine)
        engine->ProcessMessage(hWnd, message, wParam, lParam);

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void Engine::ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_ACTIVATEAPP:
        mKeyboard.ProcessMessage(message, wParam, lParam);
        mMouse.ProcessMessage(message, wParam, lParam);
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        mKeyboard.ProcessMessage(message, wParam, lParam);
        break;

    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        mMouse.ProcessMessage(message, wParam, lParam);
    }
}

void Engine::Loop()
{
    MSG msg = { 0 };
    int rc = 0;

    Timer engineClock;

    for (;;)
    {
        while (rc = ::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE) != 0)
        {
            if (msg.message == WM_QUIT || rc == -1 || mKeyboard.GetState().Escape)
                return;

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        Update(engineClock.GetTime());

        Mouse::Get().ResetScrollWheelValue();

        SetDefaultRenderTargets();

        mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), mClearColor);
        mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        Draw();

        HRESULT hr = mSwapChain->Present(1, 0);
        ThrowIfFailed(hr, Present);

		mDeviceContext->ClearState();
    }
}

void Engine::InitialiseWindow()
{
    mWndClass.hInstance = GetModuleHandle(NULL);
    mWndClass.lpszClassName = mWindowClass.c_str();
    mWndClass.style = CS_CLASSDC;
    mWndClass.lpfnWndProc = WndProc;
    mWndClass.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    mWndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);

    ATOM classAtom = ::RegisterClass(&mWndClass);
    ThrowIfNull(classAtom, RegisterClass);

    mHwnd = ::CreateWindow(mWindowClass.c_str(), mWindowName.c_str(), WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);
    ThrowIfNull(mHwnd, CreateWindow);

    ::SetLastError(0);
    LONG_PTR previous = ::SetWindowLongPtr(mHwnd, GWLP_USERDATA, (LONG_PTR)this);
    if (previous == 0 && GetLastError() != 0)
        ThrowIfNull(previous, SetWindowLongPtr);

    ::ShowWindow(mHwnd, SW_SHOW); // can return true or false on success

    BOOL ok = ::UpdateWindow(mHwnd);
    ThrowIfFalse(ok, UpdateWindow);

    mMouse.SetWindow(mHwnd);
}

void Engine::DestroyWindow()
{
    if (mHwnd)
        ::DestroyWindow(mHwnd);
    mHwnd = NULL;
}

void Engine::OnDeviceLost()
{
    ShutdownDirectX();
    CreateDevice();
    CreateSizeDependentResources();
    // TODO: implement a DeviceLostListener for components which need to know when device is lost
}

void Engine::OnWindowSizeChanged(unsigned int width, unsigned int height)
{
	// Setup the viewport
	mViewport = Viewport(0.f, 0.f, float(width), float(height));

    CreateSizeDependentResources();

    if (mRootComponent)
		mRootComponent->WindowSizeChanged(); // allow cameras to recalculate projection matrix
}

void Engine::CreateDevice()
{
    // Create device

    assert(!mDevice);
    assert(!mDeviceContext);

    UINT flags = 0;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0
    };

    HRESULT hr = 0;

    hr = ::D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, 
        featureLevels, _ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, mDevice.ReleaseAndGetAddressOf(), NULL, mDeviceContext.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, D3D11CreateDevice);

    // Retrieve factory

    ComPtr<IDXGIDevice1> pDXGIDevice;
    hr = mDevice.As(&pDXGIDevice);
    ThrowIfFailed(hr, As);

    ComPtr<IDXGIAdapter> pDXGIAdapter;
    hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
    ThrowIfFailed(hr, GetAdapter);

    hr = pDXGIAdapter->GetParent(IID_PPV_ARGS(&mFactory));
    ThrowIfFailed(hr, GetParent);

    // Create swap chain

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    hr = mFactory->CreateSwapChainForHwnd(mDevice.Get(), mHwnd, &swapChainDesc, NULL, NULL, mSwapChain.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateSwapChainForHwnd);

    mCommonStates = make_unique<CommonStates>(mDevice.Get());
	mRenderState.commonStates = mCommonStates.get();
}

void Engine::CreateSizeDependentResources()
{
    if (!mDevice || !mDeviceContext)
        return; // directx not initialised yet

    mDeviceContext->ClearState();
    mDeviceContext->Flush();

    mDepthStencilBuffer.Reset();
    mDepthStencilView.Reset();
    mRenderTargetView.Reset();

    // Resize buffers

	UINT width = UINT(mViewport.width);
	UINT height = UINT(mViewport.height);
     
    HRESULT hr = 0;

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
    hr = mSwapChain->GetDesc1(&swapChainDesc);
    ThrowIfFailed(hr, GetDesc1);

    hr = mSwapChain->ResizeBuffers(swapChainDesc.BufferCount, width, height, swapChainDesc.Format, swapChainDesc.Flags);
    ThrowIfFailed(hr, ResizeBuffers);

    // Create render target

    ComPtr<ID3D11Texture2D> backBuffer;
    hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    ThrowIfFailed(hr, GetBuffer);

    hr = mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRenderTargetView.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateRenderTargetView);

	SetDebugObjectName(mRenderTargetView.Get(), "Main Render Target View");
	SetDebugObjectName(backBuffer.Get(), "Main Back Buffer");

    // Create depth stencil

    D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.Height = height;
    depthStencilDesc.Width = width;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;

    hr = mDevice->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateTexture2D);

    hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateTexture2D);

	SetDebugObjectName(mDepthStencilView.Get(), "Main Depth Stencil View");
	SetDebugObjectName(mDepthStencilBuffer.Get(), "Main Depth Stencil");
}

void Engine::ShutdownDirectX()
{
    mCommonStates = nullptr;
	mRenderState.commonStates = nullptr;

    if (mSwapChain)
        mSwapChain->SetFullscreenState(FALSE, nullptr);

    mDepthStencilBuffer.Reset();
    mDepthStencilView.Reset();
    mRenderTargetView.Reset();
    mSwapChain.Reset();

	if (mDeviceContext)
	{
        mDeviceContext->ClearState();
        mDeviceContext->Flush();
    }
    mDeviceContext.Reset();

#ifdef _DEBUG
    ComPtr<ID3D11Debug> debugDevice;
    HRESULT hr = mDevice.As(&debugDevice);
    ThrowIfFailed(hr, As); 
    mDevice.Reset();
    mFactory.Reset();
    debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_IGNORE_INTERNAL);
#else
    mDevice.Reset();
    mFactory.Reset();
#endif

}

void Engine::SetDefaultRenderTargets()
{
    mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
    mDeviceContext->RSSetViewports(1, mViewport.Get11());
	mDeviceContext->OMSetBlendState(mCommonStates->AlphaBlend(), NULL, 0xffffff);
}