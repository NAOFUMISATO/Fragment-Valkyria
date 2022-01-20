
/*****************************************************************//**
 * \file   ModeInGame.cpp
 * \brief  モードインゲーム
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeInGame.h"
#include "FallObjectCreator.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "LargeEnemyCreator.h"
#include "PlayerCreator.h"
#include "BossStageCreator.h"
#include "GatlingCreator.h"
#include "ObjectBase.h"
#include "LoadStageFromJson.h"
#include "EffectPlayerShot.h" //仮
using namespace FragmentValkyria::Mode;

ModeInGame::ModeInGame(Game::GameMain& gameMain) : ModeBase{ gameMain } {

}

void ModeInGame::Init() {
   auto& loadJson = GetLoadJson();
   loadJson.LoadModels("ingame");
   loadJson.LoadEffects("player");
   loadJson.LoadTextures("ingame");

   auto& loadStage = _gameMain.loadStage();
   loadStage.LoadStageModels("Boss");
}

void ModeInGame::Enter() {

   auto& objFac = objFactory();
   objFac.Register("LargeEnemy", std::make_unique<Create::LargeEnemyCreator>(_gameMain));
   objFac.Register("Player", std::make_unique<Create::PlayerCreator>(_gameMain));
   objFac.Register("Stage", std::make_unique<Create::BossStageCreator>(_gameMain));
   objFac.Register("FallObject", std::make_unique<Create::FallObjectCreator>(_gameMain));
   objFac.Register("Gatling", std::make_unique<Create::GatlingCreator>(_gameMain));

   auto player = objFac.Create("Player");
   // アクターサーバーに登録※個別アクセス用
   auto& objSer = GetObjServer();
   objSer.Register("Player", player->position());
   objSer.Add(std::move(player));

   auto stage = objFac.Create("Stage");
   objSer.Add(std::move(stage));

   auto largeEnemy = objFac.Create("LargeEnemy");
   objSer.Add(std::move(largeEnemy));

   Update();
}

void ModeInGame::Input(AppFrame::Input::InputManager& input) {
   if (input.GetMouse().RightClick()) {
      // 右クリックでタイトルへ遷移
      GetModeServer().GoToMode("Title", 'L');
   }
   GetObjServer().Input(input);

   //エフェクト仮描画
   if (input.GetKeyboard().ZClick()) {
      auto efcShot = std::make_unique<Effect::EffectPlayerShot>(_gameMain);
      GetEfcServer().Add(std::move(efcShot));
   }
   GetEfcServer().Input(input);
#ifdef _DEBUG
   _padLeftX = input.GetXJoypad().LeftStickX();
   _padLeftY = input.GetXJoypad().LeftStickY();
   _padRightX = input.GetXJoypad().RightStickX();
   _padRightY = input.GetXJoypad().RightStickY();
#endif
}

void ModeInGame::Update() {
   GetObjServer().Update();
   GetEfcServer().Update();
}

void ModeInGame::Render() {
   GetObjServer().Render();
   GetEfcServer().Render();
#ifdef _DEBUG
   DrawFormatString(0, 0, GetColor(255, 255, 255), "LeftX:%d LeftY:%d", _padLeftX, _padLeftY);
   DrawFormatString(0, 15, GetColor(255, 255, 255), "RightX:%d RightY:%d", _padRightX, _padRightY);
   using Vector4 = AppFrame::Math::Vector4;
   namespace AppMath = AppFrame::Math;
   auto startX = Vector4(-10000.0, 0.0, 0.0);
   auto endX = Vector4(10000.0, 0.0, 0.0);
   DrawLine3D(AppMath::ToDX(startX), AppMath::ToDX(endX), GetColor(255, 0, 0));
   auto startY = Vector4(0.0, -10000.0, 0.0);
   auto endY = Vector4(0.0, 10000.0, 0.0);
   DrawLine3D(AppMath::ToDX(startY), AppMath::ToDX(endY), GetColor(0, 255, 0));
   auto startZ = Vector4(0.0, 0.0, -10000.0);
   auto endZ = Vector4(0.0, 0.0, 10000.0);
   DrawLine3D(AppMath::ToDX(startZ), AppMath::ToDX(endZ), GetColor(0, 0, 255));
   auto camTarget = GetObjServer().GetPosition("CamTarget");
   auto targetStartX = camTarget + Vector4(-10.0, 0.0, 0.0);
   auto targetEndX = camTarget + Vector4(10.0, 0.0, 0.0);
   DrawLine3D(AppMath::ToDX(targetStartX), AppMath::ToDX(targetEndX), GetColor(255, 0, 0));
   auto targetStartY = camTarget + Vector4(0.0, -10.0, 0.0);
   auto targetEndY = camTarget + Vector4(0.0, 10.0, 0.0);
   DrawLine3D(AppMath::ToDX(targetStartY), AppMath::ToDX(targetEndY), GetColor(0, 255, 0));
   auto targetStartZ = camTarget + Vector4(0.0, 0.0, -10.0);
   auto targetEndZ = camTarget + Vector4(10.0, 0.0, 10.0);
   DrawLine3D(AppMath::ToDX(targetStartZ), AppMath::ToDX(targetEndZ), GetColor(0, 0, 255));
#endif
  
}

void ModeInGame::Exit() {
   // アクターを削除
   GetObjServer().Clear();
   GetEfcServer().Clear();
   // デュプリケートしたモデルだけ削除
   GetResServer().DeleteDuplicateModels();
   // クリエイターを削除
   objFactory().Clear();
}
