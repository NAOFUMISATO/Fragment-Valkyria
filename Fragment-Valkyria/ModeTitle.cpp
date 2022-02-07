
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
      { "bg_x","bg_y","title_x","title_y","guide_x" ,"guide_y" });
   const int TitleBgPosX = paramMap["bg_x"];        //!< �^�C�g���w�iX�ʒu
   const int TitleBgPosY = paramMap["bg_y"];        //!< �^�C�g���w�iY�ʒu
   const int GameTitlePosX = paramMap["title_x"];   //!< �Q�[���^�C�g��X�ʒu
   const int GameTitlePosY = paramMap["title_y"];   //!< �Q�[���^�C�g��Y�ʒu
   const int AnyBottonPosX = paramMap["guide_x"];  //!< �X�^�[�g�K�C�hX�ʒu
   const int AnyBottonPosY = paramMap["guide_y"];  //!< �X�^�[�g�K�C�hY�ʒu
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
      resServer.GetTexture("End")
   };

   auto state = std::make_unique<AppFrame::State::StateServer>("AnyBotton", std::make_shared <StateAnyBotton>(*this));
   state->Register("StartSelect", std::make_shared<StateStartSelect>(*this));
   state->Register("EndSelect", std::make_shared<StateEndSelect>(*this));
   _stateServer = std::move(state);
}

void ModeTitle::Enter() {
   auto& soundServer = GetSoundServer();
   soundServer.Play("TitleBgm");
}

void ModeTitle::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().BClick()) {
      GetModeServer().GoToMode("Boss", 'L');
   }
}

void ModeTitle::Update() {
}

void ModeTitle::Render() {
   auto [bgGrHandle, titleGrHandles,] = _grHandles;
   GetTexComponent().DrawTexture(TitleBgPosX, TitleBgPosY, DefaultGraphScale, DefaultGraphAngle, bgGrHandle);
   _stateServer->Draw();
   GetTexComponent().DrawTexture(GameTitlePosX, GameTitlePosY, DefaultGraphScale, DefaultGraphAngle, titleGrHandles, 2);

}

void ModeTitle::StateBase::Draw() {
   _owner.Render();
}

void ModeTitle::StateAnyBotton::Enter() {

}

void ModeTitle::StateAnyBotton::Input(InputManager& input){
   if (input.GetXJoypad().XClick()|| input.GetXJoypad().YClick()|| input.GetXJoypad().AClick()|| input.GetXJoypad().BClick()) {
      _owner._stateServer->GoToState("StartSelect");
   }
}

void ModeTitle::StateAnyBotton::Update() {
   
}

void ModeTitle::StateAnyBotton::Draw() {
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
   _owner.GetTexComponent().DrawTexture(AnyBottonPosX, AnyBottonPosY, DefaultGraphScale, DefaultGraphAngle, guideGrHandle, 2);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


