
#include "ModeGameOver.h"
#include "GameMain.h"
using namespace FragmentValkyria::Mode;

namespace {
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

ModeGameOver::ModeGameOver(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeGameOver::Init() {
   auto& loadJson = GetLoadJson();
   loadJson.LoadTextures("over");


   auto& resServer = GetResServer();

   _grHandle = resServer.GetTexture("OverBg");

}

void ModeGameOver::Enter() {
}

void ModeGameOver::Input(AppFrame::Input::InputManager& input) {
   if (input.GetMouse().LeftClick()) {
      GetModeServer().GoToMode("Title", 'S');
   }
}

void ModeGameOver::Update() {
}

void ModeGameOver::Render() {
   GetSimpTexComponent().DrawTexture(0, 0, DefaultGraphScale, DefaultGraphAngle, _grHandle);
}


