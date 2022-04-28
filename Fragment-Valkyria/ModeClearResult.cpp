
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
#include "Game.h"

using namespace FragmentValkyria::Mode;

ModeClearResult::ModeClearResult() {
}

void ModeClearResult::Init() {
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _bgHandle = resServer.GetTexture("ClearBg");
}

void ModeClearResult::Enter() {
   _born = true;
}

void ModeClearResult::Input(AppFrame::Input::InputManager& input) {
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.sprServer().Input(input);
   if (input.GetXJoypad().AClick()) {
      auto& soundComponent = gameInstance.soundComponent();
      soundComponent.Stop("BossBattleBgm");
      soundComponent.Stop("ClearResult");
      auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
      modeServer.GoToMode("Title");
   }
}

void ModeClearResult::Update() {
   auto& sprServer = Game::Game::GetInstance().sprServer();
   if (_born) {
      sprServer.Add(std::make_unique<Clear::ClearTime>());
      sprServer.Add(std::make_unique<Clear::ClearScore>());
      _born = false;
   }
   sprServer.Update();
}

void ModeClearResult::Render() {
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.texComponent().DrawTexture(0,0,1.0,0.0,_bgHandle);
   gameInstance.sprServer().Render();
}

void ModeClearResult::Exit() {
   auto& sprServer = Game::Game::GetInstance().sprServer();
   sprServer.Clear();
}
