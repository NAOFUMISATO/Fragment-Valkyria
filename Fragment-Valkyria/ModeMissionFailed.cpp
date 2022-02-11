
/*****************************************************************//**
 * \file   ModeMissionFailed.cpp
 * \brief  ���[�h�~�b�V�������s
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeMissionFailed.h"

namespace {
   constexpr auto BgAlpha = 150;
   constexpr auto BoxWidth = 1920;      //!< DxLib::DrawBox���T�C�Y
   constexpr auto BoxHeight = 1080;     //!< DxLib::DrawBox�c�T�C�Y
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeMissionFailed::ModeMissionFailed(Game::GameMain& gameMain) :ModeBase{gameMain} {
}

void ModeMissionFailed::Init() {
   GetLoadJson().LoadTextures("over");
   _grHandle = GetResServer().GetTexture("MissionFailed");
}

void ModeMissionFailed::Enter() {
}

void ModeMissionFailed::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().AClick()) {
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