#include "ModeLoading.h"
#include "GameMain.h"
#include "ModePoor.h"
#include "ModeMovie.h"
#include "ModeClearResult.h"
#include "ModeGameOver.h"
#include "ModeMissionFailed.h"
#include "ModeMissionCompleted.h"
#include "ModeBoss.h"
namespace {
   constexpr auto DefaultScale = 1.0;
   constexpr auto DefaultAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeLoading::ModeLoading(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeLoading::Init() {
   GetLoadJson().LoadTextures("ingame");
   _grHandle = GetResServer().GetTexture("LoadingBg");
   _grHandles = GetResServer().GetTextures("Loading");
}

void ModeLoading::Update() {
   if (!_load) {
      SetUseASyncLoadFlag(true);
      auto& modeServer = GetModeServer();
      modeServer.Register("Poor", std::make_shared<Mode::ModePoor>(_gameMain));
      modeServer.Register("Movie", std::make_shared<Mode::ModeMovie>(_gameMain));
      modeServer.Register("Boss", std::make_shared<Mode::ModeBoss>(_gameMain));
      modeServer.Register("MissionFailed", std::make_shared<Mode::ModeMissionFailed>(_gameMain));
      modeServer.Register("MissionCompleted", std::make_shared<Mode::ModeMissionCompleted>(_gameMain));
      modeServer.Register("GameOver", std::make_shared<Mode::ModeGameOver>(_gameMain));
      modeServer.Register("ClearResult", std::make_shared<Mode::ModeClearResult>(_gameMain));
      _load = true;
   }
   if (GetASyncLoadNum() == 0) {
      GetModeServer().GoToMode("Poor",'L');
   }
}

void ModeLoading::Render() {
   GetTexComponent().DrawTexture(0,0, DefaultScale, DefaultAngle,_grHandle);
   GetTexComponent().DrawTexture(1420, 880, DefaultScale, DefaultAngle, _grHandles,2);
}

void ModeLoading::Exit() {
   SetUseASyncLoadFlag(false);
   GetLoadJson().LoadEffects("ingame");
}