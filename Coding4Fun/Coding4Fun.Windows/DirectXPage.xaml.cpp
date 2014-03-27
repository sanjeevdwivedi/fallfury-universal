//
// BlankPage.xaml.cpp
// Implementation of the BlankPage.xaml class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include "GameScreens\GamePlayScreen.h"
#include "Helpers\SettingsHelper.h"
#include "GameScreens\GameOverScreen.h"
#include "Helpers\TopScoreLoader.h"
#include <map>

using namespace Coding4Fun::FallFury;
using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Audio;
using namespace Coding4Fun::FallFury::Controls;
using namespace Coding4Fun::FallFury::Helpers;
using namespace Coding4Fun::FallFury::Models;

using namespace Windows::UI::ViewManagement;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Input;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::ApplicationSettings;
using namespace Windows::UI::Popups;

DirectXPage::DirectXPage()
{
	InitializeComponent();

	IsInitialDataLoaded = false;

	// Default accelerometer settings. Later on these might be 
	// overriden by the LoadSettings method.
	IsAccelerometerEnabled = true;
	IsAccelerometerInverted = false;
	IsMouseEnabled = true;

	MusicVolume = 100;
	SFXVolume = 100;


	MusicEnabled = false;
	SFXEnabled = false;

	auto dataTransferManager = Windows::ApplicationModel::DataTransfer::DataTransferManager::GetForCurrentView();
	dataTransferManager->DataRequested += ref new TypedEventHandler<Windows::ApplicationModel::DataTransfer::DataTransferManager^, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs^>(this, &DirectXPage::OnShareDataRequested);

	m_renderer = ref new ScreenManager();

	m_renderer->Initialize(
		Window::Current->CoreWindow,
		this->XAMLPage,
		DisplayProperties::LogicalDpi
		);

	Window::Current->CoreWindow->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);

	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);

	m_eventToken = CompositionTarget::Rendering += ref new EventHandler<Object^>(this, &DirectXPage::OnRendering);

	m_timer = ref new Timer();

	SetButtonEventHandlers();

	UpdateWindowSize();

	_topScores = ref new Platform::Collections::Vector<TopScore^>();
}

void DirectXPage::SetButtonEventHandlers()
{
	btnNewGame->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::NewGame_Selected);
	btnGOSubmitScore->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::SubmitTopScore_Selected);
	btnReturnToMenu->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ReturnToMenu_Selected);
	btnGOMenu->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ReturnToMenu_Selected);
	btnSOMenu->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ReturnToMenu_Selected);
	btnOKSubmitScore->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::OKSubmitScore_Selected);
	btnShowTopScores->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ShowTopScores_Selected);
	btnShare->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ShareTopScore_Selected);
	btnGORestart->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::RestartGame_Selected);
	btnYWMenu->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ReturnToMenu_Selected);
	btnSettings->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::Settings_Selected);
	btnAbout->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::About_Selected);
	btnABHome->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ReturnToMenu_Selected);
	btnYWNextLevel->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::NextLevel_Selected);
	btnPauseResume->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ResumeGame_Selected);
	btnPauseRestartLevel->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::RestartFromPause_Selected);
	btnPauseHome->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ReturnToMenu_Selected);
	btnPauseSettings->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::Settings_Selected);
	btnLSHome->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ReturnToMenu_Selected);
	btnHowTo->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::HowTo_Selected);
	btnHTHome->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::HomeFromHowTo_Selected);
	btnCGOK->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::ContinueFromWin_Selected);
}

void DirectXPage::InitializeStoryboard()
{
	TransitionRectangle->Visibility = Windows::UI::Xaml::Visibility::Visible;
	Storyboard^ loc = (Storyboard^)grdMain->Resources->Lookup("FadingIn");
	loc->Begin();
}

