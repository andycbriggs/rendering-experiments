#pragma once

struct ID3DX11Effect;

class EffectVariable;
class EffectTechnique;

class Effect
{
public:
    Effect(const std::wstring& filename);
    ~Effect();

    const std::vector<std::unique_ptr<EffectTechnique>>& GetTechniques() { return mTechniques; }
    EffectTechnique* GetTechnique(std::string techniqueName) const { return mTechniquesByName.at(techniqueName); }
    EffectVariable* GetVariable(std::string variableName) const { return mVariablesByName.at(variableName); }

private:
    std::vector<std::unique_ptr<EffectVariable>> mVariables;
    std::map<std::string, EffectVariable*> mVariablesByName;
    std::vector<std::unique_ptr<EffectTechnique>> mTechniques;
    std::map<std::string, EffectTechnique*> mTechniquesByName;

    ComPtr<ID3DX11Effect> mEffect;
};