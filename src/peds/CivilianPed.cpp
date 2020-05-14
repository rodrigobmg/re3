#include "common.h"

#include "CivilianPed.h"
#include "Phones.h"
#include "General.h"
#include "PlayerPed.h"
#include "Wanted.h"
#include "DMAudio.h"
#include "World.h"
#include "Vehicle.h"
#include "SurfaceTable.h"
#include "Weather.h"
#include "PedAttractor.h"

CCivilianPed::CCivilianPed(ePedType pedtype, uint32 mi) : CPed(pedtype)
{
	SetModelIndex(mi);
	for (int i = 0; i < ARRAY_SIZE(m_nearPeds); i++) {
		m_nearPeds[i] = nil;
	}
	m_nAttractorCycleState = 0;
	m_bAttractorUnk = (CGeneral::GetRandomNumberInRange(0.0f, 1.0f) < 1.25f);
}

void
CCivilianPed::CivilianAI(void)
{
	if (CTimer::GetTimeInMilliseconds() <= m_fleeTimer || m_objective != OBJECTIVE_NONE && !bRespondsToThreats
		|| !IsPedInControl())  {

		if (m_objective == OBJECTIVE_GUARD_SPOT)
			return;

		if (m_objective == OBJECTIVE_KILL_CHAR_ON_FOOT || m_objective == OBJECTIVE_KILL_CHAR_ANY_MEANS) {
			if (m_pedInObjective && m_pedInObjective->IsPlayer())
				return;
		}
		if (CTimer::GetTimeInMilliseconds() <= m_lookTimer)
			return;

		uint32 closestThreatFlag = ScanForThreats();
		if (closestThreatFlag == PED_FLAG_EXPLOSION) {
			float angleToFace = CGeneral::GetRadianAngleBetweenPoints(
				m_eventOrThreat.x,  m_eventOrThreat.y,
				GetPosition().x, GetPosition().y);
			SetLookFlag(angleToFace, true);
			SetLookTimer(500);

		} else if (closestThreatFlag == PED_FLAG_GUN) {
			SetLookFlag(m_threatEntity, true);
			SetLookTimer(500);
		}
		return;
	}
	uint32 closestThreatFlag = ScanForThreats();
	if (closestThreatFlag == PED_FLAG_GUN) {
		if (!m_threatEntity || !m_threatEntity->IsPed())
			return;

		CPed *threatPed = (CPed*)m_threatEntity;
		float threatDistSqr = (m_threatEntity->GetPosition() - GetPosition()).MagnitudeSqr2D();
		if (m_pedStats->m_fear <= m_pedStats->m_lawfulness) {
			if (m_pedStats->m_temper <= m_pedStats->m_fear) {
				if (!threatPed->IsPlayer() || !RunToReportCrime(CRIME_POSSESSION_GUN)) {
					if (threatDistSqr < sq(10.0f)) {
						Say(SOUND_PED_FLEE_SPRINT);
						SetFindPathAndFlee(m_threatEntity, 10000);
					} else {
						SetFindPathAndFlee(m_threatEntity->GetPosition(), 5000, true);
					}
				}
			} else if (m_objective != OBJECTIVE_NONE || GetWeapon()->IsTypeMelee()) {
				SetFindPathAndFlee(m_threatEntity, 5000);
				if (threatDistSqr < sq(20.0f)) {
					SetMoveState(PEDMOVE_RUN);
					Say(SOUND_PED_FLEE_SPRINT);
				} else {
					SetMoveState(PEDMOVE_WALK);
				}
			} else if (threatPed->IsPlayer() && FindPlayerPed()->m_pWanted->m_CurrentCops != 0)  {
				SetFindPathAndFlee(m_threatEntity, 5000);
				if (threatDistSqr < sq(10.0f)) {
					SetMoveState(PEDMOVE_RUN);
				} else {
					SetMoveState(PEDMOVE_WALK);
				}
			} else {
				SetObjective(OBJECTIVE_KILL_CHAR_ON_FOOT, m_threatEntity);
			}
		} else {
			if (threatDistSqr < sq(10.0f)) {
				Say(SOUND_PED_FLEE_SPRINT);
				SetFindPathAndFlee(m_threatEntity, 10000);
				SetMoveState(PEDMOVE_SPRINT);
			} else {
				Say(SOUND_PED_FLEE_SPRINT);
				SetFindPathAndFlee(m_threatEntity, 5000);
				SetMoveState(PEDMOVE_RUN);
			}
		}
		SetLookFlag(m_threatEntity, false);
		SetLookTimer(500);
	} else if (closestThreatFlag == PED_FLAG_DEADPEDS) {
		float eventDistSqr = (m_pEventEntity->GetPosition() - GetPosition()).MagnitudeSqr2D();
		if (IsGangMember() && m_nPedType == ((CPed*)m_pEventEntity)->m_nPedType) {
			if (eventDistSqr < sq(5.0f)) {
				SetFindPathAndFlee(m_pEventEntity, 2000);
				SetMoveState(PEDMOVE_RUN);
			}
		} else if (IsGangMember() || eventDistSqr > sq(5.0f)) {
			bool investigateDeadPed = true;
			CEntity *killerOfDeadPed = ((CPed*)m_pEventEntity)->m_threatEntity;
			if (killerOfDeadPed && killerOfDeadPed->IsPed()) {
				CVector killerPos = killerOfDeadPed->GetPosition();
				CVector deadPedPos = m_pEventEntity->GetPosition();
				if (CVector2D(killerPos - deadPedPos).MagnitudeSqr() < sq(10.0f))
					investigateDeadPed = false;
			}

#ifdef TOGGLEABLE_BETA_FEATURES
			eCrimeType crime = (((CPed*)m_pEventEntity)->m_ped_flagI40 ?
				(((CPed*)m_pEventEntity)->m_nPedType == PEDTYPE_COP ? CRIME_RUNOVER_COP : CRIME_RUNOVER_PED) :
				(((CPed*)m_pEventEntity)->m_nPedType == PEDTYPE_COP ? CRIME_SHOOT_COP : CRIME_SHOOT_PED));
			bool eligibleToReport = bMakePedsRunToPhonesToReportCrimes && killerOfDeadPed && killerOfDeadPed->IsPed() && ((CPed*)killerOfDeadPed)->IsPlayer() &&
				m_pedStats->m_fear <= m_pedStats->m_lawfulness && m_pedStats->m_temper <= m_pedStats->m_fear;
			if (IsGangMember() || !eligibleToReport || !RunToReportCrime(crime))
#endif
			if (investigateDeadPed)
				SetInvestigateEvent(EVENT_DEAD_PED, CVector2D(m_pEventEntity->GetPosition()), 1.0f, 20000, 0.0f);

		} else {
#ifdef TOGGLEABLE_BETA_FEATURES
			CEntity* killerOfDeadPed = ((CPed*)m_pEventEntity)->m_threatEntity;
			eCrimeType crime = (((CPed*)m_pEventEntity)->m_ped_flagI40 ?
				(((CPed*)m_pEventEntity)->m_nPedType == PEDTYPE_COP ? CRIME_RUNOVER_COP : CRIME_RUNOVER_PED) :
				(((CPed*)m_pEventEntity)->m_nPedType == PEDTYPE_COP ? CRIME_SHOOT_COP : CRIME_SHOOT_PED));
			bool eligibleToReport = bMakePedsRunToPhonesToReportCrimes && killerOfDeadPed && killerOfDeadPed->IsPed() && ((CPed*)killerOfDeadPed)->IsPlayer() &&
				m_pedStats->m_fear <= m_pedStats->m_lawfulness && m_pedStats->m_temper <= m_pedStats->m_fear;
			if(!eligibleToReport || !RunToReportCrime(crime))
#endif
			{
				SetFindPathAndFlee(m_pEventEntity, 5000);
				SetMoveState(PEDMOVE_RUN);
			}
		}
	} else if (closestThreatFlag == PED_FLAG_EXPLOSION) {
		CVector2D eventDistVec = m_eventOrThreat - GetPosition();
		float eventDistSqr = eventDistVec.MagnitudeSqr();
		if (eventDistSqr < sq(20.0f)) {
			Say(SOUND_PED_FLEE_SPRINT);
			SetFlee(m_eventOrThreat, 2000);
			float angleToFace = CGeneral::GetRadianAngleBetweenPoints(
				m_eventOrThreat.x, m_eventOrThreat.y,
				GetPosition().x, GetPosition().y);
			SetLookFlag(angleToFace, true);
			SetLookTimer(500);
		} else if (eventDistSqr < sq(40.0f)) {
			if (bGonnaInvestigateEvent) {
				if (CharCreatedBy != MISSION_CHAR && !IsGangMember())
					SetInvestigateEvent(EVENT_EXPLOSION, m_eventOrThreat, 6.0f, 30000, 0.0f);

			} else {
				float eventHeading = CGeneral::GetRadianAngleBetweenPoints(eventDistVec.x, eventDistVec.y, 0.0f, 0.0f);
				eventHeading = CGeneral::LimitRadianAngle(eventHeading);
				if (eventHeading < 0.0f)
					eventHeading = eventHeading + TWOPI;

				SetWanderPath(eventHeading / 8.0f);
			}
		}
	} else {
		if (m_threatEntity && m_threatEntity->IsPed()) {
			CPed *threatPed = (CPed*)m_threatEntity;
			if (m_pedStats->m_fear <= 100 - threatPed->m_pedStats->m_temper && threatPed->m_nPedType != PEDTYPE_COP) {
				if (threatPed->GetWeapon(m_currentWeapon).IsTypeMelee() || !GetWeapon()->IsTypeMelee()) {
					if (threatPed->IsPlayer() && FindPlayerPed()->m_pWanted->m_CurrentCops != 0) {
						if (m_objective == OBJECTIVE_KILL_CHAR_ON_FOOT || m_objective == OBJECTIVE_KILL_CHAR_ANY_MEANS) {
							SetFindPathAndFlee(m_threatEntity, 10000);
						}
					} else {
						SetObjective(OBJECTIVE_KILL_CHAR_ON_FOOT, m_threatEntity);
					}
				}
			} else {
				SetFindPathAndFlee(m_threatEntity, 10000, true);
			}
		}
	}
}

