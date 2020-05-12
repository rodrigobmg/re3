#include "common.h"

#include "Pools.h"

#include "Boat.h"
#include "CarCtrl.h"
#include "Population.h"
#include "ProjectileInfo.h"
#include "Streaming.h"
#include "Wanted.h"
#include "World.h"

CCPtrNodePool *CPools::ms_pPtrNodePool;
CEntryInfoNodePool *CPools::ms_pEntryInfoNodePool;
CPedPool *CPools::ms_pPedPool;
CVehiclePool *CPools::ms_pVehiclePool;
CBuildingPool *CPools::ms_pBuildingPool;
CTreadablePool *CPools::ms_pTreadablePool;
CObjectPool *CPools::ms_pObjectPool;
CDummyPool *CPools::ms_pDummyPool;
CAudioScriptObjectPool *CPools::ms_pAudioScriptObjectPool;

void
CPools::Initialise(void)
{
	ms_pPtrNodePool = new CCPtrNodePool(NUMPTRNODES);
	ms_pEntryInfoNodePool = new CEntryInfoNodePool(NUMENTRYINFOS);
	ms_pPedPool = new CPedPool(NUMPEDS);
	ms_pVehiclePool = new CVehiclePool(NUMVEHICLES);
	ms_pBuildingPool = new CBuildingPool(NUMBUILDINGS);
	ms_pTreadablePool = new CTreadablePool(NUMTREADABLES);
	ms_pObjectPool = new CObjectPool(NUMOBJECTS);
	ms_pDummyPool = new CDummyPool(NUMDUMMIES);
	ms_pAudioScriptObjectPool = new CAudioScriptObjectPool(NUMAUDIOSCRIPTOBJECTS);
}

void
CPools::ShutDown(void)
{
	debug("PtrNodes left %d\n", ms_pPtrNodePool->GetNoOfUsedSpaces());
	debug("EntryInfoNodes left %d\n", ms_pEntryInfoNodePool->GetNoOfUsedSpaces());
	debug("Peds left %d\n", ms_pPedPool->GetNoOfUsedSpaces());
	debug("Vehicles left %d\n", ms_pVehiclePool->GetNoOfUsedSpaces());
	debug("Buildings left %d\n", ms_pBuildingPool->GetNoOfUsedSpaces());
	debug("Treadables left %d\n", ms_pTreadablePool->GetNoOfUsedSpaces());
	debug("Objects left %d\n", ms_pObjectPool->GetNoOfUsedSpaces());
	debug("Dummys left %d\n", ms_pDummyPool->GetNoOfUsedSpaces());
	debug("AudioScriptObjects left %d\n", ms_pAudioScriptObjectPool->GetNoOfUsedSpaces());
	printf("Shutdown pool started\n");

	delete ms_pPtrNodePool;
	delete ms_pEntryInfoNodePool;
	delete ms_pPedPool;
	delete ms_pVehiclePool;
	delete ms_pBuildingPool;
	delete ms_pTreadablePool;
	delete ms_pObjectPool;
	delete ms_pDummyPool;
	delete ms_pAudioScriptObjectPool;

	printf("Shutdown pool done\n");
}

int32 CPools::GetPedRef(CPed *ped) { return ms_pPedPool->GetIndex(ped); }
CPed *CPools::GetPed(int32 handle) { return ms_pPedPool->GetAt(handle); }
int32 CPools::GetVehicleRef(CVehicle *vehicle) { return ms_pVehiclePool->GetIndex(vehicle); }
CVehicle *CPools::GetVehicle(int32 handle) { return ms_pVehiclePool->GetAt(handle); }
int32 CPools::GetObjectRef(CObject *object) { return ms_pObjectPool->GetIndex(object); }
CObject *CPools::GetObject(int32 handle) { return ms_pObjectPool->GetAt(handle); }

void
CPools::CheckPoolsEmpty()
{
	assert(ms_pPedPool->GetNoOfUsedSpaces() == 0);
	assert(ms_pVehiclePool->GetNoOfUsedSpaces() == 0);
	printf("pools have been cleared\n");
}


