
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

namespace {
   constexpr auto LightDiffY = 500;
}
using namespace FragmentValkyria::Mode;
using namespace FragmentValkyria;

ModeInGameBase::ModeInGameBase(Game::GameMain& gameMain) : ModeBase{gameMain} {
}

void ModeInGameBase::Init() {
   namespace AppMath = AppFrame::Math;
   // マテリアルの自己発光色設定
   MATERIALPARAM material;
   material.Diffuse = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);
   material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
   material.Ambient = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
   material.Emissive = GetColorF(0.0f, 0.0f, 0.5f, 0.0f);
   material.Power = 20.0f;
   SetMaterialParam(material);

   SetLightEnable(false);

   auto pointLightFirstPos = Vector4(-2200.0,3000.0,-2300);
   auto pointLightSecondPos = Vector4(0, 3000.0, 3000.0);
   auto pointLightThirdPos = Vector4(3000.0, 3000.0, 0);
#ifdef _DEBUG
   _lightFirstPos = pointLightFirstPos;
   _lightSecondPos = pointLightSecondPos;
   _lightThirdPos = pointLightThirdPos;
   _lightFirstPos.SetY(_lightFirstPos.GetY() + LightDiffY);
   _lightSecondPos.SetY(_lightSecondPos.GetY() + LightDiffY);
   _lightThirdPos.SetY(_lightThirdPos.GetY() + LightDiffY);
#endif
   _lightHandleFirst = CreatePointLightHandle(AppMath::ToDX(pointLightFirstPos),10000.f, 0.0000001f, 0.0006f, 0.0000001f);
   _lightHandleSecond = CreatePointLightHandle(AppMath::ToDX(pointLightSecondPos), 10000.f, 0.0000001f, 0.0006f, 0.0000001f);
   _lightHandleThird = CreatePointLightHandle(AppMath::ToDX(pointLightThirdPos), 10000.f, 0.0000001f, 0.0006f, 0.0000001f);
   SetLightDifColorHandle(_lightHandleFirst, GetColorF(1.0f, 0.5f, 0.5f, 1.0f));
   SetLightDifColorHandle(_lightHandleSecond, GetColorF(0.5f,1.0f,0.5f,1.0f));
   SetLightDifColorHandle(_lightHandleThird, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
   //SetLightSpcColorHandle(_lightHandleSecond, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

   _shadowMapHandleFirst = MakeShadowMap(8192, 8192);
   _shadowMapHandleSecond = MakeShadowMap(8192, 8192);
   _shadowMapHandleThird = MakeShadowMap(8192, 8192);

   auto shadowFirstDirection = Vector4(0.5, -0.5, 0.5);
   auto shadowSecondDirection = Vector4(0.0, -1.0, -1.0);
   auto shadowThirdDirection = Vector4(-1.0, -1.0, 0.0);

   SetShadowMapLightDirection(_shadowMapHandleFirst, AppMath::ToDX(shadowFirstDirection));
   SetShadowMapLightDirection(_shadowMapHandleSecond, AppMath::ToDX(shadowSecondDirection));
   SetShadowMapLightDirection(_shadowMapHandleThird, AppMath::ToDX(shadowThirdDirection));


   auto shadowMinArea = Vector4(-2500.0, -1.0, -2500.0);
   auto shadowMaxArea = Vector4(2000.0, 1000.0, 2000.0);

   SetShadowMapDrawArea(_shadowMapHandleFirst, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
   SetShadowMapDrawArea(_shadowMapHandleSecond, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
   SetShadowMapDrawArea(_shadowMapHandleThird, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
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
   ShadowMap_DrawSetup(_shadowMapHandleFirst);
   GetObjServer().Render();
   ShadowMap_DrawEnd();

   ShadowMap_DrawSetup(_shadowMapHandleSecond);
   GetObjServer().Render();
   ShadowMap_DrawEnd();

   ShadowMap_DrawSetup(_shadowMapHandleThird);
   GetObjServer().Render();
   ShadowMap_DrawEnd();
   SetUseShadowMap(0, _shadowMapHandleFirst);
   SetUseShadowMap(1, _shadowMapHandleSecond);
   SetUseShadowMap(2, _shadowMapHandleThird);
   GetObjServer().Render();
   SetUseShadowMap(0, -1);
   SetUseShadowMap(1, -1);
   SetUseShadowMap(2, -1);
   //TestDrawShadowMap(_shadowMapHandleFirst, 0, 0, 480, 270);
   //TestDrawShadowMap(_shadowMapHandleSecond, 500, 0, 480, 270);
   //TestDrawShadowMap(_shadowMapHandleThird, 1000, 0, 480, 270);
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
   DrawFormatString(0, 0, GetColor(255, 255, 255), "LeftX:%d LeftY:%d", _padLeftX, _padLeftY);
   DrawFormatString(0, 15, GetColor(255, 255, 255), "RightX:%d RightY:%d", _padRightX, _padRightY);
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
   //プレイヤー座標描画
   auto playerPos = GetObjServer().GetVecData("PlayerPos");
   auto [px, py, pz] = playerPos.GetXYZ();
   DrawFormatString(0, 1060, GetColor(255, 255, 255), "Player_X:%3.f Y:%3.f Z:%3.f", px, py, pz);
   //ライト描画
   DrawSphere3D(AppMath::ToDX(_lightFirstPos), 80.f, 32,
      AppMath::Utility::GetColorCode(255, 0, 0), AppMath::Utility::GetColorCode(255, 0, 0),TRUE);
   DrawSphere3D(AppMath::ToDX(_lightSecondPos), 80.f, 32,
      AppMath::Utility::GetColorCode(0, 255, 0), AppMath::Utility::GetColorCode(0, 255, 0), TRUE);
   DrawSphere3D(AppMath::ToDX(_lightThirdPos), 80.f, 32,
      AppMath::Utility::GetColorCode(0, 0, 255), AppMath::Utility::GetColorCode(0, 0, 255), TRUE);
}
#endif