#include "pch.h"
#include "ParticleSet.h"

using namespace Coding4Fun::FallFury::Screens;

ParticleSet::ParticleSet(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, float lifespan)
{
	_texture = texture;
	_lifespan = lifespan;
	_isAlive = false;
	_totalTime = 0.0f;
}

void ParticleSet::Update(float timeDelta)
{
	float quat = _lifespan / 0.016f;
	float decrement = 1.0f / quat;

	if (_totalTime <= _lifespan && _isAlive)
	{
		_totalTime += timeDelta;

		for (auto particle = _particles.begin(); particle != _particles.end(); particle++)
		{
			if (_shouldScale)
				particle->Scale += 0.2f;

			particle->Shading.a -= decrement;
			particle->Position = float2(particle->Position.x + particle->Velocity.x,
				particle->Position.y + particle->Velocity.y);

			if (!_shouldScale)
				particle->Rotation += particle->RotationVelocity;
		}
	}
	else
	{
		_totalTime = 0.0f;
		_isAlive = false;
	}
}

void ParticleSet::AddParticle(Particle& particle)
{
	_particles.push_back(particle);
}

void ParticleSet::Activate(float2 position, float2 velocity, bool randomize, bool scale)
{
	for (auto particle = _particles.begin(); particle != _particles.end(); particle++)
	{
		particle->Position = position;
		if (randomize)
			particle->Velocity = float2(RandFloat(-5.0f,5.0f), RandFloat(-5.0f, 5.0f));
		else
			particle->Velocity = float2(velocity.x + RandFloat(-0.6f, 0.6f), velocity.y + RandFloat(-0.6f, 0.6f));;
	}

	_shouldScale = scale;
	_isAlive = true;
}

void ParticleSet::Render(SpriteBatch ^spriteBatch)
{
	for (auto particle = _particles.begin(); particle != _particles.end(); particle++)
    {
		if (GamePlayScreen::Manager->IsWithinScreenBoundaries(particle->Position))
			spriteBatch->Draw(_texture.Get(), particle->Position, PositionUnits::DIPs,
			particle->Size * particle->Scale, SizeUnits::Pixels, particle->Shading, particle->Rotation);
    }
}

bool ParticleSet::IsAlive()
{
	return _isAlive;
}