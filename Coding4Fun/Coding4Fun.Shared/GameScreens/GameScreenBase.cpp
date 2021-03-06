#include "pch.h"
#include "GameScreenBase.h"

using namespace Coding4Fun::FallFury::Screens;
using namespace Windows::Graphics::Display;

namespace Coding4Fun {
	namespace FallFury {
		extern float ScreenWidth;
		extern float ScreenHeight;
	}
}

GameScreenBase::GameScreenBase()
{

}

GameScreenBase::GameScreenBase(ScreenManager^ manager)
{

}

void GameScreenBase::RenderScreen()
{

}

void GameScreenBase::Load()
{
	IsLoaded = true;
}

void GameScreenBase::OnPointerMoved(Windows::UI::Core::PointerEventArgs^ pArgs)
{

}

void GameScreenBase::OnPointerReleased(Windows::UI::Core::PointerEventArgs^ pArgs)
{

}

void GameScreenBase::OnPointerPressed(Windows::UI::Core::PointerEventArgs^ pArgs)
{

}

void GameScreenBase::Update(float timeTotal, float timeDelta)
{

}



void GameScreenBase::UpdateWindowSize()
{
	auto rWidth = Manager->m_renderTargetSize.Width;
	auto rHeight = Manager->m_renderTargetSize.Height;

#ifdef WP8
	BACKGROUND_MIDPOINT = rHeight / 2.0f;
	m_backgroundPositionA = 0;
	m_backgroundPositionB = 2 * BACKGROUND_MIDPOINT;
#else
	BACKGROUND_MIDPOINT = Coding4Fun::FallFury::ScreenWidth / 2.0f;
	m_backgroundPositionA = BACKGROUND_MIDPOINT;
	m_backgroundPositionB = m_backgroundPositionA * 3;
#endif


	// gutters on the left and right
	LoBoundX = (rWidth - Coding4Fun::FallFury::ScreenHeight) / 2.0f;
	HiBoundX = LoBoundX + Coding4Fun::FallFury::ScreenHeight;

	LoBoundY = 0;
	HiBoundY = LoBoundY + rHeight;

#ifdef WP8
	m_screenSize = float2(rWidth, rHeight);
#else
	m_screenSize = float2(Coding4Fun::FallFury::ScreenHeight, Coding4Fun::FallFury::ScreenWidth);
#endif
}

// TODO: (sanjeevd) This is where the background movement actually happens
void GameScreenBase::MoveBackground(float velocity)
{
#ifdef WP8
	if (m_backgroundPositionA <= -BACKGROUND_MIDPOINT)
		m_backgroundPositionA = m_backgroundPositionB + 2*BACKGROUND_MIDPOINT;
	
	if (m_backgroundPositionB <= -BACKGROUND_MIDPOINT)
		m_backgroundPositionB = m_backgroundPositionA + 2*BACKGROUND_MIDPOINT;
#else
	if (m_backgroundPositionA <= -BACKGROUND_MIDPOINT)
		m_backgroundPositionA = m_backgroundPositionB + (BACKGROUND_MIDPOINT * 2);

	if (m_backgroundPositionB <= -BACKGROUND_MIDPOINT)
		m_backgroundPositionB = m_backgroundPositionA + (BACKGROUND_MIDPOINT * 2);
#endif
	m_backgroundPositionA -= velocity;
	m_backgroundPositionB -= velocity;


}

void GameScreenBase::HandleKeyInput(Windows::System::VirtualKey)
{

}

Windows::Foundation::Rect GameScreenBase::GetScreenBounds()
{
	return Windows::Foundation::Rect(LoBoundX, LoBoundY, HiBoundX - LoBoundX, HiBoundX - LoBoundX);
}	

void GameScreenBase::InitializeSpriteBatch()
{
	CurrentSpriteBatch = ref new BasicSprites::SpriteBatch();
	unsigned int capacity = 5000;
	if (Manager->m_featureLevel < D3D_FEATURE_LEVEL_9_3)
	{
		capacity = min(1000, capacity);
	}

	CurrentSpriteBatch->Initialize(
		Manager->m_d3dDevice.Get(),
		capacity
		);
}

float GameScreenBase::LoBoundX::get()
{
	return m_loBoundX;
}

void GameScreenBase::LoBoundX::set(float value)
{
	m_loBoundX = value;
}

float GameScreenBase::HiBoundX::get()
{
	return m_hiBoundX;
}

void GameScreenBase::HiBoundX::set(float value)
{
	m_hiBoundX = value;
}

float GameScreenBase::LoBoundY::get()
{
	return m_loBoundY;
}

void GameScreenBase::LoBoundY::set(float value)
{
	m_loBoundY = value;
}

float GameScreenBase::HiBoundY::get()
{
	return m_hiBoundY;
}

void GameScreenBase::HiBoundY::set(float value)
{
	m_hiBoundY = value;
}

bool GameScreenBase::IsLoaded::get()
{
	return m_isLoaded;
}

void GameScreenBase::IsLoaded::set(bool value)
{
	m_isLoaded = value;
}
