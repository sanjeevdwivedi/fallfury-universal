#pragma once
#include "pch.h"
#include <vector>
#include "Particle.h"

class ParticleSet
{
public:
	ParticleSet(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, float lifespan);
	void AddParticle(Particle& particle);
	void Update(float timeDelta);
	void Activate(float2 position, float2 velocity, bool randomize, bool scale);
	void Render(SpriteBatch ^spriteBatch);
	bool IsAlive();

private:
	std::vector<Particle>					_particles;
	float									_lifespan;
	float									_totalTime;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>	_texture;
	bool									_isAlive;
	bool									_shouldScale;
	float									_scale;
};

