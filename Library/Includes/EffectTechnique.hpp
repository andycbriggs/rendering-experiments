#pragma once

struct ID3DX11EffectTechnique;

class EffectPass;

class EffectTechnique
{
public:
    EffectTechnique(ID3DX11EffectTechnique* effectTechnique);
    ~EffectTechnique();

    const std::string& GetName() const { return mName; }

    const std::vector<std::unique_ptr<EffectPass>>& GetPasses() const { return mPasses; }
    const EffectPass* GetPass(std::string name) const { return mPassesByName.at(name); }
    
private:
    std::string mName;
    ID3DX11EffectTechnique* mEffectTechnique;
    std::vector<std::unique_ptr<EffectPass>> mPasses;
    std::map<std::string, EffectPass*> mPassesByName;
};