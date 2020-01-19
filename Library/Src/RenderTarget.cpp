#include "RenderTarget.hpp"

#include "Engine.hpp"
#include "PipelineStateRestorer.hpp"
#include "Texture2D.hpp"

using namespace std;

RenderTarget::RenderTarget()
	: mClearColor(0.f, 0.f, 0.f, 0.f) // black alpha
{
}

RenderTarget::~RenderTarget() {};

void RenderTarget::Initialise(int width, int height)
{
	mViewport = Viewport(0.f, 0.f, float(width), float(height));

    mTexture = make_shared<Texture2D>(width, height, Texture2D::Usage::Bitmap);
	mDepthStencilTexture = make_shared<Texture2D>(width, height, Texture2D::Usage::Stencil);

    HRESULT hr = Engine::Get()->Device()->CreateRenderTargetView(mTexture->GetResource(), nullptr, mRenderTargetView.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateRenderTargetView);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { };
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr = Engine::Get()->Device()->CreateDepthStencilView(mDepthStencilTexture->GetResource(), &depthStencilViewDesc, mDepthStencilView.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateDepthStencilView);

	SetDebugObjectName(mRenderTargetView.Get(), "Intermediate Render Target");
	SetDebugObjectName(mDepthStencilView.Get(), "Intermediate Depth Stencil");
	SetDebugObjectName(mTexture->GetResource(), "Intermediate Render Target Texture");
	SetDebugObjectName(mDepthStencilTexture->GetResource(), "Intermediate Depth Stencil Texture");
}

void RenderTarget::Begin(const RenderState& renderState)
{
	mPipelineStateRestorer = make_unique<PipelineStateRestorer>(renderState.deviceContext);

	renderState.deviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	renderState.deviceContext->ClearRenderTargetView(mRenderTargetView.Get(), mClearColor);
	renderState.deviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	renderState.deviceContext->RSSetViewports(1, mViewport.Get11());
	renderState.deviceContext->OMSetBlendState(renderState.commonStates->AlphaBlend(), NULL, 0xffffff);
}

void RenderTarget::End()
{
	mPipelineStateRestorer.reset();
}
