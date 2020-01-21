#pragma once

#include "DirectXTKIncludes.hpp"

class Texture2D;
class PipelineStateRestorer;

struct RenderState;

class RenderTarget
{
public:
    RenderTarget();
    ~RenderTarget();

    void Initialise(int width, int height);
    void Begin(const RenderState& renderState);
    void End();
    
    void SetClearColor(const Color& color) { mClearColor = color; };

    ID3D11RenderTargetView* GetRenderTargetView() const { return mRenderTargetView.Get(); };
    ID3D11DepthStencilView* GetDepthStencilView() const { return mDepthStencilView.Get(); };

    std::shared_ptr<Texture2D>& GetTexture() { return mTexture; };
    std::shared_ptr<Texture2D>& GetDepthStencilTexture() { return mDepthStencilTexture; };

private:
    Color mClearColor;
    Viewport mViewport;

    std::shared_ptr<Texture2D> mTexture;
    std::shared_ptr<Texture2D> mDepthStencilTexture;
    std::unique_ptr<PipelineStateRestorer> mPipelineStateRestorer;

    ComPtr<ID3D11RenderTargetView> mRenderTargetView;
    ComPtr<ID3D11DepthStencilView> mDepthStencilView;
};