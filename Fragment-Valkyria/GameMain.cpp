
/*****************************************************************//**
 * \file   GameMain.cpp
 * \brief  ÉQÅ[ÉÄñ{ëÃ
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "GameMain.h"
#include "ModeTitle.h"
#include "ModeBoss.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "LoadStageFromJson.h"
#include "AppFrame.h"
#include "ModeClearResult.h"
#include "ModeGameOver.h"

using namespace FragmentValkyria::Game;

GameMain gameMain;

bool GameMain::Initialize(HINSTANCE hInstance) {
   if (!GameBase::Initialize(hInstance)) { return false; }

   auto& pathSer = pathServer();

   const AppFrame::Path::CurrentPathServer::CurrentPathMap pathToUsed{
      {"Model",{"resource/model"}},
      {"Texture",{"resource/graphic"}},
      {"Sound",{"resource/sound"}},
      {"Effect",{"resource/effect"}},
      {"Stage",{"resource/stage"}},
      {"TextureJson",{"resource/json/graphic" }},
      {"ModelJson",{"resource/json/model" }},
      {"SoundJson",{"resource/json/sound" }},
      {"EffectJson",{"resource/json/effect"}},
      {"StageJson",{"resource/json/stage"}},
      {"ParamJson",{"resource/json/param"}}
   };

   pathSer.RegistCurrentPath(pathToUsed);

   _objFactory = std::make_unique<Create::ObjectFactory>(*this);

   _loadStage = std::make_unique<Stage::LoadStageFromJson>(*this);

   _modeServer = std::make_unique<AppFrame::Mode::ModeServer>("Title", std::make_shared<Mode::ModeTitle>(*this));

   _modeServer->Register("Boss", std::make_shared<Mode::ModeBoss>(*this));
   _modeServer->Register("ClearResult", std::make_shared<Mode::ModeClearResult>(*this));
   _modeServer->Register("GameOver", std::make_shared<Mode::ModeGameOver>(*this));
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
