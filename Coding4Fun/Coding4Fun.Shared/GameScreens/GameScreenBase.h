#pragma once
#include "Xaudio2.h"
#include "Xaudio2fx.h"
#include <collection.h>
#include "DXCore\SpriteWorks.h"
#include "DXCore\\BasicLoader.h"
using namespace Windows::Foundation;
using namespace Coding4Fun::FallFury::DXCore;
using namespace Coding4Fun::FallFury::DXCore::BasicSprites;
using namespace Coding4Fun::FallFury::Entities;

ref class ScreenManager;

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Screens
		{
			ref class GameScreenBase abstract
			{
			internal:
				virtual void RenderScreen();
				virtual void Load();

				virtual void Update(float, float);
				virtual void UpdateWindowSize();

				virtual void OnPointerMoved(Windows::UI::Core::PointerEventArgs^ pArgs);
				virtual void OnPointerReleased(Windows::UI::Core::PointerEventArgs^ pArgs);
				virtual void OnPointerPressed(Windows::UI::Core::PointerEventArgs^ pArgs);

				void MoveBackground(float velocity);

				virtual void HandleKeyInput(Windows::System::VirtualKey);
				virtual Windows::Foundation::Rect GetScreenBounds();
				virtual void InitializeSpriteBatch();

				GameScreenBase(void);
				GameScreenBase(ScreenManager^);

				static property ScreenManager^							Manager;
				property SpriteBatch^									CurrentSpriteBatch;
				property GameState										ScreenType;

				property float											LoBoundX      { float get(); private: void set(float); };
				property float											HiBoundX      { float get(); private: void set(float); };
				property float											LoBoundY      { float get(); private: void set(float); };
				property float											HiBoundY      { float get(); private: void set(float); };
				property bool											IsLoaded      { bool  get(); private: void set(bool);  };
				float2													m_screenSize;

			protected private:
				// Core background textures
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_backgroundBlockA;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_backgroundBlockB;
				
				// Overlayed clouds
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_overlayA;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_overlayB;

				float													m_backgroundPositionA;
				float													m_backgroundPositionB;

				float													BACKGROUND_MIDPOINT;
				BasicLoader^				m_loader;
			private:
				float													m_loBoundX;
				float													m_hiBoundX;
				float													m_loBoundY;
				float													m_hiBoundY;
				bool													m_isLoaded;
			};
		}

	}
}