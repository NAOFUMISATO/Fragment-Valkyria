#include "ClearTime.h"
#include "GameMain.h"

namespace {
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
   _position = { 600,540,0 };
   auto [x, y] = _position.GetVec2();
   _xPositions = {
      static_cast<int>(x),
      static_cast<int>(x + 150.0),
      static_cast<int>(x + 150.0 * ThirdMultRate),
      static_cast<int>(x + 150.0 * FourthMultRate),
      static_cast<int>(x + 150.0 * FifthMultRate)
   };
}

void ClearTime::Input(AppFrame::Input::InputManager& input) {
}

void ClearTime::Update() {
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

