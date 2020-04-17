#include "common.h"


#include "Hud.h"
#include "PlayerPed.h"
#include "Text.h"
#include "User.h"
#include "Vehicle.h"
#include "World.h"
#include "Zones.h"

CPlaceName CUserDisplay::PlaceName;
COnscreenTimer CUserDisplay::OnscnTimer;
CPager CUserDisplay::Pager;
CCurrentVehicle CUserDisplay::CurrentVehicle;

CPlaceName::CPlaceName()
{
	Init();
}

void
CPlaceName::Init()
{
	m_pZone = nil;
	m_pZone2 = nil;
	m_nAdditionalTimer = 0;
}

void
CPlaceName::Process()
{
	CVector pos = CWorld::Players[CWorld::PlayerInFocus].GetPos();
	CZone *navigZone = CTheZones::FindSmallestZonePositionType(&pos, ZONE_TYPE1);
	CZone *audioZone = CTheZones::FindSmallestZonePositionType(&pos, ZONE_AUDIO);

	if (navigZone == nil) m_pZone = nil;
	if (audioZone == nil) m_pZone2 = nil;

	if (navigZone == m_pZone) {
		if (audioZone == m_pZone2 || m_pZone != nil) {
			if (navigZone != nil || audioZone != nil) {
				if (m_nAdditionalTimer != 0)
					m_nAdditionalTimer--;
			} else {
				m_nAdditionalTimer = 0;
				m_pZone = nil;
				m_pZone2 = nil;
			}
		} else {
			m_pZone2 = audioZone;
			m_nAdditionalTimer = 250;
		}
	} else {
		m_pZone = navigZone;
		m_nAdditionalTimer = 250;
	}
	Display();
}

void
CPlaceName::Display()
{
	wchar *text;
	if (m_pZone != nil)
		text = m_pZone->GetTranslatedName();
	else if (m_pZone2 != nil)
		text = m_pZone2->GetTranslatedName();
#ifdef FIX_BUGS
	else
		text = nil;
#endif
	CHud::SetZoneName(text);
}

CCurrentVehicle::CCurrentVehicle()
{
	Init();
}

void
CCurrentVehicle::Init()
{
	m_pCurrentVehicle = nil;
}

void
CCurrentVehicle::Process()
{
	if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->InVehicle())
		m_pCurrentVehicle = CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_pMyVehicle;
	else
		m_pCurrentVehicle = nil;
	Display();
}

void
CCurrentVehicle::Display()
{
	wchar *text = nil;
	if (m_pCurrentVehicle != nil)
		text = TheText.Get(((CVehicleModelInfo*)CModelInfo::GetModelInfo(m_pCurrentVehicle->GetModelIndex()))->m_gameName);
	CHud::SetVehicleName(text);
}

void
CUserDisplay::Init()
{
	PlaceName.Init();
	OnscnTimer.Init();
	Pager.Init();
	CurrentVehicle.Init();
}

void
CUserDisplay::Process()
{
	PlaceName.Process();
	OnscnTimer.Process();
	Pager.Process();
	CurrentVehicle.Process();
}