void
CCivilianPed::ProcessControl(void)
{
	CPed::ProcessControl();

	if (bWasPostponed)
		return;

	if (DyingOrDead())
		return;

	GetWeapon()->Update(m_audioEntityId);
	switch (m_nPedState) {
		case PED_WANDER_RANGE:
		case PED_WANDER_PATH:
			if (IsVisible())
				ScanForInterestingStuff();
			break;
		case PED_SEEK_ENTITY:
			if (!m_pSeekTarget) {
				RestorePreviousState();
				break;
			}
			m_vecSeekPos = m_pSeekTarget->GetPosition();

			// fall through
		case PED_SEEK_POS:
			if (Seek()) {
				if ((m_objective == OBJECTIVE_GOTO_AREA_ON_FOOT || m_objective == OBJECTIVE_RUN_TO_AREA) && m_pNextPathNode) {
					m_pNextPathNode = nil;
#ifdef TOGGLEABLE_BETA_FEATURES
				} else if (bRunningToPhone && m_objective < OBJECTIVE_FLEE_TILL_SAFE) {
					if (!isPhoneAvailable(m_phoneId)) {
						RestorePreviousState();
						crimeReporters[m_phoneId] = nil;
						m_phoneId = -1;
						bRunningToPhone = false;
					} else {
						crimeReporters[m_phoneId] = this;
						m_nPedState = PED_FACE_PHONE;
					}
#else
				} else if (bRunningToPhone) {
					if (gPhoneInfo.m_aPhones[m_phoneId].m_nState != PHONE_STATE_FREE) {
						RestorePreviousState();
						m_phoneId = -1;
					} else {
						gPhoneInfo.m_aPhones[m_phoneId].m_nState = PHONE_STATE_REPORTING_CRIME;
						m_nPedState = PED_FACE_PHONE;
					}
#endif
				} else if (m_objective != OBJECTIVE_KILL_CHAR_ANY_MEANS && m_objective != OBJECTIVE_KILL_CHAR_ON_FOOT) {
					if (m_objective == OBJECTIVE_FOLLOW_PED_IN_FORMATION) {
						if (m_moved.Magnitude() == 0.0f) {
							if (m_pedInObjective->m_nMoveState == PEDMOVE_STILL)
								m_fRotationDest = m_pedInObjective->m_fRotationCur;
						}
					} else if (m_objective == OBJECTIVE_GOTO_CHAR_ON_FOOT
						&& m_pedInObjective && m_pedInObjective->m_nMoveState != PEDMOVE_STILL) {
						SetMoveState(m_pedInObjective->m_nMoveState);
					} else if (m_objective == OBJECTIVE_GOTO_AREA_ON_FOOT || m_objective == OBJECTIVE_RUN_TO_AREA) {
						SetIdle();
					} else {
						RestorePreviousState();
					}
				}
			}
			break;
		case PED_FACE_PHONE:
			if (FacePhone())
				m_nPedState = PED_MAKE_CALL;
			break;
		case PED_MAKE_CALL:
			if (MakePhonecall())
				SetWanderPath(CGeneral::GetRandomNumber() & 7);
			break;
		case PED_MUG:
			Mug();
			break;
		case PED_SOLICIT:
			Solicit();
			break;
		case PED_UNKNOWN:
		{
			int pedsInSameState = 0;
			Idle();
			for (int i = 0; i < m_numNearPeds; ++i) {
				CPed *nearPed = m_nearPeds[i];
				if (nearPed->m_nPedType == m_nPedType && nearPed->m_nPedState == PED_UNKNOWN) {
					++pedsInSameState;
				}
			}
			if (pedsInSameState < 5) {
				for (int j = 0; j < m_numNearPeds; ++j) {
					CPed *nearPed = m_nearPeds[j];
					if (nearPed->m_nPedType == m_nPedType && nearPed->m_nPedState == PED_WANDER_PATH) {
						nearPed->m_nPedState = PED_UNKNOWN;
					}
				}
			}
			break;
		}
		case PED_DRIVING:
			if (m_nPedType != PEDTYPE_PROSTITUTE)
				break;

			if (CWorld::Players[CWorld::PlayerInFocus].m_pHooker != this)
				break;

			if (CTimer::GetTimeInMilliseconds() > CWorld::Players[CWorld::PlayerInFocus].m_nNextSexFrequencyUpdateTime) {
				if (m_nPedState == PED_DRIVING
					&& m_pMyVehicle->pDriver && m_pMyVehicle->pDriver->IsPlayer() && m_pMyVehicle->pDriver->m_nPedState == PED_DRIVING) {
					CColPoint foundCol;
					CEntity* foundEnt;

					CWorld::ProcessVerticalLine(m_pMyVehicle->GetPosition(), -100.0f,
						foundCol, foundEnt, true, false, false, false, false, false, nil);

					if (m_pMyVehicle->m_vecMoveSpeed.MagnitudeSqr() < sq(0.01f)
						&& foundCol.surfaceB != SURFACE_DEFAULT && foundCol.surfaceB != SURFACE_TARMAC && foundCol.surfaceB != SURFACE_PAVEMENT) {

						if (m_pMyVehicle->CarHasRoof()) {
							m_pMyVehicle->ApplyTurnForce(0.0f, 0.0f, CGeneral::GetRandomNumberInRange(-0.8f, -1.2f) * m_fMass,
								GetPosition().x - m_pMyVehicle->GetPosition().x, GetPosition().y - m_pMyVehicle->GetPosition().y, 0.0f);

							DMAudio.PlayOneShot(m_pMyVehicle->m_audioEntityId, SOUND_CAR_JERK, 0.0f);

							int playerSexFrequency = CWorld::Players[CWorld::PlayerInFocus].m_nSexFrequency;
							if (CWorld::Players[CWorld::PlayerInFocus].m_nMoney >= 10 && playerSexFrequency > 250) {
									CWorld::Players[CWorld::PlayerInFocus].m_nNextSexFrequencyUpdateTime = CTimer::GetTimeInMilliseconds() + playerSexFrequency;
									if (playerSexFrequency >= 350) {
										CWorld::Players[CWorld::PlayerInFocus].m_nSexFrequency = Max(250, playerSexFrequency - 30);
									} else {
										CWorld::Players[CWorld::PlayerInFocus].m_nSexFrequency = Max(250, playerSexFrequency - 10);
									}

									m_pMyVehicle->pDriver->m_fHealth = Min(125.0f, 1.0f + m_pMyVehicle->pDriver->m_fHealth);
									if (CWorld::Players[CWorld::PlayerInFocus].m_nSexFrequency == 250)
										CWorld::Players[CWorld::PlayerInFocus].m_nNextSexFrequencyUpdateTime = CTimer::GetTimeInMilliseconds() + 3000;
							} else {
								bWanderPathAfterExitingCar = true;
								CWorld::Players[CWorld::PlayerInFocus].m_pHooker = nil;
								SetObjective(OBJECTIVE_LEAVE_VEHICLE, m_pMyVehicle);
							}
						} else {
							bWanderPathAfterExitingCar = true;
							CWorld::Players[CWorld::PlayerInFocus].m_pHooker = nil;
							m_pMyVehicle->pDriver->m_fHealth = 125.0f;
							SetObjective(OBJECTIVE_LEAVE_VEHICLE, m_pMyVehicle);
						}
					} else {
						CWorld::Players[CWorld::PlayerInFocus].m_nNextSexFrequencyUpdateTime = CTimer::GetTimeInMilliseconds() + 3000;
						int playerSexFrequency = CWorld::Players[CWorld::PlayerInFocus].m_nSexFrequency;
						if (playerSexFrequency >= 1000 || playerSexFrequency <= 250)
							CWorld::Players[CWorld::PlayerInFocus].m_nSexFrequency = 1200;
						else
							CWorld::Players[CWorld::PlayerInFocus].m_nSexFrequency = 250;
					}
				} else {
					bWanderPathAfterExitingCar = true;
					CWorld::Players[CWorld::PlayerInFocus].m_pHooker = nil;
					SetObjective(OBJECTIVE_LEAVE_VEHICLE, m_pMyVehicle);
				}
			}

			if (CTimer::GetTimeInMilliseconds() > CWorld::Players[CWorld::PlayerInFocus].m_nNextSexMoneyUpdateTime) {
				int playerMoney = CWorld::Players[CWorld::PlayerInFocus].m_nMoney;
				if (playerMoney <= 1) {
					CWorld::Players[CWorld::PlayerInFocus].m_nSexFrequency = 250;
				} else {
					CWorld::Players[CWorld::PlayerInFocus].m_nMoney = Max(0, playerMoney - 1);
				}
				CWorld::Players[CWorld::PlayerInFocus].m_nNextSexMoneyUpdateTime = CTimer::GetTimeInMilliseconds() + 1000;
			}
			break;
		default:
			break;
	}
	if (IsPedInControl())
		CivilianAI();

	if (CharCreatedBy == RANDOM_CHAR) {
		UseNearbyAttractors();
	}

	if (CTimer::GetTimeInMilliseconds() > m_timerUnused) {
		m_stateUnused = 0;
		m_timerUnused = 0;
	}

	if (m_moved.Magnitude() > 0.0f)
		Avoid();
}

