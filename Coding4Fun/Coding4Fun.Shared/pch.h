#pragma once

#include <wrl.h>
#include <wrl/client.h>
#include <d3d11_2.h>
#include <d2d1_2.h>
#include <d2d1effects_1.h>
#include <dwrite_2.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <concrt.h>
#include <collection.h>
#include "App.xaml.h"
//
//#pragma once

#include <wrl.h>
#include <d3d11_1.h>
#include <d2d1_1.h>
#include <d2d1effects.h>
#include <dwrite_1.h>
#include <wincodec.h>
#include <ppltasks.h>
#include <agile.h>

#define XM_STRICT_VECTOR4 1
#include <DirectXMath.h>

#define XAUDIO2_HELPER_FUNCTIONS 1
#include <xaudio2.h>
#include <xaudio2fx.h>

#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <Mfreadwrite.h>

//#include <XInput.h>

#include <stdio.h>
#include <strsafe.h>
#include <vector>
#include <list>
#include <memory>
#include <map>
#include <queue>
#include <string>
#include <set>
#include <sstream>

#include "Helpers\StaticDataHelper.h"
#include "Helpers\LocalDiagnostics.h"
#include "Helpers\RWContract.h"
#include "DXCore\BasicLoader.h"
#include "Entities\Obstacle.h"
#include "Entities\GameState.h"
#include "GameScreens\MenuScreen.h"
#include "DXCore\SpriteWorks.h"
#include "ParticleSystem\Particle.h"
#include "Entities\SizeEnums.h"
#include "GameScreens\GameScreenBase.h"
#include "ScreenManager.h"
#include "Entities\Powerup.h"
#include "GameScreens\GamePlayScreen.h"
#include "Entities\TopScore.h"
#include "Characters\LivingEntity.h"

#include "App.xaml.h"