void DirectXPage::NewGame_Selected(MenuItem^ sender, Platform::String^ params)
{
	m_renderer->CurrentGameState = GameState::GS_LEVEL_SELECT_SINGLE;
	stkMainMenu->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	stkMainMenu->IsTapEnabled = false;
	stkLevelSelector->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void DirectXPage::SubmitTopScore_Selected(MenuItem^ sender, Platform::String^ params)
{
	m_renderer->CurrentGameState = GameState::GS_SUBMIT_SCORE;
}

void DirectXPage::ReturnToMenu_Selected(MenuItem^ sender, Platform::String^ params)
{
	m_renderer->CurrentGameState = GameState::GS_MAIN_MENU;
	StaticDataHelper::IsPaused = false;
}

void DirectXPage::RestartGame_Selected(MenuItem^ sender, Platform::String^ params)
{
	m_renderer->CurrentGameScreen = ref new GamePlayScreen(m_renderer);
}

void DirectXPage::Settings_Selected(MenuItem^ sender, Platform::String^ params)
{
	SettingsPane::GetForCurrentView()->Show();
}

void DirectXPage::ResumeGame_Selected(MenuItem^ sender, Platform::String^ params)
{
	StaticDataHelper::IsPaused = false;
}

void DirectXPage::About_Selected(MenuItem^ sender, Platform::String^ params)
{
	m_renderer->CurrentGameState = GameState::GS_ABOUT_SCREEN;
	//m_renderer->CurrentGameState = GameState::GS_FULL_WIN;
}

void DirectXPage::ContinueFromWin_Selected(MenuItem^ sender, Platform::String^ params)
{

	m_renderer->CurrentGameState = GameState::GS_WIN;
}

void DirectXPage::NextLevel_Selected(MenuItem^ sender, Platform::String^ params)
{
	if (m_renderer->Levels.HasKey(StaticDataHelper::CurrentLevelID + 1))
	{
		StaticDataHelper::CurrentLevelID++;
		m_renderer->CurrentGameScreen = ref new GamePlayScreen(m_renderer);
	}
}

void DirectXPage::ShowTopScores_Selected(MenuItem^ sender, Platform::String^ params)
{
	m_renderer->CurrentGameState = GameState::GS_TOP_SCORES;
	LoadHighscores();
}

void DirectXPage::OKSubmitScore_Selected(MenuItem^ sender, Platform::String^ params)
{
	if (!txtPlayerName->Text->IsEmpty())
	{
		ScoreWriter^ writer = ref new ScoreWriter();

		Windows::Globalization::Calendar^ c = ref new Windows::Globalization::Calendar;
		c->SetToNow();

		Windows::Foundation::IAsyncOperation<Platform::Object^>^ result = writer->WriteScoreAsync(txtPlayerName->Text,
			StaticDataHelper::CurrentLevelName, StaticDataHelper::Score.ToString(), StaticDataHelper::Medal);

		result->Completed =
			ref new AsyncOperationCompletedHandler<Platform::Object^>(this, &DirectXPage::OnScoreWriteCompleted);
	}
}

void DirectXPage::OnScoreWriteCompleted(IAsyncOperation<Platform::Object^> ^op, AsyncStatus s)
{
	if (s == AsyncStatus::Completed)
	{
		m_renderer->CurrentGameState = GameState::GS_MAIN_MENU;
	}
}

void DirectXPage::OnButtonSelected(MenuItem^ sender, Platform::String^ params)
{
	if (params == "top scores")
	{
		m_renderer->CurrentGameState = GameState::GS_TOP_SCORES;
	}
	else if (params == "ok")
	{
		m_renderer->CurrentGameState = GameState::GS_TOP_SCORES;
	}
}

void DirectXPage::RestartFromPause_Selected(MenuItem^ sender, Platform::String^ params)
{
	m_renderer->CurrentGameScreen = nullptr;
	StaticDataHelper::IsPaused = false;
}

DependencyProperty^ DirectXPage::_musicEnabled = DependencyProperty::Register("MusicEnabled",
	bool::typeid, DirectXPage::typeid, nullptr);
DependencyProperty^ DirectXPage::_sfxEnabled = DependencyProperty::Register("SFXEnabled",
	bool::typeid, DirectXPage::typeid, nullptr);
DependencyProperty^ DirectXPage::_isAccelerometerEnabled = DependencyProperty::Register("IsAccelerometerEnabled",
	bool::typeid, DirectXPage::typeid, nullptr);
DependencyProperty^ DirectXPage::_isAccelerometerInverted = DependencyProperty::Register("IsAccelerometerInverted",
	bool::typeid, DirectXPage::typeid, nullptr);
DependencyProperty^ DirectXPage::_musicVolume = DependencyProperty::Register("MusicVolume",
	int::typeid, DirectXPage::typeid, nullptr);
DependencyProperty^ DirectXPage::_sfxVolume = DependencyProperty::Register("SFXVolume",
	int::typeid, DirectXPage::typeid, nullptr);
DependencyProperty^ DirectXPage::_isMouseEnabled = DependencyProperty::Register("IsMouseEnabled",
	int::typeid, DirectXPage::typeid, nullptr);
DependencyProperty^ DirectXPage::_isRotationLocked = DependencyProperty::Register("IsRotationLocked",
	bool::typeid, DirectXPage::typeid, nullptr);

void DirectXPage::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_renderer->UpdateForWindowSizeChange();
	UpdateWindowSize();
	if (m_renderer->CurrentGameScreen != nullptr)
		m_renderer->CurrentGameScreen->UpdateWindowSize();
}

