#pragma once

#include "DirectXTKIncludes.hpp"

class PipelineStateRestorer
{
public:
    PipelineStateRestorer(ID3D11DeviceContext* deviceContext);
    ~PipelineStateRestorer();

private:
    float mBlendFactor[4];
    unsigned int mSampleMask;
    unsigned int mStencilRef;

	ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	Viewport mViewport;

	ComPtr<ID3D11DeviceContext> mDeviceContext;
    ComPtr<ID3D11RasterizerState> mRasteriserState;
    ComPtr<ID3D11BlendState> mBlendState;
    ComPtr<ID3D11DepthStencilState> mDepthStencilState;
};