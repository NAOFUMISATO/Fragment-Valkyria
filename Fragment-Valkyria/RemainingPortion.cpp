
/*****************************************************************//**
 * \file   RemainingPortion.cpp
 * \brief  残回復アイテム
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "RemainingPortion.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("playerui", { "portion_diff_x" });
   const double DiffX = paramMap["portion_diff_x"];
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("playerui", { "portion_pos" });
   const auto PortionPos = vecParamMap["portion_pos"];
   constexpr auto ThirdMultRate = 2.0;
   constexpr auto FourthMultRate = 3.0;
   constexpr auto FifthMultRate = 4.0;
   constexpr auto DefalutScale = 1.0;
   constexpr auto DefalutAngle = 0.0;
}

using namespace FragmentValkyria::Player;

RemainingPortion::RemainingPortion(Game::GameMain& gameMain) :Sprite::SpriteBase{ gameMain } {
}

void RemainingPortion::Init() {
   _grHandles = GetResServer().GetTextures("RemainingPortion");
   _position = PortionPos;
   auto [x, y] = _position.GetVec2();
   _xPositions = {
      static_cast<int>(x),
      static_cast<int>(x + DiffX),
      static_cast<int>(x + DiffX * ThirdMultRate)
   };
}

void RemainingPortion::Update() {
   auto portionStock = _gameMain.playerPortion();
   StockCheck(static_cast<int>(portionStock));
}

void RemainingPortion::Draw() {
   auto [x, y] = _position.GetVec2();
   auto [firstFlag, secondFlag, thirdFlag] = _stockFlag;
   auto [firstX, secondX, thirdX] = _xPositions;
   GetTexComponent().DrawTexture(firstX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[firstFlag]);
   GetTexComponent().DrawTexture(secondX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[secondFlag]);
   GetTexComponent().DrawTexture(thirdX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[thirdFlag]);
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
