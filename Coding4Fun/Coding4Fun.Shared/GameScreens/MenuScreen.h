#pragma once

#include "pch.h"
#include "DXCore\SpriteWorks.h"
#include "GameScreens\GameScreenBase.h"
#include "ScreenManager.h"
#include "Characters\Bear.h"
#include "Characters\Monster.h"
#include "Audio\AudioManager.h"


namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Screens
		{
			using namespace Windows::Foundation;
			using namespace Coding4Fun::FallFury::Characters;

			ref class MenuScreen : public GameScreenBase
			{
			internal:
				MenuScreen(ScreenManager^ parent);

				virtual void			RenderScreen() override;
				virtual void			Load() override;
				virtual void			Update(float timeTotal, float timeDelta) override;

			private:
				void					CreateNewMonster();

				Bear^					m_showBear;
				Monster^				m_showMonster;

				float					m_positionTimer;
				float					m_positionXAdj;
				float					m_positionYAdj;

			};
		}
	}
}
