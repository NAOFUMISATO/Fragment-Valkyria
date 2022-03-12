#include "ModeTeam.h"
#include "GameMain.h"

namespace {

}

using namespace FragmentValkyria::Mode;

ModeTeam::ModeTeam(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeTeam::Init() {
   _grHandle = GetResServer().GetTexture("TeamLogo");
}

void ModeTeam::Enter() {
   _cntInit = false;
}

void ModeTeam::Update() {
   if (!_cntInit) {
      _fadeCnt = _gameMain.modeServer().frameCount();
      _cntInit = true;
   }
   auto frame = _gameMain.modeServer().frameCount() - _fadeCnt;
   if (frame > 30) {
      GetModeServer().GoToMode("Title");
   }
}

void ModeTeam::Render() {
  GetTexComponent().DrawTexture(0, 0, 1.0, 0.0, _grHandle);
}