void
CPools::MakeSureSlotInObjectPoolIsEmpty(int32 slot)
{
	if (ms_pObjectPool->IsFreeSlot(slot)) return;

	CObject *object = ms_pObjectPool->GetSlot(slot);
	if (object->ObjectCreatedBy == TEMP_OBJECT) {
		CWorld::Remove(object);
		delete object;
	} else if (!CProjectileInfo::RemoveIfThisIsAProjectile(object)) {
		// relocate to another slot??
		CObject *newObject = new CObject(object->GetModelIndex(), false);
		CWorld::Remove(object);
#ifdef FIX_BUGS
		*newObject = *object;
#else
		memcpy(newObject, object, ms_pObjectPool->GetMaxEntrySize());
#endif
		CWorld::Add(newObject);
		object->m_rwObject = nil;
		delete object;
		newObject->m_pFirstReference = nil;
	}
}

void CPools::LoadVehiclePool(uint8* buf, uint32 size)
{
INITSAVEBUF
	int nNumCars = ReadSaveBuf<int>(buf);
	int nNumBoats = ReadSaveBuf<int>(buf);
	for (int i = 0; i < nNumCars + nNumBoats; i++) {
		uint32 type = ReadSaveBuf<uint32>(buf);
		int16 model = ReadSaveBuf<int16>(buf);
		CStreaming::RequestModel(model, STREAMFLAGS_DEPENDENCY);
		CStreaming::LoadAllRequestedModels(false);
		int32 slot = ReadSaveBuf<int32>(buf);
		CVehicle* pVehicle;
#ifdef COMPATIBLE_SAVES
		if (type == VEHICLE_TYPE_BOAT)
			pVehicle = new(slot) CBoat(model, RANDOM_VEHICLE);
		else if (type == VEHICLE_TYPE_CAR)
			pVehicle = new(slot) CAutomobile(model, RANDOM_VEHICLE);
		else
			assert(0);
		--CCarCtrl::NumRandomCars;
		pVehicle->Load(buf);
		CWorld::Add(pVehicle);
#else
		char* vbuf = new char[Max(CAutomobile::nSaveStructSize, CBoat::nSaveStructSize)];
		if (type == VEHICLE_TYPE_BOAT) {
			memcpy(vbuf, buf, sizeof(CBoat));
			SkipSaveBuf(buf, sizeof(CBoat));
			CBoat* pBoat = new(slot) CBoat(model, RANDOM_VEHICLE);
			pVehicle = pBoat;
			--CCarCtrl::NumRandomCars;
		}
		else if (type == VEHICLE_TYPE_CAR) {
			memcpy(vbuf, buf, sizeof(CAutomobile));
			SkipSaveBuf(buf, sizeof(CAutomobile));
			CStreaming::RequestModel(model, 0); // is it needed?
			CStreaming::LoadAllRequestedModels(false);
			CAutomobile* pAutomobile = new(slot) CAutomobile(model, RANDOM_VEHICLE);
			pVehicle = pAutomobile;
			CCarCtrl::NumRandomCars--; // why?
			pAutomobile->Damage = ((CAutomobile*)vbuf)->Damage;
			pAutomobile->SetupDamageAfterLoad();
		}
		else
			assert(0);
		CVehicle* pBufferVehicle = (CVehicle*)vbuf;
		pVehicle->GetMatrix() = pBufferVehicle->GetMatrix();
		pVehicle->VehicleCreatedBy = pBufferVehicle->VehicleCreatedBy;
		pVehicle->m_currentColour1 = pBufferVehicle->m_currentColour1;
		pVehicle->m_currentColour2 = pBufferVehicle->m_currentColour2;
		pVehicle->m_nAlarmState = pBufferVehicle->m_nAlarmState;
		pVehicle->m_nNumMaxPassengers = pBufferVehicle->m_nNumMaxPassengers;
		pVehicle->field_1D0[0] = pBufferVehicle->field_1D0[0];
		pVehicle->field_1D0[1] = pBufferVehicle->field_1D0[1];
		pVehicle->field_1D0[2] = pBufferVehicle->field_1D0[2];
		pVehicle->field_1D0[3] = pBufferVehicle->field_1D0[3];
		pVehicle->m_fSteerAngle = pBufferVehicle->m_fSteerAngle;
		pVehicle->m_fGasPedal = pBufferVehicle->m_fGasPedal;
		pVehicle->m_fBrakePedal = pBufferVehicle->m_fBrakePedal;
		pVehicle->bIsLawEnforcer = pBufferVehicle->bIsLawEnforcer;
		pVehicle->bIsLocked = pBufferVehicle->bIsLocked;
		pVehicle->bEngineOn = pBufferVehicle->bEngineOn;
		pVehicle->bIsHandbrakeOn = pBufferVehicle->bIsHandbrakeOn;
		pVehicle->bLightsOn = pBufferVehicle->bLightsOn;
		pVehicle->bFreebies = pBufferVehicle->bFreebies;
		pVehicle->m_fHealth = pBufferVehicle->m_fHealth;
		pVehicle->m_nCurrentGear = pBufferVehicle->m_nCurrentGear;
		pVehicle->m_fChangeGearTime = pBufferVehicle->m_fChangeGearTime;
		pVehicle->m_nTimeOfDeath = pBufferVehicle->m_nTimeOfDeath;
#ifdef FIX_BUGS //must be copypaste
		pVehicle->m_nBombTimer = pBufferVehicle->m_nBombTimer;
#else
		pVehicle->m_nTimeOfDeath = pBufferVehicle->m_nTimeOfDeath;
#endif
		pVehicle->m_nDoorLock = pBufferVehicle->m_nDoorLock;
		pVehicle->SetStatus(pBufferVehicle->GetStatus());
		pVehicle->SetType(pBufferVehicle->GetType());
		(pVehicle->GetAddressOfEntityProperties())[0] = (pBufferVehicle->GetAddressOfEntityProperties())[0];
		(pVehicle->GetAddressOfEntityProperties())[1] = (pBufferVehicle->GetAddressOfEntityProperties())[1];
		pVehicle->AutoPilot = pBufferVehicle->AutoPilot;
		CWorld::Add(pVehicle);
		delete[] vbuf;
#endif
	}
VALIDATESAVEBUF(size)
}

