#pragma once
#include "pch.h"
#include "Entities\MonsterType.h"
#include "Characters\LivingEntity.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Characters
		{
			using namespace Coding4Fun::FallFury::Entities;
			using namespace Coding4Fun::FallFury::Characters;

			private ref class Monster : public LivingEntity
			{
			internal:
				Monster(Coding4Fun::FallFury::Screens::GameScreenBase^ container, MonsterType type, bool isOnStartScreen);
				Monster(Coding4Fun::FallFury::Screens::GameScreenBase^ container, MonsterType type, float maxHealth, float currentDamage, bool isFriendly, float positionX, float positionY,
					float velocityX, float velocityY, float rotation, float lifetime, int bonus, float scale, float pixelDiff, bool isBoss);

				virtual void		Update(float, float, float2) override;
				virtual void		Update(float, float, float2, Windows::Foundation::Rect);
				virtual void		ShootAtTarget(float2) override;
				virtual void		Load() override;
				virtual void		Render() override;

				bool				CheckIfAlive();
				void				RedShade();

				bool				IsActive;

				property bool		IsOnStartScreen { bool get(); }
				property bool		IsBoss { bool get(); };
				property float		PixelDiff { float get(); };
				
			private:
				float				m_lifetimeCounter;
				float				m_stateChangeTimer;
				float				m_deathArcTimer;
				float				m_shadingTimer;
				float				m_jumpingTimer;
				float				m_shootingTimer;

				bool				m_goingRight;
				bool				m_introPlayed;
				int					m_bonus;
				float				m_pixelDiff;

				float4				m_shading;

				MonsterType			m_type;
				bool				m_isBoss;
				bool				m_isOnStartScreen;
				bool				m_deathSoundPlayed;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_spriteA;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_spriteB;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_spriteC;
			};
		}
	}
}

