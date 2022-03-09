#include "ModeLoading.h"
#include "GameMain.h"

namespace {
   constexpr auto DefaultScale = 1.0;
   constexpr auto DefaultAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeLoading::ModeLoading(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeLoading::Init() {
   GetLoadJson().LoadTextures("ingame");
   _grHandle = GetResServer().GetTexture("LoadingBg");
   _grHandles = GetResServer().GetTextures("Loading");
}

void ModeLoading::Update() {
   if (GetASyncLoadNum() == 0) {
      GetModeServer().GoToMode("Poor",'L');
   }
}

void ModeLoading::Render() {
   GetTexComponent().DrawTexture(0,0, DefaultScale, DefaultAngle,_grHandle);
   GetTexComponent().DrawTexture(1420, 880, DefaultScale, DefaultAngle, _grHandles,2);
}

void ModeLoading::Exit() {
   SetUseASyncLoadFlag(false);
}