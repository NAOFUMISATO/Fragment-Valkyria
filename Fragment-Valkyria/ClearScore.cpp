#include "ClearScore.h"
#include "GameMain.h"

using namespace FragmentValkyria::Clear;

ClearScore::ClearScore(Game::GameMain& gameMain) :SpriteBase{ gameMain } {
}

void ClearScore::Init() {
   _position = { 910,750,0 };
   auto handles = GetResServer().GetTextures("ClearScore");
   auto timer = _gameMain.ingameTimer();
   if (timer >= 60*40) {
      _grHandle = handles[2];
   }
   else if (timer >= 60 * 30 && 60 * 40 > timer) {
      _grHandle = handles[1];
   }
   else if (timer >= 60 * 20 && 60 * 30 > timer) {
      _grHandle = handles[0];
   }
   else if (60 * 20 > timer) {
      _grHandle = handles[3];
   }
}

void ClearScore::Draw() {
   SpriteBase::Draw();
}
