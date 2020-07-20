#include "common.h"

#include "Occlusion.h"
#include "Game.h"
#include "Camera.h"
#include "Vector.h"
#include "Draw.h"
#include "Timer.h"
#include "RwHelper.h"

int32 COcclusion::NumOccludersOnMap;
int16 COcclusion::FarAwayList;
int16 COcclusion::NearbyList;
int16 COcclusion::ListWalkThroughFA;
int16 COcclusion::PreviousListWalkThroughFA;
int16 COcclusion::NumActiveOccluders;
COccluder COcclusion::aOccluders[NUMOCCLUSIONVOLUMES];
CActiveOccluder COcclusion::aActiveOccluders[NUMACTIVEOCCLUDERS];

CVector gCenterOnScreen;

float gMinYInOccluder;
float gMinXInOccluder;
float gMaxYInOccluder;
float gMaxXInOccluder;

bool gOccluderCoorsValid[8];
CVector gOccluderCoorsOnScreen[8];
CVector gOccluderCoors[8];

bool bDisplayOccDebugStuff = false;

void
COcclusion::Init(void)
{
	NumOccludersOnMap = 0;
	FarAwayList = -1;
	NearbyList = -1;
	ListWalkThroughFA = -1;
	PreviousListWalkThroughFA = -1;
	bDisplayOccDebugStuff = false;
}

void
COcclusion::AddOne(float x, float y, float z, float width, float length, float height, float angle)
{
	if(NumOccludersOnMap >= NUMOCCLUSIONVOLUMES)
		return;

	aOccluders[NumOccludersOnMap].x = x;
	aOccluders[NumOccludersOnMap].y = y;
	aOccluders[NumOccludersOnMap].z = z;
	aOccluders[NumOccludersOnMap].width = width;
	aOccluders[NumOccludersOnMap].length = length;
	aOccluders[NumOccludersOnMap].height = height;
	while(angle < 0.0f) angle += 360.0f;
	while(angle > 360.0f) angle -= 360.0f;
	aOccluders[NumOccludersOnMap].angle = angle * UINT16_MAX/360.0f;
	aOccluders[NumOccludersOnMap].listIndex = FarAwayList;
	FarAwayList = NumOccludersOnMap++;
}

bool 
COccluder::NearCamera() {
	return (TheCamera.GetPosition() - CVector(x, y, z)).Magnitude() - (Max(width, length) / 2.0f) < 250.0f;
}

bool
DoesInfiniteLineCrossFiniteLine(float p1X, float p1Y, float p2X, float p2Y, float lineX, float lineY, float lineDX, float lineDY)
{
	float side1 = (p1X - lineX) * lineDX - (p1Y - lineY) * lineDY;
	float side2 = (p2X - lineX) * lineDX - (p2Y - lineY) * lineDY;
	return side1 * side2 < 0.0f;    // if points lie on opposite sides of the infinte line, the line between them crosses it
}

bool DoesInfiniteLineTouchScreen(float p1X, float p1Y, float p2X, float p2Y) {
	if (p1X > 0.0f && p1Y > 0.0f && SCREEN_WIDTH > p1X && SCREEN_HEIGHT > p1Y)
		return true;

	return (DoesInfiniteLineCrossFiniteLine(0.0f, 0.0f, SCREEN_WIDTH, 0.0f, p1X, p1Y, p2X, p2Y) ||
		DoesInfiniteLineCrossFiniteLine(0.0f, 0.0f, 0.0f, SCREEN_HEIGHT, p1X, p1Y, p2X, p2Y) ||
		DoesInfiniteLineCrossFiniteLine(SCREEN_WIDTH, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, p1X, p1Y, p2X, p2Y) ||
		DoesInfiniteLineCrossFiniteLine(0.0f, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, p1X, p1Y, p2X, p2Y));
}

bool IsPointInsideLine(float lineX, float lineY, float lineDX, float lineDY, float pX, float pY, float area = 0.0f) {
	return (pX - lineX) * lineDY - (pY - lineY) * lineDX >= area;
}

