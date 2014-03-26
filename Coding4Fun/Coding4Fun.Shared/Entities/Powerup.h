#pragma once

#include "pch.h"
#include "PowerupType.h"
#include "PowerupCategory.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Screens
		{
			ref class GamePlayScreen;
		}

		namespace Entities
		{
			
			ref class Powerup
			{
			internal:
				Powerup(void);
				Powerup(Coding4Fun::FallFury::Screens::GamePlayScreen^ screen);		

				Coding4Fun::FallFury::Screens::GamePlayScreen^			CurrentScreen;

				PowerupCategory											Category;
				PowerupType												Type;
				float2													Size;
				float2													Position;

				float													CircleScale;
				float													PulsationTimer;
				float4													CircleShading;

				float													Lifespan;
				float													Effect;
				float													Rotation;

				float													PixelDiff;

				void Render();
				void Load(std::map<PowerupType, Microsoft::WRL::ComPtr<ID3D11Texture2D>> &powerupTextures);
				void Update(float timeDelta);

				Windows::Foundation::Rect GetBoundingBox()
				{
					return Windows::Foundation::Rect(Position.x - (Size.x / 2),
						Position.y - (Size.y / 2), Size.x, Size.y);
				}

			private:
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					Texture;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					CircleTexture;
				float PulsationInterval;
			};
		}
	}
}

