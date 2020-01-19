#include "EffectVariable.hpp"

#include "Helpers.hpp"

using namespace std;

EffectVariable::EffectVariable(ID3DX11EffectVariable* effectVariable)
    : mVariable(effectVariable)
{
    D3DX11_EFFECT_VARIABLE_DESC variableDesc = { 0 };
    HRESULT hr = effectVariable->GetDesc(&variableDesc);
    ThrowIfFailed(hr, GetDesc);

    mName = variableDesc.Name;
}

void EffectVariable::SetValue(const float value) const
{
    ID3DX11EffectScalarVariable* scalarVariable = mVariable->AsScalar();
    ThrowIfNullPointer(scalarVariable, AsScalar);
    ThrowIfFalse(scalarVariable->IsValid(), IsValid);
    scalarVariable->SetFloat(value);
}

void EffectVariable::SetValue(const Matrix& value) const
{
    ID3DX11EffectMatrixVariable* matrixVariable = mVariable->AsMatrix();
    ThrowIfNullPointer(matrixVariable, AsMatrix);
    ThrowIfFalse(matrixVariable->IsValid(), IsValid);
    matrixVariable->SetMatrix((const float*)&value);
}

void EffectVariable::SetValue(const Vector3& vector) const
{
    ID3DX11EffectVectorVariable* vectorVariable = mVariable->AsVector();
    ThrowIfNullPointer(vectorVariable, AsVector);
    ThrowIfFalse(vectorVariable->IsValid(), IsValid);
    vectorVariable->SetFloatVector((const float*)&vector);
}

void EffectVariable::SetValue(const Vector4& vector) const
{
    ID3DX11EffectVectorVariable* vectorVariable = mVariable->AsVector();
    ThrowIfNullPointer(vectorVariable, AsVector);
    ThrowIfFalse(vectorVariable->IsValid(), IsValid);
    vectorVariable->SetFloatVector((const float*)&vector);
}

void EffectVariable::SetValue(const std::vector<float> vector) const
{
	ID3DX11EffectScalarVariable* scalarVariable = mVariable->AsScalar();
	ThrowIfNullPointer(scalarVariable, AsScalar);
	ThrowIfFalse(scalarVariable->IsValid(), IsValid);
	scalarVariable->SetFloatArray(vector.data(), 0, (UInt32)vector.size());
}

void EffectVariable::SetValue(const std::vector<Vector2>& vector) const
{
	ID3DX11EffectVectorVariable* vectorVariable = mVariable->AsVector();
	ThrowIfNullPointer(vectorVariable, AsVector);
	ThrowIfFalse(vectorVariable->IsValid(), IsValid);
	vectorVariable->SetFloatVectorArray((const float*)vector.data(), 0, (UInt32)vector.size());
}

void EffectVariable::SetValue(ID3D11ShaderResourceView* resourceView) const
{
    ID3DX11EffectShaderResourceVariable* shaderResourceVariable = mVariable->AsShaderResource();
    ThrowIfNullPointer(shaderResourceVariable, AsShaderResource);
    ThrowIfFalse(shaderResourceVariable->IsValid(), IsValid);
    shaderResourceVariable->SetResource(resourceView);
}