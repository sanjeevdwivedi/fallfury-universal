#include "pch.h"
#include "Bear.h"
#include "Helpers\Geometry.h"
#include <ppltasks.h>

using namespace concurrency;
using namespace Coding4Fun::FallFury::Characters;
using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Entities;

Coding4Fun::FallFury::Characters::Bear::Bear(GameScreenBase^ container)
	: LivingEntity(container)
{
	IsFriendly = true;
	m_timeElapsed = 0.0f;
	m_shading = float4(1.0f, 1.0f, 1.0f, 1.0f);
	m_shadingTimer = 0.0f;
	SetNormal();
	m_armRotationTimer = 0.0f;
	m_blinkTimer = 0.0f;
	m_weaponSize = float2(150.0f, 60.0f);
	m_capeRotation = 3.9f;
	m_weaponType = PowerupType::PLASMA_BALL;
}

void Coding4Fun::FallFury::Characters::Bear::RedShade()
{
	m_shading = float4(1.0f, 0.0f, 0.0f, 1.0f);
}

void Coding4Fun::FallFury::Characters::Bear::Update(float timeTotal, float timeDelta, float2 bearPosition)
{
	m_armRotationTimer += timeDelta;

	if (IsDead)
	{
		if (m_armRotationTimer < 1.0f)
		{
			m_rightPawRotation += 0.001f;
			m_leftPawRotation -= 0.001f;
		}
		else if (m_armRotationTimer > 1.0f && m_armRotationTimer < 2.0f)
		{
			m_rightPawRotation -= 0.001f;
			m_leftPawRotation += 0.001f;
		}
		else
		{
			m_rightPawRotation = 0.0f;
			m_leftPawRotation = 0.0f;
			m_armRotationTimer = 0.0f;
		}

		m_rightPawPosition = Position + float2(40.0f, -50.0f) * Scale;
		m_leftPawPosition = Position + float2(10.0f, 150.0f) * Scale;
		m_headPosition = (Position - float2(-160.0f, 0.0f) * Scale);
	}
	else
	{
		if (m_armRotationTimer < 0.6f)
		{
			m_rightPawRotation -= 0.005f;
			m_leftPawRotation += 0.005f;
		}
		else if (m_armRotationTimer > 0.6f && m_armRotationTimer < 1.2f)
		{
			m_rightPawRotation += 0.005f;
			m_leftPawRotation -= 0.005f;
		}
		else
		{
			m_rightPawRotation = 0.0f;
			m_leftPawRotation = 0.0f;
			m_armRotationTimer = 0.0f;
		}

		if (IsBubbleEnabled)
		{
			m_currentBubbleCounter += timeDelta;
			if (m_currentBubbleCounter > m_maxBubbleCounter)
			{
				IsBubbleEnabled = false;
				DamageDivider = 1.0f;
				m_currentBubbleCounter = 0.0f;
			}
		}

		m_headPosition = (Position - float2(0, 130) * Scale);

		if (IsCapeEnabled)
		{
			m_currentCapeCounter += timeDelta;
			if (m_currentCapeCounter <= m_maxCapeCounter)
			{
				m_capeRotationTimer += timeDelta;
				if (m_capeRotationTimer < 1.0f)
				{
					m_capeRotation += 0.009f;
				}
				else if (m_capeRotationTimer > 1.0f && m_capeRotationTimer < 2.0f)
				{
					m_capeRotation -= 0.009f;
				}
				else
				{
					m_capeRotation = 3.9f;
					m_capeRotationTimer = 0.0f;
				}
			}
			else
			{
				IsCapeEnabled = false;
				m_currentCapeCounter = 0.0f;
				Velocity.y = m_previousVelocity;
			}
		}

		// Look into adjusting this to allow custom shading
		if (m_shading.y == 0.0f && m_shadingTimer <= 0.2f)
		{
			m_shadingTimer += timeDelta;
		}
		else
		{
			m_shading = float4(1.0f, 1.0f, 1.0f, 1.0f);
			m_shadingTimer = 0.0f;
		}

		UpdateToState();

		if (!IsCapeEnabled)
		{
			m_rightPawPosition = (Position + float2(-70.0f, -30.0f) * Scale);
			m_leftPawPosition = (Position + float2(90.0f, -30.0f)* Scale);
		}
		else
		{
			m_rightPawPosition = Position + float2(18.0f,10.0f);
			m_leftPawPosition = Position + float2(-18.0f, 10.0f);
		}

		m_parachutePosition = Position - float2(0,120);

		if (IsParachuteEnabled)
		{
			if (m_currentParachuteCounter < m_maxParachuteCounter)
			{
				m_currentParachuteCounter += timeDelta;
			}
			else
			{
				IsParachuteEnabled = false;
				Velocity.y = m_previousVelocity;
				m_currentParachuteCounter = 0.0f;
			}
		}

		if (IsHelmetEnabled)
		{
			if (m_currentHelmetCounter < m_maxHelmetCounter)
			{
				m_currentHelmetCounter += timeDelta;
			}
			else
			{
				IsHelmetEnabled = false;
				DamageDivider = 1.0f;
				m_currentHelmetCounter = 0.0f;
			}
		}
	
		m_timeElapsed += timeDelta;
		m_blinkTimer += timeDelta;

		if (m_isBlinkState)
		{
			if (m_blinkTimer > 0.3f)
			{
				m_isBlinkState = false;
				m_blinkTimer = 0.0f;
			}
		}
		else
		{
			if (m_blinkTimer > 3.5f)
			{
				m_isBlinkState = true;
				m_blinkTimer = 0.0f;
			}
		}
	}
}

