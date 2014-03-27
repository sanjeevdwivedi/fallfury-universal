#pragma once

#include "GameScreenBase.h"
#include "Characters\Monster.h"
#include "Entities\AmmoItem.h"
#include "Characters\Bear.h"
#include "ParticleSystem\ParticleCore.h"
#include "Entities\LevelType.h"
#include "Entities\Button.h"
#include "Audio\AudioManager.h"
#include "Entities\Obstacle.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Screens
		{
			using namespace Windows::Devices::Sensors;
			using namespace Coding4Fun::FallFury::Characters;
			using namespace Coding4Fun::FallFury::Entities;

			ref class GamePlayScreen : public GameScreenBase
			{
			internal:
				virtual void											RenderScreen() override;
				virtual void											Load() override;
				virtual void											Update(float, float) override;
				virtual void											HandleKeyInput(Windows::System::VirtualKey) override;

				virtual void											OnPointerMoved(Windows::UI::Core::PointerEventArgs^ args) override;
				virtual void											OnPointerReleased(Windows::UI::Core::PointerEventArgs^ args) override;
				virtual void											OnPointerPressed(Windows::UI::Core::PointerEventArgs^ args) override;

				GamePlayScreen(ScreenManager^ parent);

				property bool											IsLevelLoaded { bool get(); private: void set(bool); };
				property Bear^											GameBear { Bear^ get(); private: void set(Bear^); };;

			private:
				bool													m_isLevelLoaded;
				Bear^													m_gameBear;
				LevelType												m_currentLevelType;

				std::list<Obstacle>										m_obstacles;
				std::list<Monster^>										m_monsters;
				std::list<Button^>										m_buttons;
				std::list<Powerup^>										m_powerups;

				float													m_rotationThreshold;
				float													m_accelerationMultiplier;


				float													m_lengthCounter;
				int														m_buttonPrice;
				int														m_touchCounter;

				// Powerup texture set
				std::map<PowerupType, Microsoft::WRL::ComPtr<ID3D11Texture2D>> m_powerupTextures;

				// Space level overlay. Because the space
				// level has stars and galaxies, I need
				// an extra overlay.
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_overlayGalaxyA;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_overlayGalaxyB;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_plantTextureA;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_plantTextureB;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_plantTextureC;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_plantTextureD;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_plantTextureE;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spaceObstacleA;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spaceObstacleB;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spaceObstacleC;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spaceObstacleD;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spaceObstacleE;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spaceObstacleF;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spikeObstacleLarge;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spikeObstacleMedium;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_spikeObstacleSmall;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_cloudObstacleTexture;
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_buttonTexture;

				// Handle the pulsating red screen when the bear reaches a critical
				// health level.
				Microsoft::WRL::ComPtr<ID3D11Texture2D>					m_badHealthOverlay;
				float4													m_badHealthShading;
				float													m_badHealthTimeThreshold;
				float													m_badHealthTimer;
				float													m_badHealthShadingCoefficient;

				ParticleCore											m_particleSystem;
				Windows::Devices::Sensors::Accelerometer^				m_systemAccelerometer;
				std::list<AmmoItem^>									m_ammoCollection;
				Platform::String^										m_levelFile;

				bool													m_isBackgroundMoving;
				float													m_overlayPositionA;
				float													m_overlayPositionB;
				float													m_overlayGalaxyPositionA;
				float													m_overlayGalaxyPositionB;

				double													m_xAcceleration;
			private:
				void													Shoot(float posX, float posY, float veloX, float veloY, float healthDamage, bool isFriendly, bool isBoss, SpriteBatch^  batch);

				void													AccelerometerReadingChanged(_In_ Accelerometer^, _In_ AccelerometerReadingChangedEventArgs^);

				void													UpdateBadHealthOverlay(float timeSpan);
				void													CheckForOutOfBoundsAmmo();
				void													CheckForExpiredAssets();
				void													UpdateAmmo(float timeDelta);

				void													CreateBear();
				void													CreateMonster();
				void													CreateObstacles();
				void													CreatePowerups();

				void													LoadTextures();
				void													LoadLevelXML();
				void													LoadSounds();

				void													CheckForCollisionWithAmmo(LivingEntity^ entity);
				void													UpdateObstacles();
				void													UpdateButtons();
				void													UpdatePowerups(float timeDelta);

				void													CheckForCollisionWithPowerups();
				void													CheckBearHealth();

				void                                                    OnLevelLoadCompleted(IAsyncOperation<Platform::Object^> ^op, AsyncStatus s);

				void													MoveOverlay(float velocity);
				void													MoveSpaceOverlay(float velocity);
				void													MoveMonsters(float timeTotal ,float timeDelta);

				void													DrawObstacle(Obstacle obstacle, Microsoft::WRL::ComPtr<ID3D11Texture2D> texture);

				void													StartBackground();
				void													StopBackground();
			};
		}
	}
}