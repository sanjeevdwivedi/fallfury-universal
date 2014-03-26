#include "pch.h"
#include "ParticleCore.h"

using namespace Coding4Fun::FallFury::Screens;

ParticleCore::ParticleCore()
{

}

ParticleCore::ParticleCore(GameScreenBase^ screenBase)
{
	_screenBase = screenBase;
}

ParticleCore::~ParticleCore()
{
}


void ParticleCore::CreatePreCachedParticleSets()
{
	const float LIFESPAN = 1.2f;

	BasicLoader^ Loader = ref new BasicLoader(_screenBase->Manager->m_d3dDevice.Get(), _screenBase->Manager->m_wicFactory.Get());

	// Leveraging the existing particle engine for a single "from 0 to max scale" animation
	// for picked up power-ups.
	Loader->LoadTexture("DDS\\Bear\\Customized\\bear_helmet.dds", &_textureCache["helmet"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["helmet"].Get());

	auto helmetScale = new ParticleSet(_textureCache["helmet"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(458.0f, 313.0f));
		helmetScale->AddParticle(particle);
	}

	Loader->LoadTexture("DDS\\Weapons\\knife.dds", &_textureCache["knife"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["knife"].Get());

	auto knifeScale = new ParticleSet(_textureCache["knife"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(113.0, 184.0f));
		knifeScale->AddParticle(particle);
	}

	Loader->LoadTexture("DDS\\Bear\\Customized\\bear_parachute_right.dds", &_textureCache["parachute"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["parachute"].Get());

	auto parachuteScale = new ParticleSet(_textureCache["parachute"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(458.0f, 313.0f));
		parachuteScale->AddParticle(particle);
	}

	Loader->LoadTexture("Assets\\HUD\\heartHud.png", &_textureCache["heart"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["heart"].Get());

	auto heartScale = new ParticleSet(_textureCache["heart"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(128.0f, 128.0f));
		heartScale->AddParticle(particle);
	}

	Loader->LoadTexture("DDS\\Bear\\Customized\\bubble.dds", &_textureCache["bubble"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["bubble"].Get());

	auto bubbleScale = new ParticleSet(_textureCache["bubble"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(128.0f, 128.0f));
		bubbleScale->AddParticle(particle);
	}

	Loader->LoadTexture("DDS\\Bear\\Customized\\bear_cape.dds", &_textureCache["cape"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["cape"].Get());

	auto capeScale = new ParticleSet(_textureCache["cape"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(498.0f, 375.0f));
		capeScale->AddParticle(particle);
	}

	Loader->LoadTexture("DDS\\Weapons\\axe.dds", &_textureCache["axe"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["axe"].Get());

	auto axeScale = new ParticleSet(_textureCache["axe"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(219.0f, 239.0f));
		axeScale->AddParticle(particle);
	}

	Loader->LoadTexture("DDS\\Weapons\\boomerang.dds", &_textureCache["boomerang"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["boomerang"].Get());

	auto boomerangScale = new ParticleSet(_textureCache["boomerang"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(225.0f, 205.0f));
		boomerangScale->AddParticle(particle);
	}

	Loader->LoadTexture("DDS\\Weapons\\hammer.dds", &_textureCache["hammer"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["hammer"].Get());

	auto hammerScale = new ParticleSet(_textureCache["hammer"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(194.0f, 207.0f));
		hammerScale->AddParticle(particle);
	}

	Loader->LoadTexture("DDS\\Weapons\\plasmaball.dds", &_textureCache["plasmaball"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["plasmaball"].Get());

	auto plasmaballScale = new ParticleSet(_textureCache["plasmaball"], LIFESPAN);
	for (int i = 0; i < 1; i++)
	{
		Particle particle(float2(532.0f, 209.0f));
		plasmaballScale->AddParticle(particle);
	}

	// This section applies to particle sets with multiple particles.
	Loader->LoadTexture("DDS\\button.dds", &_textureCache["button"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["button"].Get());

	Loader->LoadTexture("Assets\\Misc\\microparticle.png", &_textureCache["micro"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["micro"].Get());

	Loader->LoadTexture("DDS\\stuffing.dds", &_textureCache["Stuffing"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["Stuffing"].Get());
	
	Loader->LoadTexture("DDS\\particle.dds", &_textureCache["particle"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["particle"].Get());
	
	Loader->LoadTexture("DDS\\sparkle.dds", &_textureCache["sparkle"], nullptr);
	_screenBase->CurrentSpriteBatch->AddTexture(_textureCache["sparkle"].Get());

	auto microSet = new ParticleSet(_textureCache["micro"], 0.3f);
	for (int i = 0; i < 7; i++)
	{
		float size = RandFloat(5.0f, 5.0f);
		Particle particle(float2(size, size));
		microSet->AddParticle(particle);
	}

	auto sparkleSet = new ParticleSet(_textureCache["sparkle"], LIFESPAN);
	for (int i = 0; i < 10; i++)
	{
		float size = RandFloat(70.0f, 110.0f);
		Particle particle(float2(size, size));
		sparkleSet->AddParticle(particle);
	}

	auto capeExplosionSet = new ParticleSet(_textureCache["Stuffing"], LIFESPAN);
	for (int i = 0; i < 20; i++)
	{
		float size = RandFloat(50.0f, 100.0f);
		Particle particle(float2(size,  size), float4(RandFloat(0.3f, 1.0f), RandFloat(0.3f, 1.0f), RandFloat(0.3f, 1.0f), 1.0f));
		capeExplosionSet->AddParticle(particle);
	}

	auto smallExplosionSet = new ParticleSet(_textureCache["Stuffing"], LIFESPAN);
	for (int i = 0; i < 20; i++)
	{
		float size = RandFloat(50.0f, 100.0f);
		Particle particle(float2(size, size));
		smallExplosionSet->AddParticle(particle);
	}

	
	auto buttonExplosionSet = new ParticleSet(_textureCache["button"], LIFESPAN);
	for (int i = 0; i < 30; i++)
	{
		float size = RandFloat(50.0f, 80.0f);
		Particle particle(float2(size, size));
		buttonExplosionSet->AddParticle(particle);
	}

	_particleSetCache["ScalableHelmet"] = helmetScale;
	_particleSetCache["ScalableKnife"] = knifeScale;
	_particleSetCache["ScalableParachute"] = parachuteScale;
	_particleSetCache["ScalableHeart"] = heartScale;
	_particleSetCache["ScalableBubble"] = bubbleScale;
	_particleSetCache["ScalableCape"] = capeScale;
	_particleSetCache["ScalableAxe"] = axeScale;
	_particleSetCache["ScalableBoomerang"] = boomerangScale;
	_particleSetCache["ScalableHammer"] = hammerScale;
	_particleSetCache["ScalablePlasmaball"] = plasmaballScale;

	_particleSetCache["Buttons"] = buttonExplosionSet;
	_particleSetCache["MicroSet"] = microSet;
	_particleSetCache["SmallExplosion"] = smallExplosionSet;
	_particleSetCache["Sparkle"] = sparkleSet;
	_particleSetCache["CapeExplosionSet"] = capeExplosionSet;
}

void ParticleCore::Update(float timeDelta)
{
    for (auto set = _renderParticleSets.begin(); set != _renderParticleSets.end();)
    {
        if ((*set).IsAlive())
        {
            (*set).Update(timeDelta);
            ++set;
        }
        else
        {
            set = _renderParticleSets.erase(set);
        }
    }
}

void ParticleCore::Render()
{
	for (auto set = _renderParticleSets.begin(); set != _renderParticleSets.end(); ++set)
    {
        if ((*set).IsAlive())
        {
			(*set).Render(_screenBase->CurrentSpriteBatch);
        }
    }
}

void ParticleCore::ActivateSet(Platform::String^ name, float2 position)
{
	ActivateSet(name, position, float2(RandFloat(-10.0f, 10.0f), RandFloat(-10.0f, 10.0f)));
}


void ParticleCore::ActivateSet(Platform::String^ name, float2 position, float2 velocity)
{
	ActivateSet(name, position, velocity, false);
}

void ParticleCore::ActivateSet(Platform::String^ name, float2 position, bool randomize)
{
	ActivateSet(name, position, float2(0.0f, 0.0f), randomize);
}

void ParticleCore::ActivateSet(Platform::String^ name, float2 position, float2 velocity, bool randomize)
{
	ActivateSet(name, position, velocity, randomize, false);
}

void ParticleCore::ActivateSet(Platform::String^ name, float2 position, float2 velocity, bool randomize, bool scale)
{
	ParticleSet set = ParticleSet(*_particleSetCache[name]);
	set.Activate(position, velocity, randomize, scale);
	_renderParticleSets.push_back(set);
}