void DirectXPage::UpdateWindowSize()
{
	bool visibility = true;
	if (ApplicationView::Value == ApplicationViewState::Snapped)
		visibility = false;

	float margin = (m_renderer->m_renderTargetSize.Width - 768.0f) / 2.0f;
	if (margin < 2.0)
		visibility = false;

	if (visibility)
	{
		containerA->Width = margin;
		containerA->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Right;
		containerB->Width = margin;
		containerB->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
		containerA->Visibility = Windows::UI::Xaml::Visibility::Visible;
		containerB->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}
	else
	{
		containerA->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		containerB->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
}

void DirectXPage::OnLogicalDpiChanged(Platform::Object^ sender)
{
	m_renderer->SetDpi(DisplayProperties::LogicalDpi);
}

void DirectXPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
{
	m_timer->Update();

	if (!StaticDataHelper::IsPaused && IsInitialDataLoaded)
	{
		grdPaused->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		m_renderer->Update(m_timer->Total, m_timer->Delta);
		m_renderer->Render();
		m_renderer->Present();

		CheckHUD();

		CheckGameState();

		if (txtPlayerName->Text->IsEmpty())
		{
			txtWatermark->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else
		{
			txtWatermark->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}

		AudioManager::Update();

		if (m_renderer->CurrentGameState == GameState::GS_PLAYING)
		{
			auto screen = (GamePlayScreen^)m_renderer->CurrentGameScreen;

			if (screen->IsLevelLoaded)
			{
				grdLoading->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			}
			else
			{
				grdLoading->Visibility = Windows::UI::Xaml::Visibility::Visible;
			}
		}
	}
	else
	{
		grdPaused->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}
}

void DirectXPage::UpdateResults()
{
	TotalTime->Text = txtTimer->Text;
	TotalCoinsCollected->Text = StaticDataHelper::ButtonsCollected.ToString();
	TotalCoinsInLevel->Text = StaticDataHelper::ButtonsTotal.ToString();
	TotalEnemiesKilled->Text = StaticDataHelper::MonstersKilled.ToString();

	Coding4Fun::FallFury::Models::Monster^ tempMonster = StaticDataHelper::CurrentLevel->Monsters[StaticDataHelper::CurrentLevel->Monsters->Length - 1];

	float levelLength = tempMonster->Lifetime + tempMonster->Y;

	if (StaticDataHelper::ButtonsTotal == 0)
	{
		// there were no buttons on the level, so need a safe division
		StaticDataHelper::ButtonsTotal = 1;
	}

	float estimatedTime = levelLength / (StaticDataHelper::CurrentLevel->GameBear->Velocity / 0.016f);

	int monsterScore = static_cast<int>(StaticDataHelper::MonstersKilled * 30.0f);
	int buttonScore = static_cast<int>(StaticDataHelper::ButtonsCollected * 5.0f);

	int timeScore = 0;
	if (m_renderer->CurrentGameState != GameState::GS_GAME_OVER)
		timeScore = static_cast<int>((estimatedTime - StaticDataHelper::SecondsTotal) * 2.0f);

	StaticDataHelper::Score = monsterScore + buttonScore + timeScore;

	int maxScore = static_cast<int>(StaticDataHelper::ButtonsTotal * 5.0f + StaticDataHelper::CurrentLevel->Monsters->Length * 30.0f);

	float ratio = static_cast<float>(StaticDataHelper::Score) / static_cast<float>(maxScore);
	Windows::UI::Xaml::Media::Imaging::BitmapImage^ image;

	if (ratio >= 0.0f && ratio < 0.4f)
	{
		image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx:///Assets/Medals/bronze.png"));
		// BRONZE MEDAL
		MedalImage->Source = image;
		txtMedalType->Style = (Windows::UI::Xaml::Style^)Application::Current->Resources->Lookup("ResultScreenMedalBronze");
		StaticDataHelper::Medal = MedalType::BRONZE;
	}
	else if (ratio >= 0.4f && ratio < 0.8f)
	{
		image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx:///Assets/Medals/silver.png"));

		// SILVER MEDAL
		MedalImage->Source = image;
		txtMedalType->Style = (Windows::UI::Xaml::Style^)Application::Current->Resources->Lookup("ResultScreenMedalSilver");
		StaticDataHelper::Medal = MedalType::SILVER;
	}
	else if (ratio >= 0.8f)
	{
		image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx:///Assets/Medals/gold.png"));
		// GOLD MEDAL
		MedalImage->Source = image;
		txtMedalType->Style = (Windows::UI::Xaml::Style^)Application::Current->Resources->Lookup("ResultScreenMedalGold");
		StaticDataHelper::Medal = MedalType::GOLD;
	}

	imgWinMedal->Source = image;

	TotalScore->Text = StaticDataHelper::Score.ToString();
}

void DirectXPage::UpdateHud(GamePlayScreen^ playScreen)
{
	healthBar->Update(playScreen->GameBear->CurrentHealth, playScreen->GameBear->MaxHealth);

	txtButtons->Text = StaticDataHelper::ButtonsCollected.ToString();

	// Find a better built-in string formatting code
	if (m_renderer->CurrentGameState == GameState::GS_PLAYING && !(StaticDataHelper::IsPaused) && playScreen->IsLevelLoaded)
	{
		StaticDataHelper::SecondsTotal += m_timer->Delta;

		txtTimer->Text = StaticDataHelper::GetTimeString((int)StaticDataHelper::SecondsTotal);
	}
}

void DirectXPage::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SettingsPane::GetForCurrentView()->CommandsRequested += ref new TypedEventHandler<SettingsPane^, SettingsPaneCommandsRequestedEventArgs^>(this, &DirectXPage::OnSettingsRequested);

	LoadSettings();
	LoadLevelData();
}

void DirectXPage::OnWindowActivated(Object^ sender, Windows::UI::Core::WindowActivatedEventArgs^ e)
{
	if (e->WindowActivationState == Windows::UI::Core::CoreWindowActivationState::Deactivated)
	{
		stkMusicSfx->IsOpen = false;
	}
}

void DirectXPage::OnSettingsSelected(Windows::UI::Popups::IUICommand^ command)
{
	if (command->Id->ToString() == "musicSfx")
	{
		stkMusicSfx->Width = 346.0f;
		grdSubMusicSfx->Height = m_renderer->m_renderTargetSize.Height;
		stkMusicSfx->IsOpen = true;
	}
	else if (command->Id->ToString() == "accelerometer")
	{
		stkAccelerometerSettings->Width = 346.0f;
		grdAccelerometerSettings->Height = m_renderer->m_renderTargetSize.Height;
		stkAccelerometerSettings->IsOpen = true;
	}
	else if (command->Id->ToString() == "charMovement")
	{
		stkCharacterMovement->Width = 346.0f;
		grdCharacterMovement->Height = m_renderer->m_renderTargetSize.Height;
		stkCharacterMovement->IsOpen = true;
	}


	WindowActivationToken = Window::Current->Activated += ref new WindowActivatedEventHandler(this, &DirectXPage::OnWindowActivated);
}

void DirectXPage::OnSettingsRequested(Windows::UI::ApplicationSettings::SettingsPane^ settingsPane, Windows::UI::ApplicationSettings::SettingsPaneCommandsRequestedEventArgs^ eventArgs)
{
	if (m_renderer->CurrentGameState == GameState::GS_PLAYING)
		StaticDataHelper::IsPaused = true;

	UICommandInvokedHandler^ handler = ref new UICommandInvokedHandler(this, &DirectXPage::OnSettingsSelected);

	SettingsCommand^ generalCommand = ref new SettingsCommand("musicSfx", "Music & SFX", handler);
	eventArgs->Request->ApplicationCommands->Append(generalCommand);

	SettingsCommand^ accelerometerCommand = ref new SettingsCommand("accelerometer", "Accelerometer", handler);
	eventArgs->Request->ApplicationCommands->Append(accelerometerCommand);

	SettingsCommand^ charMovementCommand = ref new SettingsCommand("charMovement", "Character Movement", handler);
	eventArgs->Request->ApplicationCommands->Append(charMovementCommand);
}

void DirectXPage::OnKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	m_renderer->CurrentGameScreen->HandleKeyInput(e->Key);
}

