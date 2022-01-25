
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

using namespace FragmentValkyria::Mode;
using namespace FragmentValkyria;

ModeInGameBase::ModeInGameBase(Game::GameMain& gameMain) : ModeBase{gameMain} {
}

void ModeInGameBase::Init() {
   _lighting = std::make_unique<Lighting::LightAndShadow>(_gameMain);
}

void ModeInGameBase::Enter() {
   Update();
}

void ModeInGameBase::Input(AppFrame::Input::InputManager& input) {
   GetObjServer().Input(input);
   GetEfcServer().Input(input);
}

void ModeInGameBase::Update() {
   GetObjServer().Update();
   _lighting->Update();
   GetEfcServer().Update();
}

void ModeInGameBase::Render() {
   _lighting->Render();

   GetObjServer().Render();

   SetUseShadowMap(0, -1);
   SetUseShadowMap(1, -1);
   SetUseShadowMap(2, -1);
   
   GetEfcServer().Render();

#ifdef _DEBUG
   DebugDraw();
#endif
}

void ModeInGameBase::Exit() {
   GetObjServer().Clear();
   GetEfcServer().Clear();
   GetResServer().DeleteDuplicateModels();
   GetObjFactory().Clear();
}

Create::ObjectFactory& ModeInGameBase::GetObjFactory() const {
   return _gameMain.objFactory();
}

#ifdef _DEBUG
void ModeInGameBase::DebugDraw() {
   namespace AppMath = AppFrame::Math;
   using Utility = AppMath::Utility;
   DrawFormatString(0, 0, Utility::GetColorCode(255, 255, 255), "LeftX:%d LeftY:%d", _padLeftX, _padLeftY);
   DrawFormatString(0, 15, Utility::GetColorCode(255, 255, 255), "RightX:%d RightY:%d", _padRightX, _padRightY);
   auto startX = Vector4(-10000.0, 0.0, 0.0);
   auto endX = Vector4(10000.0, 0.0, 0.0);
   DrawLine3D(AppMath::ToDX(startX), AppMath::ToDX(endX), Utility::GetColorCode(255, 0, 0));
   auto startY = Vector4(0.0, -10000.0, 0.0);
   auto endY = Vector4(0.0, 10000.0, 0.0);
   DrawLine3D(AppMath::ToDX(startY), AppMath::ToDX(endY), Utility::GetColorCode(0, 255, 0));
   auto startZ = Vector4(0.0, 0.0, -10000.0);
   auto endZ = Vector4(0.0, 0.0, 10000.0);
   DrawLine3D(AppMath::ToDX(startZ), AppMath::ToDX(endZ), Utility::GetColorCode(0, 0, 255));
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
   DrawFormatString(0, 30, Utility::GetColorCode(255, 255, 255), "LargeEnemyHP:%3.f PlayerHP:%3.f", _largeEnemyHp, _playerHp);
   //プレイヤー情報描画
   auto playerPos = GetObjServer().GetVecData("PlayerPos");
   auto [px, py, pz] = playerPos.GetXYZ();
   DrawFormatString(0, 1030, GetColor(255, 255, 255), "PlyPosition_X:%f  Y:%f  Z:%f ", px, py, pz);
   auto playerRot = GetObjServer().GetVecData("PlayerRot");
   auto [rx, ry, rz] = playerRot.GetXYZ();
   DrawFormatString(0, 1045, GetColor(255, 255, 255), "PlyRotation_X:%f  Y:%f  Z:%f ", rx, ry, rz);
   auto playerFor = GetObjServer().GetVecData("PlayerFor");
   auto [fx, fy, fz] = playerFor.GetXYZ();
   DrawFormatString(0, 1060, GetColor(255, 255, 255), "PlyForward__X:%f  Y:%f  Z:%f ", fx, fy, fz);

}
#endif