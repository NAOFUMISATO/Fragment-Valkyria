
/*****************************************************************//**
 * \file   ModeTutorialSelect.cpp
 * \brief  モードチュートリアル選択
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeTutorialSelect.h"
#include "GameMain.h"

namespace {
   constexpr auto TutorialIsSkipX = 500;
   constexpr auto TutorialIsSkipY = 300;
   constexpr auto YesX = 860;
   constexpr auto YesY = 500;
   constexpr auto NoX = 860;
   constexpr auto NoY = 700;
}

using namespace FragmentValkyria::Mode;

ModeTutorialSelect::ModeTutorialSelect() {
}

void ModeTutorialSelect::Init() {
   GetLoadJson().LoadTextures("tutorial");

   auto& resServer = GetResServer();
   _handleMap = {
      {"TutorialCusor",   resServer.GetTexture("TutorialCusor")},
      {"TutorialIsSkip",  resServer.GetTexture("TutorialIsSkip")},
      {"TutorialSelectBg",resServer.GetTexture("TutorialSelectBg")},
      {"No",              resServer.GetTexture("No")},
      {"Yes",             resServer.GetTexture("Yes")}
   };

   _stateServer = std::make_unique<AppFrame::State::StateServer>("No", std::make_shared <StateNo>(*this));
   _stateServer->Register("Yes", std::make_shared<StateYes>(*this));
}

void ModeTutorialSelect::Enter() {
   _cusorX = NoX;
   _cusorY = NoY;
}

void ModeTutorialSelect::Input(AppFrame::Input::InputManager& input) {
   _stateServer->Input(input);
}

void ModeTutorialSelect::Update() {
   _stateServer->Update();
}

void ModeTutorialSelect::Render() {
   _stateServer->Draw();
}

void ModeTutorialSelect::StateBase::Draw() {
   _owner.GetTexComponent().DrawTexture(0, 0, 1.0, 0.0, _owner._handleMap["TutorialSelectBg"]);
   _owner.GetTexComponent().DrawTexture(TutorialIsSkipX, TutorialIsSkipY, 1.0, 0.0, _owner._handleMap["TutorialIsSkip"]);
   _owner.GetTexComponent().DrawTexture(YesX, YesY, 1.0, 0.0, _owner._handleMap["Yes"]);
   _owner.GetTexComponent().DrawTexture(NoX, NoY, 1.0, 0.0, _owner._handleMap["No"]);
   _owner.GetTexComponent().DrawTexture(_owner._cusorX, _owner._cusorY, 1.0, 0.0, _owner._handleMap["TutorialCusor"]);
}

void ModeTutorialSelect::StateYes::Enter() {
   _owner._cusorX = YesX;
   _owner._cusorY = YesY;
}

void ModeTutorialSelect::StateYes::Input(InputManager& input) {
   if (input.GetXJoypad().DDownClick() || input.GetKeyboard().DownClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("No");
   }
   if (input.GetXJoypad().AClick() || input.GetKeyboard().SpaceClick()) {
      auto gameInstance = Game::GameMain::GetInstance();
      gameInstance->isTutorialClear(true);
      _owner.GetSoundComponent().Play("SystemDecision");
      _owner.GetSoundComponent().Stop("TitleBgm");
      _owner.GetModeServer().GoToMode("Loading", 'S');
   }
}

void ModeTutorialSelect::StateNo::Enter() {
   _owner._cusorX = NoX;
   _owner._cusorY = NoY;
}

void ModeTutorialSelect::StateNo::Input(InputManager& input) {
   if (input.GetXJoypad().DUpClick() || input.GetKeyboard().UpClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("Yes");
   }
   if (input.GetXJoypad().AClick() || input.GetKeyboard().SpaceClick()) {
      _owner.GetSoundComponent().Play("SystemDecision");
      _owner.GetSoundComponent().Stop("TitleBgm"); 
      _owner.GetModeServer().GoToMode("Loading",'S');
   }
}
