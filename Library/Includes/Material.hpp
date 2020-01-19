#pragma once

#include "DirectXTKIncludes.hpp"
#include "NonCopyable.hpp"

class Effect;
class EffectPass;
class EffectTechnique;
class Mesh;
class Pass;

struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;
struct ID3D11InputLayout;

class Material : NonCopyable
{
public:
    Material();
    virtual ~Material();

    virtual void Initialise() = 0;
    virtual void CreateVertexBuffer(Mesh* mesh, ID3D11Buffer** vertexBuffer) = 0;

    EffectTechnique* GetCurrentTechnique() const;
	void SetCurrentTechnique(const std::string& techniqueName);

    ID3D11InputLayout* GetInputLayout(const EffectPass* effectPass) const { return mInputLayouts.at(effectPass).Get(); }

    virtual UInt32 VertexSize() const = 0;
    virtual UInt32 Offset() { return 0; }

protected:
    void CreateInputLayout(const std::string& techniqueName, const std::string& passName, const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UInt32 numElements);
        
    std::map<const EffectPass*, ComPtr<ID3D11InputLayout>> mInputLayouts;
        
    std::unique_ptr<Effect> mEffect;
    EffectTechnique* mCurrentTechnique = nullptr;
};