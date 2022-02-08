
/*****************************************************************//**
 * \file   ModeTitle.cpp
 * \brief  モードタイトル
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
      "start_x","start_y","option_x","option_y","end_x","end_y"});
   const int TitleBgPosX = paramMap["bg_x"];        //!< タイトル背景X位置
   const int TitleBgPosY = paramMap["bg_y"];        //!< タイトル背景Y位置
   const int GameTitlePosX = paramMap["title_x"];   //!< ゲームタイトルX位置
   const int GameTitlePosY = paramMap["title_y"];   //!< ゲームタイトルY位置
   const int AnyBottonPosX = paramMap["anybotton_x"];  //!< スタートガイドX位置
   const int AnyBottonPosY = paramMap["anybotton_y"];  //!< スタートガイドY位置
   const int GameStartPosX = paramMap["start_x"];
   const int GameStartPosY = paramMap["start_y"];
   const int OptionPosX = paramMap["option_x"];
   const int OptionPosY = paramMap["option_y"];
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
      resServer.GetTexture("Option"),
      resServer.GetTexture("End"),
      resServer.GetTexture("TitleCusor")
   };

   _stateServer = std::make_unique<AppFrame::State::StateServer>("AnyBotton", std::make_shared <StateAnyBotton>(*this));
   _stateServer->Register("StartSelect", std::make_shared<StateStartSelect>(*this));
   _stateServer->Register("OptionSelect", std::make_shared<StateOptionSelect>(*this));
   _stateServer->Register("EndSelect", std::make_shared<StateEndSelect>(*this));
}

void ModeTitle::Enter() {
   auto& soundServer = GetSoundServer();
   soundServer.Play("TitleBgm");
   _stateServer->PushBack("AnyBotton");
   _pushAnyBotton = false;
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
   auto [bgHandle, titleHandles, anyBottonHandles, startHandle,optionHandle, endHandle,cusorHandle] = _owner._grHandles;
   _owner.GetTexComponent().DrawTexture(TitleBgPosX, TitleBgPosY, DefaultGraphScale, DefaultGraphAngle, bgHandle);
   _owner.GetTexComponent().DrawTexture(GameTitlePosX, GameTitlePosY, DefaultGraphScale, DefaultGraphAngle, titleHandles, 2);
   if (!_owner._pushAnyBotton) {
      _owner.GetTexComponent().DrawTexture(AnyBottonPosX, AnyBottonPosX, DefaultGraphScale, DefaultGraphAngle, anyBottonHandles, 2);
   }
   if (_owner._pushAnyBotton) {
      _owner.GetTexComponent().DrawTexture(GameStartPosX, GameStartPosY, DefaultGraphScale, DefaultGraphAngle, startHandle);
      _owner.GetTexComponent().DrawTexture(OptionPosX, OptionPosY, DefaultGraphScale, DefaultGraphAngle, optionHandle);
      _owner.GetTexComponent().DrawTexture(GameEndPosX, GameEndPosY, DefaultGraphScale, DefaultGraphAngle, endHandle);
      auto [x, y] = _owner._cusorPos;
      SetDrawBlendMode(DX_BLENDMODE_ALPHA, _owner._alpha);
      _owner.GetTexComponent().DrawTexture(x, y, DefaultGraphScale, DefaultGraphAngle, cusorHandle);
      SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   }
}

void ModeTitle::StateAnyBotton::Enter() {
}

void ModeTitle::StateAnyBotton::Input(InputManager& input){
   if (input.GetXJoypad().XClick()|| input.GetXJoypad().YClick()|| input.GetXJoypad().AClick()|| input.GetXJoypad().BClick()) {
      _owner._stateServer->GoToState("StartSelect");
   }
}

void ModeTitle::StateAnyBotton::Exit() {
   _owner._pushAnyBotton = true;
}


void ModeTitle::StateStartSelect::Enter() {
   _owner._cusorPos = { GameStartPosX ,GameStartPosY };
}

void ModeTitle::StateStartSelect::Input(InputManager& input) {
   if (input.GetXJoypad().DDownClick()) {
      _owner._stateServer->GoToState("OptionSelect");
   }
   if (input.GetXJoypad().AClick()) {
      _owner.GetModeServer().GoToMode("Boss", 'L');
   }
}

void ModeTitle::StateOptionSelect::Enter() {
   _owner._cusorPos = { OptionPosX ,OptionPosY };
}

void ModeTitle::StateOptionSelect::Input(InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner._stateServer->GoToState("StartSelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      _owner._stateServer->GoToState("EndSelect");
   }
   if (input.GetXJoypad().AClick()) {
      _owner.GetModeServer().PushBack("Option");
   }
}

void ModeTitle::StateEndSelect::Enter() {
   _owner._cusorPos = { GameEndPosX ,GameEndPosY };
}

void ModeTitle::StateEndSelect::Input(InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner._stateServer->GoToState("OptionSelect");
   }
   if (input.GetXJoypad().AClick()) {
      _owner._gameMain.ShutDown();
   }
}