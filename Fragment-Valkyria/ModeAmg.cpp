
/*****************************************************************//**
 * \file   ModeAmg.cpp
 * \brief  ƒ‚[ƒhAMGƒƒS
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeAmg.h"
#include "GameMain.h"

using namespace FragmentValkyria::Mode;

ModeAmg::ModeAmg() {
}

void ModeAmg::Init() {
   GetLoadJson().LoadTextures("title");
   _grHandle = GetResServer().GetTexture("AmgLogo");
}

void ModeAmg::Enter() {
   _cntInit = false;
}

void ModeAmg::Update() {
   auto gameInstance = Game::GameMain::GetInstance();
   if (!_cntInit) {
      _fadeCnt = gameInstance->modeServer().frameCount();
      _cntInit = true;
   }
   auto frame = gameInstance->modeServer().frameCount() - _fadeCnt;
   if (frame > 30) {
      GetModeServer().GoToMode("Team");
   }
}

void ModeAmg::Render() {
   auto gameInstance = Game::GameMain::GetInstance();
   gameInstance->texComponent().DrawTexture(0, 0, 1.0, 0.0, _grHandle);
}

