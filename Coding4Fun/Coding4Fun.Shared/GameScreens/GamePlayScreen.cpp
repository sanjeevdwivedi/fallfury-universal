#include "pch.h"
#include "GameScreens\GamePlayScreen.h"
#include "Helpers\Geometry.h"
#include "Helpers\LevelDataLoader.h"

using namespace concurrency;
using namespace Windows::Graphics::Display;
using namespace Coding4Fun::FallFury;
using namespace Coding4Fun::FallFury::Entities;
using namespace Coding4Fun::FallFury::Characters;
using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Audio;
using namespace Coding4Fun::FallFury::Helpers;


namespace Coding4Fun {
	namespace FallFury {
		extern float ScreenWidth;
		extern float ScreenHeight;
	}
}
//		namespace Screens
//{
GamePlayScreen::GamePlayScreen(ScreenManager^ parent)
	: m_particleSystem(this)
{
	m_rotationThreshold = 0.2f;
	m_accelerationMultiplier = 10.0f;

	m_ammoCollection.clear();
	parent->CurrentGameState = GameState::GS_PLAYING;
	ScreenType = GameState::GS_PLAYING;
	Manager = parent;

	m_badHealthTimer = 0.0f;
	m_badHealthShading = float4(1.0f, 1.0f, 1.0f, 0.0f);
	m_badHealthShadingCoefficient = 0.01f;
	m_badHealthTimeThreshold = 1.0f;

	StartBackground();
}

void GamePlayScreen::Update(float timeTotal, float timeDelta)
{
	if (IsLevelLoaded)
	{
		UpdateBadHealthOverlay(timeDelta);

		if ((GameBear->Position.y / HiBoundY) < 0.19f)
		{
			GameBear->Position.y += GameBear->Velocity.y * (3.2f);
		}

		if (!m_isBackgroundMoving)
		{
			if (GameBear->Position.y > m_screenSize.y)
			{
				Manager->CurrentGameState = GameState::GS_GAME_OVER;
			}
			else
			{
				GameBear->Position.y += GameBear->Velocity.y * 1.5f;
			}
		}

		float compositeRotation = GameBear->Rotation - (float)m_xAcceleration / 10.0f;
		if (compositeRotation < m_rotationThreshold && compositeRotation > -m_rotationThreshold)
			GameBear->Rotation = compositeRotation;

		float compositePosition = GameBear->Position.x + ((float)m_xAcceleration * m_accelerationMultiplier);

#ifndef WP8
		// TODO: (sanjeevd) Fix this for Phone
		if (m_xAcceleration < 0)
			compositePosition -= 100.0f;
		else
			compositePosition += 100.0f;
#endif

		if (Manager->IsWithinScreenBoundaries(float2(compositePosition, GameBear->Position.y)))
		{
			GameBear->Position.x += (float)m_xAcceleration * m_accelerationMultiplier;

			// TODO: (sanjeevd) following variables only to read the values. Remove
			double gameBearPosition = GameBear->Position.x;
			double multiplier = m_accelerationMultiplier;
			int x = 10;
		}
		else
		{
			if (GameBear->Position.x > HiBoundX)
			{
				GameBear->Position.x = HiBoundX - 180.0f;
			}
			else if (GameBear->Position.x < LoBoundX)
			{
				GameBear->Position.x = LoBoundX + 180.0f;
			}
		}


		GameBear->Update(timeTotal,timeDelta,GameBear->Position);


		m_particleSystem.Update(timeDelta);

		if (m_isBackgroundMoving)
		{
			UpdatePowerups(timeDelta);
			MoveOverlay(GameBear->Velocity.y);
			MoveSpaceOverlay(GameBear->Velocity.y * 2.0f);
			MoveBackground(StaticDataHelper::BACKGROUND_SCROLL_SPEED);
			UpdateButtons();
			UpdateObstacles();
			MoveMonsters(timeTotal,timeDelta);
		}

		UpdateAmmo(timeDelta);
		CheckForCollisionWithAmmo(GameBear);
		CheckForOutOfBoundsAmmo();

		m_lengthCounter += GameBear->Velocity.y;
	}

	if (m_systemAccelerometer == nullptr || !StaticDataHelper::IsAccelerometerEnabled)
	{
		if (m_xAcceleration > 0.0f)
		{
			m_xAcceleration -= 0.015f;
		}
		else
		{
			m_xAcceleration += 0.015f;
		}
	}

	CheckForExpiredAssets();
}

void GamePlayScreen::UpdateAmmo(float timeDelta)
{
	for (auto shell = m_ammoCollection.begin(); shell != m_ammoCollection.end(); shell++)
	{	
		(*shell)->Update(timeDelta, &m_particleSystem);
	}
}

void GamePlayScreen::MoveMonsters(float timeTotal ,float timeDelta)
{
	for (auto monster = m_monsters.begin(); monster != m_monsters.end();)
	{
		Monster^ currentMonster = (*monster);

		if ((currentMonster->Position.y - GameBear->Position.y) < -Manager->m_renderTargetSize.Height / 2)
		{
			monster = m_monsters.erase(monster);
		}
		else
		{
			currentMonster->Velocity.y = GameBear->Velocity.y;

			currentMonster->Update(timeTotal, timeDelta, GameBear->Position, GetScreenBounds());
			CheckForCollisionWithAmmo(currentMonster);
			++monster;
		}
	}
}

