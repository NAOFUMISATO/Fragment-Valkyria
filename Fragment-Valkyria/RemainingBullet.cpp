#include "RemainingBullet.h"
#include "GameMain.h"

namespace{
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("playerui", { "bullet_diff_x" });
   const double DiffX = paramMap["bullet_diff_x"];
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("playerui", { "bullet_pos" });
   const auto BulletPos = vecParamMap["bullet_pos"];
   constexpr auto ThirdMultRate = 2.0;
   constexpr auto FourthMultRate = 3.0;
   constexpr auto FifthMultRate = 4.0;
   constexpr auto DefalutScale = 1.0;
   constexpr auto DefalutAngle = 0.0;
}

using namespace FragmentValkyria::Player;

RemainingBullet::RemainingBullet(Game::GameMain& gameMain) :Sprite::SpriteBase{gameMain} {
}

void RemainingBullet::Init() {
   _grHandles = GetResServer().GetTextures("RemainingBullet");
   _position = BulletPos;
   auto [x,y] = _position.GetVec2();
   _xPositions = { 
      static_cast<int>(x),
      static_cast<int>(x + DiffX),
      static_cast<int>(x + DiffX * ThirdMultRate),
      static_cast<int>(x + DiffX * FourthMultRate),
      static_cast<int>(x + DiffX * FifthMultRate)
   };
}

void RemainingBullet::Update() {
   auto bulletStock = _gameMain.objServer().GetDoubleData("PlayerBulletStock");
   StockCheck(static_cast<int>(bulletStock));
}

void RemainingBullet::Draw() {
   auto [x, y] = _position.GetVec2();
   auto [firstFlag, secondFlag, thirdFlag, fourthFlag, fifthFlag] = _stockFlag;
   auto [firstX, secondX, thirdX, fourthX, fifthX] = _xPositions;
   GetSimpTexComponent().DrawTexture(firstX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[firstFlag]);
   GetSimpTexComponent().DrawTexture(secondX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[secondFlag]);
   GetSimpTexComponent().DrawTexture(thirdX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[thirdFlag]);
   GetSimpTexComponent().DrawTexture(fourthX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[fourthFlag]);
   GetSimpTexComponent().DrawTexture(fifthX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[fifthFlag]);
}

void RemainingBullet::StockCheck(int stock) {
   switch (stock) {
   case 5:
      _stockFlag = { true,true,true,true,true };
      break;
   case 4:
      _stockFlag = { true,true,true,true,false };
      break;
   case 3:
      _stockFlag = { true,true,true,false,false };
      break;
   case 2:
      _stockFlag = { true,true,false,false,false };
      break;
   case 1:
      _stockFlag = { true,false,false,false,false };
      break;
   case 0:
      _stockFlag = { false,false,false,false,false };
      break;
   }
}
