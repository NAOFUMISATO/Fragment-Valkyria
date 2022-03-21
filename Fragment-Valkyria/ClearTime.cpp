
/*****************************************************************//**
 * \file   ClearTime.cpp
 * \brief  クリアタイムを表示する
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ClearTime.h"
#include "GameMain.h"

namespace {
   // Jsonファイルから各値を取得する
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("clear", { "number_diffx" });
   const double NumberDiffX = paramMap["number_diffx"];
   // Jsonファイルから各Vector4データを取得する
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("clear", { "time_pos" });
   const auto TimePos = vecParamMap["time_pos"];

   constexpr auto ThirdMultRate = 2.0;
   constexpr auto FourthMultRate = 3.0;
   constexpr auto FifthMultRate = 4.0;
   constexpr auto DefalutScale = 3.0;
   constexpr auto DefalutAngle = 0.0;

}
using namespace FragmentValkyria::Clear;

ClearTime::ClearTime(Game::GameMain& gameMain) : Sprite::SpriteBase{ gameMain }{
}

void ClearTime::Init() {
   _grHandles = GetResServer().GetTextures("NumberAndColon");
   _position = TimePos;
   auto [x, y] = _position.GetVec2();
   _xPositions = {
      static_cast<int>(x),
      static_cast<int>(x + NumberDiffX),
      static_cast<int>(x + NumberDiffX * ThirdMultRate),
      static_cast<int>(x + NumberDiffX * FourthMultRate),
      static_cast<int>(x + NumberDiffX * FifthMultRate)
   };
   auto timer = _gameMain.ingameTimer();
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
   GetTexComponent().DrawTexture(firstX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[fifthNo]);
   GetTexComponent().DrawTexture(secondX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[fourthNo]);
   GetTexComponent().DrawTexture(thirdX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[thirdNo]);
   GetTexComponent().DrawTexture(fourthX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[secondNo]);
   GetTexComponent().DrawTexture(fifthX, static_cast<int>(y), DefalutScale, DefalutAngle, _grHandles[firstNo]);
}

