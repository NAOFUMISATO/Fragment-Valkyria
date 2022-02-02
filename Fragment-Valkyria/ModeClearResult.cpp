#include "ModeClearResult.h"
#include "ClearTime.h"

using namespace FragmentValkyria::Mode;

ModeClearResult::ModeClearResult(Game::GameMain& gameMain) : ModeBase{gameMain} {
}

void ModeClearResult::Init() {
   GetLoadJson().LoadTextures("clear");
   _bgHandle = GetResServer().GetTexture("ClearBg");
}

void ModeClearResult::Enter() {
}

void ModeClearResult::Input(AppFrame::Input::InputManager& input) {
   GetSprServer().Input(input);
   if (input.GetXJoypad().AClick()) {
      GetModeServer().GoToMode("Title");
   }
}

void ModeClearResult::Update() {
   if (_born) {
      GetSprServer().Add(std::make_unique<Clear::ClearTime>(_gameMain));
      _born = false;
   }
   GetSprServer().Update();
}

void ModeClearResult::Render() {
   GetSimpTexComponent().DrawTexture(0,0,1.0,0.0,_bgHandle);
   GetSprServer().Render();
}

void ModeClearResult::Exit() {
   GetSprServer().Clear();
}
