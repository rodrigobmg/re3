#include "common.h"
#include "patcher.h"
#include "CarCtrl.h"

#include "Automobile.h"
#include "Camera.h"
#include "CarAI.h"
#include "CarGen.h"
#include "Curves.h"
#include "CutsceneMgr.h"
#include "General.h"
#include "IniFile.h"
#include "ModelIndices.h"
#include "PathFind.h"
#include "Ped.h"
#include "PlayerInfo.h"
#include "PlayerPed.h"
#include "Timer.h"
#include "Streaming.h"
#include "VisibilityPlugins.h"
#include "Vehicle.h"
#include "Wanted.h"
#include "World.h"
#include "Zones.h"

#define LANE_WIDTH 5.0f
#define INFINITE_Z 1000000000.0f

int &CCarCtrl::NumLawEnforcerCars = *(int*)0x8F1B38;
int &CCarCtrl::NumAmbulancesOnDuty = *(int*)0x885BB0;
int &CCarCtrl::NumFiretrucksOnDuty = *(int*)0x9411F0;
bool &CCarCtrl::bCarsGeneratedAroundCamera = *(bool*)0x95CD8A;
float& CCarCtrl::CarDensityMultiplier = *(float*)0x5EC8B4;
int32 &CCarCtrl::NumMissionCars = *(int32*)0x8F1B54;
int32 &CCarCtrl::NumRandomCars = *(int32*)0x943118;
int32 &CCarCtrl::NumParkedCars = *(int32*)0x8F29E0;
int8 &CCarCtrl::CountDownToCarsAtStart = *(int8*)0x95CD63;
int32 &CCarCtrl::MaxNumberOfCarsInUse = *(int32*)0x5EC8B8;
uint32 &CCarCtrl::LastTimeLawEnforcerCreated = *(uint32*)0x8F5FF0;
int32 (&CCarCtrl::TotalNumOfCarsOfRating)[7] = *(int32(*)[7])*(uintptr*)0x8F1A60;
int32 (&CCarCtrl::NextCarOfRating)[7] = *(int32(*)[7])*(uintptr*)0x9412AC;
int32 (&CCarCtrl::CarArrays)[7][256] = *(int32(*)[7][256])*(uintptr*)0x6EB860;

WRAPPER void CCarCtrl::SwitchVehicleToRealPhysics(CVehicle*) { EAXJMP(0x41F7F0); }
WRAPPER void CCarCtrl::AddToCarArray(int32 id, int32 vehclass) { EAXJMP(0x4182F0); }
WRAPPER void CCarCtrl::UpdateCarCount(CVehicle*, bool) { EAXJMP(0x4202E0); }
WRAPPER bool CCarCtrl::JoinCarWithRoadSystemGotoCoors(CVehicle*, CVector, bool) { EAXJMP(0x41FA00); }
WRAPPER void CCarCtrl::JoinCarWithRoadSystem(CVehicle*) { EAXJMP(0x41F820); }
WRAPPER void CCarCtrl::SteerAICarWithPhysics(CVehicle*) { EAXJMP(0x41DA60); }
WRAPPER void CCarCtrl::UpdateCarOnRails(CVehicle*) { EAXJMP(0x418880); }
WRAPPER void CCarCtrl::ScanForPedDanger(CVehicle *veh) { EAXJMP(0x418F40); }
WRAPPER void CCarCtrl::RemoveFromInterestingVehicleList(CVehicle* v) { EAXJMP(0x41F7A0); }
WRAPPER void CCarCtrl::GenerateEmergencyServicesCar(void) { EAXJMP(0x41FC50); }
WRAPPER int32 CCarCtrl::ChoosePoliceCarModel(void) { EAXJMP(0x4181F0); }
WRAPPER int32 CCarCtrl::ChooseGangCarModel(int32 gang) { EAXJMP(0x4182C0); }

void
CCarCtrl::GenerateRandomCars()
{
	if (CCutsceneMgr::IsCutsceneProcessing())
		return;
	if (NumRandomCars < 30){
		if (CountDownToCarsAtStart == 0){
			GenerateOneRandomCar();
		}
		else if (--CountDownToCarsAtStart == 0) {
			for (int i = 0; i < 50; i++)
				GenerateOneRandomCar();
			CTheCarGenerators::GenerateEvenIfPlayerIsCloseCounter = 20;
		}
	}
	/* Approximately once per 4 seconds. */
	if ((CTimer::GetTimeInMilliseconds() & 0xFFFFF000) != (CTimer::GetPreviousTimeInMilliseconds() & 0xFFFFF000))
		GenerateEmergencyServicesCar();
}

