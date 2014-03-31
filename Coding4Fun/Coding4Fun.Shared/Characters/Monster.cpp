#include "pch.h"
#include "Monster.h"
#include "Helpers\Geometry.h"
#include "Audio\AudioManager.h"

using namespace Coding4Fun::FallFury::Characters;
using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Helpers;
using namespace Coding4Fun::FallFury::Audio;
using namespace Coding4Fun::FallFury::Entities;

Coding4Fun::FallFury::Characters::Monster::Monster(GameScreenBase^ container, Coding4Fun::FallFury::Entities::MonsterType type, bool isOnStartScreen)
	: LivingEntity(container)
{
	m_type = type;
	
	IsActive = false;
	m_introPlayed = false;
	m_goingRight = false;
	m_shading = float4(1.0f, 1.0f, 1.0f, 1.0f);	
	m_isBoss = false;
	Velocity.y = 0.0f;
	m_lifetimeCounter = 0.0f;
	m_shootingTimer = 0.0f;
	m_shadingTimer = 0.0f;
	m_deathArcTimer = 0.0f;
	m_jumpingTimer = 0.0f;
	m_stateChangeTimer = 0.0f;
	m_isOnStartScreen = isOnStartScreen;
}

Coding4Fun::FallFury::Characters::Monster::Monster(GameScreenBase^ container, Coding4Fun::FallFury::Entities::MonsterType type, float maxHealth, float currentDamage, bool isFriendly, float positionX, float positionY,
				 float velocityX, float velocityY, float rotation, float lifetime, int bonus, float scale, float pixelDiff, bool isBoss)
	: LivingEntity(container)
{
	m_type = type;

	MaxHealth = maxHealth;
	CurrentDamage = currentDamage;
	IsFriendly = isFriendly;
	Position.x = positionX;
	Position.y = positionY;
	Velocity.x = velocityX;
	Velocity.y = velocityY;
	Rotation = rotation;
	Lifetime = lifetime;
	m_bonus = bonus;
	Scale = scale;
	m_pixelDiff = pixelDiff;
	m_isBoss = isBoss;
	m_isOnStartScreen  = false;
}

void Coding4Fun::FallFury::Characters::Monster::RedShade()
{
	m_shading = float4(1.0f, 0.0f, 0.0f, 1.0f);
}

bool Coding4Fun::FallFury::Characters::Monster::CheckIfAlive()
{
	if (CurrentHealth > 0)
	{
		return true;
	}
	else
	{
		if (!IsDead)
		{
			IsDead = true;
			IsActive = false;
			StaticDataHelper::MonstersKilled++;
			RedShade();
		}
		return false;
	}
}

void Coding4Fun::FallFury::Characters::Monster::Update(float timeTotal, float timeDelta, float2 bearPosition)
{
	Update(timeTotal,timeDelta,bearPosition, Windows::Foundation::Rect(0.0f, 0.0f, 0.0f, 0.0f));
}

void Coding4Fun::FallFury::Characters::Monster::Update(float timeTotal, float timeDelta, float2 bearPosition, Windows::Foundation::Rect screenBounds)
{
	if (m_lifetimeCounter < Lifetime && !IsActive && !IsDead)
	{
		Position.y -= Velocity.y;
	}	
	
	if (m_lifetimeCounter > Lifetime && !IsDead)
	{
		Position.y -= Velocity.y * 6.0f;

		IsActive = false;


		if (IsBoss && Position.y < 0)
		{
			if (StaticDataHelper::CurrentLevelID == HostContainer->Manager->Levels.Size -1)
			{
				HostContainer->Manager->CurrentGameState = GameState::GS_FULL_WIN;
			}
			else
			{
				HostContainer->Manager->CurrentGameState = GameState::GS_WIN;
			}
		}
	}

	// Death animation (arc)
	if (IsDead && !IsActive)
	{
		if (!m_deathSoundPlayed)
		{
			if (IsBoss)
			{
				AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::EnemyDeadB);
			}
			else
			{
				AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::EnemyDeadA);
			}
			m_deathSoundPlayed = true;
		}

		Position.x += Velocity.y * 1.3f;
		if (m_deathArcTimer > 0.4f)
		{
			Position.y += Velocity.y;

			if (Scale > 0.1f)
				Scale -= 0.01f;

			if (m_isBoss && Position.y > HostContainer->m_screenSize.y)
			{
				if (StaticDataHelper::CurrentLevelID == HostContainer->Manager->Levels.Size - 1)
				{
					HostContainer->Manager->CurrentGameState = GameState::GS_FULL_WIN;
				}
				else
				{
					HostContainer->Manager->CurrentGameState = GameState::GS_WIN;
				}
			}
		}
		else
		{
			Position.y -= Velocity.y;
			Scale += 0.01f;
			m_deathArcTimer += timeDelta;
		}
	}

	if (Position.y <= HostContainer->HiBoundY - 50 && !IsDead)
	{
		if (!m_introPlayed)
		{
			if (!IsOnStartScreen)
			{
				if (IsBoss)
				{
					AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::LaughB);
				}
				else
				{
					AudioManager::AudioEngineInstance.PlaySoundEffect(SoundEvent::LaughA);
				}
			}
			m_introPlayed = true;
		}

		IsActive = true;
		m_lifetimeCounter += Velocity.y;
	}

	float adjustment = 0.0f;
	adjustment = m_goingRight ? Position.x + Velocity.x : Position.x - Velocity.x;
	
	if (adjustment >= (HostContainer->LoBoundX + (Size.x * Scale) / 2.0f) 
		&& adjustment <= (HostContainer->HiBoundX - (Size.x * Scale) / 2.0f))
	{
		Position.x = adjustment;	
	}
	else
	{
		m_goingRight = !m_goingRight;
	}

	m_shootingTimer += timeDelta;
	if (m_shootingTimer > 2.0f && m_lifetimeCounter < Lifetime && IsActive)
	{
		ShootAtTarget(bearPosition);
		if (m_isBoss)
		{
			ShootAtTarget(float2(bearPosition.x - 90.0f, bearPosition.y));
			ShootAtTarget(float2(bearPosition.x + 90.0f, bearPosition.y));
		}
		m_shootingTimer = 0.0f;
	}

	m_stateChangeTimer += timeDelta;
	if (m_stateChangeTimer < 0.3f)
	{
		m_body = m_spriteA;
	}
	else if (m_stateChangeTimer > 0.3f && m_stateChangeTimer < 0.6f)
	{
		m_body = m_spriteB;
	}
	else if (m_stateChangeTimer > 0.6f && m_stateChangeTimer < 0.9f)
	{
		m_body = m_spriteC;
	}
	else if (m_stateChangeTimer > 0.9f)
	{
		m_stateChangeTimer = 0.0f;
	}

	if (IsActive)
	{
		m_jumpingTimer += timeDelta;
		if (m_jumpingTimer > 0.0f && m_jumpingTimer < 1.0f)
		{
			Position.y -= 1.0f;
		}
		else if (m_jumpingTimer >= 1.0f && m_jumpingTimer < 2.0f)
		{
			Position.y += 1.0f;
		}
		else
		{
			m_jumpingTimer = 0.0f;
		}

		if (m_shading.y == 0.0f && m_shadingTimer <= 0.2f)
		{
			m_shadingTimer += timeDelta;
		}
		else
		{
			m_shading = float4(1.0f,1.0f,1.0f,1.0f);
			m_shadingTimer = 0.0f;
		}
	}
}

