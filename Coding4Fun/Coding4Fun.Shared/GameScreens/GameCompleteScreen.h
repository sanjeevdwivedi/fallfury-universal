#pragma once
#include "ScreenManager.h"
#include "ParticleSystem\ParticleCore.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Screens
		{
			ref class GameCompleteScreen : public GameScreenBase
			{
			internal:
				GameCompleteScreen(void);
				GameCompleteScreen(ScreenManager^ base);

				virtual void											RenderScreen() override;
				virtual void											Load() override;
				virtual void											Update(float, float) override;

			private:
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_bearPrimaryTexture;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_bearSecondaryTexture;
				float2													m_bearPosition;
				float													m_bearScale;
				ParticleCore											m_particleSystem;
				bool													m_arrived;
				bool													m_initialAnimationCompleted;
				float													m_timer;
				float													m_bearPositionTimer;
				float													m_bearRotation;
			};
		}
	}
}
