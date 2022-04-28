
/*****************************************************************//**
 * \file   ModeLoading.cpp
 * \brief  ÉÇÅ[Éhì«Ç›çûÇ›
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeLoading.h"
#include "Game.h"

namespace {
   constexpr auto LoadingX = 1420;
   constexpr auto LoadingY = 880;
   constexpr auto DefaultAngle = 0.0;
   constexpr auto DefaultScale = 1.0;
}

using namespace FragmentValkyria::Mode;

ModeLoading::ModeLoading() {
}

void ModeLoading::Init() {
   auto& loadresJson = Game::Game::GetInstance().loadresJson();
   loadresJson.LoadTextures("ingame");
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandle = resServer.GetTexture("LoadingBg");
   _grHandles = resServer.GetTextures("Loading");
}

void ModeLoading::Update() {
   if (GetASyncLoadNum() == 0) {
      SetUseASyncLoadFlag(false);
      auto& gameInstance = Game::Game::GetInstance();
      auto& loadresJson = gameInstance.loadresJson();
      loadresJson.LoadModels("ingame");
      auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
      if (!gameInstance.isTutorialClear()) {
         modeServer.GoToMode("Tutorial", 'S');
      }
      else if (!gameInstance.isPoorClear()) {
         modeServer.GoToMode("Poor", 'L');
      }
      else {
         modeServer.GoToMode("Boss", 'L');
      }
   }
}

void ModeLoading::Render() {
   auto& texComponent = Game::Game::GetInstance().texComponent();
   texComponent.DrawTexture(0,0,
      DefaultScale, DefaultAngle,_grHandle);
   texComponent.DrawTexture(LoadingX, LoadingY,
      DefaultScale, DefaultAngle, _grHandles,1);
}