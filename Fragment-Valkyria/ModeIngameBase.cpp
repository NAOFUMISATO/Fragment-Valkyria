
/*****************************************************************//**
 * \file   ModeIngameBase.cpp
 * \brief  äeÉCÉìÉQÅ[ÉÄÇÃäÓíÍ
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeInGameBase.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "LoadStageFromJson.h"

using namespace FragmentValkyria::Mode;
using namespace FragmentValkyria;

ModeInGameBase::ModeInGameBase(Game::GameMain& gameMain) : ModeBase{gameMain} {
}

void ModeInGameBase::Init() {

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
   GetEfcServer().Update();
}

void ModeInGameBase::Render() {
   GetObjServer().Render();
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
   _gameMain.loadStage().ClearStageModels();
}

Create::ObjectFactory& ModeInGameBase::GetObjFactory() const {
   return _gameMain.objFactory();
}

#ifdef _DEBUG
void ModeInGameBase::DebugDraw() {
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
   auto camTarget = GetObjServer().GetVecData("CamTarget");
   auto targetStartX = camTarget + Vector4(-10.0, 0.0, 0.0);
   auto targetEndX = camTarget + Vector4(10.0, 0.0, 0.0);
   DrawLine3D(AppMath::ToDX(targetStartX), AppMath::ToDX(targetEndX), GetColor(255, 0, 0));
   auto targetStartY = camTarget + Vector4(0.0, -10.0, 0.0);
   auto targetEndY = camTarget + Vector4(0.0, 10.0, 0.0);
   DrawLine3D(AppMath::ToDX(targetStartY), AppMath::ToDX(targetEndY), GetColor(0, 255, 0));
   auto targetStartZ = camTarget + Vector4(0.0, 0.0, -10.0);
   auto targetEndZ = camTarget + Vector4(10.0, 0.0, 10.0);
   DrawLine3D(AppMath::ToDX(targetStartZ), AppMath::ToDX(targetEndZ), GetColor(0, 0, 255));
   DrawFormatString(0, 30, GetColor(255, 255, 255), "LargeEnemyHP:%3.f PlayerHP:%3.f", _largeEnemyHp, _playerHp);
}
#endif