void Coding4Fun::FallFury::Characters::Monster::ShootAtTarget(float2 lastTargetTrace)
{
	OnPulledTrigger(Position.x, Position.y, GetVelocityLegs(lastTargetTrace).x, GetVelocityLegs(lastTargetTrace).y, CurrentDamage, IsFriendly, m_isBoss, HostContainer->CurrentSpriteBatch);
}

void Coding4Fun::FallFury::Characters::Monster::Load()
{
	Loader = ref new BasicLoader(HostContainer->Manager->m_d3dDevice.Get(), HostContainer->Manager->m_wicFactory.Get());

	switch (this->m_type)
	{
	case MonsterType::MT_NIGHTMARE_A:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss01_1.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss01_2.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss01_3.dds", &m_spriteC, nullptr);
			Size = float2(504.0f, 493.0f);
			break;
		}
	case MonsterType::MT_NIGHTMARE_B:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss02_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss02_02.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss02_03.dds", &m_spriteC, nullptr);
			Size = float2(422.0f, 562.0f);
			break;
		}
	case MonsterType::MT_NIGHTMARE_C:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss03_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss03_02.dds", &m_spriteB, nullptr);
			m_spriteC = m_spriteA;
			Size = float2(343.0f, 604.0f);
			break;
		}
	case MonsterType::MT_MAGICBEAN_A:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss04_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss04_02.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss04_03.dds", &m_spriteC, nullptr);
			Size = float2(801.0f, 551.0f);
			break;
		}
	case MonsterType::MT_MAGICBEAN_B:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss05_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss05_02.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss05_03.dds", &m_spriteC, nullptr);
			Size = float2(401.0f, 746.0f);
			break;
		}
	case MonsterType::MT_MAGICBEAN_C:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss06_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss06_02.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss06_03.dds", &m_spriteC, nullptr);
			Size = float2(564.0f, 634.0f);
			break;
		}
	case MonsterType::MT_CANDYLAND_A:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss07_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss07_02.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss07_03.dds", &m_spriteC, nullptr);
			Size = float2(516.0f, 757.0f);
			break;
		}
	case MonsterType::MT_CANDYLAND_B:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss08_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss08_02.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss08_03.dds", &m_spriteC, nullptr);
			Size = float2(514.0f, 1108.0f);
			break;
		}
	case MonsterType::MT_CANDYLAND_C:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss09_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss09_02.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss09_03.dds", &m_spriteC, nullptr);
			Size = float2(536.0f, 617.0f);
			break;
		}
	case MonsterType::MT_CANDYLAND_D:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss10_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss10_02.dds", &m_spriteB, nullptr);
			m_spriteC = m_spriteA;
			Size = float2(697.0f, 521.0f);
			break;
		}
	case MonsterType::MT_CANDYLAND_E:
		{
			Loader->LoadTexture("DDS\\Bosses\\boss11_01.dds", &m_spriteA, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss11_02.dds", &m_spriteB, nullptr);
			Loader->LoadTexture("DDS\\Bosses\\boss11_03.dds", &m_spriteC, nullptr);
			Size = float2(752.0f, 896.0f);
			break;
		}
	}
	
	HostContainer->CurrentSpriteBatch->AddTexture(m_spriteA.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_spriteB.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_spriteC.Get());

	m_body = m_spriteA;
	IsLoaded = true;
}

void Coding4Fun::FallFury::Characters::Monster::Render()
{
	HostContainer->CurrentSpriteBatch->Draw(
		m_body.Get(),
		Position,
		PositionUnits::Pixels,
		Size * Scale,
		SizeUnits::Pixels,
		m_shading,
		0);
}


bool Coding4Fun::FallFury::Characters::Monster::IsBoss::get()
{
	return m_isBoss;
}

bool Coding4Fun::FallFury::Characters::Monster::IsOnStartScreen::get()
{
	return m_isOnStartScreen;
}

float Coding4Fun::FallFury::Characters::Monster::PixelDiff::get()
{
	return m_pixelDiff;
}