const int32 gFrequencyOfAttractorAttempt = 11;
const float gDistanceToSeekAttractors = 50.0f;
const float gMaxDistanceToAttract = 10.0f;

void CCivilianPed::UseNearbyAttractors()
{
	if (CWeather::Rain < 0.2f && !m_bAttractorUnk)
		return;
	if (HasAttractor())
		return;
	if (m_nAttractorCycleState != gFrequencyOfAttractorAttempt) {
		m_nAttractorCycleState++;
		return;
	}
	m_nAttractorCycleState = 0;
	if (!IsPedInControl())
		return;
	if (m_nPedState == PED_FLEE_ENTITY)
		return;

	float left = GetPosition().x - gDistanceToSeekAttractors;
	float right = GetPosition().x + gDistanceToSeekAttractors;
	float top = GetPosition().y - gDistanceToSeekAttractors;
	float bottom = GetPosition().y + gDistanceToSeekAttractors;
	int xstart = Max(0, CWorld::GetSectorIndexX(left));
	int xend = Min(NUMSECTORS_X - 1, CWorld::GetSectorIndexX(right));
	int ystart = Max(0, CWorld::GetSectorIndexY(top));
	int yend = Min(NUMSECTORS_Y - 1, CWorld::GetSectorIndexY(bottom));
	assert(xstart <= xend);
	assert(ystart <= yend);

	float minDistance = SQR(gMaxDistanceToAttract);
	C2dEffect* pClosestAttractor = nil;
	CEntity* pAttractorEntity = nil;

	for (int y = ystart; y <= yend; y++) {
		for (int x = xstart; x <= xend; x++) {
			CSector* s = CWorld::GetSector(x, y);
			for (CPtrNode* pNode = s->m_lists[ENTITYLIST_BUILDINGS].first; pNode != nil; pNode = pNode->next) {
				CEntity* pEntity = (CEntity*)pNode->item;
				if (!pEntity->IsObject())
					continue;
				// TODO: some flag check
				CBaseModelInfo* pModelInfo = CModelInfo::GetModelInfo(pEntity->GetModelIndex());
				for (int i = 0; i < pModelInfo->GetNum2dEffects(); i++) {
					C2dEffect* pEffect = pModelInfo->Get2dEffect(i);
					if (pEffect->type != EFFECT_PED_ATTRACTOR)
						continue;
					if (!IsAttractedTo(pEffect->pedattr.type))
						continue;
					CVector pos;
					CPedAttractorManager::ComputeEffectPos(pEffect, GetMatrix(), pos);
					if ((pos - GetPosition()).MagnitudeSqr() < minDistance) {
						CPedAttractorManager* pManager = GetPedAttractorManager();
						if (pManager->HasEmptySlot(pEffect) && pManager->IsApproachable(pEffect, pEntity->GetMatrix(), 0, this)) {
							pClosestAttractor = pEffect;
							pAttractorEntity = pEntity;
							minDistance = (pos - GetPosition()).MagnitudeSqr();
						}
					}
				}
			}
			for (CPtrNode* pNode = s->m_lists[ENTITYLIST_OBJECTS].first; pNode != nil; pNode = pNode->next) {
				CEntity* pEntity = (CEntity*)pNode->item;
				if (!pEntity->IsObject())
					continue;
				// TODO: some flag check
				CBaseModelInfo* pModelInfo = CModelInfo::GetModelInfo(pEntity->GetModelIndex());
				for (int i = 0; i < pModelInfo->GetNum2dEffects(); i++) {
					C2dEffect* pEffect = pModelInfo->Get2dEffect(i);
					if (pEffect->type != EFFECT_PED_ATTRACTOR)
						continue;
					if (!IsAttractedTo(pEffect->pedattr.type))
						continue;
					CVector pos;
					CPedAttractorManager::ComputeEffectPos(pEffect, GetMatrix(), pos);
					if ((pos - GetPosition()).MagnitudeSqr() < minDistance) {
						CPedAttractorManager* pManager = GetPedAttractorManager();
						if (pManager->HasEmptySlot(pEffect) && pManager->IsApproachable(pEffect, pEntity->GetMatrix(), 0, this)) {
							pClosestAttractor = pEffect;
							pAttractorEntity = pEntity;
							minDistance = (pos - GetPosition()).MagnitudeSqr();
						}
					}
				}
			}
		}
	}
	if (pClosestAttractor)
		GetPedAttractorManager()->RegisterPedWithAttractor(this, pClosestAttractor, pAttractorEntity->GetMatrix());
}

bool CCivilianPed::IsAttractedTo(int8 type)
{
	switch (type) {
	case ATTRACTOR_ATM: return true;
	case ATTRACTOR_SEAT: return true;
	case ATTRACTOR_STOP: return true;
	case ATTRACTOR_PIZZA: return true;
	case ATTRACTOR_SHELTER: return CWeather::Rain >= 0.2f;
	case ATTRACTOR_ICECREAM: return true;
	}
	return false;
}
