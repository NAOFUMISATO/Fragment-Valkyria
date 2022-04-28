
/*****************************************************************//**
 * \file   LargeEnemyHP.cpp
 * \brief  ラージエネミーの体力描画クラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "LargeEnemyHP.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ParamLargeEnemyUI.h"

namespace {
   constexpr auto OffSetLeft = 50;   //!< オフセット位置左
   constexpr auto OffSetTop = 57;    //!< オフセット位置上
   constexpr auto OffSetRight = 948;//!< オフセット位置右
   constexpr auto OffSetBottom = 87; //!< オフセット位置下
   constexpr auto MaxRate = 1.0;     //!< バー減少値の最大値

}

using namespace FragmentValkyria::Enemy;

LargeEnemyHP::LargeEnemyHP() {
   _param = std::make_unique<Param::ParamLargeEnemyUI>("bossui");
}

void LargeEnemyHP::Init() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return static_cast<unsigned char>(_param->GetIntParam(paramName));
   };
   _hp = _param->GetDoubleParam("max_hp");
   // 画像ハンドルをResourceServerから取得する
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandle = resServer.GetTexture("BossHP");
   // オフセット位置初期化
   _offSet = { OffSetLeft,OffSetTop,OffSetRight,OffSetBottom };
   // 1フレーム前の前面バーのプレイヤーHPをオフセット右座標で初期化
   _oldFrontHP = OffSetRight;
   // 前面バーカラーを初期化
   _frontColor = { _IntParam("frontcolor_red"),
      _IntParam("frontcolor_green"),_IntParam("frontcolor_blue") };
   // 背面バーカラーを初期化
   _backColor = { _IntParam("backcolor_red"),
      _IntParam("backcolor_green"),_IntParam("backcolor_blue") };
   // バーフレームの初期化
   _position = _param->GetVecParam("hp_pos");
}

void LargeEnemyHP::Update() {
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   using Utility = AppFrame::Math::Utility;
   // ゲームのフレームカウントをModeServerから取得
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto count = modeServer.frameCount();
   // ボスHPをObjectServerから取得
   auto& gameInstance = Game::Game::GetInstance();
   _hp = gameInstance.objServer().GetDoubleData("LargeEnemyHP");
   auto [left, top, right, bottom] = _offSet.GetRectParams();
   // 現在の前面HPバー右座標を線形補間で計算
   auto frontHP = std::lerp(right, (right - left) * _hp / _DoubleParam("max_hp") + left, MaxRate);
   // 背面バー減少中の処理
   if (frontHP < _oldBackHP) {
      // 現在のHPバー右座標を保存
      _oldFrontHP = frontHP;
      // 背面バー減少の終了処理
      if (_rateReset) {
         _rate = 0.0;        // 背面バー減少値のリセット
         _rateReset = false; // 背面バー減少フラグをOFF
      }
      // 背面バー減少値を進める
      _rate += _DoubleParam("redbar_speed");
   }
   // 背面バー右座標が前面バー右座標より同一座標以下の処理
   else {
      _rateReset = true;   // 背面バー減少フラグをON
      _rate = MaxRate;     // 背面バー減少値を最大値にする
   }
   // 現在の背面HPバー右座標を線形補間で計算し、指定の値ずつ減少させる
   auto backHP = std::lerp(_oldBackHP, frontHP, _rate);
   // 背面バー右座標が前面バー右座標より同一座標以下の処理
   if (frontHP >= backHP) {
      // 現在のHPバー右座標を保存
      _oldBackHP = backHP;
   }
   // 前面バーの矩形座標を更新
   auto [x, y] = _position.GetVec2();
   _frontBar.SetRectParams(
      x + left,
      y + top,
      x + frontHP,
      y + bottom
   );
   // 背面バーの矩形座標を更新
   _backBar.SetRectParams(
      x + left,
      y + top,
      x + backHP,
      y + bottom
   );
}

void LargeEnemyHP::Draw() {
   // HPが以下ならばHPバーは全て描画しない
   if (_hp > 0) {
      auto [backLeft, backTop, backRight, backBottom] = _backBar.GetRectParams();
      // 背面バー矩形の描画
      DrawBox(static_cast<int>(backLeft), static_cast<int>(backTop),
         static_cast<int>(backRight), static_cast<int>(backBottom), _backColor.GetColorCode(), TRUE);

      auto [frontLeft, frontTop, frontRight, frontBottom] = _frontBar.GetRectParams();
      // 前面バー矩形の描画
      DrawBox(static_cast<int>(frontLeft), static_cast<int>(frontTop),
         static_cast<int>(frontRight), static_cast<int>(frontBottom), _frontColor.GetColorCode(), TRUE);
   }
   // バーフレームの描画
   SpriteBase::Draw();
}

