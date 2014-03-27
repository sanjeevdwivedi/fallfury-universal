#pragma once

#include "DirectXPage.g.h"
#include "ScreenManager.h"
#include "Helpers\Timer.h"
#include "MenuItems\MenuItem.xaml.h"
#include "Controls\HealthBar.xaml.h"
#include "Audio\AudioManager.h"
#include "GameScreens\GamePlayScreen.h"
#include "Helpers\StaticDataHelper.h"
#include "Entities\TopScore.h"
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::UI::Xaml::Controls;

namespace Coding4Fun
{
	namespace FallFury
	{
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class DirectXPage sealed
		{
		public:
			DirectXPage();

			property Windows::Foundation::Collections::IVector<TopScore^>^ TopScores
			{
				Windows::Foundation::Collections::IVector<TopScore^>^ get() { return _topScores; }
				void set(Windows::Foundation::Collections::IVector<TopScore^>^ value) { _topScores = (Platform::Collections::Vector<TopScore^>^)value; }
			}

			static property DependencyProperty^ SFXVolumeProperty
			{
				DependencyProperty^ get() { return _sfxVolume; }
			}
			property int SFXVolume
			{
				int get() { return (int)GetValue(SFXVolumeProperty); }
				void set(int value)
				{
					SetValue(SFXVolumeProperty, value);
				}
			}

			static property DependencyProperty^ MusicVolumeProperty
			{
				DependencyProperty^ get() { return _musicVolume; }
			}
			property int MusicVolume
			{
				int get() { return (int)GetValue(MusicVolumeProperty); }
				void set(int value)
				{
					SetValue(MusicVolumeProperty, value);
				}
			}

			static property DependencyProperty^ IsAccelerometerEnabledProperty
			{
				DependencyProperty^ get() { return _isAccelerometerEnabled; }
			}
			property bool IsAccelerometerEnabled
			{
				bool get() { return (bool)GetValue(IsAccelerometerEnabledProperty); }
				void set(bool value)
				{
					SetValue(IsAccelerometerEnabledProperty, value);
					Coding4Fun::FallFury::Helpers::StaticDataHelper::IsAccelerometerEnabled = value;
				}
			}

			static property DependencyProperty^ IsMouseEnabledProperty
			{
				DependencyProperty^ get() { return _isMouseEnabled; }
			}
			property bool IsMouseEnabled
			{
				bool get() { return (bool)GetValue(IsMouseEnabledProperty); }
				void set(bool value)
				{
					SetValue(IsMouseEnabledProperty, value);
					Coding4Fun::FallFury::Helpers::StaticDataHelper::IsMouseEnabled = value;
				}
			}


			static property DependencyProperty^ IsAccelerometerInvertedProperty
			{
				DependencyProperty^ get() { return _isAccelerometerInverted; }
			}
			property bool IsAccelerometerInverted
			{
				bool get() { return (bool)GetValue(IsAccelerometerInvertedProperty); }
				void set(bool value)
				{
					SetValue(IsAccelerometerInvertedProperty, value);
					Coding4Fun::FallFury::Helpers::StaticDataHelper::IsAccelerometerInverted = value;
				}
			}

			static property DependencyProperty^ MusicEnabledProperty
			{
				DependencyProperty^ get() { return _musicEnabled; }
			}
			property bool MusicEnabled
			{
				bool get() { return (bool)GetValue(MusicEnabledProperty); }
				void set(bool value) { SetValue(MusicEnabledProperty, value); }
			}

			static property DependencyProperty^ SFXEnabledProperty
			{
				DependencyProperty^ get() { return _sfxEnabled; }
			}
			property bool SFXEnabled
			{
				bool get() { return (bool)GetValue(SFXEnabledProperty); }
				void set(bool value) { SetValue(SFXEnabledProperty, value); }
			}

			static property DependencyProperty^ IsRotationLockedProperty
			{
				DependencyProperty^ get() { return _isRotationLocked; }
			}
			property bool IsRotationLocked
			{
				bool get() { return (bool)GetValue(IsRotationLockedProperty); }
				void set(bool value) { SetValue(IsRotationLockedProperty, value); }
			}

		private:
			bool IsInitialDataLoaded;

			Platform::Array<String^, 1U>^ levelNames;

			void UpdateWindowSize();

			Platform::Collections::Vector<TopScore^>^ _topScores;
			GameState PreviousGameState;

			static DependencyProperty^ _onScreenControlsVisible;
			static DependencyProperty^ _musicEnabled;
			static DependencyProperty^ _sfxEnabled;
			static DependencyProperty^ _isAccelerometerInverted;
			static DependencyProperty^ _isAccelerometerEnabled;
			static DependencyProperty^ _musicVolume;
			static DependencyProperty^ _sfxVolume;
			static DependencyProperty^ _isMouseEnabled;
			static DependencyProperty^ _isRotationLocked;

			Windows::Foundation::EventRegistrationToken	WindowActivationToken;

			void QuickSortScores(Windows::Foundation::Collections::IVector<TopScore^>^ scores, int left, int right);

			void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
			void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
			void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
			void OnWindowActivated(Object^ sender, Windows::UI::Core::WindowActivatedEventArgs^ e);
			void OnLogicalDpiChanged(Platform::Object^ sender);
			void OnRendering(Object^ sender, Object^ args);
			void UpdateHud(Coding4Fun::FallFury::Screens::GamePlayScreen^);

			void OnButtonSelected(MenuItem^ sender, Platform::String^ params);
			void OKSubmitScore_Selected(MenuItem^ sender, Platform::String^ params);
			void ShowTopScores_Selected(MenuItem^ sender, Platform::String^ params);
			void ShareTopScore_Selected(MenuItem^ sender, Platform::String^ params);
			void NewGame_Selected(MenuItem^ sender, Platform::String^ params);
			void SubmitTopScore_Selected(MenuItem^ sender, Platform::String^ params);
			void ReturnToMenu_Selected(MenuItem^ sender, Platform::String^ params);
			void RestartGame_Selected(MenuItem^ sender, Platform::String^ params);
			void Settings_Selected(MenuItem^ sender, Platform::String^ params);
			void About_Selected(MenuItem^ sender, Platform::String^ params);
			void NextLevel_Selected(MenuItem^ sender, Platform::String^ params);
			void ResumeGame_Selected(MenuItem^ sender, Platform::String^ params);
			void RestartFromPause_Selected(MenuItem^ sender, Platform::String^ params);
			void HowTo_Selected(MenuItem^ sender, Platform::String^ params);
			void HomeFromHowTo_Selected(MenuItem^ sender, Platform::String^ params);
			void ContinueFromWin_Selected(MenuItem^ sender, Platform::String^ params);

			void SetButtonEventHandlers();

			void OnShareDataRequested(Windows::ApplicationModel::DataTransfer::DataTransferManager^ manager, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs^ params);
#ifndef WP8
			void OnSettingsRequested(Windows::UI::ApplicationSettings::SettingsPane^ settingsPane, Windows::UI::ApplicationSettings::SettingsPaneCommandsRequestedEventArgs^ eventArgs);
#endif
			void OnSettingsSelected(Windows::UI::Popups::IUICommand^ command);

			void LoadLevelData();

			void OnLevelLoadCompleted(Windows::Foundation::IAsyncOperation<Platform::Object^> ^op, AsyncStatus s);
			void OnScoreWriteCompleted(Windows::Foundation::IAsyncOperation<Platform::Object^> ^op, AsyncStatus s);
			void OnLevelButtonSelected(MenuItem^ sender, Platform::String^ params);

			Windows::Foundation::EventRegistrationToken m_eventToken;
#ifndef WP8
			ApplicationViewState PreviousState;
#endif
			void InitializeStoryboard();
			void AssociateStoryboardWithItem(UIElement^ element);

			int animationBeginTime; // Used to track begin time for the level selector storyboard.

			ScreenManager^ m_renderer;

			float Adjustment;

			Coding4Fun::FallFury::Helpers::Timer^ m_timer;
			void OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
			void OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void OnKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
			void dismissAudioSettings_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

			void LoadSettings();
			void CheckForMusic();
			void CheckForSFX();
			void CheckHUD();
			void CheckGameState();

			void ResetHowToStory();

			int animationCount;
			int childrenCount;
			void AnimateChildren(Grid^ container);
			void AnimateStackPanelChildren(StackPanel^ panel);

			void UpdateResults();
			void HideEverything();

			void SwitchGameState();

			void LoadHighscores();
			void OnHighscoreLoadCompleted(Windows::Foundation::IAsyncOperation<Platform::Object^> ^op, AsyncStatus s);
			void GenerateTopScoreGrid();

			void tglSFX_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void tglMusic_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void XAMLPage_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
			void btnPause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void btnResume_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void XAMLPage_LayoutUpdated(Platform::Object^ sender, Platform::Object^ e);

			void FadeInCompleted(Platform::Object^ sender, Platform::Object^ e);
			void FadeOutCompleted(Platform::Object^ sender, Platform::Object^ e);
			void StoryboardChainA_Completed(Platform::Object^ sender, Platform::Object^ e);
			void StoryboardChainB_Completed(Platform::Object^ sender, Platform::Object^ e);
			void StoryboardChainC_Completed(Platform::Object^ sender, Platform::Object^ e);

			void TransitionFromMenuToLevels(Platform::Object^ sender, Platform::Object^ e);
			void TransitionFromMenuToTopScores(Platform::Object^ sender, Platform::Object^ e);

			void tglEnableAccelerometer_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void tglInvertAccelerometer_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void dismissAccelerometerSettings_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void sldMusicVolume_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
			void sldSFXVolume_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
			void dismissCharacterMovementSettings_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void tglEnableMouseMovement_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void Storyboard_Completed_4(Platform::Object^ sender, Platform::Object^ e);
			void Storyboard_Completed_5(Platform::Object^ sender, Platform::Object^ e);
			void Storyboard_Completed_6(Platform::Object^ sender, Platform::Object^ e);
			void Storyboard_Completed_7(Platform::Object^ sender, Platform::Object^ e);
			void tglEnableScreenLock_Toggled_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		};
	}
}
