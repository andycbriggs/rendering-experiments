#pragma once

// DirectXTK
#include <CommonStates.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "DirectXTKIncludes.hpp"
#include "RenderState.hpp"

class CameraComponent;
class Component;

struct TimerTime;

struct IDXGIFactory2;
struct IDXGISwapChain1;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

class Engine
{
public:
    static Engine* Get();

    Engine(const std::wstring& windowClass, const std::wstring& windowName);
    virtual ~Engine();

    void Initialise();
    void Run();

    void SetRootComponent(Component* component);
    void SetCurrentCamera(CameraComponent* camera) { mCurrentCamera = camera; }
    void SetClearColor(Color color) { mClearColor = color; }

protected:
    virtual void Update(const TimerTime& time);
    virtual void Draw();

private:
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void Loop();
    void InitialiseWindow();
    void DestroyWindow();

    void OnDeviceLost();
    void OnWindowSizeChanged(unsigned int width, unsigned int height);
        
    void CreateDevice();
    void CreateSizeDependentResources(); // (re-)creates resources which depend on window size
    void ShutdownDirectX();

public:
    const Viewport& GetViewport() const { return mViewport; }

    ID3D11Device* Device() const { return mDevice.Get(); };
    ID3D11DeviceContext* DeviceContext() const { return mDeviceContext.Get(); };

    void SetDefaultRenderTargets();

protected:
    std::unique_ptr<Component> mRootComponent;
    CameraComponent* mCurrentCamera;

    DirectX::Keyboard mKeyboard;
    DirectX::Mouse mMouse;
    std::unique_ptr<DirectX::CommonStates> mCommonStates;

    Viewport mViewport;
    Color mClearColor = Colors::Black;

    std::wstring mWindowClass;
    std::wstring mWindowName;

    HWND mHwnd = NULL;
    WNDCLASS mWndClass = { };

    RenderState mRenderState;

    ComPtr<IDXGIFactory2> mFactory;
    ComPtr<IDXGISwapChain1> mSwapChain;

    ComPtr<ID3D11Device> mDevice;
    ComPtr<ID3D11DeviceContext> mDeviceContext;
    ComPtr<ID3D11RenderTargetView> mRenderTargetView;
    ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> mDepthStencilView;    
};