void
CCarCtrl::GenerateOneRandomCar()
{
	static int32 unk = 0;
	CPlayerInfo* pPlayer = &CWorld::Players[CWorld::PlayerInFocus];
	CVector vecTargetPos = FindPlayerCentreOfWorld(CWorld::PlayerInFocus);
	CVector2D vecPlayerSpeed = FindPlayerSpeed();
	CZoneInfo zone;
	CTheZones::GetZoneInfoForTimeOfDay(&vecTargetPos, &zone);
	pPlayer->m_nTrafficMultiplier = pPlayer->m_fRoadDensity * zone.carDensity;
	if (NumRandomCars >= pPlayer->m_nTrafficMultiplier * CarDensityMultiplier * CIniFile::CarNumberMultiplier)
		return;
	if (NumFiretrucksOnDuty + NumAmbulancesOnDuty + NumParkedCars + NumMissionCars + NumLawEnforcerCars + NumRandomCars >= MaxNumberOfCarsInUse)
		return;
	CWanted* pWanted = pPlayer->m_pPed->m_pWanted;
	int carClass;
	int carModel;
	if (pWanted->m_nWantedLevel > 1 && NumLawEnforcerCars < pWanted->m_MaximumLawEnforcerVehicles &&
		pWanted->m_CurrentCops < pWanted->m_MaxCops && (
			pWanted->m_nWantedLevel > 3 ||
			pWanted->m_nWantedLevel > 2 && CTimer::GetTimeInMilliseconds() > LastTimeLawEnforcerCreated + 5000 ||
			pWanted->m_nWantedLevel > 1 && CTimer::GetTimeInMilliseconds() > LastTimeLawEnforcerCreated + 8000)) {
		/* Last pWanted->m_nWantedLevel > 1 is unnecessary but I added it for better readability. */
		/* Wouldn't be surprised it was there originally but was optimized out. */
		carClass = COPS;
		carModel = ChoosePoliceCarModel();
	}else{
		carModel = ChooseModel(&zone, &vecTargetPos, &carClass);
		if (carClass == COPS && pWanted->m_nWantedLevel >= 1)
			/* All cop spawns with wanted level are handled by condition above. */
			/* In particular it means that cop cars never spawn if player has wanted level of 1. */
			return;
	}
	float frontX, frontY;
	float preferredDistance, angleLimit;
	bool invertAngleLimitTest;
	CVector spawnPosition;
	int32 curNodeId, nextNodeId;
	float positionBetweenNodes;
	bool testForCollision;
	CVehicle* pPlayerVehicle = FindPlayerVehicle();
	CVector2D vecPlayerVehicleSpeed;
	float fPlayerVehicleSpeed;
	if (pPlayerVehicle) {
		vecPlayerVehicleSpeed = FindPlayerVehicle()->GetMoveSpeed();
		fPlayerVehicleSpeed = vecPlayerVehicleSpeed.Magnitude();
	}
	if (TheCamera.GetForward().z < -0.9f){
		/* Player uses topdown camera. */
		/* Spawn essentially anywhere. */
		frontX = frontY = 0.707f; /* 45 degrees */
		angleLimit = -1.0f;
		invertAngleLimitTest = true;
		preferredDistance = 40.0f;
		/* BUG: testForCollision not initialized in original game. */
		testForCollision = false;
	}else if (!pPlayerVehicle){
		/* Player is not in vehicle. */
		testForCollision = true;
		frontX = TheCamera.CamFrontXNorm;
		frontY = TheCamera.CamFrontYNorm;
		switch (CTimer::GetFrameCounter() & 1) {
		case 0:
			/* Spawn a vehicle relatively far away from player. */
			/* Forward to his current direction (camera direction). */
			angleLimit = 0.707f; /* 45 degrees */
			invertAngleLimitTest = true;
			preferredDistance = 120.0f * TheCamera.GenerationDistMultiplier;
			break;
		case 1:
			/* Spawn a vehicle close to player to his side. */
			/* Kinda not within camera angle. */
			angleLimit = 0.707f; /* 45 degrees */
			invertAngleLimitTest = false;
			preferredDistance = 40.0f;
			break;
		}
	}else if (fPlayerVehicleSpeed > 0.4f){ /* 72 km/h */
		/* Player is moving fast in vehicle */
		/* Prefer spawning vehicles very far away from him. */
		frontX = vecPlayerVehicleSpeed.x / fPlayerVehicleSpeed;
		frontY = vecPlayerVehicleSpeed.y / fPlayerVehicleSpeed;
		testForCollision = false;
		switch (CTimer::GetFrameCounter() & 3) {
		case 0:
		case 1:
			/* Spawn a vehicle in a very narrow gap in front of a player */
			angleLimit = 0.85f; /* approx 30 degrees */
			invertAngleLimitTest = true;
			preferredDistance = 120.0f * TheCamera.GenerationDistMultiplier;
			break;
		case 2:
			/* Spawn a vehicle relatively far away from player. */
			/* Forward to his current direction (camera direction). */
			angleLimit = 0.707f; /* 45 degrees */
			invertAngleLimitTest = true;
			preferredDistance = 120.0f * TheCamera.GenerationDistMultiplier;
			break;
		case 3:
			/* Spawn a vehicle close to player to his side. */
			/* Kinda not within camera angle. */
			angleLimit = 0.707f; /* 45 degrees */
			invertAngleLimitTest = false;
			preferredDistance = 40.0f;
			break;
		}
	}else if (fPlayerVehicleSpeed > 0.1f){ /* 18 km/h */
		/* Player is moving moderately fast in vehicle */
		/* Spawn more vehicles to player's side. */
		frontX = vecPlayerVehicleSpeed.x / fPlayerVehicleSpeed;
		frontY = vecPlayerVehicleSpeed.y / fPlayerVehicleSpeed;
		testForCollision = false;
		switch (CTimer::GetFrameCounter() & 3) {
		case 0:
			/* Spawn a vehicle in a very narrow gap in front of a player */
			angleLimit = 0.85f; /* approx 30 degrees */
			invertAngleLimitTest = true;
			preferredDistance = 120.0f * TheCamera.GenerationDistMultiplier;
			break;
		case 1:
			/* Spawn a vehicle relatively far away from player. */
			/* Forward to his current direction (camera direction). */
			angleLimit = 0.707f; /* 45 degrees */
			invertAngleLimitTest = true;
			preferredDistance = 120.0f * TheCamera.GenerationDistMultiplier;
			break;
		case 2:
		case 3:
			/* Spawn a vehicle close to player to his side. */
			/* Kinda not within camera angle. */
			angleLimit = 0.707f; /* 45 degrees */
			invertAngleLimitTest = false;
			preferredDistance = 40.0f;
			break;
		}
	}else{
		/* Player is in vehicle but moving very slow. */
		/* Then use camera direction instead of vehicle direction. */
		testForCollision = true;
		frontX = TheCamera.CamFrontXNorm;
		frontY = TheCamera.CamFrontYNorm;
		switch (CTimer::GetFrameCounter() & 1) {
		case 0:
			/* Spawn a vehicle relatively far away from player. */
			/* Forward to his current direction (camera direction). */
			angleLimit = 0.707f; /* 45 degrees */
			invertAngleLimitTest = true;
			preferredDistance = 120.0f * TheCamera.GenerationDistMultiplier;
			break;
		case 1:
			/* Spawn a vehicle close to player to his side. */
			/* Kinda not within camera angle. */
			angleLimit = 0.707f; /* 45 degrees */
			invertAngleLimitTest = false;
			preferredDistance = 40.0f;
			break;
		}
	}
	if (!ThePaths.NewGenerateCarCreationCoors(vecTargetPos.x, vecTargetPos.y, frontX, frontY,
		preferredDistance, angleLimit, invertAngleLimitTest, &spawnPosition, &curNodeId, &nextNodeId,
		&positionBetweenNodes, carClass == COPS && pWanted->m_nWantedLevel >= 1))
		return;
	int16 colliding;
	CWorld::FindObjectsKindaColliding(spawnPosition, 10.0f, true, &colliding, 2, nil, false, true, true, false, false);
	if (colliding)
		/* If something is already present in spawn position, do not create vehicle*/
		return;
	if (!ThePaths.TestCoorsCloseness(vecTargetPos, false, spawnPosition))
		/* Testing if spawn position can reach target position via valid path. */
		return;
	int16 idInNode = 0;
	CPathNode* pCurNode = &ThePaths.m_pathNodes[curNodeId];
	CPathNode* pNextNode = &ThePaths.m_pathNodes[nextNodeId];
	while (idInNode < pCurNode->numLinks &&
		ThePaths.m_connections[idInNode + pCurNode->firstLink] != nextNodeId)
		idInNode++;
	int16 connectionId = ThePaths.m_carPathConnections[idInNode + pCurNode->firstLink];
	CCarPathLink* pPathLink = &ThePaths.m_carPathLinks[connectionId];
	int16 lanesOnCurrentRoad = pPathLink->pathNodeIndex == nextNodeId ? pPathLink->numLeftLanes : pPathLink->numRightLanes;
	CVehicleModelInfo* pModelInfo = (CVehicleModelInfo*)CModelInfo::GetModelInfo(carModel);
	if (lanesOnCurrentRoad == 0 || pModelInfo->m_vehicleType == VEHICLE_TYPE_BIKE)
		/* Not spawning vehicle if road is one way and intended direction is opposide to that way. */
		/* Also not spawning bikes but they don't exist in final game. */
		return;
	CAutomobile* pCar = new CAutomobile(carModel, RANDOM_VEHICLE);
	pCar->AutoPilot.m_nPrevRouteNode = 0;
	pCar->AutoPilot.m_nCurrentRouteNode = curNodeId;
	pCar->AutoPilot.m_nNextRouteNode = nextNodeId;
	switch (carClass) {
	case POOR:
	case RICH:
	case EXEC:
	case WORKER:
	case SPECIAL:
	case BIG:
	case TAXI:
	case MAFIA:
	case TRIAD:
	case DIABLO:
	case YAKUZA:
	case YARDIE:
	case COLOMB:
	case NINES:
	case GANG8:
	case GANG9:
	{
		pCar->AutoPilot.m_nCruiseSpeed = CGeneral::GetRandomNumberInRange(9, 14);
		if (carClass == EXEC)
			pCar->AutoPilot.m_nCruiseSpeed = CGeneral::GetRandomNumberInRange(12, 18);
		else if (carClass == POOR || carClass == SPECIAL)
			pCar->AutoPilot.m_nCruiseSpeed = CGeneral::GetRandomNumberInRange(7, 10);
		CVehicleModelInfo* pVehicleInfo = (CVehicleModelInfo*)CModelInfo::GetModelInfo(pCar->GetModelIndex());
		if (pVehicleInfo->GetColModel()->boundingBox.max.y - pVehicleInfo->GetColModel()->boundingBox.min.y > 10.0f || carClass == BIG) {
			pCar->AutoPilot.m_nCruiseSpeed *= 3;
			pCar->AutoPilot.m_nCruiseSpeed /= 4;
		}
		pCar->AutoPilot.m_fMaxTrafficSpeed = pCar->AutoPilot.m_nCruiseSpeed;
		pCar->AutoPilot.m_nCarMission = MISSION_CRUISE;
		pCar->AutoPilot.m_nAnimationId = TEMPACT_NONE;
		pCar->AutoPilot.m_nDrivingStyle = DRIVINGSTYLE_STOP_FOR_CARS;
		break;
	}
	case COPS:
		pCar->AutoPilot.m_nAnimationId = TEMPACT_NONE;
		if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_pWanted->m_nWantedLevel != 0){
			pCar->AutoPilot.m_nCruiseSpeed = CCarAI::FindPoliceCarSpeedForWantedLevel(pCar);
			pCar->AutoPilot.m_fMaxTrafficSpeed = pCar->AutoPilot.m_nCruiseSpeed / 2;
			pCar->AutoPilot.m_nCarMission = CCarAI::FindPoliceCarMissionForWantedLevel();
			pCar->AutoPilot.m_nDrivingStyle = DRIVINGSTYLE_AVOID_CARS;
		}else{
			pCar->AutoPilot.m_nCruiseSpeed = CGeneral::GetRandomNumberInRange(12, 16);
			pCar->AutoPilot.m_fMaxTrafficSpeed = pCar->AutoPilot.m_nCruiseSpeed;
			pCar->AutoPilot.m_nDrivingStyle = DRIVINGSTYLE_STOP_FOR_CARS;
			pCar->AutoPilot.m_nCarMission = MISSION_CRUISE;
		}
		if (carModel == MI_FBICAR){
			pCar->m_currentColour1 = 0;
			pCar->m_currentColour2 = 0;
			/* FBI cars are gray in carcols, but we want them black if they going after player. */
		}
	default:
		break;
	}
	if (pCar && pCar->GetModelIndex() == MI_MRWHOOP)
		pCar->m_bSirenOrAlarm = true;
	pCar->AutoPilot.m_nNextPathNodeInfo = connectionId;
	pCar->AutoPilot.m_nCurrentLane = pCar->AutoPilot.m_nPreviousLane = CGeneral::GetRandomNumber() % lanesOnCurrentRoad;
	CColBox* boundingBox = &CModelInfo::GetModelInfo(pCar->GetModelIndex())->GetColModel()->boundingBox;
	float carLength = 1.0f + (boundingBox->max.y - boundingBox->min.y) / 2;
	float distanceBetweenNodes = (pCurNode->pos - pNextNode->pos).Magnitude2D();
	/* If car is so long that it doesn't fit between two car nodes, place it directly in the middle. */
	/* Otherwise put it at least in a way that full vehicle length fits between two nodes. */
	if (distanceBetweenNodes / 2 < carLength)
		positionBetweenNodes = 0.5f;
	else
		positionBetweenNodes = min(1.0f - carLength / distanceBetweenNodes, max(carLength / distanceBetweenNodes, positionBetweenNodes));
	pCar->AutoPilot.m_nNextDirection = (curNodeId >= nextNodeId) ? 1 : -1;
	if (pCurNode->numLinks == 1){
		/* Do not create vehicle if there is nowhere to go. */
		delete pCar;
		return;
	}
	int16 nextConnection = pCar->AutoPilot.m_nNextPathNodeInfo;
	int16 newLink;
	while (nextConnection == pCar->AutoPilot.m_nNextPathNodeInfo){
		newLink = CGeneral::GetRandomNumber() % pCurNode->numLinks;
		nextConnection = ThePaths.m_carPathConnections[newLink + pCurNode->firstLink];
	}
	pCar->AutoPilot.m_nCurrentPathNodeInfo = nextConnection;
	pCar->AutoPilot.m_nCurrentDirection = (ThePaths.m_connections[newLink + pCurNode->firstLink] >= curNodeId) ? 1 : -1;
	CVector2D vecBetweenNodes = pNextNode->pos - pCurNode->pos;
	float forwardX, forwardY;
	float distBetweenNodes = vecBetweenNodes.Magnitude();
	if (distanceBetweenNodes == 0.0f){
		forwardX = 1.0f;
		forwardY = 0.0f;
	}else{
		forwardX = vecBetweenNodes.x / distBetweenNodes;
		forwardY = vecBetweenNodes.y / distBetweenNodes;
	}
	/* I think the following might be some form of SetRotateZOnly. */
	/* Setting up direction between two car nodes. */
	pCar->GetForward() = CVector(forwardX, forwardY, 0.0f);
	pCar->GetRight() = CVector(forwardY, -forwardX, 0.0f);
	pCar->GetUp() = CVector(0.0f, 0.0f, 1.0f);

	float currentPathLinkForwardX = pCar->AutoPilot.m_nCurrentDirection * ThePaths.m_carPathLinks[pCar->AutoPilot.m_nCurrentPathNodeInfo].dirX;
	float currentPathLinkForwardY = pCar->AutoPilot.m_nCurrentDirection * ThePaths.m_carPathLinks[pCar->AutoPilot.m_nCurrentPathNodeInfo].dirY;
	float nextPathLinkForwardX = pCar->AutoPilot.m_nNextDirection * ThePaths.m_carPathLinks[pCar->AutoPilot.m_nNextPathNodeInfo].dirX;
	float nextPathLinkForwardY = pCar->AutoPilot.m_nNextDirection * ThePaths.m_carPathLinks[pCar->AutoPilot.m_nNextPathNodeInfo].dirY;

	CCarPathLink* pCurrentLink = &ThePaths.m_carPathLinks[pCar->AutoPilot.m_nCurrentPathNodeInfo];
	float currentLaneCoefficient = (pCurrentLink->numLeftLanes == 0) ? (0.5f - 0.5f * pCurrentLink->numRightLanes) :
		((pCurrentLink->numRightLanes == 0) ? (0.5f - 0.5f * pCurrentLink->numLeftLanes) : 0.5f);
	float roadShiftAlongCurrentNode = (pCar->AutoPilot.m_nPreviousLane + currentLaneCoefficient) * LANE_WIDTH;
	CCarPathLink* pNextLink = &ThePaths.m_carPathLinks[pCar->AutoPilot.m_nNextPathNodeInfo];
	float nextLaneCoefficient = (pNextLink->numLeftLanes == 0) ? (0.5f - 0.5f * pNextLink->numRightLanes) :
		((pNextLink->numRightLanes == 0) ? (0.5f - 0.5f * pNextLink->numLeftLanes) : 0.5f);
	float roadShiftAlongNextNode = (pCar->AutoPilot.m_nCurrentLane + nextLaneCoefficient) * LANE_WIDTH;
	CVector positionOnCurrentLinkIncludingLane(
		pCurrentLink->posX + roadShiftAlongCurrentNode * currentPathLinkForwardY,
		pCurrentLink->posY - roadShiftAlongCurrentNode * currentPathLinkForwardX,
		0.0f);
	CVector positionOnNextLinkIncludingLane(
		pNextLink->posX + roadShiftAlongNextNode * nextPathLinkForwardY,
		pNextLink->posY - roadShiftAlongNextNode * nextPathLinkForwardX,
		0.0f);
	float directionCurrentLinkX = pCurrentLink->dirX * pCar->AutoPilot.m_nCurrentDirection;
	float directionCurrentLinkY = pCurrentLink->dirY * pCar->AutoPilot.m_nCurrentDirection;
	float directionNextLinkX = pNextLink->dirX * pCar->AutoPilot.m_nNextDirection;
	float directionNextLinkY = pNextLink->dirY * pCar->AutoPilot.m_nNextDirection;
	/* We want to make a path between two links that may not have the same forward directions a curve. */
	pCar->AutoPilot.m_nCurveSpeedScale = CCurves::CalcSpeedScaleFactor(
		&positionOnCurrentLinkIncludingLane,
		&positionOnNextLinkIncludingLane,
		directionCurrentLinkX, directionCurrentLinkY,
		directionNextLinkX, directionNextLinkY
	) * (1000.0f / pCar->AutoPilot.m_fMaxTrafficSpeed);
