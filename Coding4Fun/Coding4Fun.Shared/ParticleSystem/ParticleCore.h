#pragma once
#include "pch.h"
#include "ParticleSet.h"
#include <list>
#include <map>

class ParticleCore
{
public:
	ParticleCore();
	ParticleCore(Coding4Fun::FallFury::Screens::GameScreenBase^);
	virtual ~ParticleCore();

	void CreatePreCachedParticleSets();
	void ActivateSet(Platform::String^, float2);
	void ActivateSet(Platform::String^, float2, float2);
	void ActivateSet(Platform::String^, float2, bool);
	void ActivateSet(Platform::String^, float2, float2, bool);
	void ActivateSet(Platform::String^, float2, float2, bool, bool scale);
	void Update(float);
	void Render();

private:
	std::list<ParticleSet>													_renderParticleSets;
	std::map<Platform::String^, ParticleSet*>								_particleSetCache;
	std::map<Platform::String^, Microsoft::WRL::ComPtr<ID3D11Texture2D>>	_textureCache;
	Coding4Fun::FallFury::Screens::GameScreenBase^							_screenBase;
};

