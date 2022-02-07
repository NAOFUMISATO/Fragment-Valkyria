
/*****************************************************************//**
 * \file   ModeTitle.cpp
 * \brief  ���[�h�^�C�g��
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeTitle.h"
#include "GameMain.h"
using namespace FragmentValkyria::Mode;

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("title",
      { "bg_x","bg_y","title_x","title_y","anybotton_x" ,"anybotton_y",
      "start_x","start_y","end_x","end_y"});
   const int TitleBgPosX = paramMap["bg_x"];        //!< �^�C�g���w�iX�ʒu
   const int TitleBgPosY = paramMap["bg_y"];        //!< �^�C�g���w�iY�ʒu
   const int GameTitlePosX = paramMap["title_x"];   //!< �Q�[���^�C�g��X�ʒu
   const int GameTitlePosY = paramMap["title_y"];   //!< �Q�[���^�C�g��Y�ʒu
   const int AnyBottonPosX = paramMap["anybotton_x"];  //!< �X�^�[�g�K�C�hX�ʒu
   const int AnyBottonPosY = paramMap["anybotton_y"];  //!< �X�^�[�g�K�C�hY�ʒu
   const int GameStartPosX = paramMap["start_x"];
   const int GameStartPosY = paramMap["start_y"];
   const int GameEndPosX = paramMap["end_x"];
   const int GameEndPosY = paramMap["end_y"];
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

ModeTitle::ModeTitle(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeTitle::Init() {
   auto& loadJson = GetLoadJson();
   loadJson.LoadTextures("title");
   loadJson.LoadSounds("title");

   auto& resServer = GetResServer();
   
   _grHandles = {
      resServer.GetTexture("TitleBg"),
      resServer.GetTextures("TitleLogo"),
      resServer.GetTextures("AnyBotton"),
      resServer.GetTexture("Start"),
      resServer.GetTexture("End"),
      resServer.GetTexture("Cusor")
   };

   auto state = std::make_unique<AppFrame::State::StateServer>("AnyBotton", std::make_shared <StateAnyBotton>(*this));
   state->Register("StartSelect", std::make_shared<StateStartSelect>(*this));
   state->Register("EndSelect", std::make_shared<StateEndSelect>(*this));
   _stateServer = std::move(state);
}

void ModeTitle::Enter() {
   auto& soundServer = GetSoundServer();
   soundServer.Play("TitleBgm");
   _alpha = 255;
}

void ModeTitle::Input(AppFrame::Input::InputManager& input) {
   _stateServer->Input(input);
}

void ModeTitle::Update() {
   _stateServer->Update();
   _alpha = (_alpha + 4) % 255;
}

void ModeTitle::Render() {
   _stateServer->Draw();
}

void ModeTitle::StateBase::Draw() {
   auto [bgHandle, titleHandles, anyBottonHandles, startHandle, endHandle,cusorHandle] = _owner._grHandles;
   _owner.GetTexComponent().DrawTexture(TitleBgPosX, TitleBgPosY, DefaultGraphScale, DefaultGraphAngle, bgHandle);
   _owner.GetTexComponent().DrawTexture(GameTitlePosX, GameTitlePosY, DefaultGraphScale, DefaultGraphAngle, titleHandles, 2);
   if (_owner._anyBotton) {
      _owner.GetTexComponent().DrawTexture(AnyBottonPosX, AnyBottonPosX, DefaultGraphScale, DefaultGraphAngle, anyBottonHandles, 2);
   }
   if (!_owner._anyBotton) {
      _owner.GetTexComponent().DrawTexture(GameStartPosX, GameStartPosY, DefaultGraphScale, DefaultGraphAngle, startHandle);
      _owner.GetTexComponent().DrawTexture(GameEndPosX, GameEndPosY, DefaultGraphScale, DefaultGraphAngle, endHandle);
      auto [x, y] = _owner._cusorPos;
      SetDrawBlendMode(DX_BLENDMODE_ALPHA, _owner._alpha);
      _owner.GetTexComponent().DrawTexture(x, y, DefaultGraphScale, DefaultGraphAngle, cusorHandle);
      SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   }
}

void ModeTitle::StateAnyBotton::Enter() {
   _owner._anyBotton = true;
}

void ModeTitle::StateAnyBotton::Input(InputManager& input){
   if (input.GetXJoypad().XClick()|| input.GetXJoypad().YClick()|| input.GetXJoypad().AClick()|| input.GetXJoypad().BClick()) {
      _owner._stateServer->GoToState("StartSelect");
   }
}

void ModeTitle::StateAnyBotton::Update() {
   
}

void ModeTitle::StateAnyBotton::Exit() {
   _owner._anyBotton = false;
}


void ModeTitle::StateStartSelect::Enter() {
   _owner._cusorPos = { GameStartPosX ,GameStartPosY };
}

void ModeTitle::StateStartSelect::Input(InputManager& input) {
   if (input.GetXJoypad().DDownClick()) {
      _owner._stateServer->GoToState("EndSelect");
   }
   if (input.GetXJoypad().AClick()) {
      _owner.GetModeServer().GoToMode("Boss", 'L');
   }
}


void ModeTitle::StateStartSelect::Update() {

}

void ModeTitle::StateStartSelect::Exit() {
}

void ModeTitle::StateEndSelect::Enter() {
   _owner._cusorPos = { GameEndPosX ,GameEndPosY };
}

void ModeTitle::StateEndSelect::Input(InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner._stateServer->GoToState("StartSelect");
   }
   if (input.GetXJoypad().AClick()) {
      _owner._gameMain.ShutDown();
   }
}


void ModeTitle::StateEndSelect::Update() {

}

void ModeTitle::StateEndSelect::Exit() {

}


