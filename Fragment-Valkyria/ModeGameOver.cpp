
/*****************************************************************//**
 * \file   ModeGameOver.cpp
 * \brief  ���[�h�Q�[���I�[�o�[
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeGameOver.h"
#include "GameMain.h"
using namespace FragmentValkyria::Mode;

namespace {
   // Json�t�@�C������e�l���擾����
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("over", {"continue_x","continue_y","exit_x","exit_y"});
   const int ContinuePosX = paramMap["continue_x"];
   const int ContinuePosY = paramMap["continue_y"];
   const int ExitPosX = paramMap["exit_x"];
   const int ExitPosY = paramMap["exit_y"];
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

ModeGameOver::ModeGameOver(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeGameOver::Init() {
   auto& resServer = GetResServer();
   _handleMap = {
      {"OverBg",resServer.GetTextures("OverBg")},
      {"ContinueSelect",resServer.GetTextures("ContinueSelect")},
      {"ContinueNoSelect",resServer.GetTextures("ContinueNoSelect")},
      {"ExitSelect",resServer.GetTextures("ExitSelect")},
      {"ExitNoSelect",resServer.GetTextures("ExitNoSelect")}
   };

   _stateServer = std::make_unique<AppFrame::State::StateServer>("Continue", std::make_shared <StateContinue>(*this));
   _stateServer->Register("Exit", std::make_shared<StateExit>(*this));
}

void ModeGameOver::Enter() {
   _continueDrawHandles = _handleMap["ContinueSelect"];
   _exitDrawHandles = _handleMap["ExitNoSelect"];
}

void ModeGameOver::Input(AppFrame::Input::InputManager& input) {
   _stateServer->Input(input);
}

void ModeGameOver::Update() {
   _stateServer->Update();
}

void ModeGameOver::Render() {
   _stateServer->Draw();
}

void ModeGameOver::StateBase::Draw() {
   _owner.GetTexComponent().DrawTexture(0, 0, DefaultGraphScale, DefaultGraphAngle, _owner._handleMap["OverBg"][0]);
   _owner.GetTexComponent().DrawTexture(ContinuePosX, ContinuePosY, DefaultGraphScale, DefaultGraphAngle, _owner._continueDrawHandles, 2);
   _owner.GetTexComponent().DrawTexture(ExitPosX, ExitPosY, DefaultGraphScale, DefaultGraphAngle, _owner._exitDrawHandles, 2);
}

void ModeGameOver::StateContinue::Enter() {
   _owner._continueDrawHandles = _owner._handleMap["ContinueSelect"];
}

void ModeGameOver::StateContinue::Input(InputManager& input){
   if (input.GetXJoypad().DDownClick()) {
      _owner._stateServer->GoToState("Exit");
   }
   if (input.GetXJoypad().AClick()) {
      _owner._gameMain.isPoorClear(false);
      _owner.GetSoundComponent().Stop("TutorialBgm");
      _owner.GetSoundComponent().Stop("PoorBattleBgm");
      _owner.GetSoundComponent().Stop("BossBattleBgm");
      _owner.GetSoundComponent().Stop("GameOver");
      _owner.GetModeServer().GoToMode("Loading",'S');
   }
}

void ModeGameOver::StateContinue::Exit() {
   _owner._continueDrawHandles = _owner._handleMap["ContinueNoSelect"];
}

void ModeGameOver::StateExit::Enter() {
   _owner._exitDrawHandles= _owner._handleMap["ExitSelect"];
}

void ModeGameOver::StateExit::Input(InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner._stateServer->GoToState("Continue");
   }
   if (input.GetXJoypad().AClick()) {
      _owner.GetSoundComponent().Stop("TutorialBgm");
      _owner.GetSoundComponent().Stop("PoorBattleBgm");
      _owner.GetSoundComponent().Stop("BossBattleBgm");
      _owner.GetSoundComponent().Stop("GameOver");
      _owner.GetModeServer().GoToMode("Title");
   }
}

void ModeGameOver::StateExit::Exit() {
   _owner._exitDrawHandles = _owner._handleMap["ExitNoSelect"];
}
