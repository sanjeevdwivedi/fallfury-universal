#pragma once
#include "Audio\AudioManager.h"
//#include "ParticleSystem\ParticleCore.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Screens
		{
			ref class GameOverScreen : public GameScreenBase
			{
			internal:
				GameOverScreen(ScreenManager^ parent);
				virtual void											RenderScreen() override;
				virtual void											Load() override;
				virtual void											Update(float, float) override;

			private:
				float													m_bearRotation;
				float													m_rotationTimer;

				float													m_armRotation;
				float													m_armRotationTimer;

				float													m_bearPositionTimer;
				float													m_positionXAdj;
				float													m_positionYAdj;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_bearBody;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_bearHead;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_bearLeftPaw;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_bearRightPaw;

				Bear^													m_showBear;
			};
		}
	}
}
