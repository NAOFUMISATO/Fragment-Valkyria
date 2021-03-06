
/*****************************************************************//**
 * \file   ModeLoading.cpp
 * \brief  モード読み込み
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
   auto& loadresJson = Game::Game::GetLoadresJson();
   loadresJson.LoadTextures("ingame");
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandle = resServer.GetTexture("LoadingBg");
   _grHandles = resServer.GetTextures("Loading");
}

void ModeLoading::Update() {
   if (GetASyncLoadNum() == 0) {
      SetUseASyncLoadFlag(false);
      auto& loadresJson = Game::Game::GetLoadresJson();
      loadresJson.LoadModels("ingame");
      auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
      auto& gameInstance = Game::Game::GetInstance();
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
   auto& texComponent = Game::Game::GetTexComponent();
   texComponent.DrawTexture(0,0,
      DefaultScale, DefaultAngle,_grHandle);
   texComponent.DrawTexture(LoadingX, LoadingY,
      DefaultScale, DefaultAngle, _grHandles,1);
}