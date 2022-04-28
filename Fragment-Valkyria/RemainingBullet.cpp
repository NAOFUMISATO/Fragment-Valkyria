
/*****************************************************************//**
 * \file   RemainingBullet.cpp
 * \brief  Žc’e‚ð•\Ž¦‚·‚é
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "RemainingBullet.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ParamPlayerUI.h"

namespace{
   constexpr auto ThirdMultRate = 2.0;
   constexpr auto FourthMultRate = 3.0;
   constexpr auto FifthMultRate = 4.0;
   constexpr auto DefalutScale = 1.0;
   constexpr auto DefalutAngle = 0.0;
}

using namespace FragmentValkyria::Player;

RemainingBullet::RemainingBullet() {
   _param = std::make_unique<Param::ParamPlayerUI>("playerui");
}

void RemainingBullet::Init() {
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandles = resServer.GetTextures("RemainingBullet");
   _position = _param->GetVecParam("bullet_pos");
   auto [x,y] = _position.GetVec2();
   const auto DiffX = _param->GetIntParam("bullet_diff_x");
   _xPositions = { 
      static_cast<int>(x),
      static_cast<int>(x + DiffX),
      static_cast<int>(x + DiffX * ThirdMultRate),
      static_cast<int>(x + DiffX * FourthMultRate),
      static_cast<int>(x + DiffX * FifthMultRate)
   };
}

void RemainingBullet::Update() {
   auto& gameInstance = Game::Game::GetInstance();
   auto bulletStock = gameInstance.playerBullet();
   StockCheck(static_cast<int>(bulletStock));
}

void RemainingBullet::Draw() {
   auto [x, y] = _position.GetVec2();
   auto [firstFlag, secondFlag, thirdFlag, fourthFlag, fifthFlag] = _stockFlag;
   auto [firstX, secondX, thirdX, fourthX, fifthX] = _xPositions;
   auto& texComponent = Game::Game::GetInstance().texComponent();
   texComponent.DrawTexture(firstX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[firstFlag]);
   texComponent.DrawTexture(secondX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[secondFlag]);
   texComponent.DrawTexture(thirdX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[thirdFlag]);
   texComponent.DrawTexture(fourthX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[fourthFlag]);
   texComponent.DrawTexture(fifthX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[fifthFlag]);
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