void Coding4Fun::FallFury::Characters::Bear::UpdateToState()
{
	switch (Direction)
	{
		case TurningState::LEFT:
		{
			TurnLeft();
			break;
		}
		case TurningState::RIGHT:
		{
			TurnRight();
			break;
		}
	}
}

void Coding4Fun::FallFury::Characters::Bear::PickupPowerup(Coding4Fun::FallFury::Entities::Powerup^ powerup, ParticleCore* ParticleSystem)
{
	switch (powerup->Type)
	{
	case PowerupType::HELMET:
		{
			SetHelmet(powerup->Lifespan);
			DamageDivider = powerup->Effect;

			ParticleSystem->ActivateSet("ScalableHelmet", Position, 0.0f, false, true);

			break;
		}
	case PowerupType::PARACHUTE:
		{
			m_previousVelocity = Velocity.y;
			SetParachute(powerup->Lifespan);
			Velocity.y -= powerup->Effect;

			ParticleSystem->ActivateSet("ScalableParachute", Position, 0.0f, false, true);

			break;
		}
	case PowerupType::HEALTH:
		{
			CurrentHealth = MaxHealth;

			ParticleSystem->ActivateSet("ScalableHeart", Position, 0.0f, false, true);

			break;
		}
	case PowerupType::CAPE:
		{
			if (IsParachuteEnabled)
			{
				IsParachuteEnabled = false;
				Velocity.y = m_previousVelocity * powerup->Effect;
			}
			else
			{
				m_previousVelocity = Velocity.y;
				Velocity.y *= powerup->Effect;
			}
			
			ParticleSystem->ActivateSet("CapeExplosionSet", Position, true);
			ParticleSystem->ActivateSet("ScalableCape", Position, 0.0f, false, true);

			m_maxCapeCounter = powerup->Lifespan;
			IsCapeEnabled = true;
			break;
		}
	case PowerupType::BUBBLE:
		{
			if (IsHelmetEnabled)
			{
				IsHelmetEnabled = false;
				DamageDivider = 1.0f;
			}
			else
			{
				DamageDivider = powerup->Effect;
			}
			
			IsBubbleEnabled = true;
			m_maxBubbleCounter = powerup->Lifespan;
			ParticleSystem->ActivateSet("ScalableBubble", Position, 0.0f, false, true);

			break;
		}
	case PowerupType::KNIFE:
		{
			m_weaponType = powerup->Type;

			m_weaponTexture = m_knifeTexture;
			CurrentDamage = powerup->Effect;

			ParticleSystem->ActivateSet("ScalableKnife", Position, 0.0f, false, true);

			m_weaponSize = float2(113.0, 184.0f) * 0.5f;
			break;
		}
	case PowerupType::AXE:
		{
			m_weaponType = powerup->Type;

			m_weaponTexture = m_axeTexture;
			CurrentDamage = powerup->Effect;

			m_weaponSize = float2(219.0f, 239.0f) * 0.5f;

			ParticleSystem->ActivateSet("ScalableAxe", Position, 0.0f, false, true);
			break;
		}
	case PowerupType::BOOMERANG:
		{
			m_weaponType = powerup->Type;

			m_weaponTexture = m_boomerangTexture;
			CurrentDamage = powerup->Effect;

			m_weaponSize = float2(225.0f, 205.0f) * 0.5f;

			ParticleSystem->ActivateSet("ScalableBoomerang", Position, 0.0f, false, true);
			break;
		}
	case PowerupType::HAMMER:
		{
			m_weaponType = powerup->Type;

			m_weaponTexture = m_hammerTexture;
			CurrentDamage = powerup->Effect;

			m_weaponSize = float2(194.0f, 207.0f) * 0.5f;

			ParticleSystem->ActivateSet("ScalableHammer", Position, 0.0f, false, true);
			break;
		}
	case PowerupType::PLASMA_BALL:
		{
			m_weaponType = powerup->Type;

			m_weaponTexture = m_powerballTexture;
			CurrentDamage = powerup->Effect;

			m_weaponSize = float2(150.0f, 60.0f);

			ParticleSystem->ActivateSet("ScalablePlasmaball", Position, 0.0f, false, true);
			break;
		}
	}
}

