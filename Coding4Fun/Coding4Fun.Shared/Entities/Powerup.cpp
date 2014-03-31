#include "pch.h"
#include "Powerup.h"
#include "GameScreens\GamePlayScreen.h"

using namespace Coding4Fun::FallFury::Entities;
using namespace Coding4Fun::FallFury::Screens;

Coding4Fun::FallFury::Entities::Powerup::Powerup(void)
{
}

Coding4Fun::FallFury::Entities::Powerup::Powerup(GamePlayScreen^ screen)
{
	CurrentScreen = screen;
	PulsationInterval = 0.6f;
	CircleScale = 1.0f;
	CircleShading = float4(1.0f, 1.0f, 1.0f, 0.0f);
	Rotation = 0.0f;
}

void Coding4Fun::FallFury::Entities::Powerup::Render()
{
	CurrentScreen->CurrentSpriteBatch->Draw(
		CircleTexture.Get(),
		Position,
		PositionUnits::Pixels,
		Size * CircleScale,
		SizeUnits::Pixels,
		CircleShading,
		0);

	CurrentScreen->CurrentSpriteBatch->Draw(
		Texture.Get(),
		Position,
		PositionUnits::Pixels,
		Size,
		SizeUnits::Pixels,
		float4(1.0f, 1.0f, 1.0f, 1.0f),
		Rotation);
}

void Coding4Fun::FallFury::Entities::Powerup::Update(float timeDelta)
{
	PulsationTimer += timeDelta;
	if (PulsationTimer <= PulsationInterval)
	{
		CircleScale += 0.03f;
		CircleShading.w += 0.03f;
		Rotation -= 0.01f;
	}
	else if (PulsationTimer >= PulsationInterval && PulsationTimer <= PulsationInterval * 2.0f)
	{
		CircleScale -= 0.03f;
		CircleShading.w -= 0.03f;
		Rotation += 0.01f;
	}
	else
	{
		CircleShading.w = 0.0f;
		PulsationTimer = 0.0f;
	}
}

void Coding4Fun::FallFury::Entities::Powerup::Load(std::map<PowerupType, Microsoft::WRL::ComPtr<ID3D11Texture2D>> &powerupTextures)
{
	Texture = powerupTextures[Type];
	CircleTexture = powerupTextures[PowerupType::CIRCLE];
}

