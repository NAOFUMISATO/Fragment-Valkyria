#include "RemainingBullet.h"
#include "GameMain.h"

namespace{
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("playerui", { "bullet_diff_x" });
   const double BulletDiffX = paramMap["bullet_diff_x"];
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("playerui", { "bullet_pos" });
   const auto BulletPos = vecParamMap["bullet_pos"];
}

using namespace FragmentValkyria::Player;

RemainingBullet::RemainingBullet(Game::GameMain& gameMain) :Sprite::SpriteBase{gameMain} {
}

void RemainingBullet::Init() {
   _grHandles = GetResServer().GetTextures("RemainingBullet");
   _position = BulletPos;
   auto [x,y] = _position.GetVec2();
   _bulletPosX = { 
      static_cast<int>(x),
      static_cast<int>(x + BulletDiffX * 1.0),
      static_cast<int>(x + BulletDiffX * 2.0),
      static_cast<int>(x + BulletDiffX * 3.0),
      static_cast<int>(x + BulletDiffX * 4.0)
   };
}

void RemainingBullet::Update() {
   auto bulletStock = _gameMain.objServer().GetDoubleData("PlayerBulletStock");
   StockCheck(static_cast<int>(bulletStock));
}

void RemainingBullet::Draw() {
   auto [x, y] = _position.GetVec2();
   auto [firstFlag, secondFlag, thirdFlag, fourthFlag, fifthFlag] = _stockFlag;
   auto [firstX, secondX, thirdX, fourthX, fifthX] = _bulletPosX;
   GetSimpTexComponent().DrawTexture(firstX, static_cast<int>(y), 1.0, 0.0, _grHandles[firstFlag]);
   GetSimpTexComponent().DrawTexture(secondX, static_cast<int>(y), 1.0, 0.0, _grHandles[secondFlag]);
   GetSimpTexComponent().DrawTexture(thirdX, static_cast<int>(y), 1.0, 0.0, _grHandles[thirdFlag]);
   GetSimpTexComponent().DrawTexture(fourthX, static_cast<int>(y), 1.0, 0.0, _grHandles[fourthFlag]);
   GetSimpTexComponent().DrawTexture(fifthX, static_cast<int>(y), 1.0, 0.0, _grHandles[fifthFlag]);
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
