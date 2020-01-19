#include "PipelineStateRestorer.hpp"

PipelineStateRestorer::PipelineStateRestorer(ID3D11DeviceContext* deviceContext)
{
	mDeviceContext = deviceContext;
	mDeviceContext->OMGetRenderTargets(1, mRenderTargetView.ReleaseAndGetAddressOf(), mDepthStencilView.ReleaseAndGetAddressOf());
	UINT numViewports = 1;
	D3D11_VIEWPORT d3d11Viewport = { 0 };
	mDeviceContext->RSGetViewports(&numViewports, &d3d11Viewport);
	mViewport = d3d11Viewport;
	mDeviceContext->RSGetState(mRasteriserState.ReleaseAndGetAddressOf());
	mDeviceContext->OMGetBlendState(mBlendState.ReleaseAndGetAddressOf(), mBlendFactor, &mSampleMask);
	mDeviceContext->OMGetDepthStencilState(mDepthStencilState.ReleaseAndGetAddressOf(), &mStencilRef);
}

PipelineStateRestorer::~PipelineStateRestorer()
{
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	mDeviceContext->RSSetViewports(1, mViewport.Get11());
	mDeviceContext->RSSetState(mRasteriserState.Get());
	mDeviceContext->OMSetBlendState(mBlendState.Get(), mBlendFactor, mSampleMask);
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), mStencilRef);
}