void GamePlayScreen::CheckForExpiredAssets()
{
	for (auto l_Iter = m_obstacles.begin(); l_Iter != m_obstacles.end();)
	{
		if (l_Iter->Position.y < -300.0f)
		{
			l_Iter = m_obstacles.erase(l_Iter);
		}
		else
		{
			++l_Iter;
		}
	}

	for (auto l_Iter = m_buttons.begin(); l_Iter != m_buttons.end(); /* nothing here */ )
	{
		if ((*l_Iter)->Position.y < - 300.0f)
		{
			l_Iter = m_buttons.erase(l_Iter);
		}
		else
		{
			++l_Iter;
		}
	}
}

void GamePlayScreen::LoadSounds()
{
	AudioManager::SetGamePlayMusic(m_currentLevelType);
}

void GamePlayScreen::CheckBearHealth()
{
	if (GameBear->CurrentHealth <= 0)
	{		
		m_particleSystem.ActivateSet("Buttons",GameBear->Position, true);
		GameBear->Kill();

		StopBackground();
	}
}

void GamePlayScreen::CheckForOutOfBoundsAmmo()
{
	for (auto l_Iter = m_ammoCollection.begin(); l_Iter != m_ammoCollection.end(); /* nothing here */ )
	{
		if (!GamePlayScreen::Manager->IsWithinScreenBoundaries((*l_Iter)->Position))
		{
			l_Iter = m_ammoCollection.erase(l_Iter);
		}
		else
		{
			++l_Iter;
		}
	}

}

void GamePlayScreen::UpdateObstacles()
{
	Windows::Foundation::Rect livingEntityBoundingBox = GameBear->GetBoundingBox();

	for (auto obstacle = m_obstacles.begin(); obstacle != m_obstacles.end(); obstacle++)
	{
		// NOTE: (sanjeevd) The pixel diff is being used to determine exactly where
		// the obstacle is. This is a bit wonky as if the rest of the arithemetic is being
		// done in DIPS, pixel arithemtic breaks. 
		obstacle->Position.x = LoBoundX + obstacle->PixelDiff;

		obstacle->Position.y -= GameBear->Velocity.y;

		Windows::Foundation::Rect obstacleRect = obstacle->GetBoundingBox();
		if (livingEntityBoundingBox.IntersectsWith(obstacleRect) && obstacle->InflictsDamage)
		{
			m_particleSystem.ActivateSet("SmallExplosion", GameBear->Position, true);	

			switch(obstacle->Type)
			{
			case ObstacleType::OT_BEAN_A:
			case ObstacleType::OT_BEAN_B:
			case ObstacleType::OT_BEAN_C:
			case ObstacleType::OT_BEAN_D:
			case ObstacleType::OT_BEAN_E:
			case ObstacleType::OT_SPACE_SATELLITE:
				{
					AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::OuchB);
					AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::HardSoftCollision);
					break;
				}
			case ObstacleType::OT_CLOUD:
			case ObstacleType::OT_SPACE_BALL:
			case ObstacleType::OT_SPACE_COMET_A:
			case ObstacleType::OT_SPACE_COMET_B:
				{
					AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::OuchC);
					AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::SharpSoftCollision);
					break;
				}
			case ObstacleType::OT_SPIKE_NIGHTMARE_LARGE:
			case ObstacleType::OT_SPIKE_NIGHTMARE_MEDIUM:
			case ObstacleType::OT_SPIKE_NIGHTMARE_SMALL:
			case ObstacleType::OT_SPACE_ROCKET:
			case ObstacleType::OT_SPACE_UFO:
				{
					AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::OuchA);
					AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::SharpSoftCollision);
					break;
				}
			}

			GameBear->InflictDamage(obstacle->HealthDamage);
			obstacle->InflictsDamage = false;

			GameBear->RedShade();

			CheckBearHealth();
		}
	}
}


void GamePlayScreen::Shoot(float posX, float posY, float veloX, float veloY, float healthDamage, bool isFriendly, bool isBoss, SpriteBatch^  batch)
{
	AmmoItem^ item = ref new AmmoItem(batch);

	if (!isFriendly)
	{
		item->Size = float2(150.0f, 60.0f);

		// Monsters currently use two types of powerups, no need to add extra rotation
		// so mark them as plasma.
		item->Type = PowerupType::PLASMA_BALL;

		if (!isBoss)
		{
			item->TextureA = StaticDataHelper::MonsterWeapon;
			item->TextureB = StaticDataHelper::MonsterWeapon;
			item->TextureC = StaticDataHelper::MonsterWeapon;
		}
		else
		{
			item->TextureA = StaticDataHelper::FlameA;
			item->TextureB = StaticDataHelper::FlameB;
			item->TextureC = StaticDataHelper::FlameC;
		}
	}
	else
	{
		item->Type = GameBear->WeaponType;
	}

	item->Position = float2(posX, posY);
	item->Velocity = float2(veloX, veloY);
	item->IsFriendly = isFriendly;
	item->HealthDamage = healthDamage;

	m_ammoCollection.push_back(item);
}

