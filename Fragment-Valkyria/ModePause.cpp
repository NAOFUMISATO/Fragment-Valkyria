#include "ModePause.h"
#include "GameMain.h"

namespace {
   constexpr auto BgAlpha = 150;
   constexpr auto BoxWidth = 1920;      //!< DxLib::DrawBox横サイズ
   constexpr auto BoxHeight = 1080;     //!< DxLib::DrawBox縦サイズ
   constexpr auto DefaultScale = 1.0;
   constexpr auto DefaultAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModePause::ModePause(Game::GameMain& gameMain) :ModeBase {gameMain}{
}

void ModePause::Init() {
   _grHandle = GetResServer().GetTexture("Pause");

}

void ModePause::Enter() {
   int width, height;
   GetGraphSize(_grHandle, &width, &height);
   auto PosX = ScreenWidth / 2 - width / 2;
   auto PosY = ScreenHeight / 2 - height / 2;
   _pausePos.SetVec2(PosX, PosY);
}

void ModePause::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().StartClick()) {
      GetModeServer().PopBack();
   }
}

void ModePause::Update() {

}

void ModePause::Render() {
   auto [x, y] = _pausePos.GetVec2();
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, BgAlpha);
   DrawBox(0, 0, BoxWidth, BoxHeight, AppFrame::Math::Utility::GetColorCode(0, 0, 0), TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   GetTexComponent().DrawTexture(static_cast<int>(x), static_cast<int>(y),DefaultScale, DefaultAngle,_grHandle);
}
