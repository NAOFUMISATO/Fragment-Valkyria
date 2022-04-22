
/*****************************************************************//**
 * \file   ModeTeam.cpp
 * \brief  モードチームロゴ
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeTeam.h"
#include "GameMain.h"

using namespace FragmentValkyria::Mode;

ModeTeam::ModeTeam() {
}

void ModeTeam::Init() {
   _grHandle = GetResServer().GetTexture("TeamLogo");
}

void ModeTeam::Enter() {
   _cntInit = false;
}

void ModeTeam::Update() {
   auto gameInstance = Game::GameMain::GetInstance();
   if (!_cntInit) {
      _fadeCnt = gameInstance->modeServer().frameCount();
      _cntInit = true;
   }
   auto frame = gameInstance->modeServer().frameCount() - _fadeCnt;
   if (frame > 30) {
      GetModeServer().GoToMode("Title");
   }
}

void ModeTeam::Render() {
  GetTexComponent().DrawTexture(0, 0, 1.0, 0.0, _grHandle);
}
