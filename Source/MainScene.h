#pragma once
#include "BasicActors.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h> 
#include "PrefabBase.h"

namespace PhysicsEngine
{
	static const PxVec3 colors_palette[] = { PxVec3(46.f / 255.f,9.f / 255.f,39.f / 255.f),PxVec3(217.f / 255.f,0.f / 255.f,0.f / 255.f),
		PxVec3(255.f / 255.f,45.f / 255.f,0.f / 255.f),PxVec3(255.f / 255.f,140.f / 255.f,54.f / 255.f),PxVec3(4.f / 255.f,117.f / 255.f,111.f / 255.f) };

	class MySimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;
		TrackPiece* track;
		SpinnerActivator* activator;

		MySimulationEventCallback(TrackPiece* piece, SpinnerActivator* activ) : trigger(false), track(piece), activator(activ) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count)
		{
			//you can read the trigger information here
			for (PxU32 i = 0; i < count; i++)
			{
				//filter out contact with the planes
				if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
				{
					if (pairs[i].otherShape->getGeometryType() != PxGeometryType::eBOX)
					{
							//check if eNOTIFY_TOUCH_FOUND trigger
							if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
							{
								cerr << pairs[i].triggerActor->getName() << endl;

								if (std::strcmp(pairs[i].triggerActor->getName(), "Elevator") == 0)
								{
									track->IsMoving = true;
								}
								else if (std::strcmp(pairs[i].triggerActor->getName(), "Button1") == 0)
								{
									activator->Button1Activated = true;
								}
								else if (std::strcmp(pairs[i].triggerActor->getName(), "Button2") == 0)
								{
									activator->Button2Activated = true;
								}
								else if (std::strcmp(pairs[i].triggerActor->getName(), "Pole") == 0)
								{
									cerr << "WINNER" << endl;
									pairs[i].otherActor->setGlobalPose(PxTransform(PxVec3(0, 3, 50)));
									pairs[i].otherActor->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
								}
								else
								{
									pairs[i].otherActor->setGlobalPose(PxTransform(PxVec3(0, 3, 50)));
									pairs[i].otherActor->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
								}

								cerr << "onTrigger::eNOTIFY_TOUCH_FOUND"<< endl;
								trigger = true;
							}
							//check if eNOTIFY_TOUCH_LOST trigger
							if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
							{
								track->IsMoving = false;
								cerr << "onTrigger::eNOTIFY_TOUCH_LOST" << endl;
								trigger = false;
							}
						
					}
				}
			}
		}

		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs)
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << endl;

			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				//check eNOTIFY_TOUCH_FOUND
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					cerr << "onContact::eNOTIFY_TOUCH_FOUND" << endl;
				}
				//check eNOTIFY_TOUCH_LOST
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					cerr << "onContact::eNOTIFY_TOUCH_LOST" << endl;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
	};

	static PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		// let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		//enable continous collision detection
		//		pairFlags |= PxPairFlag::eCCD_LINEAR;


		//customise collision filtering here
		//e.g.

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			//trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
			//			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlags();
	};

	class MainScene : public Scene
	{
	public:
		MainScene();
		~MainScene();

		void SetVisualisation();
		virtual void Init();
		virtual void Update();

		MySimulationEventCallback* my_callback;

		Plane* plane;
		vector<TrackPiece*> track;
		vector<TrackPieceRotated*> trackRot;
		FlagPole* flag;
		Sphere* golfBall;
		Windmill* windmill;
		SpringBox* box;
		BallLift* lift;
		Bridge* bridge;
		Sandpit* sand;

		TrackPiece* corner;

		Cloth* hider;
		Box* obstacle1;
		Box* obstacle2;

		SpinnerActivator* activ;
	};
}
