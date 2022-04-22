
/*****************************************************************//**
 * \file   ModeIngameBase.cpp
 * \brief  各インゲームの基底
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeInGameBase.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "LoadStageFromJson.h"
#include "ObjectBase.h"
#include "ModelAnimeComponent.h"
#include "LightAndShadow.h"
#include "SpriteBase.h"
#include "SpriteServer.h"
#include "Stage.h"
#include "EffectServer.h"
#include "Gatling.h"
#include "Bullet.h"

using namespace FragmentValkyria::Mode;
using namespace FragmentValkyria;

ModeInGameBase::ModeInGameBase() {
}

void ModeInGameBase::Enter() {
   _lighting = std::make_unique<Lighting::LightAndShadow>();
   _stage = std::make_unique<Stage::Stage>();
   Update();
}

void ModeInGameBase::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().StartClick()) {
      GetModeServer().PushBack("Option");
   }
   GetObjServer().Input(input);
   GetEfcServer().Input(input);
   GetSprServer().Input(input);
#ifdef _DEBUG
   _padLeftX = input.GetXJoypad().LeftStickX();
   _padLeftY = input.GetXJoypad().LeftStickY();
   _padRightX = input.GetXJoypad().RightStickX();
   _padRightY = input.GetXJoypad().RightStickY();
#endif
}

void ModeInGameBase::Update() {
   GetObjFactory().UpdateSpawn();
   GetObjServer().Update();
   _lighting->Update();
   _stage->Update();
   GetEfcServer().Update();
   GetSprServer().Update();
   auto gameInstance = Game::GameMain::GetInstance();
   gameInstance->IngameTimerCount();
}

void ModeInGameBase::Render() {
   _lighting->Render();
   _stage->Draw();
   GetObjServer().Render();

   SetUseShadowMap(0, -1);

   GetEfcServer().Render();
   GetSprServer().Render();

#ifdef _DEBUG
   DebugDraw();
#endif
}

void ModeInGameBase::Exit() {
   IndividualEffectClear();
   GetObjServer().Clear();
   GetEfcServer().Clear();
   GetSprServer().Clear();
   GetResServer().ClearModels();
   GetObjFactory().Clear();
   DeleteLightHandleAll();
}

Create::ObjectFactory& ModeInGameBase::GetObjFactory() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->objFactory();
}

void ModeInGameBase::IndividualEffectClear() {
   for (auto& object : GetObjServer().runObjects()) {
      using enum Object::ObjectBase::ObjectType;
      if (object->GetObjType() == Gatling) {
         auto& gatling = dynamic_cast<Enemy::Gatling&>(*object);
         auto& efcGatling = gatling.efcBullet();
         efcGatling.StopEffect();
         continue;
      }
      if (object->GetObjType() == Bullet) {
         auto& bullet = dynamic_cast<Player::Bullet&>(*object);
         auto& efcBullet = bullet.efcBullet();
         efcBullet.StopEffect();
      }
   }
}

#ifdef _DEBUG
void ModeInGameBase::DebugDraw() {
   namespace AppMath = AppFrame::Math;
   using Utility = AppMath::Utility;
   // プレイヤー情報
   auto playerHp = GetObjServer().GetDoubleData("PlayerHP");
   DrawFormatString(0, 960, GetColor(255, 255, 255), "プレイヤーHP : %f", playerHp);
   auto playerPos = GetObjServer().GetVecData("PlayerPos");
   auto [px, py, pz] = playerPos.GetVec3();
   DrawFormatString(0, 980, GetColor(255, 255, 255), "プレイヤー位置_X:%f  Y:%f  Z:%f ", px, py, pz);
   auto playerRot = GetObjServer().GetVecData("PlayerRot");
   auto [rx, ry, rz] = playerRot.GetVec3();
   DrawFormatString(0, 1000, GetColor(255, 255, 255), "プレイヤー回転_X:%f  Y:%f  Z:%f ", rx, ry, rz);
   auto playerFor = GetObjServer().GetVecData("PlayerFor");
   auto [fx, fy, fz] = playerFor.GetVec3();
   DrawFormatString(0, 1020, GetColor(255, 255, 255), "プレイヤー向き_X:%f  Y:%f  Z:%f ", fx, fy, fz);
   // スティック入力値
   DrawFormatString(0, 1040, Utility::GetColorCode(255, 255, 255), "左スティック_X : %d  Y : %d", _padLeftX, _padLeftY);
   DrawFormatString(0, 1060, Utility::GetColorCode(255, 255, 255), "右スティック_X : %d  Y : %d", _padRightX, _padRightY);
   // XYZ原点軸
   auto startX = Vector4(-10000.0, 0.0, 0.0);
   auto endX = Vector4(10000.0, 0.0, 0.0);
   DrawLine3D(AppMath::ToDX(startX), AppMath::ToDX(endX), Utility::GetColorCode(255, 0, 0));
   auto startY = Vector4(0.0, -10000.0, 0.0);
   auto endY = Vector4(0.0, 10000.0, 0.0);
   DrawLine3D(AppMath::ToDX(startY), AppMath::ToDX(endY), Utility::GetColorCode(0, 255, 0));
   auto startZ = Vector4(0.0, 0.0, -10000.0);
   auto endZ = Vector4(0.0, 0.0, 10000.0);
   DrawLine3D(AppMath::ToDX(startZ), AppMath::ToDX(endZ), Utility::GetColorCode(0, 0, 255));
   // カメラ注視点
   auto camTarget = GetObjServer().GetVecData("CamTarget");
   auto targetStartX = camTarget + Vector4(-10.0, 0.0, 0.0);
   auto targetEndX = camTarget + Vector4(10.0, 0.0, 0.0);
   DrawLine3D(AppMath::ToDX(targetStartX), AppMath::ToDX(targetEndX), Utility::GetColorCode(255, 0, 0));
   auto targetStartY = camTarget + Vector4(0.0, -10.0, 0.0);
   auto targetEndY = camTarget + Vector4(0.0, 10.0, 0.0);
   DrawLine3D(AppMath::ToDX(targetStartY), AppMath::ToDX(targetEndY), Utility::GetColorCode(0, 255, 0));
   auto targetStartZ = camTarget + Vector4(0.0, 0.0, -10.0);
   auto targetEndZ = camTarget + Vector4(10.0, 0.0, 10.0);
   DrawLine3D(AppMath::ToDX(targetStartZ), AppMath::ToDX(targetEndZ), Utility::GetColorCode(0, 0, 255));
}
#endif
