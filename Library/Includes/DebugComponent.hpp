#pragma once

// DirectXTK
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "Component.hpp"

struct ID3D11DeviceContext;

class DebugComponent : public Component
{
public:
    void OnInitialise() override;
    void OnUpdate(const TimerTime& time) override;
    void OnEndDraw(const RenderState& renderState) override;
    void OnShutdown() override;

private:
    std::wstring mText;

    std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
    std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
};