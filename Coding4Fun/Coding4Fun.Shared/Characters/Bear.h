#pragma once
#include "pch.h"
#include "Characters\LivingEntity.h"
#include "Entities\Powerup.h"
#include "ParticleSystem\ParticleCore.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Characters
		{
			using namespace Coding4Fun::FallFury::Entities;

			enum class TurningState
			{
				LEFT,
				RIGHT
			};

			private ref class Bear : public LivingEntity
			{
			internal:
				Bear(Coding4Fun::FallFury::Screens::GameScreenBase^ container);

				virtual void								Update(float, float, float2) override;
				virtual void								ShootAtTarget(float2) override;
				virtual void								Load() override;
				virtual void								Render() override;
				virtual Windows::Foundation::Rect			GetBoundingBox() override;

				void										RedShade();
				void										TurnRight();
				void										TurnLeft();
				void										UpdateToState();

				void										SetNormal();
				void										SetParachute(float);
				void										SetCape(float);
				void										SetBubble(float);
				void										SetHelmet(float);
				void										PickupPowerup(Coding4Fun::FallFury::Entities::Powerup^ powerup, ParticleCore* ParticleSystem);
				void										Kill();

				void										RenderShell(float2 position, float rotation);

				property bool								IsParachuteEnabled;
				property bool								IsCapeEnabled;
				property bool								IsBubbleEnabled;
				property bool								IsHelmetEnabled;
				property TurningState						Direction;
				property PowerupType						WeaponType { PowerupType get(); }

			private:
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_head;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_leftPaw;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_rightPaw;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_headRight;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_headLeft;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_headBlinkRight;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_headBlinkLeft;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_bodyRight;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_bodyLeft;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_deadLeftArm;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_deadRightArm;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_deadBody;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_deadHead;
															
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_headHurt;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_bubble;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_inAHelmet;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_cape;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_parachute;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_parachuteLeft;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_parachuteRight;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_knifeTexture;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_axeTexture;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_boomerangTexture;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_hammerTexture;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_powerballTexture;
			
				float2										m_headPosition;
				float2										m_leftPawPosition;
				float2										m_rightPawPosition;
				float2										m_parachutePosition;
				float										m_previousVelocity;

				float4										m_shading;
				float										m_shadingTimer;
				float										m_capeRotation;
				float										m_capeRotationTimer;
				float										m_timeElapsed;
				float										m_rightPawRotation;
				float										m_leftPawRotation;
				float										m_armRotationTimer;

				float										m_maxParachuteCounter;
				float										m_currentParachuteCounter;
				float										m_maxCapeCounter;
				float										m_currentCapeCounter;
				float										m_maxHelmetCounter;
				float										m_currentHelmetCounter;
				float										m_maxBubbleCounter;
				float										m_currentBubbleCounter;
	
				float										m_blinkTimer;
				bool										m_isBlinkState;

				float2										m_weaponSize;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_weaponTexture;
				PowerupType									m_weaponType;
			};
		}
	}
}
