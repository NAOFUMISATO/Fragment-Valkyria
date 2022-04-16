
/*****************************************************************//**
 * \file   PlayerHP.cpp
 * \brief  プレイヤーHPの描画を行う
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "PlayerHP.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
   // jsonファイルから値を取得する
   auto hpParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("playerui", { "frontcolor_red" ,
      "frontcolor_green" ,"frontcolor_blue", "backcolor_red" ,"backcolor_green",
      "backcolor_blue" ,"shake_frame","shake_width","redbar_speed" });
   const unsigned char FrontColorRed = hpParamMap["frontcolor_red"];     //!< 前面バーの初期カラー赤値
   const unsigned char FrontColorGreen = hpParamMap["frontcolor_green"]; //!< 前面バーの初期カラー緑値
   const unsigned char FrontColorBlue = hpParamMap["frontcolor_blue"];   //!< 前面バーの初期カラー青値
   const unsigned char BackColorRed = hpParamMap["backcolor_red"];       //!< 背面バーの初期カラー赤値
   const unsigned char BackColorGreen = hpParamMap["backcolor_green"];   //!< 背面バーの初期カラー緑値
   const unsigned char BackColorBlue = hpParamMap["backcolor_blue"];     //!< 背面バーの初期カラー青値
   const unsigned char ShakeFrame= hpParamMap["shake_frame"];            //!< 振幅フレーム
   const double ShakeWidth = hpParamMap["shake_width"];                  //!< 振幅の大きさ
   const double RedBarSpeed = hpParamMap["redbar_speed"];                //!< 背面バーの減少速度

   auto playerParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("player", { "max_hp" });
   const double MaxHp= playerParamMap["max_hp"];                         //!< プレイヤー最大HP

   // jsonファイルからVector4の値を取得する
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("playerui", { "hp_pos" });
   const auto DefalutPos = vecParamMap["hp_pos"];                      //!< バーフレーム位置(左上座標)

   constexpr auto OffSetLeft = 5;   //!< オフセット位置左
   constexpr auto OffSetTop = 7;    //!< オフセット位置上
   constexpr auto OffSetRight = 457; //!< オフセット位置右
   constexpr auto OffSetBottom = 32; //!< オフセット位置下
   constexpr auto MaxRate = 1.0;     //!< バー減少値の最大値

}

using namespace FragmentValkyria::Player;

PlayerHP::PlayerHP(Game::GameMain& gameMain) :Sprite::SpriteBase{ gameMain } {
}

void PlayerHP::Init() {
   _hp = _gameMain.playerHp();
   // 画像ハンドルをResourceServerから取得する
   _grHandle = GetResServer().GetTexture("PlayerHP");
   _offSet = { OffSetLeft,OffSetTop,OffSetRight,OffSetBottom };    // オフセット位置初期化
   auto [left, top, right, bottom] = _offSet.GetRectParams();
   // 1フレーム前の前面バーのプレイヤーHPを線形補間により座標を求め初期化
   _oldFrontHP = std::lerp(right, (right - left) * _hp / MaxHp + left, MaxRate);
   _frontColor = { FrontColorRed,FrontColorGreen,FrontColorBlue }; // 前面バーカラーを初期化
   _backColor = { BackColorRed,BackColorGreen,BackColorBlue };     // 背面バーカラーを初期化
   _position = DefalutPos;                                         // バーフレームの初期化
   _shake = false;                                                 // 振動フラグOFF
   _rateReset = false;                                             // 背面バー減少フラグをOFF
}

void PlayerHP::Update() {
   using Utility = AppFrame::Math::Utility;
   // ゲームのフレームカウントをModeServerから取得
   auto count = _gameMain.modeServer().frameCount();
   // HPバー振動の処理
   BarShake(count);
   // プレイヤーHPを取得
   _hp = _gameMain.playerHp();
   auto [left, top, right, bottom] = _offSet.GetRectParams();
   // 現在の前面HPバー右座標を線形補間で計算
   auto frontHP = std::lerp(right, (right - left) * _hp / MaxHp + left, MaxRate);
   // HP減少を検知した際の処理
   if (frontHP < _oldFrontHP) {
      // 振動フラグON
      _shake = true;
      // 振動フレームのリセット
      _shakeCnt = count;
   }
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
      _rate += RedBarSpeed;
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

void PlayerHP::Draw() {
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

void PlayerHP::BarShake(unsigned int count) {
   // HPバー振動の処理
   if (_shake) {
      // 振動し始めてから指定フレーム以内の処理
      if (count - _shakeCnt <= ShakeFrame) {
         auto [posX, posY] = DefalutPos.GetVec2();
         // Utilityクラスから等確率分布の乱数を指定範囲内で取得
         auto randX = AppFrame::Math::Utility::GetRandom(posX - ShakeWidth, posX + ShakeWidth);
         auto randY = AppFrame::Math::Utility::GetRandom(posY - ShakeWidth, posY + ShakeWidth);
         // バーフレームの座標を乱数で更新
         _position.SetVec2(randX, randY);
      }
      // 振動し始めてから指定フレーム以上の処理
      else {
         _position = DefalutPos; // 位置のリセット
         _shake = false;         // 振動フラグOFF
      }
   }
}

