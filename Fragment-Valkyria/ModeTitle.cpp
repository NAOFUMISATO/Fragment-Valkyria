
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
      { "bg_x","bg_y","title_x","title_y","title_animespeed","anybutton_x" ,"anybutton_y","anybutton_animespeed",
      "start_x","start_y","start_animespeed","option_x","option_y","option_animespeed","end_x","end_y","end_animespeed" });
   const int TitleBgPosX = paramMap["bg_x"];                        //!< �^�C�g���w�iX�ʒu
   const int TitleBgPosY = paramMap["bg_y"];                        //!< �^�C�g���w�iY�ʒu
   const int GameTitlePosX = paramMap["title_x"];                   //!< �Q�[���^�C�g��X�ʒu
   const int GameTitlePosY = paramMap["title_y"];                   //!< �Q�[���^�C�g��Y�ʒu
   const int GameTitleAnimeSpeed = paramMap["title_animespeed"];    //!< �Q�[���^�C�g���A�j���[�V�������x
   const int AnyButtonPosX = paramMap["anybutton_x"];               //!< �v���X�G�j�C�{�^��X�ʒu
   const int AnyButtonPosY = paramMap["anybutton_y"];               //!< �v���X�G�j�C�{�^��Y�ʒu
   const int AnyButtonAnimeSpeed = paramMap["anybutton_animespeed"];//!< �v���X�G�j�C�{�^���A�j���[�V�������x
   const int GameStartPosX = paramMap["start_x"];                   //!< �Q�[���J�n�Z���N�gX�ʒu
   const int GameStartPosY = paramMap["start_y"];                   //!< �Q�[���J�n�Z���N�gY�ʒu
   const int GameStartAnimeSpeed = paramMap["start_animespeed"];    //!< �Q�[���J�n�A�j���[�V�������x
   const int OptionPosX = paramMap["option_x"];                     //!< �I�v�V�����Z���N�gX�ʒu
   const int OptionPosY = paramMap["option_y"];                     //!< �I�v�V�����Z���N�gY�ʒu
   const int OptionAnimeSpeed = paramMap["option_animespeed"];      //!< �I�v�V�����A�j���[�V�������x
   const int GameEndPosX = paramMap["end_x"];                       //!< �Q�[���I���Z���N�gX�ʒu
   const int GameEndPosY = paramMap["end_y"];                       //!< �Q�[���I���Z���N�gY�ʒu
   const int GameEndAnimeSpeed = paramMap["end_animespeed"];        //!< �Q�[���I���A�j���[�V�������x

   constexpr auto DefaultAnimeSpeed = 1;                            //!< �e�摜�A�j���[�V�������x
   constexpr auto TitleBgCX = 0;                                    //!< �^�C�g���w�i�摜X��_
   constexpr auto TitleBgCY = 0;                                    //!< �^�C�g���w�i�摜Y��_
   constexpr auto DefaultGraphScale = 1.0;                          //!< �e�摜�g�嗦
   constexpr auto DefaultGraphAngle = 0.0;                          //!< �e�摜�p�x
   constexpr auto FirstInputFrame = 120;                            //!< �G�j�C�{�^������ڍs��A�Q�[���J�n���󂯕t���Ȃ��t���[����
}

