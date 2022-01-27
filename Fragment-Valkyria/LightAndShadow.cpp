#include "LightAndShadow.h"
#include "GameMain.h"

namespace {
   constexpr auto LightDiffY = 100.0;
}

using namespace FragmentValkyria::Lighting;

LightAndShadow::LightAndShadow(Game::GameMain& gameMain) :_gameMain{gameMain} {
   Init();
}

void LightAndShadow::Init() {
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

   auto followLightPos = _gameMain.objServer().GetVecData("PlayerHeadPos");
   followLightPos.SetY(followLightPos.GetY() + 300.0);
   _lightPositions = { 
      {-3000.0,3000.0,0},
      followLightPos
   };

   auto [firstLightPos, secondLightPos] = _lightPositions;
   
   _lightHandles = {
      CreatePointLightHandle(AppMath::ToDX(firstLightPos), 10000.f, 0.00000001f, 0.0000005f, 0.0000001f),
      CreatePointLightHandle(AppMath::ToDX(secondLightPos),5000.f, 0.00001f, 0.00005f, 0.00001f)
   };

   auto [firstLightHandle, secondLightHandle] = _lightHandles;

   SetLightDifColorHandle(firstLightHandle, GetColorF(0.5f, 1.0f, 0.5f, 1.0f));
   SetLightDifColorHandle(secondLightHandle, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));

   _shadowHandle = MakeShadowMap(8192, 8192);

   auto shadowMinArea = Vector4(-2500.0, -1.0, -2500.0);
   auto shadowMaxArea = Vector4(2000.0, 1000.0, 2000.0);

   SetShadowMapDrawArea(_shadowHandle, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
}

void LightAndShadow::Update() {
   namespace AppMath = AppFrame::Math;

   auto plyHeadPos= _gameMain.objServer().GetVecData("PlayerHeadPos");
   plyHeadPos.SetY(plyHeadPos.GetY() + 300.0);
   
   _lightPositions = {
      {-3000,3000.0,0},
      {plyHeadPos}
   };

   auto [firstLightPos, secondLightPos] = _lightPositions;
   auto [firstLightHandle, secondLightHandle] = _lightHandles;

   SetLightPositionHandle(secondLightHandle, AppFrame::Math::ToDX(secondLightPos));

   auto shadowDirection = (plyHeadPos - firstLightPos).Normalize();

   SetShadowMapLightDirection(_shadowHandle, AppMath::ToDX(shadowDirection));
}

void LightAndShadow::Render() {

   ShadowMap_DrawSetup(_shadowHandle);
   _gameMain.objServer().Render();
   ShadowMap_DrawEnd();

   SetUseShadowMap(0, _shadowHandle);

#ifdef _DEBUG
   //ライト描画
   namespace AppMath = AppFrame::Math;
   using Utility = AppFrame::Math::Utility;
   auto [firstLightPos, secondLightPos] = _lightPositions;
   firstLightPos.SetY(firstLightPos.GetY() + LightDiffY);
   secondLightPos.SetY(secondLightPos.GetY() + LightDiffY);
   DrawSphere3D(AppMath::ToDX(firstLightPos), 80.f, 32,
      Utility::GetColorCode(255, 0, 0), Utility::GetColorCode(255, 0, 0), TRUE);
   DrawSphere3D(AppMath::ToDX(secondLightPos), 80.f, 32,
      Utility::GetColorCode(0, 255, 0), Utility::GetColorCode(0, 255, 0), TRUE);
#endif
}

