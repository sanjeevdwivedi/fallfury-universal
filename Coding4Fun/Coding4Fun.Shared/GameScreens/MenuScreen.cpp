#include "pch.h"
#include "MenuScreen.h"

using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Audio;
using namespace Windows::Graphics::Display;
using namespace Coding4Fun::FallFury::Helpers;

MenuScreen::MenuScreen(ScreenManager^ parent)
{
	ScreenType = GameState::GS_MAIN_MENU;
	Manager = parent;
	m_positionTimer = 0.0f;

	m_showBear = ref new Bear(this);
	m_showMonster = ref new Monster(this, MonsterType::MT_NIGHTMARE_C, true);
}

void MenuScreen::RenderScreen()
{
	CurrentSpriteBatch->Begin();

#ifdef WP8
	// TODO: (sanjeevd) everything should probably be in DIPs or Pixels, for Phone, the following is converted to Pixels
	CurrentSpriteBatch->Draw(
		m_backgroundBlockA.Get(),
		float2(m_screenSize.x/2, m_backgroundPositionA),
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

	if (m_showMonster->IsLoaded)
		m_showMonster->Render();

#ifdef WP8
	CurrentSpriteBatch->Draw(
		m_overlayA.Get(),
		float2(m_screenSize.x / 2, m_backgroundPositionA),
		PositionUnits::Pixels,
		m_screenSize,
		SizeUnits::Pixels);

	CurrentSpriteBatch->Draw(
		m_overlayB.Get(),
		float2(m_screenSize.x / 2 ,m_backgroundPositionB),
		PositionUnits::Pixels,
		m_screenSize,
		SizeUnits::Pixels);
#else
	CurrentSpriteBatch->Draw(
		m_overlayA.Get(),
		float2(Manager->m_windowBounds.Width/2, m_backgroundPositionA),
		PositionUnits::DIPs,
		m_screenSize,
		SizeUnits::Pixels);
	
	CurrentSpriteBatch->Draw(
		m_overlayB.Get(),
		float2(Manager->m_windowBounds.Width / 2 ,m_backgroundPositionB),
		PositionUnits::DIPs,
		m_screenSize,
		SizeUnits::Pixels);
#endif
	CurrentSpriteBatch->End();
}

void MenuScreen::Load()
{
	GameScreenBase::Load();
	InitializeSpriteBatch();

	float scale = 0.6f;
	m_showBear->Scale = scale;
	m_showBear->RotationVelocity = 2.5f / (7.0f + 3.0f * scale);
	m_showBear->Position = float2(Manager->m_windowBounds.Width / 2, Manager->m_windowBounds.Height / 2 - 200.0f);

	m_showBear->Load();

	CreateNewMonster();

	m_loader = ref new BasicLoader(Manager->m_d3dDevice.Get(), Manager->m_wicFactory.Get());

	m_loader->LoadTexture("Assets\\Backgrounds\\generic_blue_a.png", &m_backgroundBlockA, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\generic_blue_b.png", &m_backgroundBlockB, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_a.png", &m_overlayA, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_b.png", &m_overlayB, nullptr);

	CurrentSpriteBatch->AddTexture(m_backgroundBlockA.Get());
	CurrentSpriteBatch->AddTexture(m_backgroundBlockB.Get());
	CurrentSpriteBatch->AddTexture(m_overlayA.Get());
	CurrentSpriteBatch->AddTexture(m_overlayB.Get());

	UpdateWindowSize();

	if (AudioManager::MusicCategory != GameState::GS_MAIN_MENU)
		AudioManager::SetMainMenuMusic();

}

void MenuScreen::Update(float timeTotal, float timeDelta)
{
	MoveBackground(StaticDataHelper::BACKGROUND_SCROLL_SPEED);

	if (m_showBear->IsLoaded)
		m_showBear->Update(timeTotal, timeDelta, float2(Manager->m_windowBounds.Width / 2, Manager->m_windowBounds.Height / 2));

	if (m_showMonster->IsLoaded)
		m_showMonster->Update(timeTotal, timeDelta, m_showBear->Position, GetScreenBounds());
	
	m_positionTimer += timeDelta;

	if (m_positionYAdj == 0.0f)
	{
		m_positionYAdj = RandFloat(0.1f, 1.2f);
		m_positionXAdj = RandFloat(-1.0f, 1.0f);
	}

	if (m_positionTimer < 4.0f)
	{
		m_showBear->Position.x += m_positionXAdj;
		m_showBear->Position.y += m_positionYAdj;
		m_showBear->Rotation += 0.001f;
		m_showBear->Scale += 0.0008f;
		
		m_showMonster->Scale += 0.002f;
	}
	else if (m_positionTimer > 4.0f && m_positionTimer < 8.0f)
	{
		m_showBear->Position.x -= m_positionXAdj;
		m_showBear->Position.y -= m_positionYAdj;
		m_showBear->Rotation -= 0.001f;
		m_showBear->Scale -= 0.0008f;
	}
	else
	{
		m_positionYAdj = 0.0f;
		m_positionTimer = 0.0f;
	}

	m_showMonster->Position.y -= m_showMonster->Velocity.y;
	if (m_showMonster->Position.y < -m_showMonster->Size.y)
	{
		CreateNewMonster();
	}
}

void MenuScreen::CreateNewMonster()
{
	m_showMonster = ref new Monster(this, (MonsterType)(rand() % (int)MonsterType::MT_CANDYLAND_E), true);
	m_showMonster->Scale = 0.3f;
	m_showMonster->Velocity.y = 1.0f;
	m_showMonster->Load();

	m_showMonster->Position = float2(RandFloat(LoBoundX, HiBoundX), Manager->m_windowBounds.Height + m_showMonster->Size.y * m_showMonster->Scale);
}