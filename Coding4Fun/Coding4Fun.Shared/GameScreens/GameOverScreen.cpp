#include "pch.h"
#include "GameOverScreen.h"

using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Audio;
using namespace Coding4Fun::FallFury::Helpers;

GameOverScreen::GameOverScreen(ScreenManager^ parent)
{
	m_bearPositionTimer = 0.0f;
	Manager = parent;
	ScreenType = GameState::GS_GAME_OVER;
	m_showBear = ref new Bear(this);

	if (AudioManager::MusicCategory != GameState::GS_GAME_OVER)
		AudioManager::SetGameOverMusic();
}

void GameOverScreen::RenderScreen()
{
	CurrentSpriteBatch->Begin();

#ifdef WP8
	// TODO: (sanjeevd) everything should probably be in DIPs or Pixels, for Phone, the following is converted to Pixels
	CurrentSpriteBatch->Draw(
		m_backgroundBlockA.Get(),
		float2(m_screenSize.x / 2, m_backgroundPositionA),
		PositionUnits::Pixels,
		m_screenSize,
		SizeUnits::Pixels);

	CurrentSpriteBatch->Draw(
		m_backgroundBlockB.Get(),
		float2(m_screenSize.x / 2, m_backgroundPositionB),
		PositionUnits::Pixels,
		m_screenSize,
		SizeUnits::Pixels);
#else
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
#endif

	if (m_showBear->IsLoaded)	
		m_showBear->Render();

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

	CurrentSpriteBatch->End();
}

void GameOverScreen::Update(float totalTime, float timeDelta)
{
	if (m_showBear->IsLoaded)
		m_showBear->Update(totalTime, timeDelta, float2(Manager->m_windowBounds.Width / 2, Manager->m_windowBounds.Height / 2));

	MoveBackground(StaticDataHelper::BACKGROUND_SCROLL_SPEED * 1.3f);

	m_rotationTimer += timeDelta;
	if (m_rotationTimer < 0.7f)
	{
		m_bearRotation += 0.002f;
	}
	else if (m_rotationTimer > 0.7f && m_rotationTimer < 1.4f)
	{
		m_bearRotation -= 0.002f;
	}
	else
	{
		m_bearRotation = -1.0f;
		m_rotationTimer = 0.0f;
	}

	if (m_positionYAdj == 0.0f)
	{
		m_positionYAdj = RandFloat(0.1f,1.2f);
		m_positionXAdj = RandFloat(-1.0f,1.0f);
	}

	m_bearPositionTimer += timeDelta;
	if (m_bearPositionTimer < 4.0f)
	{
		m_showBear->Position.x += m_positionXAdj;
		m_showBear->Position.y += m_positionYAdj;
		m_showBear->Rotation += 0.001f;
		m_showBear->Scale += 0.0008f;
	}
	else if (m_bearPositionTimer > 4.0f && m_bearPositionTimer < 8.0f)
	{
		m_showBear->Position.x -= m_positionXAdj;
		m_showBear->Position.y -= m_positionYAdj;
		m_showBear->Rotation -= 0.001f;
		m_showBear->Scale -= 0.0008f;
	}
	else
	{
		m_positionYAdj = 0.0f;
		m_bearPositionTimer = 0.0f;
	}
}

void GameOverScreen::Load()
{
	GameScreenBase::Load();
	InitializeSpriteBatch();

	m_loader = ref new BasicLoader(Manager->m_d3dDevice.Get(), Manager->m_wicFactory.Get());

	float scale = 0.6f;
	m_showBear->Scale = scale;
	m_showBear->RotationVelocity = 2.5f / (7.0f + 3.0f * scale);
	m_showBear->Position = float2(Manager->m_windowBounds.Width / 2, Manager->m_windowBounds.Height / 2);
	m_showBear->IsCapeEnabled = true;

	m_showBear->Load();
	m_showBear->Kill();

	m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_a.png", &m_overlayA, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_b.png", &m_overlayB, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\generic_blue_a.png", &m_backgroundBlockA, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\generic_blue_b.png", &m_backgroundBlockB, nullptr);

	CurrentSpriteBatch->AddTexture(m_backgroundBlockA.Get());
	CurrentSpriteBatch->AddTexture(m_backgroundBlockB.Get());
	CurrentSpriteBatch->AddTexture(m_overlayA.Get());
	CurrentSpriteBatch->AddTexture(m_overlayB.Get());

	UpdateWindowSize();
}
