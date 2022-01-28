#include "LightAndShadow.h"
#include "GameMain.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("lightshadow", { "followlight_diff_y" ,"fixedlight_area" ,"fixedlight_atten_first" ,
   "fixedlight_atten_second","fixedlight_atten_third","followlight_area","followlight_atten_first" ,"followlight_atten_second" ,
     "followlight_atten_third","fixedlight_red","fixedlight_green","fixedlight_blue","fixedlight_alpha","followlight_red" ,"followlight_green" ,
     "followlight_blue","followlight_alpha","shadow_resolution" });
   const double FollowLightDiffY = paramMap["followlight_diff_y"];
   const float FixedLightArea = paramMap["fixedlight_area"];
   const float FixedLightAttenFirst = paramMap["fixedlight_atten_first"];
   const float FixedLightAttenSecond = paramMap["fixedlight_atten_second"];
   const float FixedLightAttenThird= paramMap["fixedlight_atten_third"];
   const float FollowLightArea = paramMap["followlight_area"];
   const float FollowLightAttenFirst = paramMap["followlight_atten_first"];
   const float FollowLightAttenSecond = paramMap["followlight_atten_second"];
   const float FollowLightAttenThird = paramMap["followlight_atten_third"];
   const float FixedLightRed = paramMap["fixedlight_red"];
   const float FixedLightGreen = paramMap["fixedlight_green"];
   const float FixedLightBlue = paramMap["fixedlight_blue"];
   const float FixedLightAlpha = paramMap["fixedlight_alpha"];
   const float FollowLightRed = paramMap["followlight_red"];
   const float FollowLightGreen = paramMap["followlight_green"];
   const float FollowLightBlue = paramMap["followlight_blue"];
   const float FollowLightAlpha = paramMap["followlight_alpha"];
   const int ShadowResolution = paramMap["shadow_resolution"];
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("lightshadow", { "fixedlight_pos" ,"shaodw_minarea" ,"shaodw_maxarea" });
   const auto FixedLightPos = vecParamMap["fixedlight_pos"];
   const auto ShadowMinArea = vecParamMap["shaodw_minarea"];
   const auto ShadowMaxArea = vecParamMap["shaodw_maxarea"];

#ifdef _DEBUG
   constexpr auto MatDifRed = 0.0f;
   constexpr auto MatDifGreen = 0.0f;
   constexpr auto MatDifBlue = 0.0f;
   constexpr auto MatDifAlpha = 1.0f;
   constexpr auto MatSpeRed = 0.0f;
   constexpr auto MatSpeGreen = 0.0f;
   constexpr auto MatSpeBlue = 0.0f;
   constexpr auto MatSpeAlpha = 0.0f;
   constexpr auto MatAmbRed = 0.0f;
   constexpr auto MatAmbGreen = 0.0f;
   constexpr auto MatAmbBlue = 0.0f;
   constexpr auto MatAmbAlpha = 0.0f;
   constexpr auto MatEmiRed = 0.0f;
   constexpr auto MatEmiGreen = 0.0f;
   constexpr auto MatEmiBlue = 0.5f;
   constexpr auto MatEmiAlpha = 0.0f;
   constexpr auto MatPower = 20.0f;
   constexpr auto LightSphereDiffY = 100.0;
   constexpr auto LightSphereRadius = 80.0f;
   constexpr auto LightSphereDiv = 32;
   constexpr auto LightSphereRed = 0;
   constexpr auto LightSphereGreen = 128;
   constexpr auto LightSphereBlue = 200;
#endif
}

using namespace FragmentValkyria::Lighting;

LightAndShadow::LightAndShadow(Game::GameMain& gameMain) :_gameMain{gameMain} {
   Init();
}

void LightAndShadow::Init() {
   namespace AppMath = AppFrame::Math;
#ifdef _DEBUG
   // マテリアルの自己発光色設定
   MATERIALPARAM material;
   material.Diffuse = GetColorF(MatDifRed, MatDifGreen, MatDifBlue, MatDifAlpha);
   material.Specular = GetColorF(MatSpeRed, MatSpeGreen, MatSpeBlue, MatSpeAlpha);
   material.Ambient = GetColorF(MatAmbRed, MatAmbGreen, MatAmbBlue, MatAmbAlpha);
   material.Emissive = GetColorF(MatEmiRed, MatEmiGreen, MatEmiBlue, MatEmiAlpha);
   material.Power = MatPower;
   SetMaterialParam(material);
#endif

   SetLightEnable(false);
   auto followLightPos = _gameMain.objServer().GetVecData("PlayerHeadPos");
   followLightPos.SetY(followLightPos.GetY() + FollowLightDiffY);
   _lightPositions = { 
      FixedLightPos,
      followLightPos
   };

   auto [firstLightPos, secondLightPos] = _lightPositions;
   
   _lightHandles = {
      CreatePointLightHandle(AppMath::ToDX(firstLightPos), FixedLightArea,FixedLightAttenFirst, FixedLightAttenSecond, FixedLightAttenThird),
      CreatePointLightHandle(AppMath::ToDX(secondLightPos),FollowLightArea, FollowLightAttenFirst, FollowLightAttenSecond, FollowLightAttenThird)
   };

   auto [firstLightHandle, secondLightHandle] = _lightHandles;

   SetLightDifColorHandle(firstLightHandle, GetColorF(FixedLightRed, FixedLightGreen, FixedLightBlue, FixedLightAlpha));
   SetLightDifColorHandle(secondLightHandle, GetColorF(FollowLightRed, FollowLightGreen, FollowLightBlue, FollowLightAlpha));

   _shadowHandle = MakeShadowMap(ShadowResolution, ShadowResolution);

   auto shadowMinArea = ShadowMinArea;
   auto shadowMaxArea = ShadowMaxArea;

   SetShadowMapDrawArea(_shadowHandle, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
}

void LightAndShadow::Update() {
   namespace AppMath = AppFrame::Math;

   auto plyHeadPos= _gameMain.objServer().GetVecData("PlayerHeadPos");
   plyHeadPos.SetY(plyHeadPos.GetY() + FollowLightDiffY);
   
   _lightPositions = {
      FixedLightPos,
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
   firstLightPos.SetY(firstLightPos.GetY() + LightSphereDiffY);
   secondLightPos.SetY(secondLightPos.GetY() + LightSphereDiffY);
   DrawSphere3D(AppMath::ToDX(firstLightPos), LightSphereRadius, LightSphereDiv,
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue),
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue), TRUE);
   DrawSphere3D(AppMath::ToDX(secondLightPos), LightSphereRadius, LightSphereDiv,
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue), 
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue), TRUE);
#endif
}

