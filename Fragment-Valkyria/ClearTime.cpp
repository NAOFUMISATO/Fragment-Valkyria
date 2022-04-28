
/*****************************************************************//**
 * \file   ClearTime.cpp
 * \brief  クリアタイムを表示する
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ClearTime.h"
#include "Game.h"
#include "ParamModeClear.h"

namespace {
   constexpr auto ThirdMultRate = 2.0;
   constexpr auto FourthMultRate = 3.0;
   constexpr auto FifthMultRate = 4.0;
   constexpr auto DefalutScale = 3.0;
   constexpr auto DefalutAngle = 0.0;

}
using namespace FragmentValkyria::Clear;

ClearTime::ClearTime() {
   _param = std::make_unique<Param::ParamModeClear>("clear");
}

void ClearTime::Init() {
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandles = resServer.GetTextures("NumberAndColon");
   _position = _param->GetVecParam("time_pos");
   auto [x, y] = _position.GetVec2();
   const auto NumberDiffX= _param->GetIntParam("number_diffx");
   _xPositions = {
      static_cast<int>(x),
      static_cast<int>(x + NumberDiffX),
      static_cast<int>(x + NumberDiffX * ThirdMultRate),
      static_cast<int>(x + NumberDiffX * FourthMultRate),
      static_cast<int>(x + NumberDiffX * FifthMultRate)
   };
   auto& gameInstance = Game::Game::GetInstance();
   auto timer = gameInstance.ingameTimer();
   auto allSec = timer / 60;
   auto sec = allSec % 60;
   auto minute = allSec / 60;
   _animeNos = {
      sec % static_cast<Uint>(std::pow(10, 1)) ,
      sec % static_cast<Uint>(std::pow(10, 2)) / static_cast<Uint>(std::pow(10, 1)),
      static_cast<Uint>(_grHandles.size() - 1),
      minute % static_cast<Uint>(std::pow(10, 1)),
      minute % static_cast<Uint>(std::pow(10, 2)) / static_cast<Uint>(std::pow(10, 1))
   };
}

void ClearTime::Draw(){
   auto [x, y] = _position.GetVec2();
   auto [firstX, secondX, thirdX, fourthX, fifthX] = _xPositions;
   auto [firstNo, secondNo, thirdNo, fourthNo, fifthNo] = _animeNos;
   auto& texComponent = Game::Game::GetInstance().texComponent();
   texComponent.DrawTexture(firstX, static_cast<int>(y),
      DefalutScale, DefalutAngle, _grHandles[fifthNo]);
   texComponent.DrawTexture(secondX, static_cast<int>(y),
      DefalutScale, DefalutAngle, _grHandles[fourthNo]);
   texComponent.DrawTexture(thirdX, static_cast<int>(y),
      DefalutScale, DefalutAngle, _grHandles[thirdNo]);
   texComponent.DrawTexture(fourthX, static_cast<int>(y),
      DefalutScale, DefalutAngle, _grHandles[secondNo]);
   texComponent.DrawTexture(fifthX, static_cast<int>(y),
      DefalutScale, DefalutAngle, _grHandles[firstNo]);
}

