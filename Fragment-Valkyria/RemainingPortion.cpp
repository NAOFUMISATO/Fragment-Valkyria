
/*****************************************************************//**
 * \file   RemainingPortion.cpp
 * \brief  残回復アイテム
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "RemainingPortion.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ParamPlayerUI.h"

namespace {
   constexpr auto ThirdMultRate = 2.0;
   constexpr auto FourthMultRate = 3.0;
   constexpr auto FifthMultRate = 4.0;
   constexpr auto DefalutScale = 1.0;
   constexpr auto DefalutAngle = 0.0;
}

using namespace FragmentValkyria::Player;

RemainingPortion::RemainingPortion() {
   _param = std::make_unique<Param::ParamPlayerUI>( "playerui");
}

void RemainingPortion::Init() {
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandles = resServer.GetTextures("RemainingPortion");
   _position = _param->GetVecParam("portion_pos");
   auto [x, y] = _position.GetVec2();
   const auto DiffX = _param->GetIntParam("portion_diff_x");
   _xPositions = {
      static_cast<int>(x),
      static_cast<int>(x + DiffX),
      static_cast<int>(x + DiffX * ThirdMultRate)
   };
}

void RemainingPortion::Update() {
   auto& gameInstance = Game::Game::GetInstance();
   auto portionStock = gameInstance.playerPortion();
   StockCheck(static_cast<int>(portionStock));
}

void RemainingPortion::Draw() {
   auto [x, y] = _position.GetVec2();
   auto [firstFlag, secondFlag, thirdFlag] = _stockFlag;
   auto [firstX, secondX, thirdX] = _xPositions;
   auto& texComponent = Game::Game::GetInstance().texComponent();
   texComponent.DrawTexture(firstX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[firstFlag]);
   texComponent.DrawTexture(secondX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[secondFlag]);
   texComponent.DrawTexture(thirdX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[thirdFlag]);
}

void RemainingPortion::StockCheck(int stock) {
   switch (stock) {
   case 3:
      _stockFlag = { true,true,true };
      break;
   case 2:
      _stockFlag = { true,true,false };
      break;
   case 1:
      _stockFlag = { true,false,false };
      break;
   case 0:
      _stockFlag = { false,false,false };
      break;
   }
}