void CPools::SaveVehiclePool(uint8* buf, uint32* size)
{
INITSAVEBUF
	int nNumCars = 0;
	int nNumBoats = 0;
	int nPoolSize = GetVehiclePool()->GetSize();
	for (int i = 0; i < nPoolSize; i++) {
		CVehicle* pVehicle = GetVehiclePool()->GetSlot(i);
		if (!pVehicle)
			continue;
		bool bHasPassenger = false;
		for (int j = 0; j < ARRAY_SIZE(pVehicle->pPassengers); j++) {
			if (pVehicle->pPassengers[j])
				bHasPassenger = true;
		}
		if (!pVehicle->pDriver && !bHasPassenger) {
			if (pVehicle->IsCar() && pVehicle->VehicleCreatedBy == MISSION_VEHICLE)
				++nNumCars;
			if (pVehicle->IsBoat() && pVehicle->VehicleCreatedBy == MISSION_VEHICLE)
				++nNumBoats;
		}
	}
	*size = nNumCars * (sizeof(uint32) + sizeof(int16) + sizeof(int32) + CAutomobile::nSaveStructSize) + sizeof(int) +
		nNumBoats * (sizeof(uint32) + sizeof(int16) + sizeof(int32) + CBoat::nSaveStructSize) + sizeof(int);
	WriteSaveBuf(buf, nNumCars);
	WriteSaveBuf(buf, nNumBoats);
	for (int i = 0; i < nPoolSize; i++) {
		CVehicle* pVehicle = GetVehiclePool()->GetSlot(i);
		if (!pVehicle)
			continue;
		bool bHasPassenger = false;
		for (int j = 0; j < ARRAY_SIZE(pVehicle->pPassengers); j++) {
			if (pVehicle->pPassengers[j])
				bHasPassenger = true;
		}
		if (!pVehicle->pDriver && !bHasPassenger) {
#ifdef COMPATIBLE_SAVES
			if ((pVehicle->IsCar() || pVehicle->IsBoat()) && pVehicle->VehicleCreatedBy == MISSION_VEHICLE) {
				WriteSaveBuf<uint32>(buf, pVehicle->m_vehType);
				WriteSaveBuf<int16>(buf, pVehicle->GetModelIndex());
				WriteSaveBuf<int32>(buf, GetVehicleRef(pVehicle));
				pVehicle->Save(buf);
			}
#else
			if (pVehicle->IsCar() && pVehicle->VehicleCreatedBy == MISSION_VEHICLE) {
				WriteSaveBuf(buf, (uint32)pVehicle->m_vehType);
				WriteSaveBuf(buf, pVehicle->GetModelIndex());
				WriteSaveBuf(buf, GetVehicleRef(pVehicle));
				memcpy(buf, pVehicle, sizeof(CAutomobile));
				SkipSaveBuf(buf, sizeof(CAutomobile));
			}
			if (pVehicle->IsBoat() && pVehicle->VehicleCreatedBy == MISSION_VEHICLE) {
				WriteSaveBuf(buf, (uint32)pVehicle->m_vehType);
				WriteSaveBuf(buf, pVehicle->GetModelIndex());
				WriteSaveBuf(buf, GetVehicleRef(pVehicle));
				memcpy(buf, pVehicle, sizeof(CBoat));
				SkipSaveBuf(buf, sizeof(CBoat));
			}
#endif
		}
	}
VALIDATESAVEBUF(*size)
}

