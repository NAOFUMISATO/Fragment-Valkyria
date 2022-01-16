
/*****************************************************************//**
 * \file   GameMain.cpp
 * \brief  ゲーム本体
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "GameMain.h"
#include "ModeTitle.h"
#include "ModeInGame.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "LoadStageFromJson.h"
#include "AppFrame.h"

using namespace FragmentValkyria::Game;

GameMain gameMain;

const std::filesystem::path AppFrame::Resource::LoadJson::_paramJsonCurrentPath = "resource/json/param";

bool GameMain::Initialize(HINSTANCE hInstance) {
   if (!GameBase::Initialize(hInstance)) { return false; }

   // マテリアルの自己発光色を暗い青色にする
#ifdef _DEBUG
   MATERIALPARAM material;
   material.Diffuse = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);
   material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
   material.Ambient = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
   material.Emissive = GetColorF(0.0f, 0.0f, 0.5f, 0.0f);
   material.Power = 20.0f;
   SetMaterialParam(material);
#endif

   _objFactory = std::make_unique<Create::ObjectFactory>(*this);

   _loadStage = std::make_unique<Stage::LoadStageFromJson>(*this);

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

   _modeServer = std::make_unique<AppFrame::Mode::ModeServer>("Title", std::make_shared<Mode::ModeTitle>(*this));
   _modeServer->Register("InGame", std::make_shared<Mode::ModeInGame>(*this));

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
