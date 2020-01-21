#pragma once

#include "DirectXTKIncludes.hpp"

namespace DirectX
{
    class CommonStates;
};

class CameraComponent;
class Light;
class DirectionalLight;
class PointLight;

struct RenderState
{
    ID3D11DeviceContext* deviceContext = nullptr;
    Matrix worldMatrix = Matrix::Identity;
    Matrix viewProjectionMatrix;
    Matrix worldViewProjectionMatrix;
    Vector3 cameraTranslation;

    Light* ambientLight = nullptr;
    DirectionalLight* directionalLight = nullptr;
    PointLight* pointLight = nullptr;

    CommonStates* commonStates = nullptr;
};