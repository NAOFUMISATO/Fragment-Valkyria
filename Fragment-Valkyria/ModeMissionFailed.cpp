
/*****************************************************************//**
 * \file   ModeMissionFailed.cpp
 * \brief  モードミッション失敗
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeMissionFailed.h"
#include "GameMain.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("over", { "missionfailed_x","missionfailed_y","missionfailed_animespeed","bg_alpha" });
   const int MissionFailedX = paramMap["missionfailed_x"];
   const int MissionFailedY = paramMap["missionfailed_y"];
   const int MissionFailedAnimeSpeed = paramMap["missionfailed_animespeed"];
   const int BgAlpha = paramMap["bg_alpha"];
   constexpr auto BoxWidth = 1920;      //!< DxLib::DrawBox横サイズ
   constexpr auto BoxHeight = 1080;     //!< DxLib::DrawBox縦サイズ
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
   constexpr auto FailedAnimeSpeed = 1;
}

using namespace FragmentValkyria::Mode;

ModeMissionFailed::ModeMissionFailed(Game::GameMain& gameMain) :ModeBase{gameMain} {
}

void ModeMissionFailed::Init() {
   GetLoadJson().LoadTextures("over");
   _grHandles = GetResServer().GetTextures("MissionFailed");
}

void ModeMissionFailed::Enter() {
   _animeNo = 0;
   _cntInit = false;
}

void ModeMissionFailed::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().AClick()) {
      GetModeServer().PopBack();
      GetModeServer().GoToMode("GameOver", 'S');
   }
}

void ModeMissionFailed::Update() {
   if (!_cntInit) {
      _animeCnt = _gameMain.modeServer().frameCount();
      _cntInit = true;
   }
   auto gameCount = static_cast<int>(_gameMain.modeServer().frameCount());
   auto frame = gameCount - _animeCnt;
   auto allNum = std::get<0>(GetResServer().GetTextureInfo("MissionFailed").GetDivParams());
   if (frame < allNum * FailedAnimeSpeed) {
      _animeNo = frame / FailedAnimeSpeed % allNum;
   }
}

void ModeMissionFailed::Render() {
   SetDrawBlendMode(DX_BLENDMODE_ALPHA,BgAlpha);
   DrawBox(0, 0, BoxWidth, BoxHeight, AppFrame::Math::Utility::GetColorCode(0, 0, 0), TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   DrawGraph(MissionFailedX, MissionFailedY, _grHandles[_animeNo], true);
}