void CPools::SaveObjectPool(uint8* buf, uint32* size)
{
INITSAVEBUF
	CProjectileInfo::RemoveAllProjectiles();
	CObject::DeleteAllTempObjects();
	int nObjects = 0;
	int nPoolSize = GetObjectPool()->GetSize();
	for (int i = 0; i < nPoolSize; i++) {
		CObject* pObject = GetObjectPool()->GetSlot(i);
		if (!pObject)
			continue;
		if (pObject->ObjectCreatedBy == MISSION_OBJECT)
			++nObjects;
	}
	*size = nObjects * (sizeof(int16) + sizeof(int) + sizeof(CCompressedMatrixNotAligned) + sizeof(uint32) +
		sizeof(float) + sizeof(CCompressedMatrixNotAligned) + sizeof(uint32) + sizeof(int8) + 7 * sizeof(bool) + sizeof(float) +
		sizeof(int8) + sizeof(int8) + sizeof(uint32) + 2 * sizeof(uint32)) + sizeof(int);
	WriteSaveBuf(buf, nObjects);
	for (int i = 0; i < nPoolSize; i++) {
		CObject* pObject = GetObjectPool()->GetSlot(i);
		if (!pObject)
			continue;
		if (pObject->ObjectCreatedBy == MISSION_OBJECT) {
			bool bIsPickup = pObject->bIsPickup;
			bool bPickupObjWithMessage = pObject->bPickupObjWithMessage;
			bool bOutOfStock = pObject->bOutOfStock;
			bool bGlassCracked = pObject->bGlassCracked;
			bool bGlassBroken = pObject->bGlassBroken;
			bool bHasBeenDamaged = pObject->bHasBeenDamaged;
			bool bUseVehicleColours = pObject->bUseVehicleColours;
			CCompressedMatrixNotAligned tmp;
			WriteSaveBuf(buf, pObject->GetModelIndex());
			WriteSaveBuf(buf, GetObjectRef(pObject));
			tmp.CompressFromFullMatrix(pObject->GetMatrix());
			WriteSaveBuf(buf, tmp);
			WriteSaveBuf(buf, (uint32)0); // game writes ununitialized data here
			WriteSaveBuf(buf, pObject->m_fUprootLimit);
			tmp.CompressFromFullMatrix(pObject->m_objectMatrix);
			WriteSaveBuf(buf, tmp);
			WriteSaveBuf(buf, (uint32)0); // same
			WriteSaveBuf(buf, pObject->ObjectCreatedBy);
			WriteSaveBuf(buf, bIsPickup);
			WriteSaveBuf(buf, bPickupObjWithMessage);
			WriteSaveBuf(buf, bOutOfStock);
			WriteSaveBuf(buf, bGlassCracked);
			WriteSaveBuf(buf, bGlassBroken);
			WriteSaveBuf(buf, bHasBeenDamaged);
			WriteSaveBuf(buf, bUseVehicleColours);
			WriteSaveBuf(buf, pObject->m_fCollisionDamageMultiplier);
			WriteSaveBuf(buf, pObject->m_nCollisionDamageEffect);
			WriteSaveBuf(buf, pObject->m_nSpecialCollisionResponseCases);
			WriteSaveBuf(buf, pObject->m_nEndOfLifeTime);
#ifdef COMPATIBLE_SAVES
			pObject->SaveEntityFlags(buf);
#else
			WriteSaveBuf(buf, (pObject->GetAddressOfEntityProperties())[0]);
			WriteSaveBuf(buf, (pObject->GetAddressOfEntityProperties())[1]);
#endif
		}
	}
VALIDATESAVEBUF(*size)
}

