
/*****************************************************************//**
 * \file   GameMain.cpp
 * \brief  ゲーム本体
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "GameMain.h"
#include "ModeTitle.h"
#include "ModeOption.h"
#include "ModeAmg.h"
#include "ModeTeam.h"
#include "ModeLoading.h"
#include "ModePoor.h"
#include "ModeClearResult.h"
#include "ModeGameOver.h"
#include "ModeMissionFailed.h"
#include "ModeMissionCompleted.h"
#include "ModeBoss.h"
#include "ModeTutorial.h"
#include "ModeTutorialSelect.h"

using namespace FragmentValkyria::Game;

bool GameMain::Initialize(HINSTANCE hInstance) {
   if (!GameBase::Initialize(hInstance)) { return false; }

   _objFactory = std::make_unique<Create::ObjectFactory>();

   _objServer = std::make_unique<Object::ObjectServer>();

   _sprServer = std::make_unique <Sprite::SpriteServer>();

   _efcServer = std::make_unique<Effect::EffectServer>();

   _loadStage = std::make_unique<Stage::LoadStageFromJson>();
   // 各ファイルへのカレントパスをサーバーに登録する
   CurrentPathRegist();
   // 各モード(シーン)をサーバーに登録する
   ModeRegist();

   return true;
}

void GameMain::Run() {
   GameBase::Run();
}

void GameMain::ShutDown() {
   GameBase::ShutDown();
}

void GameMain::Input() {
   GameBase::Input();
}

void GameMain::Update() {
   GameBase::Update();
}

void GameMain::Render() {
   GameBase::Render();
}

void GameMain::CurrentPathRegist(){
   const AppFrame::Path::CurrentPathServer::CurrentPathMap pathToUsed{
      {"Model","resource/model"},
      {"Texture","resource/graphic"},
      {"Sound","resource/sound"},
      {"Effect","resource/effect"},
      {"Stage","resource/stage"},
      {"Movie","resource/movie"},
      {"TextureJson","resource/json/graphic" },
      {"ModelJson","resource/json/model" },
      {"SoundJson","resource/json/sound" },
      {"EffectJson","resource/json/effect"},
      {"StageJson","resource/json/stage"},
      {"SpawnJson","resource/json/spawntable"},
      {"ParamJson","resource/json/param"}
   };
   pathServer().RegistCurrentPath(pathToUsed);
}

void GameMain::ModeRegist() {
   _modeServer = std::make_unique<AppFrame::Mode::ModeServer>("Amg", std::make_shared<Mode::ModeAmg>());
   _modeServer->Register("Team", std::make_shared<Mode::ModeTeam>());
   _modeServer->Register("Title", std::make_shared<Mode::ModeTitle>());
   _modeServer->Register("Option", std::make_shared<Mode::ModeOption>());
   _modeServer->Register("Loading", std::make_shared<Mode::ModeLoading>());
   _modeServer->Register("TutorialSelect", std::make_shared<Mode::ModeTutorialSelect>());
   _loadresJson->LoadEffects("ingame");
   SetUseASyncLoadFlag(true);
   _modeServer->Register("Tutorial", std::make_shared<Mode::ModeTutorial>());
   _modeServer->Register("Poor", std::make_shared<Mode::ModePoor>());
   _modeServer->Register("Boss", std::make_shared<Mode::ModeBoss>());
   _modeServer->Register("MissionFailed", std::make_shared<Mode::ModeMissionFailed>());
   _modeServer->Register("MissionCompleted", std::make_shared<Mode::ModeMissionCompleted>());
   _modeServer->Register("GameOver", std::make_shared<Mode::ModeGameOver>());
   _modeServer->Register("ClearResult", std::make_shared<Mode::ModeClearResult>());
}
