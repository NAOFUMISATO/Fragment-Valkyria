#include "PlayerHP.h"
#include "GameMain.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("playerhp", { "frontcolor_red" ,"frontcolor_green" ,
   "frontcolor_blue", "backcolor_red" ,"backcolor_green","backcolor_blue" ,"shake_frame","shake_width","redbar_speed","max_hp" });
   const unsigned char FrontColorRed = paramMap["frontcolor_red"];
   const unsigned char FrontColorGreen = paramMap["frontcolor_green"];
   const unsigned char FrontColorBlue = paramMap["frontcolor_blue"];
   const unsigned char BackColorRed = paramMap["backcolor_red"];
   const unsigned char BackColorGreen = paramMap["backcolor_green"];
   const unsigned char BackColorBlue = paramMap["backcolor_blue"];
   const unsigned char ShakeFrame= paramMap["shake_frame"];
   const double ShakeWidth = paramMap["shake_width"];
   const double RedBarSpeed = paramMap["redbar_speed"];
   const double MaxHp= paramMap["max_hp"];
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("playerhp", { "defalut_pos" });
   const auto DefalutPos = vecParamMap["defalut_pos"];

   constexpr auto OffSetLeft = 18;
   constexpr auto OffSetTop = 27;
   constexpr auto OffSetRight = 327;
   constexpr auto OffSetBottom = 84;
   constexpr auto MaxRate = 1.0;

}

using namespace FragmentValkyria::Player;

PlayerHP::PlayerHP(Game::GameMain& gameMain) :Sprite::SpriteBase{ gameMain } {
}

void PlayerHP::Init() {
   _grHandle = GetResServer().GetTexture("PlayerHP");
   _offSet = { OffSetLeft,OffSetTop,OffSetRight,OffSetBottom };
   _oldFrontHP = OffSetRight;
   _frontColor = { FrontColorRed,FrontColorGreen,FrontColorBlue };
   _backColor = { BackColorRed,BackColorGreen,BackColorBlue };
   _position= DefalutPos;
}

void PlayerHP::Update() {
   using Utility = AppFrame::Math::Utility;
   auto count = _gameMain.modeServer().frameCount();
   if (_shake) {
      if (count - _shakeCnt <= ShakeFrame) {
         auto [posX,posY] = DefalutPos.GetVec2();
         auto rand = Utility::GetRandom(posX - ShakeWidth, posY + ShakeWidth);
         _position.SetVec2(rand, rand);
      }
      else {
         _position= DefalutPos;
         _shake = false;
      }
   }
   _hp = _gameMain.objServer().GetDoubleData("PlayerHP");
   auto [left, top, right,  bottom] = _offSet.GetRectParams();
   auto frontHP = std::lerp(right, (right - left) * _hp / MaxHp + left, MaxRate);
   if (frontHP < _oldFrontHP) {
      _shake = true;
      _shakeCnt = count;
   }
   if (frontHP  < _oldBackHP) {
      _oldFrontHP = frontHP;
      if(_rateReset){
         _rate = 0.0;
         _rateReset = false;
      }
      _rate += RedBarSpeed;
   }
   else {
      _rateReset = true;
      _rate = MaxRate;
   }
   auto backHP = std::lerp(_oldBackHP, frontHP, _rate);
   if (frontHP >= backHP) {
      _oldBackHP = backHP;
   }
   // バーの座標(左,上),(右,下)を更新
   auto [x, y] = _position.GetVec2();
   _frontBar.SetRectParams(
      x + left,
      y + top,
      x + frontHP,
      y + bottom
   );
   _frontTriangle.SetTriangleParams(
      std::make_pair(x + frontHP, y + top),
      std::make_pair(x + frontHP, y + bottom),
      std::make_pair(x + frontHP + (y + bottom - (y + top)), y + top)
   );

   _backBar.SetRectParams(
      x + left,
      y + top,
      x + backHP,
      y + bottom
   );
   _backTriangle.SetTriangleParams(
      std::make_pair(x + backHP, y + top),
      std::make_pair(x + backHP, y + bottom),
      std::make_pair(x + backHP + (y + bottom - (y + top)), y + top)
   );
}

void PlayerHP::Draw() {
   if (_hp > 0){
      auto [backLeft, backTop, backRight, backBottom] = _backBar.GetRectParams();
      DrawBox(static_cast<int>(backLeft), static_cast<int>(backTop),
         static_cast<int>(backRight), static_cast<int>(backBottom), _backColor.GetColorCode(), TRUE);
      auto [backFirstApex, backSecondApex, backThirdApex] = _backTriangle.GetTriangleParams();
      auto [backFirstX, backFirstY] = backFirstApex;
      auto [backSecondX, backSecondY] = backSecondApex;
      auto [backThirdX, backThirdY] = backThirdApex;
      DrawTriangle(static_cast<int>(backFirstX), static_cast<int>(backFirstY), static_cast<int>(backSecondX),
         static_cast<int>(backSecondY), static_cast<int>(backThirdX), static_cast<int>(backThirdY), _backColor.GetColorCode(), TRUE);

      auto [frontLeft, frontTop, frontRight, frontBottom] = _frontBar.GetRectParams();
      DrawBox(static_cast<int>(frontLeft), static_cast<int>(frontTop),
         static_cast<int>(frontRight), static_cast<int>(frontBottom), _frontColor.GetColorCode(), TRUE);
      auto [frontFirstApex, frontSecondApex, frontThirdApex] = _frontTriangle.GetTriangleParams();
      auto [frontFirstX, frontFirstY] = frontFirstApex;
      auto [frontSecondX, frontSecondY] = frontSecondApex;
      auto [frontThirdX, frontThirdY] = frontThirdApex;
      DrawTriangle(static_cast<int>(frontFirstX), static_cast<int>(frontFirstY), static_cast<int>(frontSecondX),
         static_cast<int>(frontSecondY), static_cast<int>(frontThirdX), static_cast<int>(frontThirdY), _frontColor.GetColorCode(), TRUE);
   }
   SpriteBase::Draw();
}