bool doOnce = false;
void GamePlayScreen::AccelerometerReadingChanged(_In_ Accelerometer^ accelerometer, _In_ AccelerometerReadingChangedEventArgs^ args)
{
	if (StaticDataHelper::IsAccelerometerEnabled)
	{
		// TODO: (sanjeevd) hacky hacky, accelerometer logic is completely broken
			auto currentOrientation = DisplayProperties::CurrentOrientation;
			double accelValue;

			double accelX = args->Reading->AccelerationX;
			double accelY = args->Reading->AccelerationY;
			if (currentOrientation == DisplayOrientations::Portrait)
#ifdef WP8
				accelValue = args->Reading->AccelerationX;
#else
				// TODO: (sanjeevd) This is broken. On Phone, the Y sensor permanently reads -1 when vertical
				// This should actually read from X value
				accelValue = args->Reading->AccelerationY;
#endif
			else if (currentOrientation == DisplayOrientations::PortraitFlipped)
#ifdef WP8
				accelValue = -args->Reading->AccelerationX;
#else
				accelValue = -args->Reading->AccelerationY;
#endif
			else if (currentOrientation == DisplayOrientations::Landscape)
#ifdef WP8
				accelValue = args->Reading->AccelerationY;
#else
				accelValue = args->Reading->AccelerationX;
#endif
			else if (currentOrientation == DisplayOrientations::LandscapeFlipped)
#ifdef WP8
				accelValue = -args->Reading->AccelerationY;
#else
				accelValue = -args->Reading->AccelerationX;
#endif
			else
				accelValue = 0.0f;

			if (StaticDataHelper::IsAccelerometerInverted)
				m_xAcceleration = -accelValue;
			else
				m_xAcceleration = accelValue;
	}
}

void GamePlayScreen::OnPointerMoved(Windows::UI::Core::PointerEventArgs^ args)
{
	if (StaticDataHelper::IsMouseEnabled)
	{
		if (GameBear != nullptr)
		{
			m_touchCounter++;
			if (GameBear->IsWithinScreenBoundaries(GameBear->Size.x, 0.0f, GetScreenBounds()))
			{
				m_xAcceleration =  (args->CurrentPoint->RawPosition.X - GameBear->Position.x) / m_screenSize.x;
			}
		}
	}
}

void GamePlayScreen::OnPointerReleased(Windows::UI::Core::PointerEventArgs^ args)
{
	if (m_touchCounter < 20)
	{
		if (GameBear != nullptr && IsLevelLoaded)
			GameBear->ShootAtTarget(float2(args->CurrentPoint->RawPosition.X, args->CurrentPoint->RawPosition.Y));
	}
	m_touchCounter = 0;
}

void GamePlayScreen::OnPointerPressed(Windows::UI::Core::PointerEventArgs^ args)
{
	//TODO: (sanjeevd) hacked up the pointer pressed event on WP8 to apply acceleration
#ifdef WP8
	if (StaticDataHelper::IsMouseEnabled)
	{
		if (GameBear != nullptr)
		{
			m_touchCounter++;
			if (GameBear->IsWithinScreenBoundaries(GameBear->Size.x, 0.0f, GetScreenBounds()))
			{
				double currentPointX = args->CurrentPoint->RawPosition.X;
				double gameBearPointX = GameBear->Position.x;
				m_xAcceleration = (args->CurrentPoint->RawPosition.X - GameBear->Position.x) / m_screenSize.x;
				if (m_xAcceleration > 0) m_xAcceleration = 0.85f;
				if (m_xAcceleration < 0) m_xAcceleration = -0.85f;
			}
		}
	}
#endif
}

void GamePlayScreen::CheckForCollisionWithPowerups()
{
	Powerup^ currentPowerup;
	Windows::Foundation::Rect livingEntityBoundingBox = GameBear->GetBoundingBox();

	for (auto powerup = m_powerups.begin(); powerup != m_powerups.end();)
	{
		currentPowerup = (*powerup);

		if (Geometry::IsInProximity(GameBear->Position,currentPowerup->Position,100))
		{
			Windows::Foundation::Rect obstacleRect = currentPowerup->GetBoundingBox();
			if (livingEntityBoundingBox.IntersectsWith(obstacleRect))
			{
				AudioManager::AudioEngineInstance.PlaySoundEffect(GenericPowerup);

				GameBear->PickupPowerup(currentPowerup, &m_particleSystem);

				powerup = m_powerups.erase(powerup);
			}
			else
				++powerup;
		}
		else
		{
			++powerup;
		}
	}
}

