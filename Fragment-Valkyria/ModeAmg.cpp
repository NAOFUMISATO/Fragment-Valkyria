
/*****************************************************************//**
 * \file   ModeAmg.cpp
 * \brief  ƒ‚[ƒhAMGƒƒS
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeAmg.h"
#include "Game.h"

namespace {
   constexpr auto FadeCount = 30;
}

using namespace FragmentValkyria::Mode;

ModeAmg::ModeAmg() {
}

void ModeAmg::Init() {
   auto& loadresJson = Game::Game::GetLoadresJson();
   loadresJson.LoadTextures("title");
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandle = resServer.GetTexture("AmgLogo");
}

void ModeAmg::Enter() {
   _cntInit = false;
}

void ModeAmg::Update() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   if (!_cntInit) {
      _fadeCnt = modeServer.frameCount();
      _cntInit = true;
   }
   auto frame = modeServer.frameCount() - _fadeCnt;
   if (frame > FadeCount) {
      modeServer.GoToMode("Team");
   }
}

void ModeAmg::Render() {
   auto& texComponent = Game::Game::GetTexComponent();
   texComponent.DrawTexture(0, 0, 1.0, 0.0, _grHandle);
}

