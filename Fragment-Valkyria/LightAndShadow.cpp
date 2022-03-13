
/*****************************************************************//**
 * \file   LightAndShadow.cpp
 * \brief  光源及び落ち影の処理を行う
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "LightAndShadow.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
   // jsonファイルから値を取得する
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("lightshadow", { "fog_enuble","fog_red","fog_green","fog_blue",
      "fog_start","global_ambred","global_ambgreen","global_ambblue","global_ambalpha","alllight_enuble","alllight_difred",
      "alllight_difgreen","alllight_difblue","alllight_difalpha","fog_end" ,"fixedlight_area" ,"fixedlight_atten_first" ,
      "fixedlight_atten_second","fixedlight_atten_third","alllight_spcred","alllight_spcgreen","alllight_spcblue","alllight_spcalpha",
      "alllight_ambred","alllight_ambgreen","alllight_ambblue","alllight_ambalpha","fixedlight_difred","fixedlight_difgreen",
      "fixedlight_difblue","fixedlight_difalpha","fixedlight_spcred","fixedlight_spcgreen","fixedlight_spcblue","fixedlight_spcalpha",
      "fixedlight_ambred","fixedlight_ambgreen","fixedlight_ambblue","fixedlight_ambalpha","shadow_resolution" });
   const bool FogEnuble = paramMap["fog_enuble"];                             //!< フォグの有無
   const int FogRed= paramMap["fog_red"];                                     //!< フォグ色(赤)
   const int FogGreen = paramMap["fog_green"];                                //!< フォグ色(緑)
   const int FogBlue = paramMap["fog_blue"];                                  //!< フォグ色(青)
   const float FogStart = paramMap["fog_start"];                              //!< フォグが始まる距離
   const float FogEnd = paramMap["fog_end"];                                  //!< フォグの色のみになる距離
   const float GlobalAmbRed = paramMap["global_ambred"];                      //!< 全てのモデルに適応される環境光(赤)
   const float GlobalAmbGreen = paramMap["global_ambgreen"];                  //!< 全てのモデルに適応される環境光(緑)
   const float GlobalAmbBlue = paramMap["global_ambblue"];                    //!< 全てのモデルに適応される環境光(青)
   const float GlobalAmbAlpha = paramMap["global_ambalpha"];                  //!< 全てのモデルに適応される環境光(透明度)
   const bool AllLightEnuble = paramMap["alllight_enuble"];                   //!< 標準光源の有無
   const float AllLightDifRed = paramMap["alllight_difred"];                  //!< 標準光源の拡散反射光(赤)
   const float AllLightDifGreen = paramMap["alllight_difgreen"];              //!< 標準光源の拡散反射光(緑)
   const float AllLightDifBlue = paramMap["alllight_difblue"];                //!< 標準光源の拡散反射光(青)
   const float AllLightDifAlpha = paramMap["alllight_difalpha"];              //!< 標準光源の拡散反射光(透明度)
   const float AllLightSpcRed = paramMap["alllight_spcred"];                  //!< 標準光源の鏡面反射光(赤)
   const float AllLightSpcGreen = paramMap["alllight_spcgreen"];              //!< 標準光源の鏡面反射光(緑)
   const float AllLightSpcBlue = paramMap["alllight_spcblue"];                //!< 標準光源の鏡面反射光(青)
   const float AllLightSpcAlpha = paramMap["alllight_spcalpha"];              //!< 標準光源の鏡面反射光(透明度)
   const float AllLightAmbRed = paramMap["alllight_ambred"];                  //!< 標準光源の環境光(赤)
   const float AllLightAmbGreen = paramMap["alllight_ambgreen"];              //!< 標準光源の環境光(緑)
   const float AllLightAmbBlue = paramMap["alllight_ambblue"];                //!< 標準光源の環境光(青)
   const float AllLightAmbAlpha = paramMap["alllight_ambalpha"];              //!< 標準光源の環境光(透明度)
   const float FixedLightArea = paramMap["fixedlight_area"];                  //!< 固定光源の照射範囲
   const float FixedLightAttenFirst = paramMap["fixedlight_atten_first"];     //!< 固定光源の距離減衰パラメーター1(除算値)
   const float FixedLightAttenSecond = paramMap["fixedlight_atten_second"];   //!< 固定光源の距離減衰パラメーター2(除算値)
   const float FixedLightAttenThird= paramMap["fixedlight_atten_third"];      //!< 固定光源の距離減衰パラメーター3(除算値)
   const float FixedLightDifRed = paramMap["fixedlight_difred"];              //!< 固定光源の拡散反射光(赤)
   const float FixedLightDifGreen = paramMap["fixedlight_difgreen"];          //!< 固定光源の拡散反射光(緑)
   const float FixedLightDifBlue = paramMap["fixedlight_difblue"];            //!< 固定光源の拡散反射光(青)
   const float FixedLightDifAlpha = paramMap["fixedlight_difalpha"];          //!< 固定光源の拡散反射光(透明度)
   const float FixedLightSpcRed = paramMap["fixedlight_spcred"];              //!< 固定光源の鏡面反射光(赤)
   const float FixedLightSpcGreen = paramMap["fixedlight_spcgreen"];          //!< 固定光源の鏡面反射光(緑)
   const float FixedLightSpcBlue = paramMap["fixedlight_spcblue"];            //!< 固定光源の鏡面反射光(青)
   const float FixedLightSpcAlpha = paramMap["fixedlight_spcalpha"];          //!< 固定光源の鏡面反射光(透明度)
   const float FixedLightAmbRed = paramMap["fixedlight_ambred"];              //!< 固定光源の環境光(赤)
   const float FixedLightAmbGreen = paramMap["fixedlight_ambgreen"];          //!< 固定光源の環境光(緑)
   const float FixedLightAmbBlue = paramMap["fixedlight_ambblue"];            //!< 固定光源の環境光(青)
   const float FixedLightAmbAlpha = paramMap["fixedlight_ambalpha"];          //!< 固定光源の環境光(透明度)
   const int ShadowResolution = paramMap["shadow_resolution"];                //!< 落ち影の解像度
   // jsonファイルからVector4型の値を取得する
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("lightshadow", { "fixedlight_pos" ,"shaodw_minarea" ,"shaodw_maxarea" });
   const auto FixedLightPos = vecParamMap["fixedlight_pos"];                  //!< 固定光源位置
   const auto ShadowMinArea = vecParamMap["shaodw_minarea"];                  //!< 落ち影描画範囲最小値
   const auto ShadowMaxArea = vecParamMap["shaodw_maxarea"];                  //!< 落ち影描画範囲最大値

#ifdef _DEBUG
   constexpr auto MatDifRed = 0.0f;           //!< マテリアルの拡散反射光(赤)
   constexpr auto MatDifGreen = 0.0f;         //!< マテリアルの拡散反射光(緑)
   constexpr auto MatDifBlue = 0.0f;          //!< マテリアルの拡散反射光(青)
   constexpr auto MatDifAlpha = 1.0f;         //!< マテリアルの拡散反射光(透明度)
   constexpr auto MatSpeRed = 0.0f;           //!< マテリアルの鏡面反射光(赤)
   constexpr auto MatSpeGreen = 0.0f;         //!< マテリアルの鏡面反射光(緑)
   constexpr auto MatSpeBlue = 0.0f;          //!< マテリアルの鏡面反射光(青)
   constexpr auto MatSpeAlpha = 0.0f;         //!< マテリアルの鏡面反射光(透明度)
   constexpr auto MatAmbRed = 0.0f;           //!< マテリアルの環境光(赤)
   constexpr auto MatAmbGreen = 0.0f;         //!< マテリアルの環境光(緑)
   constexpr auto MatAmbBlue = 0.0f;          //!< マテリアルの環境光(青)
   constexpr auto MatAmbAlpha = 0.0f;         //!< マテリアルの環境光(透明度)
   constexpr auto MatEmiRed = 0.0f;           //!< マテリアルの放射光(赤)
   constexpr auto MatEmiGreen = 0.0f;         //!< マテリアルの放射光(緑)
   constexpr auto MatEmiBlue = 0.5f;          //!< マテリアルの放射光(青)
   constexpr auto MatEmiAlpha = 0.0f;         //!< マテリアルの放射光(透明度)
   constexpr auto MatPower = 20.0f;           //!< マテリアルの鏡面反射強度
   constexpr auto LightSphereDiffY = 100.0;   //!< 光源球体の光源からのY差分
   constexpr auto LightSphereRadius = 80.0f;  //!< 光源球体半径
   constexpr auto LightSphereDiv = 32;        //!< 光源球体分割数
   constexpr auto LightSphereRed = 0;         //!< 光源球体色(赤)
   constexpr auto LightSphereGreen = 128;     //!< 光源球体色(緑)
   constexpr auto LightSphereBlue = 200;      //!< 光源球体色(青)
#endif
}

using namespace FragmentValkyria::Lighting;

LightAndShadow::LightAndShadow(Game::GameMain& gameMain) :_gameMain{gameMain} {
   namespace AppMath = AppFrame::Math;
#ifdef _DEBUG
   // マテリアルのライティング情報の設定
   MATERIALPARAM material;
   material.Diffuse = GetColorF(MatDifRed, MatDifGreen, MatDifBlue, MatDifAlpha);
   material.Specular = GetColorF(MatSpeRed, MatSpeGreen, MatSpeBlue, MatSpeAlpha);
   material.Ambient = GetColorF(MatAmbRed, MatAmbGreen, MatAmbBlue, MatAmbAlpha);
   material.Emissive = GetColorF(MatEmiRed, MatEmiGreen, MatEmiBlue, MatEmiAlpha);
   material.Power = MatPower;
   SetMaterialParam(material);
#endif
   // フォグを有効にする
   SetFogEnable(FogEnuble);
   // フォグの色の設定
   SetFogColor(FogRed, FogGreen, FogBlue);
   // フォグの始点と終点の設定
   SetFogStartEnd(FogStart, FogEnd);
   // 標準光源を有効にするか
   SetLightEnable(AllLightEnuble);
   // 全てのモデルに適応される環境光の設定
   SetGlobalAmbientLight(GetColorF(GlobalAmbRed, GlobalAmbGreen, GlobalAmbBlue, GlobalAmbAlpha));
   // 標準光源の拡散反射光を設定
   SetLightDifColor(GetColorF(AllLightDifRed, AllLightDifGreen, AllLightDifBlue, AllLightDifAlpha));
   // 標準光源の鏡面反射光を設定
   SetLightSpcColor(GetColorF(AllLightSpcRed, AllLightSpcGreen, AllLightSpcBlue, AllLightSpcAlpha));
   // 標準光源の環境光を設定
   SetLightAmbColor(GetColorF(AllLightAmbRed, AllLightAmbGreen, AllLightAmbBlue, AllLightAmbAlpha));
   // 光源位置の初期化
   _lightPosition = FixedLightPos;
   // 光源の位置、影響範囲及び距離減衰率を設定し、ポイント光源を生成、ライトハンドルに情報を保存する
   _lightHandle = CreatePointLightHandle(AppMath::ToDX(_lightPosition),
      FixedLightArea, FixedLightAttenFirst, FixedLightAttenSecond, FixedLightAttenThird);
   // 光源の拡散反射光を設定
   SetLightDifColorHandle(_lightHandle, GetColorF(FixedLightDifRed, FixedLightDifGreen, FixedLightDifBlue, FixedLightDifAlpha));
   // 光源の鏡面反射光を設定
   SetLightSpcColorHandle(_lightHandle, GetColorF(FixedLightSpcRed, FixedLightSpcGreen, FixedLightSpcBlue, FixedLightSpcAlpha));
   // 光源の環境光を設定
   SetLightAmbColorHandle(_lightHandle, GetColorF(FixedLightAmbRed, FixedLightAmbGreen, FixedLightAmbBlue, FixedLightAmbAlpha));
   // シャドウマップの解像度を設定しシャドウマップを生成、シャドウマップハンドルに保存する
   _shadowHandle = MakeShadowMap(ShadowResolution, ShadowResolution);
   auto shadowMaxArea = ShadowMaxArea;
   auto shadowMinArea = ShadowMinArea;
   // シャドウマップの影響範囲を設定
   SetShadowMapDrawArea(_shadowHandle, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
}

void LightAndShadow::Update() {
   namespace AppMath = AppFrame::Math;
   // プレイヤーの頭位置をObjectServerから取得
   auto playerHeadPos= _gameMain.objServer().GetVecData("PlayerHeadPos");
   // プレイヤー頭座標から固定光源座標へのベクトルを求め正規化し、シャドウマップの向きとする
   auto shadowDirection = (playerHeadPos - _lightPosition).Normalize();
   // シャドウマップの向きを更新
   SetShadowMapLightDirection(_shadowHandle, AppMath::ToDX(shadowDirection));
}

void LightAndShadow::Render() {
   // シャドウマップの描画を行う
   ShadowMap_DrawSetup(_shadowHandle);
   // シャドウマップ描画用にObjectServerの描画を回す
   _gameMain.objServer().Render();
   // シャドウマップの描画を終了する
   ShadowMap_DrawEnd();
   // 使用するシャドウマップハンドルの設定
   SetUseShadowMap(0, _shadowHandle);

#ifdef _DEBUG
   //光源球体のデバッグ描画
   namespace AppMath = AppFrame::Math;
   using Utility = AppFrame::Math::Utility;
   auto spherePos = _lightPosition;
   spherePos.SetY(_lightPosition.GetY() + LightSphereDiffY);
   DrawSphere3D(AppMath::ToDX(spherePos), LightSphereRadius, LightSphereDiv,
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue),
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue), TRUE);
#endif
}

void LightAndShadow::SetDifColor(float red, float green, float blue) {
   SetLightDifColorHandle(_lightHandle, GetColorF(red, green, blue, FixedLightDifAlpha));
}

void LightAndShadow::SetAmbColor(float red, float green, float blue) {
   SetLightAmbColorHandle(_lightHandle, GetColorF(red, green, blue, FixedLightDifAlpha));
}