void GamePlayScreen::CheckForCollisionWithAmmo(LivingEntity^ entity)
{
	if (entity != nullptr)
	{
		Windows::Foundation::Rect livingEntityBoundingBox = entity->GetBoundingBox();

		if (entity->IsFriendly)
		{
			for (auto ammo = m_ammoCollection.begin(); ammo != m_ammoCollection.end();)
			{
				if (!(*ammo)->IsFriendly)
				{
					Windows::Foundation::Rect ammoBoundingBox = (*ammo)->GetBoundingBox();

					if (livingEntityBoundingBox.IntersectsWith(ammoBoundingBox))
					{
						m_particleSystem.ActivateSet("SmallExplosion",entity->Position, true);

						entity->InflictDamage((*ammo)->HealthDamage);
						GameBear->RedShade();

						AudioManager::AudioEngineInstance.PlaySoundEffect(OuchA);
						AudioManager::AudioEngineInstance.PlaySoundEffect(HardSoftCollision);

						CheckBearHealth();

						ammo = m_ammoCollection.erase(ammo);
					}
					else
					{
						++ammo;
					}
				}
				else
				{
					++ammo;
				}
			}
		}
		else
		{
			for (auto ammo = m_ammoCollection.begin(); ammo != m_ammoCollection.end();)
			{
				if ((*ammo)->IsFriendly)
				{
					Windows::Foundation::Rect ammoBoundingBox = (*ammo)->GetBoundingBox();

					if (livingEntityBoundingBox.IntersectsWith(ammoBoundingBox))
					{ 
						Monster^ monster = ((Monster^)entity);

						if (!monster->IsDead && monster->IsActive)
						{
							m_particleSystem.ActivateSet("SmallExplosion", entity->Position, true);
							entity->InflictDamage((*ammo)->HealthDamage);

							monster->RedShade();
							monster->CheckIfAlive();

							AudioManager::AudioEngineInstance.PlaySoundEffect(SharpSoftCollision);
						}
						ammo = m_ammoCollection.erase(ammo);
					}
					else
					{
						++ammo;
					}
				}
				else
				{
					++ammo;
				}
			}
		}
	}
}

void GamePlayScreen::RenderScreen()
{
	if (IsLevelLoaded)
	{
		CurrentSpriteBatch->Begin();

#ifdef WP8
		// TODO: (sanjeevd) everything should probably be in Pixels or Pixels, for Phone, the following is converted to Pixels
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
			PositionUnits::Pixels,
			m_screenSize,
			SizeUnits::Pixels);

		CurrentSpriteBatch->Draw(
			m_backgroundBlockB.Get(),
			float2(Manager->m_windowBounds.Width / 2, m_backgroundPositionB),
			PositionUnits::Pixels,
			m_screenSize,
			SizeUnits::Pixels);
#endif


		CurrentSpriteBatch->Draw(
			m_overlayA.Get(),
			float2(Manager->m_windowBounds.Width / 2, m_overlayPositionA),
			PositionUnits::Pixels,
			float2(Coding4Fun::FallFury::ScreenHeight, Coding4Fun::FallFury::ScreenWidth),
			SizeUnits::Pixels);

		CurrentSpriteBatch->Draw(
			m_overlayB.Get(),
			float2(Manager->m_windowBounds.Width / 2, m_overlayPositionB),
			PositionUnits::Pixels,
			float2(Coding4Fun::FallFury::ScreenHeight, Coding4Fun::FallFury::ScreenWidth),
			SizeUnits::Pixels);

		if (m_currentLevelType == LevelType::SPACE)
		{
			CurrentSpriteBatch->Draw(
				m_overlayGalaxyA.Get(),
				float2(Manager->m_windowBounds.Width / 2, m_overlayGalaxyPositionA),
				PositionUnits::Pixels,
				float2(Coding4Fun::FallFury::ScreenHeight, Coding4Fun::FallFury::ScreenWidth),
				SizeUnits::Pixels);

			CurrentSpriteBatch->Draw(
				m_overlayGalaxyB.Get(),
				float2(Manager->m_windowBounds.Width / 2, m_overlayGalaxyPositionB),
				PositionUnits::Pixels,
				float2(Coding4Fun::FallFury::ScreenHeight, Coding4Fun::FallFury::ScreenWidth),
				SizeUnits::Pixels);
		}

		CurrentSpriteBatch->Draw(
			m_badHealthOverlay.Get(),
			float2(Manager->m_windowBounds.Width / 2, Manager->m_windowBounds.Height / 2),
			PositionUnits::Pixels,
			m_screenSize,
			SizeUnits::Pixels,
			m_badHealthShading,
			0.0f);


		if (!m_ammoCollection.empty())
		{
			for (auto shell = m_ammoCollection.begin(); shell != m_ammoCollection.end(); shell++)
			{	
				if (!(*shell)->IsFriendly)
				{
					(*shell)->Render();
				}
				else
				{
					GameBear->RenderShell((*shell)->Position, (*shell)->Rotation);
				}
			}
		}

		for (auto button = m_buttons.begin(); button != m_buttons.end(); button++)
		{
			Button^ loopButton = (*button);

			if (loopButton->Position.y <= HiBoundY + 200.0f && loopButton->Position.y > -200.0f)
			{
				CurrentSpriteBatch->Draw(
					m_buttonTexture.Get(),
					loopButton->Position,
					PositionUnits::Pixels,
					loopButton->Size,
					SizeUnits::Pixels);
			}
		}

		if (!m_powerups.empty())
		{
			for (auto powerup = m_powerups.begin(); powerup != m_powerups.end(); powerup++)
			{
				Powerup^ loopPowerup = (*powerup);
				if (loopPowerup->Position.y <= HiBoundY + 200.0f && loopPowerup->Position.y > -200.0f)
				{
					loopPowerup->Render();
				}
			}
		}

		GameBear->Render();

		m_particleSystem.Render();

		for (auto obstacle = m_obstacles.begin(); obstacle != m_obstacles.end(); obstacle++)
		{
			if (obstacle->Position.y <= HiBoundY + 200.0f && obstacle->Position.y > -200.0f)
			{
				switch(obstacle->Type)
				{
				case ObstacleType::OT_SPIKE_NIGHTMARE_LARGE:
					{
						DrawObstacle((*obstacle), m_spikeObstacleLarge.Get());
						break;
					}
				case ObstacleType::OT_SPIKE_NIGHTMARE_MEDIUM:
					{
						DrawObstacle((*obstacle), m_spikeObstacleMedium.Get());
						break;
					}
				case ObstacleType::OT_SPIKE_NIGHTMARE_SMALL:
					{
						DrawObstacle((*obstacle), m_spikeObstacleSmall.Get());
						break;
					}
				case ObstacleType::OT_CLOUD:
					{
						DrawObstacle((*obstacle), m_cloudObstacleTexture.Get());
						break;
					}
				case ObstacleType::OT_BEAN_A:
					{
						DrawObstacle((*obstacle), m_plantTextureA);
						break;
					}
				case ObstacleType::OT_BEAN_B:
					{
						DrawObstacle((*obstacle), m_plantTextureB);
						break;
					}
				case ObstacleType::OT_BEAN_C:
					{
						DrawObstacle((*obstacle), m_plantTextureC);
						break;
					}
				case ObstacleType::OT_BEAN_D:
					{
						DrawObstacle((*obstacle), m_plantTextureD);
						break;
					}
				case ObstacleType::OT_BEAN_E:
					{
						DrawObstacle((*obstacle), m_plantTextureE);
						break;
					}
				case ObstacleType::OT_SPACE_BALL:
					{
						DrawObstacle((*obstacle), m_spaceObstacleF);
						break;
					}
				case ObstacleType::OT_SPACE_COMET_A:
					{
						DrawObstacle((*obstacle), m_spaceObstacleB);
						break;
					}
				case ObstacleType::OT_SPACE_COMET_B:
					{
						DrawObstacle((*obstacle), m_spaceObstacleC);
						break;
					}
				case ObstacleType::OT_SPACE_ROCKET:
					{
						DrawObstacle((*obstacle), m_spaceObstacleA);
						break;
					}
				case ObstacleType::OT_SPACE_SATELLITE:
					{
						DrawObstacle((*obstacle), m_spaceObstacleD);
						break;
					}
				case ObstacleType::OT_SPACE_UFO:
					{
						DrawObstacle((*obstacle), m_spaceObstacleE);
						break;
					}
				}
			}

		}


		for (auto monster = m_monsters.begin(); monster != m_monsters.end(); monster++)
		{
			(*monster)->Render();
		}

		CurrentSpriteBatch->End();
	}
}