void CPools::LoadObjectPool(uint8* buf, uint32 size)
{
INITSAVEBUF
	int nObjects = ReadSaveBuf<int>(buf);
	for (int i = 0; i < nObjects; i++) {
		int16 mi = ReadSaveBuf<int16>(buf);
		int ref = ReadSaveBuf<int>(buf);
		char* obuf = new char[sizeof(CObject)];
		CObject* pBufferObject = (CObject*)obuf;
		CCompressedMatrixNotAligned tmp;
		tmp = ReadSaveBuf<CCompressedMatrixNotAligned>(buf);
		tmp.DecompressIntoFullMatrix(pBufferObject->GetMatrix());
		ReadSaveBuf<uint32>(buf);
		pBufferObject->m_fUprootLimit = ReadSaveBuf<float>(buf);
		tmp = ReadSaveBuf<CCompressedMatrixNotAligned>(buf);
		tmp.DecompressIntoFullMatrix(pBufferObject->m_objectMatrix);
		ReadSaveBuf<uint32>(buf);
		pBufferObject->ObjectCreatedBy = ReadSaveBuf<int8>(buf);
		pBufferObject->bIsPickup = ReadSaveBuf<bool>(buf);
		pBufferObject->bPickupObjWithMessage = ReadSaveBuf<bool>(buf);
		pBufferObject->bOutOfStock = ReadSaveBuf<bool>(buf);
		pBufferObject->bGlassCracked = ReadSaveBuf<bool>(buf);
		pBufferObject->bGlassBroken = ReadSaveBuf<bool>(buf);
		pBufferObject->bHasBeenDamaged = ReadSaveBuf<bool>(buf);
		pBufferObject->bUseVehicleColours = ReadSaveBuf<bool>(buf);
		pBufferObject->m_fCollisionDamageMultiplier = ReadSaveBuf<float>(buf);
		pBufferObject->m_nCollisionDamageEffect = ReadSaveBuf<uint8>(buf);
		pBufferObject->m_nSpecialCollisionResponseCases = ReadSaveBuf<uint8>(buf);
		pBufferObject->m_nEndOfLifeTime = ReadSaveBuf<uint32>(buf);
#ifndef COMPATIBLE_SAVES
		(pBufferObject->GetAddressOfEntityProperties())[0] = ReadSaveBuf<uint32>(buf);
		(pBufferObject->GetAddressOfEntityProperties())[1] = ReadSaveBuf<uint32>(buf);
#endif
		if (GetObjectPool()->GetSlot(ref >> 8))
			CPopulation::ConvertToDummyObject(GetObjectPool()->GetSlot(ref >> 8));
		CObject* pObject = new(ref) CObject(mi, false);
		pObject->GetMatrix() = pBufferObject->GetMatrix();
#ifdef COMPATIBLE_SAVES
		pObject->LoadEntityFlags(buf);
#endif
		pObject->m_fUprootLimit = pBufferObject->m_fUprootLimit;
		pObject->m_objectMatrix = pBufferObject->m_objectMatrix;
		pObject->ObjectCreatedBy = pBufferObject->ObjectCreatedBy;
		pObject->bIsPickup = pBufferObject->bIsPickup;
		pObject->bPickupObjWithMessage = pBufferObject->bPickupObjWithMessage;
		pObject->bOutOfStock = pBufferObject->bOutOfStock;
		pObject->bGlassCracked = pBufferObject->bGlassCracked;
		pObject->bGlassBroken = pBufferObject->bGlassBroken;
		pObject->bHasBeenDamaged = pBufferObject->bHasBeenDamaged;
		pObject->bUseVehicleColours = pBufferObject->bUseVehicleColours;
		pObject->m_fCollisionDamageMultiplier = pBufferObject->m_fCollisionDamageMultiplier;
		pObject->m_nCollisionDamageEffect = pBufferObject->m_nCollisionDamageEffect;
		pObject->m_nSpecialCollisionResponseCases = pBufferObject->m_nSpecialCollisionResponseCases;
		pObject->m_nEndOfLifeTime = pBufferObject->m_nEndOfLifeTime;
#ifndef COMPATIBLE_SAVES
		(pObject->GetAddressOfEntityProperties())[0] = (pBufferObject->GetAddressOfEntityProperties())[0];
		(pObject->GetAddressOfEntityProperties())[1] = (pBufferObject->GetAddressOfEntityProperties())[1];
#endif
		pObject->bHasCollided = false;
		CWorld::Add(pObject);
		delete[] obuf;
	}
VALIDATESAVEBUF(size)
}

