
/*****************************************************************//**
 * \file   ModeTeam.cpp
 * \brief  モードチームロゴ
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeTeam.h"
#include "Game.h"

namespace {
   constexpr auto FadeFrame = 30;
}

using namespace FragmentValkyria::Mode;

ModeTeam::ModeTeam() {
}

void ModeTeam::Init() {
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandle = resServer.GetTexture("TeamLogo");
}

void ModeTeam::Enter() {
   _cntInit = false;
}

void ModeTeam::Update() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   if (!_cntInit) {
      _fadeCnt = modeServer.frameCount();
      _cntInit = true;
   }
   auto frame = modeServer.frameCount() - _fadeCnt;
   if (frame > FadeFrame) {
      modeServer.GoToMode("Title");
   }
}

void ModeTeam::Render() {
   auto& texComponent = Game::Game::GetTexComponent();
   texComponent.DrawTexture(0, 0, 1.0, 0.0, _grHandle);
}