void GamePlayScreen::UpdateBadHealthOverlay(float timeSpan)
{
	float healthPercentage = GameBear->CurrentHealth / GameBear->MaxHealth;
	if (healthPercentage <= 0.2f && healthPercentage >= 0.15f)
	{
		m_badHealthTimeThreshold = 1.0f;
		m_badHealthShadingCoefficient = 0.01f;
	}
	else if (healthPercentage < 0.15f && healthPercentage >= 0.1f)
	{
		m_badHealthTimeThreshold = 0.7f;
		m_badHealthShadingCoefficient = 0.02f;
	}
	else if (healthPercentage < 0.1f && healthPercentage > 0.0f)
	{
		m_badHealthTimeThreshold = 0.5f;
		m_badHealthShadingCoefficient = 0.04f;
	}
	else
	{
		m_badHealthShadingCoefficient = 0.0f;
		m_badHealthShading.w = 0.0f;
		m_badHealthTimeThreshold = 0.0f;
	}

	m_badHealthTimer += timeSpan;

	if (m_badHealthTimer <= m_badHealthTimeThreshold)
	{
		if (m_badHealthShading.w < 1.0f)
			m_badHealthShading.w += m_badHealthShadingCoefficient;
	}
	else if (m_badHealthTimer > m_badHealthTimeThreshold && m_badHealthTimer <= m_badHealthTimeThreshold * 2.0f)
	{
		if (m_badHealthShading.w > m_badHealthShadingCoefficient)
			m_badHealthShading.w -= m_badHealthShadingCoefficient;
	}
	else
	{
		m_badHealthShading.w = 0.0f;
		m_badHealthTimer = 0.0f;
	}
}

