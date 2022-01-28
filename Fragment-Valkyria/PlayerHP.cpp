#include "PlayerHP.h"
#include "GameMain.h"
using namespace FragmentValkyria::Player;

PlayerHP::PlayerHP(Game::GameMain& gameMain) :Sprite::SpriteBase{ gameMain } {
}

void PlayerHP::Init() {
   _grHandle = GetResServer().GetTexture("PlayerHP");
   _offSet = { 18,27,327,84 };
   _frontHP = 327.0;
   _frontColor = { 0,255,0 };
   _backColor = { 255,0,0 };
   _position.SetVec2(30.0,30.0);
}

void PlayerHP::Update() {
   using Utility = AppFrame::Math::Utility;
   auto count = _gameMain.modeServer().frameCount();
   if (_shake) {
      if (count - _cnt <= 60) {
         auto rand = Utility::GetRandom(30.0 - 2.0, 30.0 + 2.0);
         _position.SetVec2(rand, rand);
      }
      else {
         _position.SetVec2(30.0, 30.0);
         _shake = false;
      }
   }
   _hp = _gameMain.objServer().GetDoubleData("PlayerHP");
   auto [left, top, right,  bottom] = _offSet.GetRectParams();
   auto frontHP = std::lerp(right, (right - left) * _hp / 100.0 + left, 1.0);
   if (frontHP < _frontHP) {
      _shake = true;
      _cnt = count;
   }
   if (frontHP  < _backHP) {
      _frontHP = frontHP;
      if(_subFlag){
         _rate = 0.0;
         _subFlag = false;
      }
      _rate += 0.005;
   }
   else {
      _subFlag = true;
      _rate = 1.0;
   }
   auto backHP = std::lerp(_backHP, frontHP, _rate);
   if (frontHP >= backHP) {
      _backHP = backHP;
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
   if (_hp >0) {
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

