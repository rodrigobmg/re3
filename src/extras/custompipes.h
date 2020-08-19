#pragma once

#ifdef EXTENDED_PIPELINES
#ifdef LIBRW

namespace CustomPipes {



struct CustomMatExt
{
	rw::Texture *glossTex;
	bool haveGloss;
};
extern rw::int32 CustomMatOffset;
inline CustomMatExt *GetCustomMatExt(rw::Material *mat) {
	return PLUGINOFFSET(CustomMatExt, mat, CustomMatOffset);
}


struct Color
{
	float r, g, b, a;
	Color(void) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};

class InterpolatedValue
{
public:
	virtual void Read(char *s, int line, int field) = 0;
};

class InterpolatedFloat : public InterpolatedValue
{
public:
	float data[24][NUMWEATHERS];
	float curInterpolator;
	float curVal;

	InterpolatedFloat(float init);
	void Read(char *s, int line, int field);
	float Get(void);
};

class InterpolatedColor : public InterpolatedValue
{
public:
	Color data[24][NUMWEATHERS];
	float curInterpolator;
	Color curVal;

	InterpolatedColor(const Color &init);
	void Read(char *s, int line, int field);
	Color Get(void);
};

class InterpolatedLight : public InterpolatedColor
{
public:
	InterpolatedLight(const Color &init) : InterpolatedColor(init) {}
	void Read(char *s, int line, int field);
};

char *ReadTweakValueTable(char *fp, InterpolatedValue &interp);





void CustomPipeRegister(void);
void CustomPipeRegisterGL(void);
void CustomPipeInit(void);
void CustomPipeShutdown(void);
void SetTxdFindCallback(void);

extern bool bRenderingEnvMap;
extern int32 EnvMapSize;
extern rw::Camera *EnvMapCam;
extern rw::Texture *EnvMapTex;
extern rw::Texture *EnvMaskTex;
void EnvMapRender(void);

enum {
	VEHICLEPIPE_MATFX,
	VEHICLEPIPE_NEO
};
extern int32 VehiclePipeSwitch;
extern float VehicleShininess;
extern float VehicleSpecularity;
extern InterpolatedFloat Fresnel;
extern InterpolatedFloat Power;
extern InterpolatedLight DiffColor;
extern InterpolatedLight SpecColor;
extern rw::ObjPipeline *vehiclePipe;
void CreateVehiclePipe(void);
void DestroyVehiclePipe(void);
void AttachVehiclePipe(rw::Atomic *atomic);
void AttachVehiclePipe(rw::Clump *clump);

extern float LightmapMult;
extern InterpolatedFloat WorldLightmapBlend;
extern rw::ObjPipeline *worldPipe;
void CreateWorldPipe(void);
void DestroyWorldPipe(void);
void AttachWorldPipe(rw::Atomic *atomic);
void AttachWorldPipe(rw::Clump *clump);

extern float GlossMult;
extern rw::ObjPipeline *glossPipe;
void CreateGlossPipe(void);
void DestroyGlossPipe(void);
void AttachGlossPipe(rw::Atomic *atomic);
void AttachGlossPipe(rw::Clump *clump);
rw::Texture *GetGlossTex(rw::Material *mat);

extern float RimlightMult;
extern InterpolatedColor RampStart;
extern InterpolatedColor RampEnd;
extern InterpolatedFloat Offset;
extern InterpolatedFloat Scale;
extern InterpolatedFloat Scaling;
extern rw::ObjPipeline *rimPipe;
extern rw::ObjPipeline *rimSkinPipe;
void CreateRimLightPipes(void);
void DestroyRimLightPipes(void);
void AttachRimPipe(rw::Atomic *atomic);
void AttachRimPipe(rw::Clump *clump);

}

#endif
#endif
