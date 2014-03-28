#include "pch.h"
#include "ScreenManager.h"
#include <DirectXMath.h>
#include "GameScreens\GamePlayScreen.h"
#include "GameScreens\GameOverScreen.h"
#include "GameScreens\GameWinScreen.h"
#include "GameScreens\GenericCloudScreen.h"
#include "GameScreens\GameCompleteScreen.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Windows::Graphics::Display;
using namespace Coding4Fun::FallFury::Characters;
using namespace Coding4Fun::FallFury::Screens;

ScreenManager::ScreenManager() 
{
	CurrentGameState = GameState::GS_MAIN_MENU;
}

void ScreenManager::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();
}

void ScreenManager::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();
}

void ScreenManager::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();
}

void ScreenManager::Update(float timeTotal, float timeDelta)
{
	if (CurrentGameScreen != nullptr)
		CurrentGameScreen->Update(timeTotal, timeDelta);
	
	CheckScreenType();
}

void ScreenManager::Render()
{
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(),reinterpret_cast<float*>(&D2D1::ColorF(D2D1::ColorF::Black)));

	if (CurrentGameScreen != nullptr)
	{
		if (!CurrentGameScreen->IsLoaded)
		{
			LoadScreen();
		}
		else
		{
			CurrentGameScreen->RenderScreen();
		}
	}
}

void ScreenManager::CheckScreenType()
{
	switch(CurrentGameState)
	{
	case GameState::GS_LEVEL_SELECT_SINGLE:
		{
			if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_LEVEL_SELECT_SINGLE)
			{
				CurrentGameScreen = ref new GenericCloudScreen(this);
				CurrentGameScreen->ScreenType = GameState::GS_LEVEL_SELECT_SINGLE;
			}
			break;
		}
	case GameState::GS_PLAYING:
			{
				if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_PLAYING)
				{
					CurrentGameScreen = ref new GamePlayScreen(this);				
				}
				break;
			}
	case GameState::GS_GAME_OVER:
			{
				if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_GAME_OVER)
				{
					CurrentGameScreen = ref new GameOverScreen(this);
				}
				break;
			}
	case GameState::GS_MAIN_MENU:
			{
				if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_MAIN_MENU)
				{
					CurrentGameScreen = ref new MenuScreen(this);
				}
				break;
			}
	case GameState::GS_SUBMIT_SCORE:
			{
				if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_SUBMIT_SCORE)
				{
					CurrentGameScreen = ref new GenericCloudScreen(this);
					CurrentGameScreen->ScreenType = GameState::GS_SUBMIT_SCORE;
				}
				break;
	case GameState::GS_TOP_SCORES:
				{
					if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_TOP_SCORES)
					{
						CurrentGameScreen = ref new GenericCloudScreen(this);
						CurrentGameScreen->ScreenType = GameState::GS_TOP_SCORES;
					}
					break;
				}
	case GameState::GS_WIN:
				{
					if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_WIN)
					{
						CurrentGameScreen = ref new GameWinScreen(this);
					}
					break;
				}
			}
	case GameState::GS_ABOUT_SCREEN:
				{
					if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_ABOUT_SCREEN)
					{
						CurrentGameScreen = ref new GenericCloudScreen(this);
						CurrentGameScreen->ScreenType = GameState::GS_ABOUT_SCREEN;
					}
					break;
				}
	case GameState::GS_HOW_TO:
				{
					if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_HOW_TO)
					{
						CurrentGameScreen = ref new GenericCloudScreen(this);
						CurrentGameScreen->ScreenType = GameState::GS_HOW_TO;
					}
					break;
				}
	case GameState::GS_FULL_WIN:
				{
					if (CurrentGameScreen == nullptr || CurrentGameScreen->ScreenType != GameState::GS_FULL_WIN)
					{
						CurrentGameScreen = ref new GameCompleteScreen(this);
						CurrentGameScreen->ScreenType = GameState::GS_FULL_WIN;
					}
					break;
				}
	}
}

void ScreenManager::OnPointerMoved(CoreWindow^ window,PointerEventArgs^ pArgs)
{
	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->OnPointerMoved(pArgs);
	}
}

void ScreenManager::OnPointerReleased(CoreWindow^ window,PointerEventArgs^ pArgs)
{
	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->OnPointerReleased(pArgs);
	}
}

void ScreenManager::OnPointerPressed(CoreWindow^ window,PointerEventArgs^ pArgs)
{
	if (CurrentGameScreen != nullptr)
	{
		CurrentGameScreen->OnPointerPressed(pArgs);
	}
}

void ScreenManager::LoadScreen()
{
	CurrentGameScreen->Load();
}

bool ScreenManager::IsWithinScreenBoundaries(float2 position)
{
	// TODO: (sanjeevd) following variables only for checking values
	float loX = CurrentGameScreen->LoBoundX;
	float hiX = CurrentGameScreen->HiBoundX;

	if (position.x < CurrentGameScreen->LoBoundX || position.x > CurrentGameScreen->HiBoundX || position.y < CurrentGameScreen->LoBoundY || position.y > CurrentGameScreen->HiBoundY)
		return false;
	else
		return true;
}