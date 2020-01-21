#include "Component.hpp"

#include "RenderState.hpp"

Component::Component() {};
Component::~Component() {};

void Component::Initialise()
{
    OnInitialise();

    for (auto& child : mChildren)
        child->Initialise();
}

void Component::Update(const TimerTime& time)
{
    OnUpdate(time);

    if (mCustomUpdateFunction)
        mCustomUpdateFunction(*this, time);

    for (auto& child : mChildren)
        child->Update(time);
}

void Component::Draw(const RenderState& renderState)
{
    RenderState localRenderState = renderState;
    UpdateLocalRenderState(renderState, localRenderState);

    OnBeginDraw(localRenderState);
    OnDraw(localRenderState);
    DrawChildren(localRenderState);
    OnEndDraw(localRenderState);
}

void Component::Shutdown()
{
    OnShutdown();

    for (auto& child : mChildren)
        child->Shutdown();
}
 
void Component::WindowSizeChanged()
{
    OnWindowSizeChanged();

    for (auto& child : mChildren)
        child->WindowSizeChanged();
}

void Component::SetScale(const Vector3& scale)
{
    mScale = scale;
    UpdateLocalMatrix();
}

void Component::SetTranslation(const Vector3& translation)
{
    mTranslation = translation;
    UpdateLocalMatrix();
}

void Component::SetRotation(const Vector3& rotation)
{
    mRotation = rotation;
    UpdateLocalMatrix();
}

Vector3 Component::GetGlobalScale() const
{
    if (mParent)
        return mScale + mParent->GetGlobalScale();
    else
        return mScale;
}

Vector3 Component::GetGlobalTranslation() const
{
    if (mParent)
        return mTranslation + mParent->GetGlobalTranslation();
    else
        return mTranslation;
}

Vector3 Component::GetGlobalRotation() const
{
    if (mParent)
        return mRotation + mParent->GetGlobalRotation();
    else
        return mRotation;
}

void Component::AddChild(Component* component)
{
    mChildren.emplace_back(component);
    
    component->SetParent(this);
}

void Component::SetParent(Component* component)
{
    mParent = component;
}

void Component::UpdateLocalRenderState(const RenderState& parentRenderState, RenderState& localRenderState)
{
    localRenderState.worldMatrix = mLocalMatrix * parentRenderState.worldMatrix;
    localRenderState.worldViewProjectionMatrix = localRenderState.worldMatrix * parentRenderState.viewProjectionMatrix;
    
    OnUpdateLocalRenderState(parentRenderState, localRenderState);
}

void Component::DrawChildren(const RenderState& renderState)
{
    for (auto& child : mChildren)
        child->Draw(renderState);
}

void Component::UpdateLocalMatrix()
{
    mLocalMatrix = Matrix::CreateScale(mScale) * Matrix::CreateFromYawPitchRoll(mRotation.x, mRotation.y, mRotation.z) * Matrix::CreateTranslation(mTranslation);
}