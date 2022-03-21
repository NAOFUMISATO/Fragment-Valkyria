
/*****************************************************************//**
 * \file   ModeMissionCompleted.cpp
 * \brief  モードミッション完遂
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeMissionCompleted.h"

namespace {
   // Jsonファイルから各値を取得する
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("clear", {"missioncomp_x","missioncomp_y","missioncomp_animespeed","bg_alpha" });
   const int MissionCompX = paramMap["missioncomp_x"];
   const int MissionCompY = paramMap["missioncomp_y"];
   const int MissionCompAnimeSpeed = paramMap["missioncomp_animespeed"];
   const int BgAlpha= paramMap["bg_alpha"];
   constexpr auto BoxWidth = 1920;
   constexpr auto BoxHeight = 1080;
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeMissionCompleted::ModeMissionCompleted(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeMissionCompleted::Init() {
   GetLoadJson().LoadTextures("clear");
   _grHandles = GetResServer().GetTextures("MissionCompleted");
}

void ModeMissionCompleted::Enter() {
   GetSoundComponent().Play("ClearResult");
}

void ModeMissionCompleted::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().AClick()) {
      GetModeServer().PopBack();
      GetModeServer().GoToMode("ClearResult", 'S');
   }
}

void ModeMissionCompleted::Render() {
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, BgAlpha);
   DrawBox(0, 0, BoxWidth, BoxHeight, AppFrame::Math::Utility::GetColorCode(0, 0, 0), TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   GetTexComponent().DrawTexture(MissionCompX, MissionCompY, DefaultGraphScale, DefaultGraphAngle, _grHandles, MissionCompAnimeSpeed);
}