
/*****************************************************************//**
 * \file   PlayerHP.cpp
 * \brief  プレイヤーHPの描画を行う
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "PlayerHP.h"
#include "GameMain.h"

namespace {
   // jsonファイルから値を取得する
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("playerhp", { "frontcolor_red" ,
      "frontcolor_green" ,"frontcolor_blue", "backcolor_red" ,"backcolor_green",
      "backcolor_blue" ,"shake_frame","shake_width","redbar_speed","max_hp" });
   const unsigned char FrontColorRed = paramMap["frontcolor_red"];     //!< 前面バーの初期カラー赤値
   const unsigned char FrontColorGreen = paramMap["frontcolor_green"]; //!< 前面バーの初期カラー緑値
   const unsigned char FrontColorBlue = paramMap["frontcolor_blue"];   //!< 前面バーの初期カラー青値
   const unsigned char BackColorRed = paramMap["backcolor_red"];       //!< 背面バーの初期カラー赤値
   const unsigned char BackColorGreen = paramMap["backcolor_green"];   //!< 背面バーの初期カラー緑値
   const unsigned char BackColorBlue = paramMap["backcolor_blue"];     //!< 背面バーの初期カラー青値
   const unsigned char ShakeFrame= paramMap["shake_frame"];            //!< 振幅フレーム
   const double ShakeWidth = paramMap["shake_width"];                  //!< 振幅の大きさ
   const double RedBarSpeed = paramMap["redbar_speed"];                //!< 背面バーの減少速度
   const double MaxHp= paramMap["max_hp"];                             //!< プレイヤー最大HP
   // jsonファイルからVector4の値を取得する
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("playerhp", { "defalut_pos" });
   const auto DefalutPos = vecParamMap["defalut_pos"];                 //!< バーフレーム位置(左上座標)

   constexpr auto OffSetLeft = 18;   //!< オフセット位置左
   constexpr auto OffSetTop = 27;    //!< オフセット位置上
   constexpr auto OffSetRight = 327; //!< オフセット位置右
   constexpr auto OffSetBottom = 84; //!< オフセット位置下
   constexpr auto MaxRate = 1.0;     //!< バー減少値の最大値

}

using namespace FragmentValkyria::Player;

PlayerHP::PlayerHP(Game::GameMain& gameMain) :Sprite::SpriteBase{ gameMain } {
}

void PlayerHP::Init() {
   // 画像ハンドルをResourceServerから取得する
   _grHandle = GetResServer().GetTexture("PlayerHP");
   _offSet = { OffSetLeft,OffSetTop,OffSetRight,OffSetBottom };    // オフセット位置初期化
   _oldFrontHP = OffSetRight;                                      // 1フレーム前の前面バーのプレイヤーHPをオフセット右座標で初期化
   _frontColor = { FrontColorRed,FrontColorGreen,FrontColorBlue }; // 前面バーカラーを初期化
   _backColor = { BackColorRed,BackColorGreen,BackColorBlue };     // 背面バーカラーを初期化
   _position = DefalutPos;                                         // バーフレームの初期化
}

void PlayerHP::Update() {
   using Utility = AppFrame::Math::Utility;
   // ゲームのフレームカウントをModeServerから取得
   auto count = _gameMain.modeServer().frameCount();
   // HPバー振動の処理
   BarShake(count);
   // プレイヤーHPをObjectServerから取得
   _hp = _gameMain.objServer().GetDoubleData("PlayerHP");
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
   // 前面バーの三角形座標を更新
   _frontTriangle.SetTriangleParams(
      std::make_pair(x + frontHP, y + top),
      std::make_pair(x + frontHP, y + bottom),
      std::make_pair(x + frontHP + (y + bottom - (y + top)), y + top)
   );
   // 背面バーの矩形座標を更新
   _backBar.SetRectParams(
      x + left,
      y + top,
      x + backHP,
      y + bottom
   );
   // 背面バーの三角形座標を更新
   _backTriangle.SetTriangleParams(
      std::make_pair(x + backHP, y + top),
      std::make_pair(x + backHP, y + bottom),
      std::make_pair(x + backHP + (y + bottom - (y + top)), y + top)
   );
}

void PlayerHP::Draw() {
   // HPが以下ならばHPバーは全て描画しない
   if (_hp > 0){
      auto [backLeft, backTop, backRight, backBottom] = _backBar.GetRectParams();
      // 背面バー矩形の描画
      DrawBox(static_cast<int>(backLeft), static_cast<int>(backTop),
         static_cast<int>(backRight), static_cast<int>(backBottom), _backColor.GetColorCode(), TRUE);
      auto [backFirstApex, backSecondApex, backThirdApex] = _backTriangle.GetTriangleParams();
      auto [backFirstX, backFirstY] = backFirstApex;
      auto [backSecondX, backSecondY] = backSecondApex;
      auto [backThirdX, backThirdY] = backThirdApex;
      // 背面バー三角形の描画
      DrawTriangle(static_cast<int>(backFirstX), static_cast<int>(backFirstY), static_cast<int>(backSecondX),
         static_cast<int>(backSecondY), static_cast<int>(backThirdX), static_cast<int>(backThirdY), _backColor.GetColorCode(), TRUE);

      auto [frontLeft, frontTop, frontRight, frontBottom] = _frontBar.GetRectParams();
      // 前面バー矩形の描画
      DrawBox(static_cast<int>(frontLeft), static_cast<int>(frontTop),
         static_cast<int>(frontRight), static_cast<int>(frontBottom), _frontColor.GetColorCode(), TRUE);
      auto [frontFirstApex, frontSecondApex, frontThirdApex] = _frontTriangle.GetTriangleParams();
      auto [frontFirstX, frontFirstY] = frontFirstApex;
      auto [frontSecondX, frontSecondY] = frontSecondApex;
      auto [frontThirdX, frontThirdY] = frontThirdApex;
      // 前面バー三角形の描画
      DrawTriangle(static_cast<int>(frontFirstX), static_cast<int>(frontFirstY), static_cast<int>(frontSecondX),
         static_cast<int>(frontSecondY), static_cast<int>(frontThirdX), static_cast<int>(frontThirdY), _frontColor.GetColorCode(), TRUE);
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
         auto rand = AppFrame::Math::Utility::GetRandom(posX - ShakeWidth, posY + ShakeWidth);
         // バーフレームの座標を乱数で更新
         _position.SetVec2(rand, rand);
      }
      // 振動し始めてから指定フレーム以上の処理
      else {
         _position = DefalutPos; // 位置のリセット
         _shake = false;         // 振動フラグOFF
      }
   }
}