void Coding4Fun::FallFury::Characters::Bear::ShootAtTarget(float2 lastTargetTrace)
{
	OnPulledTrigger(Position.x, Position.y, GetVelocityLegs(lastTargetTrace).x, GetVelocityLegs(lastTargetTrace).y, CurrentDamage, IsFriendly, false, HostContainer->CurrentSpriteBatch);
}

void Coding4Fun::FallFury::Characters::Bear::TurnLeft()
{
	m_body = m_bodyLeft;

	if (m_shading.y != 0.0f)
	{
		if (!m_isBlinkState)
		{
			m_head = m_headLeft;
		}
		else
		{
			m_head = m_headBlinkLeft;
		}
	}
	else
	{
		m_head = m_headHurt;
	}

	if (IsHelmetEnabled)
		m_head = m_inAHelmet;


	if (IsParachuteEnabled)
		m_parachute = m_parachuteLeft;

	Direction = TurningState::LEFT;
}


void Coding4Fun::FallFury::Characters::Bear::TurnRight()
{
	m_body = m_bodyRight;

	if (m_shading.y != 0.0f)
	{
		if (!m_isBlinkState)
		{
			m_head = m_headRight;
		}
		else
		{
			m_head = m_headBlinkRight;
		}
	}
	else
	{
		m_head = m_headHurt;
	}

	if (IsHelmetEnabled)
		m_head = m_inAHelmet;

	if (IsParachuteEnabled)
		m_parachute = m_parachuteRight;

	Direction = TurningState::RIGHT;
}