void CPools::SavePedPool(uint8* buf, uint32* size)
{
INITSAVEBUF
	int nNumPeds = 0;
	int nPoolSize = GetPedPool()->GetSize();
	for (int i = 0; i < nPoolSize; i++) {
		CPed* pPed = GetPedPool()->GetSlot(i);
		if (!pPed)
			continue;
		if (!pPed->bInVehicle && pPed->m_nPedType == PEDTYPE_PLAYER1)
			nNumPeds++;
	}
	*size = sizeof(int) + nNumPeds * (sizeof(uint32) + sizeof(int16) + sizeof(int) + CPlayerPed::nSaveStructSize +
		sizeof(CWanted::MaximumWantedLevel) + sizeof(CWanted::nMaximumWantedLevel) + MAX_MODEL_NAME);
	WriteSaveBuf(buf, nNumPeds);
	for (int i = 0; i < nPoolSize; i++) {
		CPed* pPed = GetPedPool()->GetSlot(i);
		if (!pPed)
			continue;
		if (!pPed->bInVehicle && pPed->m_nPedType == PEDTYPE_PLAYER1) {
			WriteSaveBuf(buf, pPed->m_nPedType);
			WriteSaveBuf(buf, pPed->GetModelIndex());
			WriteSaveBuf(buf, GetPedRef(pPed));
#ifdef COMPATIBLE_SAVES
			pPed->Save(buf);
#else
			memcpy(buf, pPed, sizeof(CPlayerPed));
			SkipSaveBuf(buf, sizeof(CPlayerPed));
#endif
			WriteSaveBuf(buf, CWanted::MaximumWantedLevel);
			WriteSaveBuf(buf, CWanted::nMaximumWantedLevel);
			memcpy(buf, CModelInfo::GetModelInfo(pPed->GetModelIndex())->GetName(), MAX_MODEL_NAME);
			SkipSaveBuf(buf, MAX_MODEL_NAME);
		}
	}
VALIDATESAVEBUF(*size);
}