bool CalcScreenCoors(CVector const &in, CVector *out, float *outw, float *outh) {
	*out = TheCamera.m_viewMatrix * in;

	if (out->z <= 1.0f) return false;
	
	float recip = 1.0f / out->z;
	out->x *= SCREEN_WIDTH * recip;
	out->y *= SCREEN_HEIGHT * recip;

	float fovScale = DefaultFOV / CDraw::GetFOV();

	*outw = fovScale * recip * SCREEN_WIDTH;
	*outh = fovScale * recip * SCREEN_HEIGHT;

	return true;
}

bool CalcScreenCoors(CVector const &in, CVector *out) {
	*out = TheCamera.m_viewMatrix * in;

	if (out->z <= 1.0f) return false;

	float recip = 1.0f / out->z;
	out->x *= SCREEN_WIDTH * recip;
	out->y *= SCREEN_HEIGHT * recip;

	return true;
}

bool 
COccluder::ProcessLineSegment(int corner1, int corner2, CActiveOccluder *occl) {
	if (!gOccluderCoorsValid[corner1] && !gOccluderCoorsValid[corner2])
		return false;

	float x1, y1, x2, y2;

	CVector origin3d, direction3d;
	if (!gOccluderCoorsValid[corner1]) {
		float clipDist1 = Abs((TheCamera.m_viewMatrix * gOccluderCoors[corner1]).z - 1.1f);
		float clipDist2 = Abs((TheCamera.m_viewMatrix * gOccluderCoors[corner2]).z - 1.1f);
		float ratio = clipDist2 / (clipDist1 + clipDist2);
		CVector clippedCoors = (1.0f - ratio) * gOccluderCoors[corner2] + ratio * gOccluderCoors[corner1];

		if (!CalcScreenCoors(clippedCoors, &origin3d, &x1, &y1))
			return true;
	}
	else {
		origin3d = gOccluderCoorsOnScreen[corner1];
	}

	if (!gOccluderCoorsValid[corner2]) {
		float clipDist1 = Abs((TheCamera.m_viewMatrix * gOccluderCoors[corner1]).z - 1.1f);
		float clipDist2 = Abs((TheCamera.m_viewMatrix * gOccluderCoors[corner2]).z - 1.1f);
		float ratio = clipDist1 / (clipDist1 + clipDist2);
		CVector clippedCoors = (1.0f - ratio) * gOccluderCoors[corner2] + ratio * gOccluderCoors[corner1];

		if (!CalcScreenCoors(clippedCoors, &direction3d, &x2, &y2))
			return true;
	}
	else {
		direction3d = gOccluderCoorsOnScreen[corner2];
	}

	gMinXInOccluder = Min(Min(gMinXInOccluder, origin3d.x), direction3d.x);
	gMaxXInOccluder = Max(Max(gMaxXInOccluder, origin3d.x), direction3d.x);
	gMinYInOccluder = Min(Min(gMinYInOccluder, origin3d.y), direction3d.y);
	gMaxYInOccluder = Max(Max(gMaxYInOccluder, origin3d.y), direction3d.y);

	CVector2D origin = (CVector2D)origin3d;
	CVector2D direction = (CVector2D)direction3d - origin;

	if (!IsPointInsideLine(origin.x, origin.y, direction.x, direction.y, gCenterOnScreen.x, gCenterOnScreen.y, 0.0f)) {
		origin += direction;
		direction *= -1.0f;
	}

	float magnitude = direction.Magnitude();

	occl->lines[occl->linesCount].origin = origin;
	occl->lines[occl->linesCount].direction = direction / magnitude;
	occl->lines[occl->linesCount].length = magnitude;

	if (!DoesInfiniteLineTouchScreen(origin.x, origin.y, direction.x, direction.y))
		return !IsPointInsideLine(origin.x, origin.y, direction.x, direction.y, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

	occl->linesCount++;
	
	return false;
}

bool 
COccluder::ProcessOneOccluder(CActiveOccluder *occl) { 
	float outX, outY;

	occl->linesCount = 0;
	CVector pos(x, y, z);

	if (!CalcScreenCoors(pos, &gCenterOnScreen, &outX, &outY) || gCenterOnScreen.z < -150.0f || gCenterOnScreen.z > 300.0f) {
		return false;
	}

	occl->radius = Max(width, length) * 0.35f + gCenterOnScreen.z;

	CVector vec[3];
	
	vec[0].x = width / 2.0f * Sin(angle / 63556.0f*TWOPI);
	vec[0].y = -width / 2.0f * Cos(angle / 63556.0f*TWOPI);
	vec[0].z = 0.0f;

	vec[1].x = length / 2.0f * Cos(angle / 63556.0f*TWOPI);
	vec[1].y = length / 2.0f * Sin(angle / 63556.0f*TWOPI);
	vec[1].z = 0.0f;

	vec[2].x = 0.0f;
	vec[2].y = 0.0f;
	vec[2].z = height / 2.0f;

	bool aChecks[6]; int counter = -1;
	for (int i = 0; i < 3; i++) {
		aChecks[++counter] = DotProduct((pos + vec[i] - TheCamera.GetPosition()), vec[i]) < 0.0f;
		aChecks[++counter] = DotProduct((pos - vec[i] - TheCamera.GetPosition()), -vec[i]) < 0.0f;
	}

	//calculating vertices of a box
	gOccluderCoors[0] = pos + vec[0] + vec[1] + vec[2];
	gOccluderCoors[1] = pos - vec[0] + vec[1] + vec[2];
	gOccluderCoors[2] = pos + vec[0] - vec[1] + vec[2];
	gOccluderCoors[3] = pos - vec[0] - vec[1] + vec[2];
	gOccluderCoors[4] = pos + vec[0] + vec[1] - vec[2];
	gOccluderCoors[5] = pos - vec[0] + vec[1] - vec[2];
	gOccluderCoors[6] = pos + vec[0] - vec[1] - vec[2];
	gOccluderCoors[7] = pos - vec[0] - vec[1] - vec[2];

	for(int i = 0; i < 8; i++)
		gOccluderCoorsValid[i] = CalcScreenCoors(gOccluderCoors[i], &gOccluderCoorsOnScreen[i], &outX, &outY);

	gMinYInOccluder = 999999.875f;
	gMinXInOccluder = 999999.875f;
	gMaxYInOccluder = -999999.875f;
	gMaxXInOccluder = -999999.875f;

	if (aChecks[2] != aChecks[0] && ProcessLineSegment(0, 4, occl))
		return false;
	if (aChecks[3] != aChecks[0] && ProcessLineSegment(2, 6, occl))
		return false;
	if (aChecks[4] != aChecks[0] && ProcessLineSegment(0, 2, occl))
		return false;
	if (aChecks[5] != aChecks[0] && ProcessLineSegment(4, 6, occl))
		return false;
	if (aChecks[2] != aChecks[1] && ProcessLineSegment(1, 5, occl))
		return false;
	if (aChecks[3] != aChecks[1] && ProcessLineSegment(3, 7, occl))
		return false;
	if (aChecks[4] != aChecks[1] && ProcessLineSegment(1, 3, occl))
		return false;
	if (aChecks[5] != aChecks[1] && ProcessLineSegment(5, 7, occl))
		return false;
	if (aChecks[4] != aChecks[2] && ProcessLineSegment(0, 1, occl))
		return false;
	if (aChecks[3] != aChecks[4] && ProcessLineSegment(2, 3, occl))
		return false;
	if (aChecks[5] != aChecks[3] && ProcessLineSegment(6, 7, occl))
		return false;
	if (aChecks[2] != aChecks[5] && ProcessLineSegment(4, 5, occl))
		return false;
		
	if (gMaxXInOccluder - gMinXInOccluder < SCREEN_WIDTH * 0.1f ||
		gMaxYInOccluder - gMinYInOccluder < SCREEN_HEIGHT * 0.07f)
		return false;

	return true;
}

bool
COcclusion::OccluderHidesBehind(CActiveOccluder *occl1, CActiveOccluder *occl2) {
	for (int i = 0; i < occl1->linesCount; i++) {
		for (int j = 0; j < occl2->linesCount; j++) {
			if (!IsPointInsideLine(occl2->lines[j].origin.x, occl2->lines[j].origin.y, occl2->lines[j].direction.x,
				occl2->lines[j].direction.y, occl1->lines[i].origin.x, occl1->lines[i].origin.y, 0.0f)) 
					return false;
			
			
			if (!IsPointInsideLine(occl2->lines[j].origin.x, occl2->lines[j].origin.y, occl2->lines[j].direction.x,
				occl2->lines[j].direction.y, (occl1->lines[i].origin.x + occl1->lines[i].direction.x * occl1->lines[i].length),
				(occl1->lines[i].origin.y + occl1->lines[i].direction.y * occl1->lines[i].length), 0.0f)) 
					return false;
		}
	}

	return true;
}

void
COcclusion::ProcessBeforeRendering(void)
{
	NumActiveOccluders = 0;

	if (CGame::currArea != AREA_MAIN_MAP)
		return;

	if (ListWalkThroughFA == -1) {
		PreviousListWalkThroughFA = -1;
		ListWalkThroughFA = FarAwayList;
	}

	int i;
	for (i = 0; i < 16 && ListWalkThroughFA != -1; i++) {
		if (aOccluders[ListWalkThroughFA].NearCamera()) {
			int prevListWalkThroughFA = ListWalkThroughFA;

			if (PreviousListWalkThroughFA == -1) {
				FarAwayList = aOccluders[ListWalkThroughFA].listIndex;
			}
			else {
				aOccluders[PreviousListWalkThroughFA].listIndex = aOccluders[ListWalkThroughFA].listIndex;
			}

			int prevNearbyList = NearbyList;
			ListWalkThroughFA = aOccluders[ListWalkThroughFA].listIndex;
			NearbyList = prevListWalkThroughFA;
			aOccluders[prevListWalkThroughFA].listIndex = prevNearbyList;
		}
		else {
			PreviousListWalkThroughFA = ListWalkThroughFA;
			ListWalkThroughFA = aOccluders[ListWalkThroughFA].listIndex;
		}
	}

	int prevNearbyList = -1;
	int tmpNearbyList = NearbyList;
	int indexTmpNearbyList, storeTmpNearbyList, prevFarAwayList;
	while (tmpNearbyList != -1)
	{
		if (NumActiveOccluders < NUMACTIVEOCCLUDERS && aOccluders[tmpNearbyList].ProcessOneOccluder(&aActiveOccluders[NumActiveOccluders]))
			++NumActiveOccluders;

		indexTmpNearbyList = tmpNearbyList;
		if (aOccluders[indexTmpNearbyList].NearCamera())
		{
			prevNearbyList = tmpNearbyList;
			tmpNearbyList = aOccluders[indexTmpNearbyList].listIndex;

		}
		else
		{
			storeTmpNearbyList = tmpNearbyList;
			if (prevNearbyList == -1) {
				NearbyList = aOccluders[indexTmpNearbyList].listIndex;
			}
			else {
				aOccluders[prevNearbyList].listIndex = aOccluders[indexTmpNearbyList].listIndex;
			}
			tmpNearbyList = aOccluders[indexTmpNearbyList].listIndex;
			prevFarAwayList = FarAwayList;
			FarAwayList = storeTmpNearbyList;
			aOccluders[storeTmpNearbyList].listIndex = prevFarAwayList;
		}
	}

	//printf("NumActiveOccluders = %d\n", NumActiveOccluders);

	for (i = 0; i < NumActiveOccluders; i++) {
		for (int j = 0; j < NumActiveOccluders; j++) {
			if (i != j || aActiveOccluders[j].radius < aActiveOccluders[i].radius) {
				if (OccluderHidesBehind(&aActiveOccluders[i], &aActiveOccluders[j])) {
					for (int k = i; k < NumActiveOccluders - 1; k++) {
						for (int l = 0; l < aActiveOccluders[k].linesCount; l++)
							aActiveOccluders[k].lines[l] = aActiveOccluders[k + 1].lines[l];
						aActiveOccluders[k].linesCount = aActiveOccluders[k + 1].linesCount;
						aActiveOccluders[k].radius = aActiveOccluders[k + 1].radius;
					}
				}
			}
		}
	}
}

bool CActiveOccluder::IsPointWithinOcclusionArea(float pX, float pY, float area) {
	for (int i = 0; i < linesCount; i++) {
		if (!IsPointInsideLine(lines[i].origin.x, lines[i].origin.y, lines[i].direction.x, lines[i].direction.y, pX, pY, area))
			return false;
	}

	return true;
}

bool COcclusion::IsAABoxOccluded(CVector pos, float width, float length, float height) {
	return false;

	CVector coors;
	float outW, outH;

	if (!NumActiveOccluders || !CalcScreenCoors(pos, &coors, &outW, &outH))
		return false;

	float side = CVector(width, length, height).Magnitude() / 4.0f;
	float area = Max(outW, outH) * side; 

	CVector minCorner, maxCorner;

	minCorner.x = pos.x - width / 2.0f;
	minCorner.y = pos.y - length / 2.0f;
	minCorner.z = pos.z - height / 2.0f;

	maxCorner.x = pos.x + width / 2.0f;
	maxCorner.y = pos.y + length / 2.0f;
	maxCorner.z = pos.z + height / 2.0f;

	for (int i = 0; i < NumActiveOccluders; i++) {
		if (coors.z - (side * 0.85f) > aActiveOccluders[i].radius) {
			if (aActiveOccluders[i].IsPointWithinOcclusionArea(coors.x, coors.y, area))
				return true;

			if (aActiveOccluders[i].IsPointWithinOcclusionArea(coors.x, coors.y, 0.0f)) {
				if (CalcScreenCoors(minCorner, &coors) && !aActiveOccluders[i].IsPointWithinOcclusionArea(coors.x, coors.y, 0.0f)) continue;
				if (CalcScreenCoors(CVector(maxCorner.x, maxCorner.y, minCorner.z), &coors) && !aActiveOccluders[i].IsPointWithinOcclusionArea(coors.x, coors.y, 0.0f)) continue;
				if (CalcScreenCoors(CVector(maxCorner.x, minCorner.y, maxCorner.z), &coors) && !aActiveOccluders[i].IsPointWithinOcclusionArea(coors.x, coors.y, 0.0f)) continue;
				if (CalcScreenCoors(CVector(minCorner.x, maxCorner.y, maxCorner.z), &coors, &outW, &outH) && !aActiveOccluders[i].IsPointWithinOcclusionArea(coors.x, coors.y, 0.0f)) continue;

				return true;
			}
		}
	}

	return false;
}

bool COcclusion::IsPositionOccluded(CVector pos, float side) {
	return false;

	CVector coors;
	float width, height;

	if (!NumActiveOccluders || !CalcScreenCoors(pos, &coors, &width, &height))
		return false;

	float area = Max(width, height) * side;

	for (int i = 0; i < NumActiveOccluders; i++) {
		if (coors.z - (side * 0.85f) > aActiveOccluders[i].radius) 
			if (aActiveOccluders[i].IsPointWithinOcclusionArea(coors.x, coors.y, area))
				return true;
	}

	return false;
}

void COcclusion::Render() {
	if (!bDisplayOccDebugStuff || !(CTimer::GetTimeInMilliseconds() & 0x200))
		return;

	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, FALSE);

	for (int i = 0; i < NumActiveOccluders; i++) {
		for (int j = 0; j < aActiveOccluders[i].linesCount; j++) {
			RwIm2DVertex vertexbufferT[2];
			RwIm2DVertexSetScreenX(&vertexbufferT[0], aActiveOccluders[i].lines[j].origin.x);
			RwIm2DVertexSetScreenY(&vertexbufferT[0], aActiveOccluders[i].lines[j].origin.y);
			RwIm2DVertexSetScreenX(&vertexbufferT[1], 
				aActiveOccluders[i].lines[j].origin.x + aActiveOccluders[i].lines[j].direction.x * aActiveOccluders[i].lines[j].length);
			RwIm2DVertexSetScreenY(&vertexbufferT[1], 
				aActiveOccluders[i].lines[j].origin.y + aActiveOccluders[i].lines[j].direction.y * aActiveOccluders[i].lines[j].length);
			RwIm2DVertexSetIntRGBA(&vertexbufferT[0], 255, 255, 0, 255);
			RwIm2DVertexSetIntRGBA(&vertexbufferT[1], 255, 255, 0, 255);
			RwIm2DRenderLine(vertexbufferT, 2, 0, 1);
		}
	}

	DefinedState();
}