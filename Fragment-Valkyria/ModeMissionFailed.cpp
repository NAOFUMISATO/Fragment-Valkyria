#include "ModeMissionFailed.h"

namespace {
   constexpr auto BgAlpha = 150;
   constexpr auto BoxWidth = 1920;      //!< DxLib::DrawBox横サイズ
   constexpr auto BoxHeight = 1080;     //!< DxLib::DrawBox縦サイズ
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeMissionFailed::ModeMissionFailed(Game::GameMain& gameMain) :ModeBase{gameMain} {
}

void ModeMissionFailed::Init() {
   auto& loadJson = GetLoadJson();
   loadJson.LoadTextures("over");
   auto& resServer = GetResServer();
   _grHandle = resServer.GetTexture("MissionFailed");
}

void ModeMissionFailed::Enter() {
}

void ModeMissionFailed::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().BClick()) {
      GetModeServer().PopBack();
      GetModeServer().GoToMode("GameOver", 'S');
   }
}

void ModeMissionFailed::Update() {

}

void ModeMissionFailed::Render() {
   SetDrawBlendMode(DX_BLENDMODE_ALPHA,BgAlpha);
   DrawBox(0, 0, BoxWidth, BoxHeight, AppFrame::Math::Utility::GetColorCode(0, 0, 0), TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   GetTexComponent().DrawTexture(0,0, DefaultGraphScale, DefaultGraphAngle,_grHandle);
}