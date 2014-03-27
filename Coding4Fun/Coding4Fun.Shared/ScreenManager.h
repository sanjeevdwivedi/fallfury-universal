#pragma once

#include <wrl.h>
#include "DXCore\DirectXBase.h"
#include "Entities\GameState.h"
#include "Entities\LevelType.h"
#include "pch.h"
#include "Audio\AudioEngine.h"
#include "GameScreens\GameScreenBase.h"

using namespace Coding4Fun::FallFury::Entities;

ref class ScreenManager sealed : public DirectXBase
{
public:
	ScreenManager();
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	virtual void OnPointerMoved(Windows::UI::Core::CoreWindow^ window, Windows::UI::Core::PointerEventArgs^ pArgs) override;
	virtual void OnPointerReleased(Windows::UI::Core::CoreWindow^ window, Windows::UI::Core::PointerEventArgs^ pArgs) override;
	virtual void OnPointerPressed(Windows::UI::Core::CoreWindow^ window, Windows::UI::Core::PointerEventArgs^ pArgs) override;

	void Update(float timeTotal, float timeDelta);
internal:
	GameState			CurrentGameState;

	Coding4Fun::FallFury::Screens::GameScreenBase^		CurrentGameScreen;
	Coding4Fun::FallFury::Entities::LevelType			CurrentLevelType;

	bool				IsWithinScreenBoundaries(float2);
	void				LoadScreen();
	void				CheckScreenType();

	Platform::Collections::Map<int, Platform::String^> Levels;
};