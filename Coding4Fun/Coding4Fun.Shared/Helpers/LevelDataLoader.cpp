#include "pch.h"
#include "LevelDataLoader.h"
#include "Entities\Powerup.h"

using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Helpers;
using namespace Coding4Fun::FallFury::Entities;

Coding4Fun::FallFury::Helpers::LevelDataLoader::LevelDataLoader(void)
{
}

Coding4Fun::FallFury::Helpers::LevelDataLoader::LevelDataLoader(Coding4Fun::FallFury::Models::Level^ level, GamePlayScreen^ screen)
{
	PlayScreen = screen;
	CurrentLevel = level;
}

float Coding4Fun::FallFury::Helpers::LevelDataLoader::GetXPosition(float pixels)
{
	return PlayScreen->LoBoundX + pixels;
}


void Coding4Fun::FallFury::Helpers::LevelDataLoader::SetupBear(Coding4Fun::FallFury::Characters::Bear ^gameBear)
{
	gameBear->Position = float2(GetXPosition(CurrentLevel->GameBear->StartPosition), 0);	
	gameBear->MaxHealth = CurrentLevel->GameBear->MaxHealth;
	gameBear->CurrentHealth = CurrentLevel->GameBear->MaxHealth;
	gameBear->CurrentDamage = CurrentLevel->GameBear->Damage;
	gameBear->Velocity.y = CurrentLevel->GameBear->Velocity;
	gameBear->MaxCriticalDamage = CurrentLevel->GameBear->CriticalDamage;
	gameBear->Rotation = 0.0f;
}

void Coding4Fun::FallFury::Helpers::LevelDataLoader::SetupObstacles(std::list<Coding4Fun::FallFury::Entities::Obstacle> &obstacles)
{
	int size = CurrentLevel->Obstacles->Length;

	FallFury::Models::Obstacle^ currentObstacle;

	for (int i = 0; i < size; i++)
	{
		currentObstacle = CurrentLevel->Obstacles[i];	
		
		Coding4Fun::FallFury::Entities::Obstacle newObstacle;
		newObstacle.HealthDamage = currentObstacle->HealthDamage;		
		newObstacle.Type = (Coding4Fun::FallFury::Entities::ObstacleType)currentObstacle->Type;
		newObstacle.Scale = currentObstacle->Scale;

		switch(newObstacle.Type)
		{
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_CLOUD:
				{
					newObstacle.Size = float2(316.0f, 120.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPIKE_NIGHTMARE_LARGE:
				{
					newObstacle.Size = float2(238.0f, 221.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPIKE_NIGHTMARE_MEDIUM:
				{
					newObstacle.Size = float2(444.0f, 252.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPIKE_NIGHTMARE_SMALL:
				{
					newObstacle.Size = float2(467.0f, 241.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_BEAN_A:
				{
					newObstacle.Size = float2(539.0f, 335.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_BEAN_B:
				{
					newObstacle.Size = float2(257.0f, 316.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_BEAN_C:
				{
					newObstacle.Size = float2(344.0f, 292.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_BEAN_D:
				{
					newObstacle.Size = float2(377.0f, 380.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_BEAN_E:
				{
					newObstacle.Size = float2(413.0f, 348.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPACE_ROCKET:
				{
					// NOTE: (sanjeevd) This simply scales the assets appropriately.
					newObstacle.Size = float2(318.0f, 345.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPACE_COMET_A:
				{
					newObstacle.Size = float2(357.0f, 357.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPACE_COMET_B:
				{
					newObstacle.Size = float2(252.0f, 252.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPACE_SATELLITE:
				{
					newObstacle.Size = float2(290.0f, 375.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPACE_UFO:
				{
					newObstacle.Size = float2(387.0f, 355.0f) * newObstacle.Scale;
					break;
				}
		case Coding4Fun::FallFury::Entities::ObstacleType::OT_SPACE_BALL:
				{
					newObstacle.Size = float2(342.0f, 359.0f) * newObstacle.Scale;
					break;
				}
		}

		newObstacle.PixelDiff = currentObstacle->X;
		newObstacle.InflictsDamage = currentObstacle->InflictsDamage;
		newObstacle.Position = float2(GetXPosition(currentObstacle->X),currentObstacle->Y);
		newObstacle.Rotation = currentObstacle->Rotation;

		obstacles.push_back(newObstacle);
	}
}

void Coding4Fun::FallFury::Helpers::LevelDataLoader::SetupMonsters(std::list<Coding4Fun::FallFury::Characters::Monster^> &monsters)
{
	int size = CurrentLevel->Monsters->Length;

	FallFury::Models::Monster^ currentMonster;

	for (int i = 0; i < size; i++)
	{
		currentMonster = CurrentLevel->Monsters[i];
		
		Coding4Fun::FallFury::Characters::Monster^ monster = ref new Coding4Fun::FallFury::Characters::Monster(
			PlayScreen,                              // container
			(Coding4Fun::FallFury::Entities::MonsterType)currentMonster->Type,       // type
			currentMonster->MaxHealth,               // maxHealth
			currentMonster->Damage,                  // currentDamage
			false,                                   // isFriendly
			GetXPosition(currentMonster->X),         // positionX
			currentMonster->Y,                       // positionY
			currentMonster->VelocityX,               // velocityX
			PlayScreen->GameBear->Velocity.y,        // velocityY
			0.0f,                                    // rotation
			currentMonster->Lifetime,                // lifetime
			currentMonster->Bonus,                   // bonus
			currentMonster->Scale,                   // scale
			currentMonster->X,                       // pixelDiff
			i == size - 1);                          // isBoss

		monsters.push_back(monster);
	}
}

void Coding4Fun::FallFury::Helpers::LevelDataLoader::SetupButtons(std::list<Coding4Fun::FallFury::Entities::Button^> &buttons, int &buttonPrice)
{
	int size = CurrentLevel->Buttons->Length;

	FallFury::Models::Button^ currentButton;

	for (int i = 0; i < size; i++)
	{
		currentButton = CurrentLevel->Buttons[i];	
		
		Coding4Fun::FallFury::Entities::Button^ button = ref new Coding4Fun::FallFury::Entities::Button();
		button->Size = float2(54.0f, 54.0f);
		button->Position = float2(GetXPosition(currentButton->X), currentButton->Y);
		button->PixelDiff = currentButton->X;

		buttons.push_back(button);
	}

	buttonPrice = CurrentLevel->LevelMeta->ButtonPrice;
}

void Coding4Fun::FallFury::Helpers::LevelDataLoader::SetupPowerups(std::list<Coding4Fun::FallFury::Entities::Powerup^> &powerups)
{
	int size = CurrentLevel->Powerups->Length;

	FallFury::Models::Powerup^ currentPowerup;

	for (int i = 0; i < size; i++)
	{
		currentPowerup = CurrentLevel->Powerups[i];	
		
		Coding4Fun::FallFury::Entities::Powerup^ powerup = ref new Coding4Fun::FallFury::Entities::Powerup(PlayScreen);
		powerup->Size = float2(100.0f, 100.0f);
		powerup->Position = float2(GetXPosition(currentPowerup->X), currentPowerup->Y);
		powerup->Category = (Coding4Fun::FallFury::Entities::PowerupCategory)currentPowerup->Category;
		powerup->Type = (Coding4Fun::FallFury::Entities::PowerupType)currentPowerup->Type;
		powerup->Lifespan = currentPowerup->Lifespan;
		powerup->Effect = currentPowerup->Effect;
		powerup->PixelDiff = currentPowerup->X;


		powerups.push_back(powerup);
	}
}
