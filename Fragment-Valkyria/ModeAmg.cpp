#include "ModeAmg.h"
#include "GameMain.h"

namespace{

}

using namespace FragmentValkyria::Mode;

ModeAmg::ModeAmg(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeAmg::Init() {
   GetLoadJson().LoadTextures("title");
   _grHandle = GetResServer().GetTexture("AmgLogo");
}

void ModeAmg::Enter() {
   _cntInit = false;
}

void ModeAmg::Update() {
   if (!_cntInit) {
      _fadeCnt = _gameMain.modeServer().frameCount();
      _cntInit = true;
   }
   auto frame = _gameMain.modeServer().frameCount() - _fadeCnt;
   if (frame > 30) {
      GetModeServer().GoToMode("Team");
   }
}

void ModeAmg::Render() {
   _gameMain.texComponent().DrawTexture(0, 0, 1.0, 0.0, _grHandle);
}