#ifdef FIX_BUGS
	/* Casting timer to float is very unwanted. In this case it's not awful */
	/* but in CAutoPilot::ModifySpeed it can even cause crashes (see SilentPatch). */
	pCar->AutoPilot.m_nTimeEnteredCurve = CTimer::GetTimeInMilliseconds() -
		(uint32)((0.5f + positionBetweenNodes) * pCar->AutoPilot.m_nCurveSpeedScale);
#else
	pCar->AutoPilot.m_nTotalSpeedScaleFactor = CTimer::GetTimeInMilliseconds() -
		(0.5f + positionBetweenNodes) * pCar->AutoPilot.m_nSpeedScaleFactor;
#endif
	uint32 timeAlreadyInCurve = CTimer::GetTimeInMilliseconds() - pCar->AutoPilot.m_nTimeEnteredCurve;
	float positionAlongCurve = (float)timeAlreadyInCurve / pCar->AutoPilot.m_nCurveSpeedScale;
	CVector directionCurrentLink(directionCurrentLinkX, directionCurrentLinkY, 0.0f);
	CVector directionNextLink(directionNextLinkX, directionNextLinkY, 0.0f);
	CVector positionIncludingCurve;
	CVector directionIncludingCurve;
	CCurves::CalcCurvePoint(
		&positionOnCurrentLinkIncludingLane,
		&positionOnNextLinkIncludingLane,
		&directionCurrentLink,
		&directionNextLink,
		positionAlongCurve,
		pCar->AutoPilot.m_nCurveSpeedScale,
		&positionIncludingCurve,
		&directionIncludingCurve
	);
	CVector vectorBetweenNodes = pCurNode->pos - pNextNode->pos;
	CVector finalPosition = positionIncludingCurve + vectorBetweenNodes * 2.0f / vectorBetweenNodes.Magnitude();
	finalPosition.z = positionBetweenNodes * pNextNode->pos.z +
		(1.0f - positionBetweenNodes) * pCurNode->pos.z;
	float groundZ = INFINITE_Z;
	CColPoint colPoint;
	CEntity* pEntity;
	if (CWorld::ProcessVerticalLine(finalPosition, 1000.0f, colPoint, pEntity, true, false, false, false, true, false, nil))
		groundZ = colPoint.point.z;
	if (CWorld::ProcessVerticalLine(finalPosition, -1000.0f, colPoint, pEntity, true, false, false, false, true, false, nil)){
		if (ABS(colPoint.point.z - finalPosition.z) < ABS(groundZ - finalPosition.z))
			groundZ = colPoint.point.z;
	}
	if (groundZ == INFINITE_Z || ABS(groundZ - finalPosition.z) > 7.0f) {
		/* Failed to find ground or too far from expected position. */
		delete pCar;
		return;
	}
	finalPosition.z = groundZ + pCar->GetHeightAboveRoad();
	pCar->GetPosition() = finalPosition;
	pCar->SetMoveSpeed(directionIncludingCurve / 60.0f);
	CVector2D speedDifferenceWithTarget = (CVector2D)pCar->GetMoveSpeed() - vecPlayerSpeed;
	CVector2D distanceToTarget = positionIncludingCurve - vecTargetPos;
	switch (carClass) {
	case POOR:
	case RICH:
	case EXEC:
	case WORKER:
	case SPECIAL:
	case BIG:
	case TAXI:
	case MAFIA:
	case TRIAD:
	case DIABLO:
	case YAKUZA:
	case YARDIE:
	case COLOMB:
	case NINES:
	case GANG8:
	case GANG9:
		pCar->m_status = STATUS_SIMPLE;
		break;
	case COPS:
		pCar->m_status = (pCar->AutoPilot.m_nCarMission == MISSION_CRUISE) ? STATUS_SIMPLE : STATUS_PHYSICS;
		pCar->ChangeLawEnforcerState(1);
		break;
	default:
		break;
	}
	CVisibilityPlugins::SetClumpAlpha(pCar->GetClump(), 0);
	if (!pCar->GetIsOnScreen()){
		if ((vecTargetPos - pCar->GetPosition()).Magnitude2D() > 50.0f) {
			/* Too far away cars that are not visible aren't needed. */
			delete pCar;
			return;
		}
	}else if((vecTargetPos - pCar->GetPosition()).Magnitude2D() > TheCamera.GenerationDistMultiplier * 130.0f ||
		(vecTargetPos - pCar->GetPosition()).Magnitude2D() < TheCamera.GenerationDistMultiplier * 110.0f){
		delete pCar;
		return;
	}else if((TheCamera.GetPosition() - pCar->GetPosition()).Magnitude2D() < 90.0f * TheCamera.GenerationDistMultiplier){
		delete pCar;
		return;
	}
	CVehicleModelInfo* pVehicleModel = (CVehicleModelInfo*)CModelInfo::GetModelInfo(pCar->GetModelIndex());
	float radiusToTest = pVehicleModel->GetColModel()->boundingSphere.radius;
	if (testForCollision){
		CWorld::FindObjectsKindaColliding(pCar->GetPosition(), radiusToTest + 20.0f, true, &colliding, 2, nil, false, true, false, false, false);
		if (colliding){
			delete pCar;
			return;
		}
	}
	CWorld::FindObjectsKindaColliding(pCar->GetPosition(), radiusToTest, true, &colliding, 2, nil, false, true, false, false, false);
	if (colliding){
		delete pCar;
		return;
	}
	if (speedDifferenceWithTarget.x * distanceToTarget.x +
		speedDifferenceWithTarget.y * distanceToTarget.y >= 0.0f){
		delete pCar;
		return;
	}
	pVehicleModel->AvoidSameVehicleColour(&pCar->m_currentColour1, &pCar->m_currentColour2);
	CWorld::Add(pCar);
	if (carClass == COPS)
		CCarAI::AddPoliceOccupants(pCar);
	else
		pCar->SetUpDriver();
	if ((CGeneral::GetRandomNumber() & 0x3F) == 0){ /* 1/64 probability */
		pCar->m_status = STATUS_PHYSICS;
		pCar->AutoPilot.m_nDrivingStyle = DRIVINGSTYLE_AVOID_CARS;
		pCar->AutoPilot.m_nCruiseSpeed += 10;
	}
	if (carClass == COPS)
		LastTimeLawEnforcerCreated = CTimer::GetTimeInMilliseconds();
}

