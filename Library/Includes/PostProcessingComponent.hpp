#pragma once

#include "Component.hpp"

class Mesh;
class RenderTarget;
class Texture2D;

struct RenderState;

class PostProcessingComponent : public Component
{
public:
    PostProcessingComponent();
    ~PostProcessingComponent();

    void OnInitialise() override;
	void OnBeginDraw(const RenderState& renderState) override;
	void OnEndDraw(const RenderState& renderState) override;
    
	virtual void OnWindowSizeChanged() override;
	virtual void OnDrawPostProcessing(const RenderState& renderState, const Texture2D* inputTexture) = 0;

private:
	void RecreateRenderTarget();

	std::unique_ptr<RenderTarget> mRenderTarget;

protected:
    std::shared_ptr<Mesh> mMesh;

	UInt32 mIndexCount;
	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;
};