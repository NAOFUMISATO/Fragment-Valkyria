
/*****************************************************************//**
 * \file   ModeGameOver.cpp
 * \brief  モードゲームオーバー
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeGameOver.h"
#include "Game.h"
#include "ParamModeGameOver.h"

namespace {
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeGameOver::ModeGameOver() {
   _param = std::make_unique<Param::ParamModeGameOver>("over");
}

void ModeGameOver::Init() {
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _handleMap = {
      {"OverBg",resServer.GetTextures("OverBg")},
      {"ContinueSelect",resServer.GetTextures("ContinueSelect")},
      {"ContinueNoSelect",resServer.GetTextures("ContinueNoSelect")},
      {"ExitSelect",resServer.GetTextures("ExitSelect")},
      {"ExitNoSelect",resServer.GetTextures("ExitNoSelect")}
   };

   _stateServer = std::make_unique<AppFrame::State::StateServer>("Continue", 
      std::make_shared <StateContinue>(*this));
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
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   auto& texComponent = Game::Game::GetInstance().texComponent();
   texComponent.DrawTexture(0, 0, DefaultGraphScale, DefaultGraphAngle,
      _owner._handleMap["OverBg"][0]);
   texComponent.DrawTexture(_IntParam("continue_x"), _IntParam("continue_y"),
      DefaultGraphScale, DefaultGraphAngle, _owner._continueDrawHandles, 2);
   texComponent.DrawTexture(_IntParam("exit_x"), _IntParam("exit_y"),
      DefaultGraphScale, DefaultGraphAngle, _owner._exitDrawHandles, 2);
}

void ModeGameOver::StateContinue::Enter() {
   _owner._continueDrawHandles = _owner._handleMap["ContinueSelect"];
}

void ModeGameOver::StateContinue::Input(InputManager& input){
   if (input.GetXJoypad().DDownClick()) {
      _owner._stateServer->GoToState("Exit");
   }
   if (input.GetXJoypad().AClick()) {
      auto& gameInstance = Game::Game::GetInstance();
      gameInstance.isPoorClear(false);
      auto& soundComponent = gameInstance.soundComponent();
      soundComponent.Stop("TutorialBgm");
      soundComponent.Stop("PoorBattleBgm");
      soundComponent.Stop("BossBattleBgm");
      soundComponent.Stop("GameOver");
      auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
      modeServer.GoToMode("Loading",'S');
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
      auto& soundComponent = Game::Game::GetInstance().soundComponent();
      soundComponent.Stop("TutorialBgm");
      soundComponent.Stop("PoorBattleBgm");
      soundComponent.Stop("BossBattleBgm");
      soundComponent.Stop("GameOver");
      auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
      modeServer.GoToMode("Title");
   }
}

void ModeGameOver::StateExit::Exit() {
   _owner._exitDrawHandles = _owner._handleMap["ExitNoSelect"];
}
