
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
   _param = std::make_unique<Param::ParamLightShadow>(_gameMain, "lightshadow");
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   /**
    * \brief float型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _FloatParam = [&](std::string paramName) {
      return _param->GetFloatParam(paramName);
   };
   /**
    * \brief bool型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _BoolParam = [&](std::string paramName) {
      return _param->GetBoolParam(paramName);
   };
   /**
    * \brief Vector4型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _VecParam = [&](std::string paramName) {
      return _param->GetVecParam(paramName);
   };
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
   SetFogEnable(_BoolParam("fog_enuble"));
   // フォグの色の設定
   SetFogColor(_IntParam("fog_red"), _IntParam("fog_green"), _IntParam("fog_blue"));
   // フォグの始点と終点の設定
   SetFogStartEnd(_FloatParam("fog_start"), _FloatParam("fog_end"));
   // 標準光源を有効にするか
   SetLightEnable(_BoolParam("alllight_enuble"));
   // 全てのモデルに適応される環境光の設定
   SetGlobalAmbientLight(GetColorF(_FloatParam("global_ambred"), _FloatParam("global_ambgreen"), 
      _FloatParam("global_ambblue"), _FloatParam("global_ambalpha")));
   // 標準光源の拡散反射光を設定
   SetLightDifColor(GetColorF(_FloatParam("alllight_difred"), _FloatParam("alllight_difgreen"),
      _FloatParam("alllight_difblue"), _FloatParam("alllight_difalpha")));
   // 標準光源の鏡面反射光を設定
   SetLightSpcColor(GetColorF(_FloatParam("alllight_spcred"), _FloatParam("alllight_spcgreen"),
      _FloatParam("alllight_spcblue"), _FloatParam("alllight_spcalpha")));
   // 標準光源の環境光を設定
   SetLightAmbColor(GetColorF(_FloatParam("alllight_ambred"), _FloatParam("alllight_ambgreen"),
      _FloatParam("alllight_ambblue"), _FloatParam("alllight_ambalpha")));
   // 光源位置の初期化
   _lightPosition = _VecParam("fixedlight_pos");
   // 光源の位置、影響範囲及び距離減衰率を設定し、ポイント光源を生成、ライトハンドルに情報を保存する
   _lightHandle = CreatePointLightHandle(AppMath::ToDX(_lightPosition),
      _FloatParam("fixedlight_area"), _FloatParam("fixedlight_atten_first"),
      _FloatParam("fixedlight_atten_second"), _FloatParam("fixedlight_atten_third"));
   // 光源の拡散反射光を設定
   SetLightDifColorHandle(_lightHandle, GetColorF(_FloatParam("fixedlight_difred"), 
      _FloatParam("fixedlight_difgreen"), _FloatParam("fixedlight_difblue"), _FloatParam("fixedlight_difalpha")));
   // 光源の鏡面反射光を設定
   SetLightSpcColorHandle(_lightHandle, GetColorF(_FloatParam("fixedlight_spcred"),
      _FloatParam("fixedlight_spcgreen"), _FloatParam("fixedlight_spcblue"), _FloatParam("fixedlight_spcalpha")));
   // 光源の環境光を設定
   SetLightAmbColorHandle(_lightHandle, GetColorF(_FloatParam("fixedlight_ambred"),
      _FloatParam("fixedlight_ambgreen"), _FloatParam("fixedlight_ambblue"), _FloatParam("fixedlight_ambalpha")));
   // シャドウマップの解像度を設定しシャドウマップを生成、シャドウマップハンドルに保存する
   const auto ShadowResolution = _IntParam("shadow_resolution");
   _shadowHandle = MakeShadowMap(ShadowResolution, ShadowResolution);
   auto shadowMaxArea = _VecParam("shadow_maxarea");
   auto shadowMinArea = _VecParam("shadow_minarea");
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
   SetLightDifColorHandle(_lightHandle, GetColorF(red, green, blue, _param->GetFloatParam("fixedlight_difalpha")));
}

void LightAndShadow::SetAmbColor(float red, float green, float blue) {
   SetLightAmbColorHandle(_lightHandle, GetColorF(red, green, blue, _param->GetFloatParam("fixedlight_ambalpha")));
}