void Coding4Fun::FallFury::Characters::Bear::Load()
{
	Loader = ref new BasicLoader(HostContainer->Manager->m_d3dDevice.Get(), HostContainer->Manager->m_wicFactory.Get());

	Loader->LoadTexture("DDS\\Bear\\bear_body_left.dds", &m_bodyLeft, nullptr);
	Loader->LoadTexture("DDS\\Bear\\bear_body_right.dds", &m_bodyRight, nullptr);

	Loader->LoadTexture("DDS\\Bear\\bear_head_left.dds", &m_headLeft, nullptr);
	Loader->LoadTexture("DDS\\Bear\\bear_head_right.dds", &m_headRight, nullptr);

	Loader->LoadTexture("DDS\\Bear\\bear_headblink_left.dds", &m_headBlinkLeft, nullptr);
	Loader->LoadTexture("DDS\\Bear\\bear_headblink_right.dds", &m_headBlinkRight, nullptr);

	Loader->LoadTexture("Assets\\Bear\\bear_leftarm.png", &m_leftPaw, nullptr);
	Loader->LoadTexture("Assets\\Bear\\bear_rightarm.png", &m_rightPaw, nullptr);

	Loader->LoadTexture("DDS\\Bear\\Customized\\bear_parachute_right.dds", &m_parachuteRight, nullptr);
	Loader->LoadTexture("DDS\\Bear\\Customized\\bear_parachute_left.dds", &m_parachuteLeft, nullptr);

	Loader->LoadTexture("DDS\\Bear\\Customized\\bear_cape.dds", &m_cape, nullptr);

	Loader->LoadTexture("DDS\\Bear\\Customized\\bubble.dds", &m_bubble, nullptr);
	
	Loader->LoadTexture("DDS\\Bear\\Customized\\bear_helmet.dds", &m_inAHelmet, nullptr);
	Loader->LoadTexture("DDS\\Bear\\bear_head_hurt.dds", &m_headHurt, nullptr);

	Loader->LoadTexture("DDS\\Weapons\\axe.dds", &m_axeTexture, nullptr);
	Loader->LoadTexture("DDS\\Weapons\\knife.dds", &m_knifeTexture, nullptr);
	Loader->LoadTexture("DDS\\Weapons\\boomerang.dds", &m_boomerangTexture, nullptr);
	Loader->LoadTexture("DDS\\Weapons\\hammer.dds", &m_hammerTexture, nullptr);
	Loader->LoadTexture("DDS\\Weapons\\plasmaball.dds", &m_powerballTexture, nullptr);

	Loader->LoadTexture("Assets\\Bear\\bear_dead_body.png", &m_deadBody, nullptr);
    HostContainer->CurrentSpriteBatch->AddTexture(m_deadBody.Get());

	Loader->LoadTexture("Assets\\Bear\\bear_dead_head.png", &m_deadHead, nullptr);
    HostContainer->CurrentSpriteBatch->AddTexture(m_deadHead.Get());

	Loader->LoadTexture("Assets\\Bear\\bear_dead_leftarm.png", &m_deadLeftArm, nullptr);
    HostContainer->CurrentSpriteBatch->AddTexture(m_deadLeftArm.Get());

	Loader->LoadTexture("Assets\\Bear\\bear_dead_rightarm.png", &m_deadRightArm, nullptr);
    HostContainer->CurrentSpriteBatch->AddTexture(m_deadRightArm.Get());
	


	//TODO: Manage this stack through an array instead of having individual textures.

	HostContainer->CurrentSpriteBatch->AddTexture(m_axeTexture.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_knifeTexture.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_boomerangTexture.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_hammerTexture.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_powerballTexture.Get());

	HostContainer->CurrentSpriteBatch->AddTexture(m_bodyLeft.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_bodyRight.Get());

	HostContainer->CurrentSpriteBatch->AddTexture(m_headLeft.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_headRight.Get());

	HostContainer->CurrentSpriteBatch->AddTexture(m_headBlinkLeft.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_headBlinkRight.Get());
				 
	HostContainer->CurrentSpriteBatch->AddTexture(m_leftPaw.Get());		   
	HostContainer->CurrentSpriteBatch->AddTexture(m_rightPaw.Get());

	HostContainer->CurrentSpriteBatch->AddTexture(m_parachuteLeft.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_parachuteRight.Get());

	HostContainer->CurrentSpriteBatch->AddTexture(m_inAHelmet.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_cape.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_bubble.Get());
	HostContainer->CurrentSpriteBatch->AddTexture(m_headHurt.Get());
	
	IsLoaded = true;
	m_weaponTexture = m_powerballTexture;
}

Windows::Foundation::Rect Coding4Fun::FallFury::Characters::Bear::GetBoundingBox()
{
	return Windows::Foundation::Rect(Position.x - (Size.x / 2),
		Position.y - (Size.y / 2), Size.x, Size.y);
}

void Coding4Fun::FallFury::Characters::Bear::SetBubble(float timeSpan)
{
	IsBubbleEnabled = true;
}