void DirectXPage::dismissAudioSettings_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	stkMusicSfx->IsOpen = false;
	Window::Current->Activated -= WindowActivationToken;
	SettingsPane::GetForCurrentView()->Show();
}

void DirectXPage::LoadSettings()
{
	AudioManager::Initialize();
	AudioManager::SetMainMenuMusic();

	if (SettingsHelper::Exists("mouseEnabled"))
		IsMouseEnabled = safe_cast<bool>(SettingsHelper::Read("mouseEnabled"));

	if (SettingsHelper::Exists("accEnabled"))
		IsAccelerometerEnabled = safe_cast<bool>(SettingsHelper::Read("accEnabled"));

	if (SettingsHelper::Exists("accInverted"))
		IsAccelerometerInverted = safe_cast<bool>(SettingsHelper::Read("accInverted"));

	if (SettingsHelper::Exists("musicVolume"))
	{
		MusicVolume = (int)safe_cast<double>(SettingsHelper::Read("musicVolume"));
	}

	if (SettingsHelper::Exists("sfxVolume"))
	{
		SFXVolume = (int)safe_cast<double>(SettingsHelper::Read("sfxVolume"));
	}

	if (SettingsHelper::Exists("musicEnabled"))
	{
		MusicEnabled = safe_cast<bool>(SettingsHelper::Read("musicEnabled"));
	}
	else
	{
		MusicEnabled = true;
	}

	if (SettingsHelper::Exists("sfxEnabled"))
	{
		SFXEnabled = safe_cast<bool>(SettingsHelper::Read("sfxEnabled"));
	}
	else
	{
		SFXEnabled = true;
	}

	if (SettingsHelper::Exists("rotationLocked"))
	{
		IsRotationLocked = safe_cast<bool>(SettingsHelper::Read("rotationLocked"));

		if (IsRotationLocked)
		{
			if (SettingsHelper::Exists("rotation"))
			{
				int currentRotation = safe_cast<int>(SettingsHelper::Read("rotation"));
				DisplayProperties::AutoRotationPreferences = (DisplayOrientations)(currentRotation);
			}
			else
			{
				DisplayProperties::AutoRotationPreferences = DisplayOrientations::Portrait;
			}
		}
		else
		{
			DisplayProperties::AutoRotationPreferences = DisplayOrientations::Landscape | DisplayOrientations::LandscapeFlipped
				| DisplayOrientations::Portrait | DisplayOrientations::PortraitFlipped;
		}
	}
	else
	{
		DisplayProperties::AutoRotationPreferences = DisplayOrientations::Portrait;
		IsRotationLocked = true;
	}

	AudioManager::IsSFXStarted = SFXEnabled;
	if (SFXEnabled)
	{
		AudioManager::AudioEngineInstance.StartSFX();
	}
	else
	{
		AudioManager::AudioEngineInstance.SuspendSFX();
	}

	AudioManager::IsMusicStarted = MusicEnabled;
	if (MusicEnabled)
	{
		AudioManager::AudioEngineInstance.StartMusic();
	}
	else
	{
		AudioManager::AudioEngineInstance.SuspendMusic();
	}

	AudioManager::SetMusicVolume(MusicVolume);
	AudioManager::SetSFXVolume(SFXVolume);

	IsInitialDataLoaded = true;
}