int32
CCarCtrl::ChooseModel(CZoneInfo* pZone, CVector* pPos, int* pClass) {
	int32 model = -1;;
	while (model == -1 || !CStreaming::HasModelLoaded(model)){
		int rnd = CGeneral::GetRandomNumberInRange(0, 1000);
		if (rnd < pZone->carThreshold[0])
			model = CCarCtrl::ChooseCarModel((*pClass = POOR));
		else if (rnd < pZone->carThreshold[1])
			model = CCarCtrl::ChooseCarModel((*pClass = RICH));
		else if (rnd < pZone->carThreshold[2])
			model = CCarCtrl::ChooseCarModel((*pClass = EXEC));
		else if (rnd < pZone->carThreshold[3])
			model = CCarCtrl::ChooseCarModel((*pClass = WORKER));
		else if (rnd < pZone->carThreshold[4])
			model = CCarCtrl::ChooseCarModel((*pClass = SPECIAL));
		else if (rnd < pZone->carThreshold[5])
			model = CCarCtrl::ChooseCarModel((*pClass = BIG));
		else if (rnd < pZone->copThreshold)
			*pClass = COPS, model = CCarCtrl::ChoosePoliceCarModel();
		else if (rnd < pZone->gangThreshold[0])
			model = CCarCtrl::ChooseGangCarModel((*pClass = MAFIA) - MAFIA);
		else if (rnd < pZone->gangThreshold[1])
			model = CCarCtrl::ChooseGangCarModel((*pClass = TRIAD) - MAFIA);
		else if (rnd < pZone->gangThreshold[2])
			model = CCarCtrl::ChooseGangCarModel((*pClass = DIABLO) - MAFIA);
		else if (rnd < pZone->gangThreshold[3])
			model = CCarCtrl::ChooseGangCarModel((*pClass = YAKUZA) - MAFIA);
		else if (rnd < pZone->gangThreshold[4])
			model = CCarCtrl::ChooseGangCarModel((*pClass = YARDIE) - MAFIA);
		else if (rnd < pZone->gangThreshold[5])
			model = CCarCtrl::ChooseGangCarModel((*pClass = COLOMB) - MAFIA);
		else if (rnd < pZone->gangThreshold[6])
			model = CCarCtrl::ChooseGangCarModel((*pClass = NINES) - MAFIA);
		else if (rnd < pZone->gangThreshold[7])
			model = CCarCtrl::ChooseGangCarModel((*pClass = GANG8) - MAFIA);
		else if (rnd < pZone->gangThreshold[8])
			model = CCarCtrl::ChooseGangCarModel((*pClass = GANG9) - MAFIA);
		else
			model = CCarCtrl::ChooseCarModel((*pClass = TAXI));
	}
	return model;
}

