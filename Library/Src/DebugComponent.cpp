#include "DebugComponent.hpp"

#include "CameraComponent.hpp"
#include "DirectXTKIncludes.hpp"
#include "Engine.hpp"
#include "PipelineStateRestorer.hpp"
#include "Timer.hpp"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

void DebugComponent::OnInitialise()
{
    mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(Engine::Get()->DeviceContext());
    mSpriteFont = std::make_unique<DirectX::SpriteFont>(Engine::Get()->Device(), L"Content\\Consolas_12.spritefont");
}

void DebugComponent::OnUpdate(const TimerTime& time)
{
    std::wostringstream wss;
	wss << L"intervalTime: " << (round(time.intervalTime * 1000.f * 100.f) / 100.f) << L"ms" << std::endl
		<< L"totalTime: " << (round(time.totalTime * 100.f) / 100.f) << L"s" << std::endl
		<< L"fps: " << 1. / time.intervalTime << std::endl;

	wss << "width: " << Engine::Get()->GetViewport().width << ", height: " << Engine::Get()->GetViewport().height << std::endl;

	/*const Camera* currentCamera = Engine::Get()->GetCurrentCamera();

	wss << L"camera position " << currentCamera->GetGlobalTranslation() << std::endl;
	wss << L"camera rotation " << currentCamera->GetGlobalRotation() << std::endl;*/

    mText = wss.str();
}

void DebugComponent::OnEndDraw(const RenderState& renderState)
{
	PipelineStateRestorer pipelineStateRestorer(renderState.deviceContext);

    mSpriteBatch->Begin();
    mSpriteFont->DrawString(mSpriteBatch.get(), mText.c_str(), DirectX::XMFLOAT2(10,10));
    mSpriteBatch->End();
}

void DebugComponent::OnShutdown()
{
    mSpriteFont.reset(nullptr);
    mSpriteBatch.reset(nullptr);
}