void GamePlayScreen::DrawObstacle(Coding4Fun::FallFury::Entities::Obstacle obstacle, Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
{
	CurrentSpriteBatch->Draw(
		texture.Get(),
		obstacle.Position,
		PositionUnits::Pixels,
		obstacle.Size,
		SizeUnits::Pixels,
		float4(1.0f, 1.0f, 1.0f, 1.0f),
		obstacle.Rotation);
}

void GamePlayScreen::Load()
{
	GameScreenBase::Load();
	UpdateWindowSize();

	StaticDataHelper::ButtonsCollected = 0;
	StaticDataHelper::ButtonsTotal = 0;
	StaticDataHelper::MonstersKilled = 0;
	StaticDataHelper::SecondsTotal = 0;

	m_systemAccelerometer = Windows::Devices::Sensors::Accelerometer::GetDefault();

	if (m_systemAccelerometer != nullptr)
	{
		m_systemAccelerometer->ReadingChanged += ref new TypedEventHandler<Accelerometer^, AccelerometerReadingChangedEventArgs^>
			(this, &GamePlayScreen::AccelerometerReadingChanged);
	}

	m_xAcceleration = 0;

	m_overlayPositionA = BACKGROUND_MIDPOINT;
	m_overlayPositionB = m_overlayPositionA * 3;

	m_overlayGalaxyPositionA = BACKGROUND_MIDPOINT;
	m_overlayGalaxyPositionB = m_overlayGalaxyPositionA * 3;

	LoadLevelXML();
}

void GamePlayScreen::LoadLevelXML()
{
	Coding4Fun::FallFury::Reader^ reader = ref new Coding4Fun::FallFury::Reader();

	Platform::String^ LevelName = Manager->Levels.Lookup(StaticDataHelper::CurrentLevelID);

	Windows::Foundation::IAsyncOperation<Platform::Object^>^ result = 
		reader->ReadXmlAsync(LevelName, Coding4Fun::FallFury::Models::XmlType::LEVEL);

	result->Completed =
		ref new AsyncOperationCompletedHandler<Platform::Object^>(this, &GamePlayScreen::OnLevelLoadCompleted);
}

void GamePlayScreen::OnLevelLoadCompleted(IAsyncOperation<Platform::Object^> ^op, AsyncStatus s)
{
	if (s == AsyncStatus::Completed)
	{
		InitializeSpriteBatch();	
		m_loader = ref new BasicLoader(Manager->m_d3dDevice.Get(), Manager->m_wicFactory.Get());
		CreateBear();

		LevelDataLoader^ loader = ref new LevelDataLoader((Coding4Fun::FallFury::Models::Level^)op->GetResults(), this);
		loader->SetupBear(GameBear);
		loader->SetupObstacles(m_obstacles);
		loader->SetupMonsters(m_monsters);
		loader->SetupButtons(m_buttons, m_buttonPrice);
		loader->SetupPowerups(m_powerups);
		m_currentLevelType = (LevelType) loader->CurrentLevel->LevelMeta->LevelType;

		StaticDataHelper::CurrentLevel = loader->CurrentLevel;
		StaticDataHelper::ButtonsTotal = loader->CurrentLevel->Buttons->Length;

		LoadTextures();

		CreateMonster();
		CreatePowerups();

		IsLevelLoaded = true;

		GameBear->TurnRight();
		m_particleSystem.CreatePreCachedParticleSets();
		LoadSounds();
	}
}

void GamePlayScreen::CreatePowerups()
{
	for (auto powerup = m_powerups.begin(); powerup != m_powerups.end(); powerup++)
	{
		(*powerup)->Load(m_powerupTextures);
	}
}

void GamePlayScreen::UpdatePowerups(float timeDelta)
{
	if (m_powerups.size() > 0)
	{
		for (auto powerup = m_powerups.begin(); powerup != m_powerups.end(); powerup++)
		{
			(*powerup)->Update(timeDelta);
			(*powerup)->Position.x = (*powerup)->PixelDiff + LoBoundX;
			(*powerup)->Position.y -= GameBear->Velocity.y;
		}
	}

	CheckForCollisionWithPowerups();
}

void GamePlayScreen::LoadTextures()
{
	m_loader->LoadTexture("DDS\\Levels\\Nightmare\\spikesLarge.dds", &m_spikeObstacleLarge, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\Nightmare\\spikesMedium.dds", &m_spikeObstacleMedium, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\Nightmare\\spikesSmall.dds", &m_spikeObstacleSmall, nullptr);

	m_loader->LoadTexture("DDS\\Levels\\Nightmare\\cloudBig.dds", &m_cloudObstacleTexture, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\MagicBean\\obstacle01.dds", &m_plantTextureA, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\MagicBean\\obstacle02.dds", &m_plantTextureB, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\MagicBean\\obstacle03.dds", &m_plantTextureC, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\MagicBean\\obstacle04.dds", &m_plantTextureD, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\MagicBean\\obstacle05.dds", &m_plantTextureE, nullptr);

	m_loader->LoadTexture("DDS\\Levels\\Space\\obstacleSpace01.dds", &m_spaceObstacleA, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\Space\\obstacleSpace02.dds", &m_spaceObstacleB, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\Space\\obstacleSpace03.dds", &m_spaceObstacleC, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\Space\\obstacleSpace04.dds", &m_spaceObstacleD, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\Space\\obstacleSpace05.dds", &m_spaceObstacleE, nullptr);
	m_loader->LoadTexture("DDS\\Levels\\Space\\obstacleSpace06.dds", &m_spaceObstacleF, nullptr);
	m_loader->LoadTexture("Assets\\Backgrounds\\badhealth.png", &m_badHealthOverlay, nullptr);

	switch (m_currentLevelType)
	{
	case LevelType::SPACE:
		{
			m_loader->LoadTexture("Assets\\Backgrounds\\generic_dark_blue_a.png", &m_backgroundBlockA, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\generic_dark_blue_b.png", &m_backgroundBlockB, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\star_overlay_a.png", &m_overlayA, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\star_overlay_b.png", &m_overlayB, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\galaxy_overlay_a.png", &m_overlayGalaxyA, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\galaxy_overlay_b.png", &m_overlayGalaxyB, nullptr);

			CurrentSpriteBatch->AddTexture(m_overlayGalaxyA.Get());
			CurrentSpriteBatch->AddTexture(m_overlayGalaxyB.Get());
			break;
		}
	case LevelType::NIGHTMARE:
		{
			m_loader->LoadTexture("Assets\\Backgrounds\\generic_red_a.png", &m_backgroundBlockA, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\generic_red_b.png", &m_backgroundBlockB, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_a.png", &m_overlayA, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_b.png", &m_overlayB, nullptr);
			break;
		}
	case LevelType::MAGIC_BEANS:
		{
			m_loader->LoadTexture("Assets\\Backgrounds\\generic_blue_a.png", &m_backgroundBlockA, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\generic_blue_b.png", &m_backgroundBlockB, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_a.png", &m_overlayA, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_b.png", &m_overlayB, nullptr);

			break;
		}
	case LevelType::DREAM:
		{
			m_loader->LoadTexture("DDS\\Levels\\Dream\\TEST_backgroundDream_01.dds", &m_backgroundBlockA, nullptr);
			m_loader->LoadTexture("DDS\\Levels\\Dream\\TEST_backgroundDream_02.dds", &m_backgroundBlockB, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_a.png", &m_overlayA, nullptr);
			m_loader->LoadTexture("Assets\\Backgrounds\\cloud_overlay_b.png", &m_overlayB, nullptr);
			break;
		}
	}

	CurrentSpriteBatch->AddTexture(m_backgroundBlockA.Get());
	CurrentSpriteBatch->AddTexture(m_backgroundBlockB.Get());

	CurrentSpriteBatch->AddTexture(m_spikeObstacleLarge.Get());
	CurrentSpriteBatch->AddTexture(m_spikeObstacleMedium.Get());
	CurrentSpriteBatch->AddTexture(m_spikeObstacleSmall.Get());

	CurrentSpriteBatch->AddTexture(m_cloudObstacleTexture.Get());
	CurrentSpriteBatch->AddTexture(m_overlayA.Get());
	CurrentSpriteBatch->AddTexture(m_overlayB.Get());

	CurrentSpriteBatch->AddTexture(m_plantTextureA.Get());
	CurrentSpriteBatch->AddTexture(m_plantTextureB.Get());
	CurrentSpriteBatch->AddTexture(m_plantTextureC.Get());
	CurrentSpriteBatch->AddTexture(m_plantTextureD.Get());
	CurrentSpriteBatch->AddTexture(m_plantTextureE.Get());

	CurrentSpriteBatch->AddTexture(m_spaceObstacleA.Get());
	CurrentSpriteBatch->AddTexture(m_spaceObstacleB.Get());
	CurrentSpriteBatch->AddTexture(m_spaceObstacleC.Get());
	CurrentSpriteBatch->AddTexture(m_spaceObstacleD.Get());
	CurrentSpriteBatch->AddTexture(m_spaceObstacleE.Get());
	CurrentSpriteBatch->AddTexture(m_spaceObstacleF.Get());

	CurrentSpriteBatch->AddTexture(m_badHealthOverlay.Get());

	m_loader->LoadTexture("DDS\\Weapons\\fire01.dds", &StaticDataHelper::FlameA, nullptr);
	m_loader->LoadTexture("DDS\\Weapons\\fire02.dds", &StaticDataHelper::FlameB, nullptr);
	m_loader->LoadTexture("DDS\\Weapons\\fire03.dds", &StaticDataHelper::FlameC, nullptr);

	CurrentSpriteBatch->AddTexture(StaticDataHelper::FlameA.Get());
	CurrentSpriteBatch->AddTexture(StaticDataHelper::FlameB.Get());
	CurrentSpriteBatch->AddTexture(StaticDataHelper::FlameC.Get());

	m_loader->LoadTexture("DDS\\Weapons\\plasma_enemy.dds", &StaticDataHelper::MonsterWeapon, nullptr);
	m_loader->LoadTexture("DDS\\button.dds", &m_buttonTexture, nullptr);

	CurrentSpriteBatch->AddTexture(StaticDataHelper::MonsterWeapon.Get());
	CurrentSpriteBatch->AddTexture(m_buttonTexture.Get());

	Microsoft::WRL::ComPtr<ID3D11Texture2D> powerupTexture;

	m_loader->LoadTexture("DDS\\Powerups\\powerupHealth.dds", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::HEALTH] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("Assets\\Powerups\\helmet.png", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::HELMET] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("Assets\\Powerups\\parachute.png", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::PARACHUTE] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("Assets\\Powerups\\bubble.png", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::BUBBLE] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("Assets\\Powerups\\cape.png", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::CAPE] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("DDS\\Powerups\\powerupAxe.dds", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::AXE] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("DDS\\Powerups\\powerupBoomerang.dds", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::BOOMERANG] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("DDS\\Powerups\\powerupHammer.dds", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::HAMMER] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("DDS\\Powerups\\powerupKnife.dds", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::KNIFE] =  powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("DDS\\Powerups\\powerupPlasmaBall.dds", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::PLASMA_BALL] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());

	m_loader->LoadTexture("Assets\\Misc\\circle.png", &powerupTexture, nullptr);
	m_powerupTextures[PowerupType::CIRCLE] = powerupTexture;
	CurrentSpriteBatch->AddTexture(powerupTexture.Get());
}

void GamePlayScreen::CreateMonster()
{
	for (auto monster = m_monsters.begin(); monster != m_monsters.end(); monster++)
	{
		Monster^ currentMonster = (*monster);
		currentMonster->CurrentHealth = currentMonster->MaxHealth;
		currentMonster->OnPulledTrigger += ref new LivingEntity::ShootEventHandler(this, &GamePlayScreen::Shoot);
		currentMonster->Load();
	}
}

void GamePlayScreen::CreateBear()
{
	GameBear = ref new Bear(this);
	float scale = 0.2f;
	GameBear->Scale = scale;
	GameBear->RotationVelocity = 2.5f / (7.0f + 3.0f * GameBear->Scale);
	GameBear->IsFriendly = true;
	GameBear->CurrentDamage = 15.0f;
	GameBear->OnPulledTrigger += ref new LivingEntity::ShootEventHandler(this, &GamePlayScreen::Shoot);
	GameBear->Size = float2(400 * scale, 300 * scale);
	GameBear->Position = float2(LoBoundX + (HiBoundX - LoBoundX) / 2.0f, 0.0f);
	GameBear->Velocity.y = 17.0f;

	GameBear->Load();
}

void GamePlayScreen::UpdateButtons()
{	
	Windows::Foundation::Rect livingEntityBoundingBox = GameBear->GetBoundingBox();

	for (auto button = m_buttons.begin(); button != m_buttons.end();)
	{
		auto currentButton = (*button);

		currentButton->Position.x = (*button)->PixelDiff + LoBoundX;
		currentButton->Position.y -= GameBear->Velocity.y;


		Windows::Foundation::Rect obstacleRect = currentButton->GetBoundingBox();
		if (livingEntityBoundingBox.IntersectsWith(obstacleRect))
		{
			AudioManager::AudioEngineInstance.StopSoundEffect(Coin);
			AudioManager::AudioEngineInstance.PlaySoundEffect(Coin);

			m_particleSystem.ActivateSet("Sparkle", currentButton->Position,float2(RandFloat(-6.0f,6.0f),RandFloat(-10.0f, -5.0f)));

			StaticDataHelper::ButtonsCollected++;

			button = m_buttons.erase(button);
		}
		else
			++button;

	}
}


void GamePlayScreen::HandleKeyInput(Windows::System::VirtualKey key)
{
	if (key == Windows::System::VirtualKey::Right)
	{
		if (GameBear->IsWithinScreenBoundaries(GameBear->Size.x, 0.0f, GetScreenBounds()))
		{
			GameBear->Direction = TurningState::RIGHT;

			m_xAcceleration = 0.8f;

			if (GameBear->Rotation >= -m_rotationThreshold)
				GameBear->Rotation -= 0.02f;
		}
	}
	else if (key == Windows::System::VirtualKey::Left)
	{
		if (GameBear->IsWithinScreenBoundaries(-GameBear->Size.x, 0.0f, GetScreenBounds()))
		{
			GameBear->Direction = TurningState::LEFT;

			m_xAcceleration = -0.8f;

			if (GameBear->Rotation <= m_rotationThreshold)
				GameBear->Rotation += 0.02f;
		}
	}
}

void GamePlayScreen::MoveOverlay(float velocity)
{
	if (m_overlayPositionA <= -BACKGROUND_MIDPOINT)
		m_overlayPositionA = m_overlayPositionB + (BACKGROUND_MIDPOINT * 2);

	if (m_overlayPositionB <= -BACKGROUND_MIDPOINT)
		m_overlayPositionB = m_overlayPositionA + (BACKGROUND_MIDPOINT * 2);

	m_overlayPositionA -= velocity;
	m_overlayPositionB -= velocity;
}

void GamePlayScreen::MoveSpaceOverlay(float velocity)
{
	if (m_overlayGalaxyPositionA <= -BACKGROUND_MIDPOINT)
		m_overlayGalaxyPositionA = m_overlayGalaxyPositionB + (BACKGROUND_MIDPOINT * 2);

	if (m_overlayGalaxyPositionB <= -BACKGROUND_MIDPOINT)
		m_overlayGalaxyPositionB = m_overlayGalaxyPositionA + (BACKGROUND_MIDPOINT * 2);

	m_overlayGalaxyPositionA -= velocity;
	m_overlayGalaxyPositionB -= velocity;
}

void GamePlayScreen::StartBackground()
{
	m_isBackgroundMoving = true;
}

void GamePlayScreen::StopBackground()
{
	m_isBackgroundMoving = false;
}

bool GamePlayScreen::IsLevelLoaded::get()
{
	return m_isLevelLoaded;
}

void GamePlayScreen::IsLevelLoaded::set(bool value)
{
	m_isLevelLoaded = value;
}

Coding4Fun::FallFury::Characters::Bear^ GamePlayScreen::GameBear::get()
{
	return m_gameBear;
}

void GamePlayScreen::GameBear::set(Coding4Fun::FallFury::Characters::Bear^ value)
{
	m_gameBear = value;
}
 //}
 //}
 //}