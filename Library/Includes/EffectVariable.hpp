#pragma once

#include "DirectXTKIncludes.hpp"

class EffectVariable
{
public:
    EffectVariable(ID3DX11EffectVariable* effectVariable);

    const std::string& GetName() const { return mName; }
        
    void SetValue(const float value) const;
    void SetValue(const Matrix& value) const;
    void SetValue(const Vector3& vector) const;
    void SetValue(const Vector4& vector) const;
    void SetValue(const std::vector<float> vector) const;
    void SetValue(const std::vector<Vector2>& vector) const;
    void SetValue(ID3D11ShaderResourceView* texture) const;

private:
    std::string mName;
    ID3DX11EffectVariable* mVariable;
};