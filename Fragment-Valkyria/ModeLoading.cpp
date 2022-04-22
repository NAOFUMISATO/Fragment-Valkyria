
/*****************************************************************//**
 * \file   ModeLoading.cpp
 * \brief  ƒ‚[ƒh“Ç‚Ýž‚Ý
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeLoading.h"
#include "GameMain.h"

namespace {
   constexpr auto DefaultScale = 1.0;
   constexpr auto DefaultAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeLoading::ModeLoading() {
}

void ModeLoading::Init() {
   GetLoadJson().LoadTextures("ingame");
   _grHandle = GetResServer().GetTexture("LoadingBg");
   _grHandles = GetResServer().GetTextures("Loading");
}

void ModeLoading::Update() {
   if (GetASyncLoadNum() == 0) {
      SetUseASyncLoadFlag(false);
      GetLoadJson().LoadModels("ingame");
      auto gameInstance = Game::GameMain::GetInstance();
      if (!gameInstance->isTutorialClear()) {
         GetModeServer().GoToMode("Tutorial", 'S');
      }
      else if (!gameInstance->isPoorClear()) {
         GetModeServer().GoToMode("Poor", 'L');
      }
      else {
         GetModeServer().GoToMode("Boss", 'L');
      }
   }
}

void ModeLoading::Render() {
   GetTexComponent().DrawTexture(0,0, DefaultScale, DefaultAngle,_grHandle);
   GetTexComponent().DrawTexture(1420, 880, DefaultScale, DefaultAngle, _grHandles,1);
}