void DirectXPage::tglSFX_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SettingsHelper::Save("sfxEnabled", tglSFX->IsOn);
	SFXEnabled = tglSFX->IsOn;

	AudioManager::IsSFXStarted = SFXEnabled;
	if (SFXEnabled)
	{
		AudioManager::AudioEngineInstance.StartSFX();
	}
	else
	{
		AudioManager::AudioEngineInstance.SuspendSFX();
	}
}


void DirectXPage::tglMusic_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SettingsHelper::Save("musicEnabled", tglMusic->IsOn);
	MusicEnabled = tglMusic->IsOn;

	AudioManager::IsMusicStarted = MusicEnabled;
	if (MusicEnabled)
	{
		AudioManager::AudioEngineInstance.StartMusic();
	}
	else
	{
		AudioManager::AudioEngineInstance.SuspendMusic();
	}
}

void DirectXPage::CheckHUD()
{
	if (m_renderer->CurrentGameState == GameState::GS_PLAYING)
	{
		GamePlayScreen^ screen = (GamePlayScreen^)m_renderer->CurrentGameScreen;
		if (screen->IsLevelLoaded)
		{
			screen->GameBear->Position.x += Adjustment;

			UpdateHud(screen);
		}
	}
	else
	{
		Hud->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
}

void DirectXPage::LoadHighscores()
{
	Coding4Fun::FallFury::Reader^ reader = ref new Coding4Fun::FallFury::Reader();
	auto result = reader->ReadXmlAsync("highscores.xml", Coding4Fun::FallFury::Models::XmlType::HIGHSCORE);
	result->Completed =
		ref new AsyncOperationCompletedHandler<Platform::Object^>(this, &DirectXPage::OnHighscoreLoadCompleted);
}

void DirectXPage::OnHighscoreLoadCompleted(IAsyncOperation<Platform::Object^> ^op, AsyncStatus s)
{
	if (s == AsyncStatus::Completed)
	{
		TopScoreLoader^ loader = ref new TopScoreLoader((Coding4Fun::FallFury::Models::TopScoreSet^)op->GetResults());
		TopScores = loader->Extract();

		QuickSortScores(TopScores, 0, TopScores->Size - 1);

		Platform::Collections::Vector<TopScore^>^ scores = ref new Platform::Collections::Vector<TopScore^>();

		unsigned int countTo = 7;
		if (TopScores->Size < countTo)
		{
			countTo = TopScores->Size;
		}

		for (unsigned int i = 0; i < countTo; i++)
		{
			scores->Append(TopScores->GetAt(i));
		}

		TopScores = scores;

		GenerateTopScoreGrid();
	}
}

void DirectXPage::QuickSortScores(Windows::Foundation::Collections::IVector<TopScore^>^ scores, int left, int right)
{
	int i = left, j = right;
	TopScore^ tempValue;

	TopScore^ pivotItem = scores->GetAt((left + right) / 2);
	while (i <= j)
	{
		while (scores->GetAt(i)->Score > pivotItem->Score)
		{
			i++;
		}
		while (scores->GetAt(j)->Score < pivotItem->Score)
		{
			j--;
		}

		if (i <= j)
		{
			tempValue = scores->GetAt(i);
			scores->SetAt(i, scores->GetAt(j));
			scores->SetAt(j, tempValue);
			i++;
			j--;
		}
	}

	if (left < j)
		QuickSortScores(scores, left, j);
	if (i < right)
		QuickSortScores(scores, i, right);
}

void DirectXPage::GenerateTopScoreGrid()
{
	ItemsControl^ itemContainer = ref new ItemsControl();

	for (int i = 0; i< (int)TopScores->Size; i++)
	{
		Grid^ rowGrid = ref new Grid();
		rowGrid->Margin = Thickness(24, 24, 24, 0);

		ColumnDefinition^ definitionA = ref new ColumnDefinition();
		definitionA->Width = 200;
		ColumnDefinition^ definitionB = ref new ColumnDefinition();
		definitionB->Width = 270;
		ColumnDefinition^ definitionC = ref new ColumnDefinition();
		definitionC->Width = 150;
		ColumnDefinition^ definitionD = ref new ColumnDefinition();
		definitionD->Width = 50;

		rowGrid->ColumnDefinitions->Append(definitionA);
		rowGrid->ColumnDefinitions->Append(definitionB);
		rowGrid->ColumnDefinitions->Append(definitionC);
		rowGrid->ColumnDefinitions->Append(definitionD);

		TextBlock^ tBlock = ref new TextBlock();
		tBlock->Text = TopScores->GetAt(i)->Name;
		tBlock->SetValue(Grid::ColumnProperty, (Object^)0);
		tBlock->Style = (Windows::UI::Xaml::Style^)Application::Current->Resources->Lookup("ScoreText");
		tBlock->Margin = Thickness(0, 0, 12, 0);

		rowGrid->Children->Append(tBlock);

		TextBlock^ tBlockB = ref new TextBlock();
		tBlockB->Text = TopScores->GetAt(i)->Level;
		tBlockB->SetValue(Grid::ColumnProperty, (Object^)1);
		tBlockB->Style = (Windows::UI::Xaml::Style^)Application::Current->Resources->Lookup("ScoreText");
		tBlockB->Margin = Thickness(0, 0, 12, 0);
		rowGrid->Children->Append(tBlockB);

		TextBlock^ tBlockC = ref new TextBlock();
		tBlockC->Text = TopScores->GetAt(i)->Score.ToString();
		tBlockC->SetValue(Grid::ColumnProperty, (Object^)2);
		tBlockC->Style = (Windows::UI::Xaml::Style^)Application::Current->Resources->Lookup("ScoreText");
		rowGrid->Children->Append(tBlockC);

		Image^ image = ref new Image();
		image->SetValue(Grid::ColumnProperty, (Object^)3);
		image->Margin = Thickness(0, -12, 0, 0);

		MedalType mType = TopScores->GetAt(i)->ScoreMedalType;

		switch (mType)
		{
		case MedalType::BRONZE:
		{
			Windows::UI::Xaml::Media::Imaging::BitmapImage^ medalImage = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx:///Assets/Medals/bronze.png"));
			image->Source = medalImage;

			break;
		}
		case MedalType::SILVER:
		{
			Windows::UI::Xaml::Media::Imaging::BitmapImage^ medalImage = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx:///Assets/Medals/silver.png"));
			image->Source = medalImage;

			break;
		}
		case MedalType::GOLD:
		{
			Windows::UI::Xaml::Media::Imaging::BitmapImage^ medalImage = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx:///Assets/Medals/gold.png"));
			image->Source = medalImage;

			break;
		}
		default:
			break;
		}
		rowGrid->Children->Append(image);

		itemContainer->Items->Append(rowGrid);
	}

	TopScoreItemContainer->Children->Clear();
	TopScoreItemContainer->Children->Append(itemContainer);
}

void DirectXPage::CheckGameState()
{
	if (PreviousGameState != m_renderer->CurrentGameState)
	{
		HideEverything();

		InitializeStoryboard();

		PreviousGameState = m_renderer->CurrentGameState;
	}
}

void DirectXPage::SwitchGameState()
{
	switch (m_renderer->CurrentGameState)
	{
	case GameState::GS_FULL_WIN:
	{
		grdCompleteWin->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	case GameState::GS_PLAYING:
	{
		Hud->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	case GameState::GS_MAIN_MENU:
	{
		stkMainMenu->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	case GameState::GS_GAME_OVER:
	{
		UpdateResults();
		ResultPanel->Visibility = Windows::UI::Xaml::Visibility::Visible;
		grdGameOver->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	case GameState::GS_SUBMIT_SCORE:
	{
		grdSubmitScore->Visibility = Windows::UI::Xaml::Visibility::Visible;
		txtSubmitScoreView->Text = StaticDataHelper::Score.ToString();
		break;
	}
	case GameState::GS_TOP_SCORES:
	{
		grdTopScores->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	case GameState::GS_WIN:
	{
		UpdateResults();
		grdWinner->Visibility = Windows::UI::Xaml::Visibility::Visible;
		ResultPanel->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	case GameState::GS_ABOUT_SCREEN:
	{
		grdAbout->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	case GameState::GS_LEVEL_SELECT_SINGLE:
	{
		animationBeginTime = 0;

		stkLevelSelector->Visibility = Windows::UI::Xaml::Visibility::Visible;
		break;
	}
	case GameState::GS_HOW_TO:
	{
		grdHowTo->Visibility = Windows::UI::Xaml::Visibility::Visible;
		Storyboard^ howToInitialBoard = (Storyboard^)grdMain->Resources->Lookup("StoryboardChainA");
		howToInitialBoard->Begin();
		break;
	}
	default:
		break;
	}

	Storyboard^ loc = (Storyboard^)grdMain->Resources->Lookup("FadingOut");
	loc->Begin();
}

void DirectXPage::HideEverything()
{
	for (uint i = 0; i < grdMain->Children->Size; i++)
	{
		grdMain->Children->GetAt(i)->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
}

void DirectXPage::LoadLevelData()
{
	Coding4Fun::FallFury::Reader^ reader = ref new Coding4Fun::FallFury::Reader();

	Windows::Foundation::IAsyncOperation<Platform::Object^>^ result = reader->ReadXmlAsync("core.xml", Coding4Fun::FallFury::Models::XmlType::TIERS);

	result->Completed =
		ref new AsyncOperationCompletedHandler<Platform::Object^>(this, &DirectXPage::OnLevelLoadCompleted);
}

void DirectXPage::OnLevelLoadCompleted(IAsyncOperation<Platform::Object^> ^op, AsyncStatus s)
{
	if (s == AsyncStatus::Completed)
	{
		auto set = (Coding4Fun::FallFury::Models::TierSet^)op->GetResults();

		auto tiers = set->Tiers;

		int levelCounter = 0;

		for (auto tier = tiers->begin(); tier != tiers->end(); tier++)
		{
			StackPanel^ panel = ref new StackPanel();

			TextBlock^ levelTierTitle = ref new TextBlock();
			levelTierTitle->Text = (*tier)->Name;
			levelTierTitle->Style = (Windows::UI::Xaml::Style^)Application::Current->Resources->Lookup("LevelSelectTierItemText");
			levelTierTitle->RenderTransform = ref new TranslateTransform();
			panel->Children->Append(levelTierTitle);

			levelNames = (*tier)->LevelNames;
			auto levelFiles = (*tier)->LevelFiles;

			int max = levelNames->Length;

			for (int i = 0; i < max; i++)
			{
				MenuItem^ item = ref new MenuItem();
				item->Tag = levelCounter;
				item->Label = levelNames[i];
				item->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
				item->OnButtonSelected += ref new MenuItem::ButtonSelected(this, &DirectXPage::OnLevelButtonSelected);
				item->RenderTransform = ref new TranslateTransform();

				m_renderer->Levels.Insert(levelCounter, levelFiles[i]);

				panel->Children->Append(item);
				levelCounter++;
			}

			stkLevelContainer->Items->Append(panel);
		}
	}
}


void DirectXPage::OnLevelButtonSelected(MenuItem^ sender, Platform::String^ params)
{
	StaticDataHelper::CurrentLevelID = (int)sender->Tag;
	StaticDataHelper::CurrentLevelName = params;

	m_renderer->CurrentGameScreen = ref new GamePlayScreen(m_renderer);
}

void DirectXPage::btnPause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	StaticDataHelper::IsPaused = true;
}


void DirectXPage::XAMLPage_LayoutUpdated(Platform::Object^ sender, Platform::Object^ e)
{
	ApplicationViewState currentState = ApplicationView::Value;

	if (currentState != PreviousState)
	{
		if (ApplicationView::Value == ApplicationViewState::Snapped)
		{
			StaticDataHelper::IsPaused = true;
			stkPauseControls->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			stkSnapped->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else
		{
			if (PreviousGameState == GameState::GS_PLAYING)
			{
				stkPauseControls->Visibility = Windows::UI::Xaml::Visibility::Visible;
			}
			else
			{
				StaticDataHelper::IsPaused = false;
			}

			// Snap view closed, make sure to hide the panel that covers everything
			stkSnapped->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}

		PreviousState = currentState;
	}
}


void DirectXPage::FadeInCompleted(Platform::Object^ sender, Platform::Object^ e)
{
	SwitchGameState();
}


void DirectXPage::FadeOutCompleted(Platform::Object^ sender, Platform::Object^ e)
{
	TransitionRectangle->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void DirectXPage::ShareTopScore_Selected(MenuItem^ sender, Platform::String^ params)
{
	Windows::ApplicationModel::DataTransfer::DataTransferManager::ShowShareUI();
}

void DirectXPage::OnShareDataRequested(Windows::ApplicationModel::DataTransfer::DataTransferManager^ manager, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs^ params)
{
	auto request = params->Request;
	request->Data->Properties->Title = "Share Score";
	request->Data->Properties->Description = "Tell your friends how much you scored in [DEN'S PROJECT]!";
	request->Data->SetText("I just scored " + StaticDataHelper::Score.ToString() + " in [DEN'S PROJECT]! Beat me! http://dennisdel.com");
}

void DirectXPage::tglEnableAccelerometer_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SettingsHelper::Save("accEnabled", tglEnableAccelerometer->IsOn);
	IsAccelerometerEnabled = tglEnableAccelerometer->IsOn;
}


void DirectXPage::tglInvertAccelerometer_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SettingsHelper::Save("accInverted", tglInvertAccelerometer->IsOn);
	IsAccelerometerInverted = tglInvertAccelerometer->IsOn;
}

void DirectXPage::dismissAccelerometerSettings_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	stkAccelerometerSettings->IsOpen = false;
	Window::Current->Activated -= WindowActivationToken;
	SettingsPane::GetForCurrentView()->Show();
}

void DirectXPage::sldMusicVolume_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	SettingsHelper::Save("musicVolume", e->NewValue);
	AudioManager::SetMusicVolume((int)e->NewValue);
}


void DirectXPage::sldSFXVolume_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	SettingsHelper::Save("sfxVolume", e->NewValue);
	AudioManager::SetSFXVolume((int)e->NewValue);
}


void DirectXPage::dismissCharacterMovementSettings_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	stkCharacterMovement->IsOpen = false;
	Window::Current->Activated -= WindowActivationToken;
	SettingsPane::GetForCurrentView()->Show();
}


void DirectXPage::tglEnableMouseMovement_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SettingsHelper::Save("mouseEnabled", tglEnableMouseMovement->IsOn);
	IsMouseEnabled = tglEnableMouseMovement->IsOn;
}

void DirectXPage::HowTo_Selected(MenuItem^ sender, Platform::String^ params)
{
	m_renderer->CurrentGameState = GameState::GS_HOW_TO;
}

void DirectXPage::StoryboardChainA_Completed(Platform::Object^ sender, Platform::Object^ e)
{
	// The opacity for the Movement section was set to 0
	Storyboard^ board = (Storyboard^)grdMain->Resources->Lookup("StoryboardChainB");
	board->Begin();
}


void DirectXPage::StoryboardChainB_Completed(Platform::Object^ sender, Platform::Object^ e)
{
	Storyboard^ board = (Storyboard^)grdMain->Resources->Lookup("StoryboardChainC");
	board->Begin();
}


void DirectXPage::StoryboardChainC_Completed(Platform::Object^ sender, Platform::Object^ e)
{
	Storyboard^ board = (Storyboard^)grdMain->Resources->Lookup("StoryboardChainA");
	board->Begin();
}

void DirectXPage::HomeFromHowTo_Selected(MenuItem^ sender, Platform::String^ params)
{
	ResetHowToStory();
	ReturnToMenu_Selected(sender, params);
}

void DirectXPage::ResetHowToStory()
{
	Storyboard^ board = (Storyboard^)grdMain->Resources->Lookup("StoryboardChainA");
	board->Stop();
	board = (Storyboard^)grdMain->Resources->Lookup("StoryboardChainB");
	board->Stop();
	board = (Storyboard^)grdMain->Resources->Lookup("StoryboardChainC");
	board->Stop();

	stkHowToMonsters->Opacity = 0;
	stkHowToMovement->Opacity = 1;
	stkHowToPowerups->Opacity = 0;
}

void Coding4Fun::FallFury::DirectXPage::tglEnableScreenLock_Toggled_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SettingsHelper::Save("rotationLocked", tglEnableScreenLock->IsOn);
	IsRotationLocked = tglEnableScreenLock->IsOn;

	if (IsRotationLocked)
	{
		SettingsHelper::Save("rotation", safe_cast<int>(DisplayProperties::CurrentOrientation));
		DisplayProperties::AutoRotationPreferences = DisplayProperties::CurrentOrientation;
	}
	else
	{
		DisplayProperties::AutoRotationPreferences = DisplayOrientations::Landscape | DisplayOrientations::LandscapeFlipped
			| DisplayOrientations::Portrait | DisplayOrientations::PortraitFlipped;
	}
}
