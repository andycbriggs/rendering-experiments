#include "ColorFilterComponent.hpp"

#include "Engine.hpp"
#include "Mesh.hpp"

#include "PhongMaterial.hpp"
#include "Texture2D.hpp"

#include "BloomComponent.hpp"
#include "BlurComponent.hpp"
#include "Component.hpp"
#include "CameraComponent.hpp"
#include "MeshComponent.hpp"
#include "SkyboxComponent.hpp"
#include "DebugComponent.hpp"
#include "WASDComponent.hpp"
#include "LookAtComponent.hpp"
#include "LightingComponent.hpp"

using namespace std;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCommand)
{
    try
    {
		std::unique_ptr<Engine> engine = std::make_unique<Engine>(L"Engine", L"Engine");
		engine->Initialise();
		//engine->SetClearColor(Color(Colors::DarkGray));

		shared_ptr<Texture2D> uvTexture = make_shared<Texture2D>("Content\\Textures\\muse_uv.jpg");
		shared_ptr<Texture2D> faceTexture = make_shared<Texture2D>("Content\\Textures\\CubeFaces.png");
		shared_ptr<Texture2D> earthTexture = make_shared<Texture2D>("Content\\Textures\\EarthComposite.jpg");

		DebugComponent* debug = new DebugComponent();

		PostProcessingComponent* postEffect = new BloomComponent();

		CameraComponent* camera = new CameraComponent();

		Component* cameraController = new WASDComponent();
		cameraController->SetTranslation({ 50.f, 100.f, 50.f });
		cameraController->SetRotation({ toRadians(45.f), toRadians(-60.f), toRadians(45.f) });

		MeshComponent* ground = new MeshComponent();
		ground->SetTexture(uvTexture);
		ground->SetMesh(Mesh::CreatePlane());
		ground->SetScale({ 100.f, 100.f, 1.f });
		ground->SetRotation({ 0.f, toRadians(-90.f), 0.f });

		MeshComponent* sphere = new MeshComponent();
		sphere->SetTexture(earthTexture);
		sphere->SetMesh(Mesh::CreateUVSphere());
		sphere->SetScale(Vector3(20.f));
		sphere->SetTranslation({ 0.f, 10.f, 0.f });

		sphere->SetCustomUpdate([](Component& self, const TimerTime& time) {
			self.SetRotation(Vector3{ float(time.totalTime) * 0.2f , 0.f , 0.f });
		});

		MeshComponent* cube = new MeshComponent();
		cube->SetTexture(faceTexture);
		cube->SetMesh(Mesh::CreateUVSphere());
		cube->SetScale(Vector3(10.f));

		cube->SetCustomUpdate([](Component& self, const TimerTime& time) {
			float x = sinf(float(time.totalTime))* 40.f;
			float z = cosf(float(time.totalTime))* 40.f;
			self.SetRotation(-Vector3{ float(time.totalTime) * 0.2f ,float(time.totalTime) * 0.2f , float(time.totalTime) * 0.2f });
			self.SetTranslation({ x, 10.f, z });
		});

		LightingComponent* lighting = new LightingComponent();
		lighting->SetAmbientColor(Color(Colors::White) * 0.5f);
		lighting->SetDirectionalDirection(Vector3::Down + Vector3::Right + Vector3::Backward);
		lighting->SetDirectionalColor(Color(Colors::White) * 0.2f);
		lighting->SetPointLightRadius(200.f);
		lighting->SetPointLightColor(Color(Colors::White));
		lighting->SetPointLightTranslation({ 40.f, 20.f, 0.f });
		lighting->SetSpotLightTranslation({ 0.f, 25.f, 50.f });
		lighting->SetSpotLightDirection({ 0.f, toRadians(-15.f), 0.f });

		MeshComponent* projectorView = new MeshComponent();
		projectorView->SetTexture(lighting->ShadowMapRenderTarget().GetTexture());
		projectorView->SetMesh(Mesh::CreatePlane());
		projectorView->SetScale({ 100.f, 100.f, 1.f });
		projectorView->SetTranslation({ 0.f, 50.f, -50.f });

		Component* scene = new Component();

		// scene graph
		debug->AddChild(postEffect);
			postEffect->AddChild(cameraController);
				cameraController->AddChild(camera);
			postEffect->AddChild(lighting);
				lighting->AddChild(scene);
					scene->AddChild(sphere);
					scene->AddChild(ground);
					scene->AddChild(cube);
			postEffect->AddChild(projectorView);

		engine->SetCurrentCamera(camera);
		engine->SetRootComponent(debug);

        engine->Run();
    }
    catch (const std::exception &e)
    {
        OutputDebugStringA(e.what());
    }

}