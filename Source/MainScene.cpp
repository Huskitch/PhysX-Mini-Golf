#include "MainScene.h"

namespace PhysicsEngine
{
	MainScene::MainScene()
	{

	}

	MainScene::~MainScene()
	{

	}

	void MainScene::SetVisualisation()
	{
		px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_HORIZONTAL, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_VERTICAL, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_BENDING, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_SHEARING, 1.0f);
	}

	void MainScene::Init()
	{
		SetVisualisation();

		GetMaterial()->setRestitution(0.0f);

		plane = new Plane();
		plane->SetColor(color_palette[3]);
		//plane->SetColor(PxVec3(210.f / 255.f, 210.f / 255.f, 210.f / 255.f));
		plane->SetTrigger(true);
		AddActor(plane);

		golfBall = new Sphere(PxTransform(PxVec3(0, 3, 50)), 0.3f, 15.0f);
		golfBall->SetName("Ball");
		AddActor(golfBall);

		for (int i = 0; i < 5; i++)
		{
			track.push_back(new TrackPiece(this, new PxVec3(0.0f, 0.0f, i * 12.0f), 0.0f, 2.0f));
		}

		for (int i = 0; i < 3; i++)
		{
			trackRot.push_back(new TrackPieceRotated(this, new PxVec3(i * 12.0f + 10.0f, 10.0f, -18.0f), 0.0f, 2.0f));
		}

		flag = new FlagPole(this, new PxVec3(38.0f, 10.5f, -18.0f), 0.0f, 1.0f);

		windmill = new Windmill(this, new PxVec3(0.0f, 2.0f, 5.0f), new PxVec3(0, 0, 0), 0.97f);

		lift = new BallLift(this, new PxVec3(0.0f, 2.0f, 30.0f), new PxVec3(0, 0, 0), 1.0f);

		box = new SpringBox(this, new PxVec3(0.0f, 0.5f, 17.0f), 0.0f, 1.0f);

		hider = new Cloth(PxTransform(PxVec3(4, 1.5f, -6), PxQuat(0.0, -0.707, 0.0, 0.707)), PxVec2(1.0f, 8.f), 40, 40);
		hider->SetColor(color_palette[2]);
		((PxCloth*)hider->GetPxActor())->setStretchConfig(PxClothFabricPhaseType::eSHEARING, PxClothStretchConfig(0.2f));
		AddActor(hider);

		bridge = new Bridge(this, new PxVec3(0.0f, 0.0f, -9.0f), new PxVec3(0, 0, 0), 1.0f);

		corner = new TrackPiece(this, new PxVec3(0.0f, 0.0f, -18.0f), 0.0f, 2.0f, true);

		obstacle1 = new Box(PxTransform(PxVec3(-1.2f, 0.45f, 40.0f), PxQuat(0.0f, -0.383f, 0.0f, 0.924f)), PxVec3(3.0f, 1.0f, 0.5f), 1.0f);
		obstacle1->SetKinematic(true);
		obstacle1->SetColor(color_palette[2]);
		AddActor(obstacle1);

		obstacle2 = new Box(PxTransform(PxVec3(1.2f, 0.45f, 45.0f), PxQuat(0.0f, 0.383f, 0.0f, 0.924f)), PxVec3(3.0f, 1.0f, 0.5f), 1.0f);
		obstacle2->SetKinematic(true);
		obstacle2->SetColor(color_palette[2]);
		AddActor(obstacle2);

		activ = new SpinnerActivator(this, lift);

		sand = new Sandpit(this, new PxVec3(25.0f, 10.45f, -18.0f), PxQuat(0.0f, 0.0f, 0.0f, 0.0f), 1.0f);

		my_callback = new MySimulationEventCallback(corner, activ);
		px_scene->setSimulationEventCallback(my_callback);
	}

	void MainScene::Update()
	{
		windmill->Update();
		corner->Update();
		activ->Update();
		lift->Update();
	}
}