ModeTitle::ModeTitle(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeTitle::Init() {
   GetLoadJson().LoadSounds("title");

   auto& resServer = GetResServer();
   
   _handleMap = {
      {"TitleBg",          resServer.GetTextures("TitleBg") },
      {"TitleLogo",        resServer.GetTextures("TitleLogo") },
      {"AnyButton",        resServer.GetTextures("AnyButton") },
      {"GameStartNoSelect",resServer.GetTextures("GameStartNoSelect") },
      {"GameStartSelect",  resServer.GetTextures("GameStartSelect") },
      {"OptionNoSelect",   resServer.GetTextures("OptionNoSelect") },
      {"OptionSelect",     resServer.GetTextures("OptionSelect") },
      {"GameEndNoSelect",  resServer.GetTextures("GameEndNoSelect") },
      {"GameEndSelect",    resServer.GetTextures("GameEndSelect") }
   };

   _logoHandle = _handleMap["TitleLogo"][0];
   _startDrawHandles = _handleMap["GameStartNoSelect"];
   _optionDrawHandles= _handleMap["OptionNoSelect"];
   _endDrawHandles= _handleMap["GameEndNoSelect"];

   _stateServer = std::make_unique<AppFrame::State::StateServer>("AnyButton", std::make_shared <StateAnyButton>(*this));
   _stateServer->Register("StartSelect", std::make_shared<StateStartSelect>(*this));
   _stateServer->Register("OptionSelect", std::make_shared<StateOptionSelect>(*this));
   _stateServer->Register("EndSelect", std::make_shared<StateEndSelect>(*this));
}

void ModeTitle::Enter() {
   GetSoundComponent().Play("TitleBgm");
   _stateServer->PushBack("AnyButton");
   _logoHandle = _handleMap["TitleLogo"][0];
   _cntInit = false;
}

void ModeTitle::Input(AppFrame::Input::InputManager& input) {
   _stateServer->Input(input);
}

void ModeTitle::Update() {
   LogoAnimation();
   _stateServer->Update();
}

void ModeTitle::Render() {
   _stateServer->Draw();
}

void ModeTitle::LogoAnimation() {
   if (!_cntInit) {
      _logoCnt = _gameMain.modeServer().frameCount();
      _cntInit = true;
   }
   auto gameCount = static_cast<int>(_gameMain.modeServer().frameCount());
   auto frame = gameCount - _logoCnt;
   auto allNum = std::get<0>(GetResServer().GetTextureInfo("TitleLogo").GetDivParams());
   if (frame < allNum * GameTitleAnimeSpeed) {
      _logoHandle = _handleMap["TitleLogo"][(frame / GameTitleAnimeSpeed) % allNum];
   }
}

void ModeTitle::StateBase::Draw() {
   auto handleMap = _owner._handleMap;
   _owner.GetTexComponent().TransDrawTexture(TitleBgPosX, TitleBgPosY, TitleBgCX, TitleBgCY,
      DefaultGraphScale, DefaultGraphAngle, handleMap["TitleBg"], DefaultAnimeSpeed, false, false);
   DrawGraph(GameTitlePosX, GameTitlePosY, _owner._logoHandle, true);
   if (!_owner._pushAnyButton) {
      _owner.GetTexComponent().DrawTexture(AnyButtonPosX, AnyButtonPosX, DefaultGraphScale, DefaultGraphAngle, handleMap["AnyButton"], AnyButtonAnimeSpeed);
   }
   if (_owner._pushAnyButton) {
      _owner.GetTexComponent().DrawTexture(GameStartPosX, GameStartPosY, DefaultGraphScale, DefaultGraphAngle, _owner._startDrawHandles, GameStartAnimeSpeed);
      _owner.GetTexComponent().DrawTexture(OptionPosX, OptionPosY, DefaultGraphScale, DefaultGraphAngle, _owner._optionDrawHandles, OptionAnimeSpeed);
      _owner.GetTexComponent().DrawTexture(GameEndPosX, GameEndPosY, DefaultGraphScale, DefaultGraphAngle, _owner._endDrawHandles, GameEndAnimeSpeed);
   }
}

void ModeTitle::StateAnyButton::Enter() {
   _owner._pushAnyButton = false;
}

void ModeTitle::StateAnyButton::Input(InputManager& input){
   if (input.GetXJoypad().XClick()|| input.GetXJoypad().YClick()|| input.GetXJoypad().AClick()|| input.GetXJoypad().BClick()) {
      _owner._stateServer->GoToState("StartSelect");
   }
}

void ModeTitle::StateAnyButton::Exit() {
   _owner._pushAnyButton = true;
   _owner._firstInputCnt = _owner.GetModeServer().frameCount();
}


void ModeTitle::StateStartSelect::Enter() {
   _owner._startDrawHandles = _owner._handleMap["GameStartSelect"];
}

void ModeTitle::StateStartSelect::Input(InputManager& input) {
   auto frameCount = _owner.GetModeServer().frameCount() - _owner._firstInputCnt;
   if (input.GetXJoypad().DDownClick()) {
      _owner._stateServer->GoToState("OptionSelect");
   }
   if (frameCount > FirstInputFrame) {
      if (input.GetXJoypad().AClick()) {
         if (GetASyncLoadNum() > 0) {
            _owner.GetModeServer().GoToMode("Loading", 'S');
         }
         else {
            _owner.GetModeServer().GoToMode("Poor");
            SetUseASyncLoadFlag(false);
         }
      }
   }
}

void ModeTitle::StateStartSelect::Exit() {
   _owner._startDrawHandles = _owner._handleMap["GameStartNoSelect"];
}

void ModeTitle::StateOptionSelect::Enter() {
   _owner._optionDrawHandles = _owner._handleMap["OptionSelect"];
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

void ModeTitle::StateOptionSelect::Exit() {
   _owner._optionDrawHandles = _owner._handleMap["OptionNoSelect"];
}

void ModeTitle::StateEndSelect::Enter() {
   _owner._endDrawHandles = _owner._handleMap["GameEndSelect"];
}

void ModeTitle::StateEndSelect::Input(InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner._stateServer->GoToState("OptionSelect");
   }
   if (input.GetXJoypad().AClick()) {
      _owner._gameMain.ShutDown();
   }
}

void ModeTitle::StateEndSelect::Exit() {
   _owner._endDrawHandles = _owner._handleMap["GameEndNoSelect"];
}