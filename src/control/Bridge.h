#pragma once
#include "Entity.h"

class CBridge
{
private:
	enum bridgeStates
	{
		STATE_BRIDGE_LOCKED,
		STATE_LIFT_PART_IS_UP,
		STATE_LIFT_PART_MOVING_DOWN,
		STATE_LIFT_PART_IS_DOWN,
		STATE_LIFT_PART_ABOUT_TO_MOVE_UP,
		STATE_LIFT_PART_MOVING_UP
	};


	static CEntity *&pLiftRoad, *&pLiftPart, *&pWeight;
	static int &State, &OldState;
	static float &DefaultZLiftPart, &DefaultZLiftRoad, &DefaultZLiftWeight;
	static float& OldLift;
	static uint32& TimeOfBridgeBecomingOperational;

public:
	static void Init();
	static void Update();
	static bool ShouldLightsBeFlashing();
	static void FindBridgeEntities();
	static bool ThisIsABridgeObjectMovingUp(int);
};
