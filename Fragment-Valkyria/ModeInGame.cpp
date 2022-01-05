
/*****************************************************************//**
 * \file   ModeInGame.cpp
 * \brief  モードインゲーム
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeInGame.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "PlayerCreator.h"
#include "StageCreator.h"
#include "LoadJson.h"
using namespace FragmentValkyria::Mode;

ModeInGame::ModeInGame(Game::GameMain& game) : ModeBase(game), _gameMain(game) {
}

void ModeInGame::Init() {
   auto& loadJson = GetLoadJson();
   loadJson.LoadModels("player");
   loadJson.LoadModels("skysphere");
   loadJson.LoadModels("ground");
}

void ModeInGame::Enter() {

   auto& objFac = objFactory();
   objFac.Register("Player", std::make_unique<Create::PlayerCreator>());
   objFac.Register("Stage", std::make_unique<Create::StageCreator>());

   auto player = objFac.Create("Player");
   // アクターサーバーに登録※個別アクセス用
   auto& objSer = objServer();
   objSer.Register("Player", player->position());
   objSer.Add(std::move(player));

   auto stage = objFac.Create("Stage");
   objSer.Add(std::move(stage));

   Update();
}

void ModeInGame::Input(AppFrame::Input::InputManager& input) {
   if (input.GetMouse().RightClick()) {
      // 右クリックでタイトルへ遷移
      GetModeServer().GoToMode("Title", 'L');
   }
   objServer().Input(input);

#ifdef _DEBUG
   _padLeftX = input.GetXJoypad().LeftStickX();
   _padLeftY = input.GetXJoypad().LeftStickY();
   _padRightX = input.GetXJoypad().RightStickX();
   _padRightY = input.GetXJoypad().RightStickY();
#endif
}

void ModeInGame::Update() {
   objServer().Update();
}

void ModeInGame::Render() {
   objServer().Render();
#ifdef _DEBUG
   DrawFormatString(0, 0, GetColor(255, 255, 255), "LeftX:%d LeftY:%d", _padLeftX, _padLeftY);
   DrawFormatString(0, 100, GetColor(255, 255, 255), "RightX:%d RightY:%d", _padRightX, _padRightY);
#endif
  
}

void ModeInGame::Exit() {
   // アクターを削除
   objServer().Clear();
   // デュプリケートしたモデルだけ削除
   GetResServer().DeleteDuplicateModels();
   // クリエイターを削除
   objFactory().Clear();
}
