#include "LightAndShadow.h"
#include "GameMain.h"

namespace {
   constexpr auto LightDiffY = 500;
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

   _lightPositions = {
      {-2200.0,3000.0,-2300.0},
      {0, 3000.0, 3000.0},
      {3000.0, 3000.0, 0}
   };

   auto [firstLightPos, secondLightPos, thirdLightPos] = _lightPositions;
   auto& first = std::get<0>(_lightHandles);
   first = CreateSpotLightHandle(AppMath::ToDX(firstLightPos), { 0.0f,-1.0f,0.0f }, DX_PI_F / 3.0f, DX_PI_F / 6.0f,1000.f, 0.0001f, 0.00006f, 0.000001f);
   //_lightHandles = {
   //   CreatePointLightHandle(AppMath::ToDX(firstLightPos),1000.f, 0.0001f, 0.00002f, 0.00001f),
   //   CreatePointLightHandle(AppMath::ToDX(secondLightPos), 10000.f, 0.0000001f, 0.0002f, 0.0000001f),
   //   CreatePointLightHandle(AppMath::ToDX(thirdLightPos), 10000.f, 0.0000001f, 0.0002f, 0.0000001f)
   //};

   auto [firstLightHandle, secondLightHandle, thirdLightHandle] = _lightHandles;

   //SetLightDifColorHandle(firstLightHandle, GetColorF(1.0f, 0.5f, 0.5f, 1.0f));
   SetLightDifColorHandle(secondLightHandle, GetColorF(0.5f, 1.0f, 0.5f, 1.0f));
   SetLightDifColorHandle(thirdLightHandle, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));

   _shadowHandles = {
      MakeShadowMap(8192, 8192),
      MakeShadowMap(8192, 8192),
      MakeShadowMap(8192, 8192)
   };

   auto shadowMinArea = Vector4(-2500.0, -1.0, -2500.0);
   auto shadowMaxArea = Vector4(2000.0, 1000.0, 2000.0);

   auto [firstShadowHandle, secondShadowHandle, thirdShadowHandle] = _shadowHandles;

   SetShadowMapDrawArea(firstShadowHandle, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
   SetShadowMapDrawArea(secondShadowHandle, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
   SetShadowMapDrawArea(thirdShadowHandle, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
}

void LightAndShadow::Update() {
   namespace AppMath = AppFrame::Math;
   auto plyHeadPos = _gameMain.objServer().GetVecData("PlayerHeadPos");
   //
   auto [firstLightHandle, secondLightHandle, thirdLightHandle] = _lightHandles;
   auto [hx,hy,hz] = plyHeadPos.GetXYZ();
   auto lightPos = Vector4(hx, hy + 300.0,hz);
   SetLightPositionHandle(firstLightHandle, AppFrame::Math::ToDX(lightPos));
   //
   auto [firstLightPos, secondLightPos, thirdLightPos] = _lightPositions;
   
   auto firstShadowDirection = (plyHeadPos - firstLightPos).Normalize();
   auto secondShadowDirection = (plyHeadPos - secondLightPos).Normalize();
   auto thirdShadowDirection = (plyHeadPos - thirdLightPos).Normalize();

   auto [firstShadowHandle, secondShadowHandle, thirdShadowHandle] = _shadowHandles;

   SetShadowMapLightDirection(firstShadowHandle, AppMath::ToDX(firstShadowDirection));
   SetShadowMapLightDirection(secondShadowHandle, AppMath::ToDX(secondShadowDirection));
   SetShadowMapLightDirection(thirdShadowHandle, AppMath::ToDX(thirdShadowDirection));
}

void LightAndShadow::Render() {
   auto [firstShadowHandle, secondShadowHandle, thirdShadowHandle] = _shadowHandles;
   //ShadowMap_DrawSetup(firstShadowHandle);
   //_gameMain.objServer().Render();
   //ShadowMap_DrawEnd();

   //ShadowMap_DrawSetup(secondShadowHandle);
   //_gameMain.objServer().Render();
   //ShadowMap_DrawEnd();
   //
   //ShadowMap_DrawSetup(thirdShadowHandle);
   //_gameMain.objServer().Render();
   //ShadowMap_DrawEnd();

   //SetUseShadowMap(0, firstShadowHandle);
   //SetUseShadowMap(1, secondShadowHandle);
   //SetUseShadowMap(2, thirdShadowHandle);

#ifdef _DEBUG
   //ライト描画
   namespace AppMath = AppFrame::Math;
   using Utility = AppFrame::Math::Utility;
   auto [firstLightPos, secondLightPos, thirdLightPos] = _lightPositions;
   firstLightPos.SetY(firstLightPos.GetY() + LightDiffY);
   secondLightPos.SetY(secondLightPos.GetY() + LightDiffY);
   thirdLightPos.SetY(thirdLightPos.GetY() + LightDiffY);
   DrawSphere3D(AppMath::ToDX(firstLightPos), 80.f, 32,
      Utility::GetColorCode(255, 0, 0), Utility::GetColorCode(255, 0, 0), TRUE);
   DrawSphere3D(AppMath::ToDX(secondLightPos), 80.f, 32,
      Utility::GetColorCode(0, 255, 0), Utility::GetColorCode(0, 255, 0), TRUE);
   DrawSphere3D(AppMath::ToDX(thirdLightPos), 80.f, 32,
      Utility::GetColorCode(0, 0, 255), Utility::GetColorCode(0, 0, 255), TRUE);
#endif
}