int32
CCarCtrl::ChooseCarModel(int32 vehclass)
{
	int32 model = -1;
	switch (vehclass) {
	case POOR:
	case RICH:
	case EXEC:
	case WORKER:
	case SPECIAL:
	case BIG:
	case TAXI:
	{
		if (TotalNumOfCarsOfRating[vehclass] == 0)
			debug("ChooseCarModel : No cars of type %d have been declared\n");
		model = CarArrays[vehclass][NextCarOfRating[vehclass]];
		int32 total = TotalNumOfCarsOfRating[vehclass];
		NextCarOfRating[vehclass] += 1 + CGeneral::GetRandomNumberInRange(0, total - 1);
		while (NextCarOfRating[vehclass] >= total)
			NextCarOfRating[vehclass] -= total;
		//NextCarOfRating[vehclass] %= total;
		TotalNumOfCarsOfRating[vehclass] = total; /* why... */
	}
	default:
		break;
	}
	return model;
}

bool
CCarCtrl::MapCouldMoveInThisArea(float x, float y)
{
	// bridge moves up and down
	return x > -342.0f && x < -219.0f &&
		y > -677.0f && y < -580.0f;
}

STARTPATCHES
InjectHook(0x416580, &CCarCtrl::GenerateRandomCars, PATCH_JUMP);
InjectHook(0x417EC0, &CCarCtrl::ChooseModel, PATCH_JUMP);
ENDPATCHES