void CPools::LoadPedPool(uint8* buf, uint32 size)
{
INITSAVEBUF
	int nPeds = ReadSaveBuf<int>(buf);
	for (int i = 0; i < nPeds; i++) {
		uint32 pedtype = ReadSaveBuf<uint32>(buf);
		int16 model = ReadSaveBuf<int16>(buf);
		int ref = ReadSaveBuf<int>(buf);
#ifdef COMPATIBLE_SAVES
		CPed* pPed;

		char name[MAX_MODEL_NAME];
		// Unfortunate hack: player model is stored after ped structure.
		// It could be avoided by just using "player" because in practice it is always true.
		memcpy(name, buf + CPlayerPed::nSaveStructSize + 2 * sizeof(int32), MAX_MODEL_NAME);
		CStreaming::RequestSpecialModel(model, name, STREAMFLAGS_DONT_REMOVE);
		CStreaming::LoadAllRequestedModels(false);

		if (pedtype == PEDTYPE_PLAYER1)
			pPed = new(ref) CPlayerPed();
		else
			assert(0);

		pPed->Load(buf);
		if (pedtype == PEDTYPE_PLAYER1) {
			CWanted::MaximumWantedLevel = ReadSaveBuf<int32>(buf);
			CWanted::nMaximumWantedLevel = ReadSaveBuf<int32>(buf);
			SkipSaveBuf(buf, MAX_MODEL_NAME);
		}

		if (pedtype == PEDTYPE_PLAYER1) {
			pPed->m_wepAccuracy = 100;
			CWorld::Players[0].m_pPed = (CPlayerPed*)pPed;
		}
		CWorld::Add(pPed);
#else
		char* pbuf = new char[sizeof(CPlayerPed)];
		CPlayerPed* pBufferPlayer = (CPlayerPed*)pbuf;
		CPed* pPed;
		char name[MAX_MODEL_NAME];
		// the code implies that there was idea to load non-player ped
		if (pedtype == PEDTYPE_PLAYER1) { // always true
			memcpy(pbuf, buf, sizeof(CPlayerPed));
			SkipSaveBuf(buf, sizeof(CPlayerPed));
			CWanted::MaximumWantedLevel = ReadSaveBuf<int32>(buf);
			CWanted::nMaximumWantedLevel = ReadSaveBuf<int32>(buf);
			memcpy(name, buf, MAX_MODEL_NAME);
			SkipSaveBuf(buf, MAX_MODEL_NAME);
		}
		CStreaming::RequestSpecialModel(model, name, STREAMFLAGS_DONT_REMOVE);
		CStreaming::LoadAllRequestedModels(false);
		if (pedtype == PEDTYPE_PLAYER1) {
			CPlayerPed* pPlayerPed = new(ref) CPlayerPed();
			for (int i = 0; i < ARRAY_SIZE(pPlayerPed->m_nTargettableObjects); i++)
				pPlayerPed->m_nTargettableObjects[i] = pBufferPlayer->m_nTargettableObjects[i];
			pPlayerPed->m_fMaxStamina = pBufferPlayer->m_fMaxStamina;
			pPed = pPlayerPed;
		}
		pPed->GetPosition() = pBufferPlayer->GetPosition();
		pPed->m_fHealth = pBufferPlayer->m_fHealth;
		pPed->m_fArmour = pBufferPlayer->m_fArmour;
		pPed->CharCreatedBy = pBufferPlayer->CharCreatedBy;
		pPed->m_currentWeapon = 0;
		pPed->m_maxWeaponTypeAllowed = pBufferPlayer->m_maxWeaponTypeAllowed;
		for (int i = 0; i < WEAPONTYPE_TOTAL_INVENTORY_WEAPONS; i++)
			pPed->m_weapons[i] = pBufferPlayer->m_weapons[i];

		if (pedtype == PEDTYPE_PLAYER1) {
			pPed->m_wepAccuracy = 100;
			CWorld::Players[0].m_pPed = (CPlayerPed*)pPed;
		}
		CWorld::Add(pPed);
		delete[] pbuf;
#endif
	}
VALIDATESAVEBUF(size)
}
