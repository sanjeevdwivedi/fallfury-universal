#include "pch.h"
#include "GameWinScreen.h"

using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Audio;
using namespace Coding4Fun::FallFury::Helpers;

GameWinScreen::GameWinScreen(void)
{

}

GameWinScreen::GameWinScreen(ScreenManager^ manager)
{
	ScreenType = GameState::GS_WIN;
	Manager = manager;

	if (AudioManager::MusicCategory != GameState::GS_WIN)
		AudioManager::SetGameWonMusic();
}

void GameWinScreen::RenderScreen()
{
	CurrentSpriteBatch->Begin();

	CurrentSpriteBatch->Draw(
		m_backgroundBlockA.Get(), 
		float2(Manager->m_windowBounds.Width / 2, m_backgroundPositionA),
		PositionUnits::Pixels,
		m_screenSize,
		SizeUnits::Pixels);
	
	CurrentSpriteBatch->Draw(
		m_backgroundBlockB.Get(),
		float2(Manager->m_windowBounds.Width / 2 ,m_backgroundPositionB),
		PositionUnits::Pixels,
		m_screenSize,
		SizeUnits::Pixels);

	CurrentSpriteBatch->Draw(
		m_overlayA.Get(),
		float2(Manager->m_windowBounds.Width / 2, m_backgroundPositionA),
		PositionUnits::Pixels,
		m_screenSize,
		SizeUnits::Pixels);
	
	CurrentSpriteBatch->Draw(
		m_overlayB.Get(),
		float2(Manager->m_windowBounds.Width /2 ,m_backgroundPositionB),
		PositionUnits::Pixels,
		m_screenSize,
		SizeUnits::Pixels);
	
	CurrentSpriteBatch->End();
}

void GameWinScreen::Load()
{
	GameScreenBase::Load();
	InitializeSpriteBatch();

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
}

void GameWinScreen::Update(float timeTotal, float timeDelta)
{
	MoveBackground(StaticDataHelper::BACKGROUND_SCROLL_SPEED);
}