void Coding4Fun::FallFury::Characters::Bear::SetCape(float timeSpan)
{
	IsCapeEnabled = true;
}

void Coding4Fun::FallFury::Characters::Bear::SetHelmet(float timeSpan)
{
	IsHelmetEnabled = true;
	m_maxHelmetCounter = timeSpan;

	m_head = m_inAHelmet;
}

void Coding4Fun::FallFury::Characters::Bear::SetParachute(float timeSpan)
{
	IsParachuteEnabled = true;
	m_maxParachuteCounter = timeSpan;

	m_parachute = m_parachuteRight;
}

void Coding4Fun::FallFury::Characters::Bear::SetNormal()
{
	IsParachuteEnabled = false;
	IsCapeEnabled = false;
	IsBubbleEnabled = false;
	IsHelmetEnabled = false;
}

void Coding4Fun::FallFury::Characters::Bear::Kill()
{
	IsDead = true;
	Rotation = -1.0f;
	m_head = m_deadHead;
	m_body = m_deadBody;
	m_leftPaw = m_deadLeftArm;
	m_rightPaw = m_deadRightArm;
}

void Coding4Fun::FallFury::Characters::Bear::Render()
{
	if (!IsDead)
	{
		if (IsParachuteEnabled)
		{
			IsCapeEnabled = false;

			HostContainer->CurrentSpriteBatch->Draw(
				m_parachute.Get(),
				m_parachutePosition,
				PositionUnits::DIPs,
				float2(721.0f, 855.0f) * Scale,
				SizeUnits::Pixels,
				m_shading,
				0);
		}

		if (!IsCapeEnabled)
		{

			if (Direction == TurningState::LEFT)
			{
			
				HostContainer->CurrentSpriteBatch->Draw(
					m_leftPaw.Get(),
					m_leftPawPosition,
					PositionUnits::DIPs,
					float2(522.0f, 141.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					m_leftPawRotation + 0.5f);

				HostContainer->CurrentSpriteBatch->Draw(
					m_body.Get(),
					Position,
					PositionUnits::DIPs,
					float2(400.0f, 400.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					Rotation);

				HostContainer->CurrentSpriteBatch->Draw(
					m_rightPaw.Get(),
					m_rightPawPosition,
					PositionUnits::DIPs,
					float2(522.0f, 141.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					m_rightPawRotation - 0.5f);
			}
			else
			{
				HostContainer->CurrentSpriteBatch->Draw(
					m_rightPaw.Get(),
					m_rightPawPosition,
					PositionUnits::DIPs,
					float2(522.0f, 141.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					m_rightPawRotation - 0.5f);

				HostContainer->CurrentSpriteBatch->Draw(
					m_body.Get(),
					Position,
					PositionUnits::DIPs,
					float2(400.0f, 400.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					Rotation);

				HostContainer->CurrentSpriteBatch->Draw(
					m_leftPaw.Get(),
					m_leftPawPosition,
					PositionUnits::DIPs,
					float2(522.0f, 141.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					m_leftPawRotation + 0.5f);
			}

			HostContainer->CurrentSpriteBatch->Draw(
				m_head.Get(),
				m_headPosition,
				PositionUnits::DIPs,
				float2(458.0f, 313.0f) * Scale,
				SizeUnits::Pixels,
				m_shading,
				Rotation);
		}
		else
		{
			IsParachuteEnabled = false;

			HostContainer->CurrentSpriteBatch->Draw(
				m_cape.Get(),
				Position + float2(15.0f,0.0f),
				PositionUnits::DIPs,
				float2(498.0f, 375.0f) * Scale,
				SizeUnits::Pixels,
				m_shading,
				m_capeRotation);

			if (Direction == TurningState::LEFT)
			{
				HostContainer->CurrentSpriteBatch->Draw(
					m_leftPaw.Get(),
					m_leftPawPosition,
					PositionUnits::DIPs,
					float2(454.0f, 109.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					3.14f + m_leftPawRotation + 0.5f);

				HostContainer->CurrentSpriteBatch->Draw(
					m_body.Get(),
					Position,
					PositionUnits::DIPs,
					float2(400.0f, 400.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					3.14f);

				HostContainer->CurrentSpriteBatch->Draw(
					m_rightPaw.Get(),
					m_rightPawPosition,
					PositionUnits::DIPs,
					float2(454.0f, 109.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					3.14f + m_rightPawRotation - 0.5f);
			}
			else
			{
				HostContainer->CurrentSpriteBatch->Draw(
					m_rightPaw.Get(),
					m_rightPawPosition,
					PositionUnits::DIPs,
					float2(454.0f, 109.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					3.14f + m_rightPawRotation - 0.5f);

				HostContainer->CurrentSpriteBatch->Draw(
					m_body.Get(),
					Position,
					PositionUnits::DIPs,
					float2(400.0f, 400.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					3.14f);

				HostContainer->CurrentSpriteBatch->Draw(
					m_leftPaw.Get(),
					m_leftPawPosition,
					PositionUnits::DIPs,
					float2(454.0f, 109.0f) * Scale,
					SizeUnits::Pixels,
					m_shading,
					3.14f + m_leftPawRotation + 0.5f);
			}

			HostContainer->CurrentSpriteBatch->Draw(
				m_head.Get(),
				m_headPosition + float2(0.0f, 80.0f),
				PositionUnits::DIPs,
				float2(458.0f, 313.0f) * Scale,
				SizeUnits::Pixels,
				m_shading,
				3.14f);
		}

		if (IsBubbleEnabled)
		{
			HostContainer->CurrentSpriteBatch->Draw(
				m_bubble.Get(),
				Position,
				PositionUnits::DIPs,
				float2(988.0f, 988.0f) * Scale,
				SizeUnits::Pixels,
				m_shading,
				0);
		}
	}
	else
	{
		if (IsCapeEnabled)
		{
			HostContainer->CurrentSpriteBatch->Draw(
				m_cape.Get(),
				Position + float2(130.0f,-90.0f),
				PositionUnits::DIPs,
				float2(498.0f, 375.0f) * Scale,
				SizeUnits::Pixels,
				m_shading,
				m_capeRotation * -0.6f);
		}

		HostContainer->CurrentSpriteBatch->Draw(
			m_rightPaw.Get(),
			m_rightPawPosition,
            PositionUnits::DIPs,
            float2(1.0f, 1.0f) * Scale,
            SizeUnits::Normalized,
			float4(1.0f,1.0f,1.0f,1.0f),
			m_rightPawRotation);

		HostContainer->CurrentSpriteBatch->Draw(
            m_body.Get(),
			Position,
            PositionUnits::DIPs,
            float2(1.0f, 1.0f) * Scale,
            SizeUnits::Normalized,
			float4(1.0f,1.0f,1.0f,1.0f),
			Rotation);

		HostContainer->CurrentSpriteBatch->Draw(
			m_head.Get(),
			m_headPosition,
            PositionUnits::DIPs,
            float2(1.0f, 1.0f) * Scale,
            SizeUnits::Normalized,
			float4(1.0f,1.0f,1.0f,1.0f),
			Rotation + 0.1f);

		HostContainer->CurrentSpriteBatch->Draw(
			m_leftPaw.Get(),
			m_leftPawPosition,
            PositionUnits::DIPs,
            float2(1.0f, 1.0f) * Scale,
            SizeUnits::Normalized,
			float4(1.0f,1.0f,1.0f,1.0f),
			-m_leftPawRotation);
	}
}

void Coding4Fun::FallFury::Characters::Bear::RenderShell(float2 position, float rotation)
{
	HostContainer->CurrentSpriteBatch->Draw(
			m_weaponTexture.Get(),
			position,
			PositionUnits::DIPs,
			m_weaponSize,
			SizeUnits::Pixels,
			float4(1.0f, 1.0f, 1.0f, 1.0f),
			rotation);
}

Coding4Fun::FallFury::Entities::PowerupType Coding4Fun::FallFury::Characters::Bear::WeaponType::get()
{
	return m_weaponType;
}