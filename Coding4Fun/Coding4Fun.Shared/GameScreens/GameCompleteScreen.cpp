#include "pch.h"
#include "GameCompleteScreen.h"

using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Audio;
using namespace Coding4Fun::FallFury::Helpers;

GameCompleteScreen::GameCompleteScreen(void)
{
}

GameCompleteScreen::GameCompleteScreen(ScreenManager^ base)
{
	m_particleSystem = ParticleCore(this);
	m_bearPosition = float2(Manager->m_windowBounds.Width / 2 ,-200.0f);
	m_arrived = false;
	m_initialAnimationCompleted = false;
	m_bearScale = 0.4f;
	m_bearPositionTimer = 0.0f;
	m_bearRotation = 0;

	Manager = base;

	if (AudioManager::MusicCategory != GameState::GS_WIN)
		AudioManager::SetGameWonMusic();
}

void GameCompleteScreen::RenderScreen()
{
	CurrentSpriteBatch->Begin();

	CurrentSpriteBatch->Draw(
		m_backgroundBlockA.Get(),
		float2(Manager->m_windowBounds.Width / 2, m_backgroundPositionA),
		PositionUnits::DIPs,
		m_screenSize,
		SizeUnits::Pixels);
	
	CurrentSpriteBatch->Draw(
		m_backgroundBlockB.Get(),
		float2(Manager->m_windowBounds.Width / 2, m_backgroundPositionB),
		PositionUnits::DIPs,
		m_screenSize,
		SizeUnits::Pixels);

	CurrentSpriteBatch->Draw(
		m_overlayA.Get(),
		float2(Manager->m_windowBounds.Width / 2, m_backgroundPositionA),
		PositionUnits::DIPs,
		m_screenSize,
		SizeUnits::Pixels);
	
	CurrentSpriteBatch->Draw(
		m_overlayB.Get(),
		float2(Manager->m_windowBounds.Width / 2, m_backgroundPositionB),
		PositionUnits::DIPs,
		m_screenSize,
		SizeUnits::Pixels);

	CurrentSpriteBatch->Draw(
		m_bearPrimaryTexture.Get(),
		m_bearPosition,
		PositionUnits::DIPs,
		float2(1.0f, 1.0f) * m_bearScale,
		SizeUnits::Normalized,
		float4(1.0f, 1.0f, 1.0f, 1.0f),
		m_bearRotation);
	
	m_particleSystem.Render();

	CurrentSpriteBatch->End();
}

void GameCompleteScreen::Load()
{
	GameScreenBase::Load();
	InitializeSpriteBatch();

	m_particleSystem.CreatePreCachedParticleSets();

	m_loader = ref new BasicLoader(Manager->m_d3dDevice.Get(), Manager->m_wicFactory.Get());

	m_loader->LoadTexture("Assets\\Bear\\winner_down.png", &m_bearPrimaryTexture, nullptr);
	m_loader->LoadTexture("Assets\\Bear\\winner_up.png", &m_bearSecondaryTexture, nullptr);

	m_loader->LoadTexture("Assets\\Backgrounds\\generic_blue_a.png", &m_backgroundBlockA, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\generic_blue_b.png", &m_backgroundBlockB, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_a.png", &m_overlayA, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_b.png", &m_overlayB, nullptr);
	
	CurrentSpriteBatch->AddTexture(m_bearPrimaryTexture.Get());
	CurrentSpriteBatch->AddTexture(m_bearSecondaryTexture.Get());
	CurrentSpriteBatch->AddTexture(m_backgroundBlockA.Get());
	CurrentSpriteBatch->AddTexture(m_backgroundBlockB.Get());
	CurrentSpriteBatch->AddTexture(m_overlayA.Get());
	CurrentSpriteBatch->AddTexture(m_overlayB.Get());

	UpdateWindowSize();
}

void GameCompleteScreen::Update(float timeTotal, float timeDelta)
{
	MoveBackground(StaticDataHelper::BACKGROUND_SCROLL_SPEED);
	m_particleSystem.Update(timeDelta);

	if (m_bearPosition.y < Manager->m_windowBounds.Height / 2)
	{
		m_bearPosition.y += 15.0f;
		m_bearScale -= 0.005f;
	}
	else
	{
		if (m_bearScale < 0.7f)
			m_bearScale += 0.05f;

		m_arrived = true;
	}

	if (m_arrived && !m_initialAnimationCompleted)
	{
		m_bearPrimaryTexture = m_bearSecondaryTexture;
		m_particleSystem.ActivateSet("CapeExplosionSet", float2(Manager->m_windowBounds.Width / 2, Manager->m_windowBounds.Height / 2), true);	
		m_initialAnimationCompleted = true;
	}

	if (m_initialAnimationCompleted)
	{
		m_timer += timeDelta;
		if (m_timer > 0.5f)
		{
			m_particleSystem.ActivateSet("CapeExplosionSet", float2(RandFloat(LoBoundX, HiBoundX), RandFloat(LoBoundY, HiBoundY)), true);	
			m_timer = 0.0f;
		}

		m_bearPositionTimer += timeDelta;

		if (m_bearPositionTimer < 1.0f)
		{
			m_bearPosition.y += 1.0f;
			m_bearRotation += 0.001f;
		}
		else if (m_bearPositionTimer >= 1.0f && m_bearPositionTimer < 2.0f)
		{
			m_bearPosition.y -= 1.0f;
			m_bearRotation -= 0.001f;
		}
		else
		{
			m_bearPositionTimer = 0.0f;
		}
	}
}
