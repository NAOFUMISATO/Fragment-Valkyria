
/*****************************************************************//**
 * \file   GameMain.cpp
 * \brief  �Q�[���{��
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "GameMain.h"
#include "ModeTitle.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "LoadStageFromJson.h"
#include "ObjectServer.h"
#include "SpriteServer.h"
#include "EffectServer.h"
#include "ModeOption.h"
#include "ModeAmg.h"
#include "ModeTeam.h"
#include "ModeLoading.h"

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
      {"Movie",{"resource/movie"}},
      {"TextureJson",{"resource/json/graphic" }},
      {"ModelJson",{"resource/json/model" }},
      {"SoundJson",{"resource/json/sound" }},
      {"EffectJson",{"resource/json/effect"}},
      {"StageJson",{"resource/json/stage"}},
      {"SpawnJson",{"resource/json/spawntable"}},
      {"ParamJson",{"resource/json/param"}}
   };

   pathSer.RegistCurrentPath(pathToUsed);

   _objFactory = std::make_unique<Create::ObjectFactory>(*this);

   _objServer = std::make_unique<Object::ObjectServer>();

   _sprServer = std::make_unique <Sprite::SpriteServer>();

   _efcServer = std::make_unique<Effect::EffectServer>();

   _loadStage = std::make_unique<Stage::LoadStageFromJson>(*this);

   _modeServer = std::make_unique<AppFrame::Mode::ModeServer>("Amg", std::make_shared<Mode::ModeAmg>(*this));
   _modeServer->Register("Team", std::make_shared<Mode::ModeTeam>(*this));
   _modeServer->Register("Title", std::make_shared<Mode::ModeTitle>(*this));
   _modeServer->Register("Option", std::make_shared<Mode::ModeOption>(*this));
   _modeServer->Register("Loading", std::make_shared<Mode::ModeLoading>(*this));
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
