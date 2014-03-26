#pragma once

#include "pch.h"
#include "Helpers\BasicMath.h"
#include <string>
#include "Characters\LivingEntityState.h"


namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Characters
		{
			private ref class LivingEntity abstract
			{
			internal:
				LivingEntity(Coding4Fun::FallFury::Screens::GameScreenBase^ container);			

			internal:
				bool											IsLoaded;

				float2											Velocity;
				float											Rotation;
				float											RotationVelocity;
				float											Scale;
				float2											Size;
				float2											Position;

				bool											IsFriendly;
				float											MaxHealth;
				float											CurrentHealth;
	
				float											CurrentDamage;
				float											MaxCriticalDamage;
				float											ChanceofCriticalDamage;

				float											Lifetime;
				

				LivingEntityState								CurrentState;
				bool											IsDead;
				float											DamageDivider;

			protected private:	
				Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_body;
				Coding4Fun::FallFury::Screens::GameScreenBase^ 	HostContainer;
				BasicLoader^									Loader;
				
			internal:				
				delegate void ShootEventHandler(float positionX, float positionY, float velocityX, float velocityY, float, bool, bool, SpriteBatch^);
				event ShootEventHandler^ OnPulledTrigger;

				float2										GetVelocityLegs(float2 position);
				
				virtual void								Update(float, float, float2);
				virtual bool								IsWithinScreenBoundaries(float, float, Windows::Foundation::Rect);
				virtual void								ShootAtTarget(float2);
				virtual Windows::Foundation::Rect			GetBoundingBox();

				virtual void								Load();
				virtual void								Render();
				virtual void								InflictDamage(float damage);
			};
		}
	}
}
