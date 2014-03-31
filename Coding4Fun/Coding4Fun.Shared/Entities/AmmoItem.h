#pragma once
#include "ParticleSystem\ParticleCore.h"
#include "DXCore\SpriteWorks.h"
#include "Entities\Powerup.h"
#include "Entities\PowerupCategory.h"
#include "Entities\PowerupType.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Entities
		{
			ref class AmmoItem
			{
			internal:
				AmmoItem()
				{
				}
				AmmoItem(SpriteBatch^ batch)
				{
					totalTime = 0.0f;
					Rotation = 0.0f;
					CurrentSpriteBatch = batch;
				}

				float		Rotation;
				float		HealthDamage;
				float2		Position;
				float2		Velocity;
				float2		Size;
				PowerupType Type;

				bool		IsFriendly;

				SpriteBatch^	CurrentSpriteBatch;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>	MainTexture;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>	TextureA;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>	TextureB;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>	TextureC;

				float totalTime;

				Windows::Foundation::Rect GetBoundingBox()
				{
					return Windows::Foundation::Rect(Position.x - (Size.x / 2),
						Position.y - (Size.y / 2), Size.x, Size.y);
				}

				void Update(float timeDelta, ParticleCore* particleCore)
				{
					if (Type == PowerupType::PLASMA_BALL)
					{
						if ((Velocity.x > 0 && Velocity.y > 0) || (Velocity.x > 0 && Velocity.y <0))
						{
							Rotation = CalculateRadians(Velocity);
							Rotation += CalculateRadiansFromAngle(180) - 2 * Rotation;
						}
						else
						{
							Rotation = CalculateRadians(Velocity);
						}
					}
					else
					{
						Rotation += 0.2f;
					}

					Position.x -= Velocity.x;
					Position.y -= Velocity.y;

					totalTime += timeDelta;

					if (totalTime <= 0.1f)
					{
						MainTexture = TextureA;
					}
					else if (totalTime > 0.1f && totalTime <= 0.2f)
					{
						MainTexture = TextureB;
					}
					else
					{
						MainTexture = TextureC;
					}

					if (totalTime >= 0.3f)
					{
						totalTime = 0.0f;
					}

				}

				void Render()
				{
					if (MainTexture != nullptr)
					{
						CurrentSpriteBatch->Draw(
							MainTexture.Get(),
							Position,
							PositionUnits::Pixels,
							Size,
							SizeUnits::Pixels,
							float4(1.0f,1.0f,1.0f,1.0f),
							Rotation);
					}
				}
			};
		}
	}
}