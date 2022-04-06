
/*****************************************************************//**
 * \file   ModeClearResult.cpp
 * \brief  モードクリア画面
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeClearResult.h"
#include "ClearTime.h"
#include "ClearScore.h"
#include "SpriteServer.h"

using namespace FragmentValkyria::Mode;

ModeClearResult::ModeClearResult(Game::GameMain& gameMain) : ModeBase{gameMain} {
}

void ModeClearResult::Init() {
   _bgHandle = GetResServer().GetTexture("ClearBg");
}

void ModeClearResult::Enter() {
   _born = true;
}

void ModeClearResult::Input(AppFrame::Input::InputManager& input) {
   GetSprServer().Input(input);
   if (input.GetXJoypad().AClick()) {
      GetSoundComponent().Stop("BossBattleBgm");
      GetSoundComponent().Stop("ClearResult");
      GetModeServer().GoToMode("Title");
   }
}

void ModeClearResult::Update() {
   if (_born) {
      auto& sprServer = GetSprServer();
      sprServer.Add(std::make_unique<Clear::ClearTime>(_gameMain));
      sprServer.Add(std::make_unique<Clear::ClearScore>(_gameMain));
      _born = false;
   }
   GetSprServer().Update();
}

void ModeClearResult::Render() {
   GetTexComponent().DrawTexture(0,0,1.0,0.0,_bgHandle);
   GetSprServer().Render();
}

void ModeClearResult::Exit() {
   GetSprServer().Clear();
}
