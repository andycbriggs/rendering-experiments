#pragma once

#include "DirectXTKIncludes.hpp"
#include "NonCopyable.hpp"

struct RenderState;
struct TimerTime;

class Component : NonCopyable
{
public:
	using UpdateFunction = std::function<void(Component&, const TimerTime&)>;

	Component();
    virtual ~Component();

	void Initialise();
	void Update(const TimerTime& time);
    void Draw(const RenderState& renderState);
	void Shutdown();
	void WindowSizeChanged();

	void SetCustomUpdate(UpdateFunction updateFunction) { mCustomUpdateFunction = updateFunction; }

	void SetScale(const Vector3& scale); // local
	void SetTranslation(const Vector3& translation); // local
	void SetRotation(const Vector3& rotation); // local euler angle in radians

	const Vector3& GetScale() const { return mScale; }
	const Vector3& GetTranslation() const { return mTranslation; }
	const Vector3& GetRotation() const { return mRotation; } // euler angle in radians

	Vector3 GetGlobalScale() const;
	Vector3 GetGlobalTranslation() const;
	Vector3 GetGlobalRotation() const;

	const Matrix& GetLocalMatrix() const { return mLocalMatrix; }

	void AddChild(Component* component); // takes ownership
	void SetParent(Component* component); 

protected:
	virtual void OnInitialise() {};
	virtual void OnBeginDraw(const RenderState& localRenderState) {};
	virtual void OnDraw(const RenderState& localRenderState) {};
	virtual void OnEndDraw(const RenderState& localRenderState) {};
	virtual void OnShutdown() {};
	virtual void OnUpdate(const TimerTime& time) {};
    virtual void OnWindowSizeChanged() {};
	virtual void OnUpdateLocalRenderState(const RenderState& parentRenderState, RenderState& localRenderState) {};

	void UpdateLocalRenderState(const RenderState& parentRenderState, RenderState& localRenderState);
	void DrawChildren(const RenderState& localRenderState);

protected:
	void UpdateLocalMatrix();

	UpdateFunction mCustomUpdateFunction;

	Vector3 mScale = Vector3::One;
	Vector3 mTranslation;
	Vector3 mRotation;

	Matrix mLocalMatrix;

	Component* mParent = nullptr;
	std::vector<std::unique_ptr<Component>> mChildren;
};