
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
      { "bg_x","bg_y","title_x","title_y","title_animespeed","anybutton_x" ,"anybutton_y","anybutton_animespeed",
      "start_x","start_y","start_animespeed","option_x","option_y","option_animespeed","end_x","end_y","end_animespeed" });
   const int TitleBgPosX = paramMap["bg_x"];                        //!< タイトル背景X位置
   const int TitleBgPosY = paramMap["bg_y"];                        //!< タイトル背景Y位置
   const int GameTitlePosX = paramMap["title_x"];                   //!< ゲームタイトルX位置
   const int GameTitlePosY = paramMap["title_y"];                   //!< ゲームタイトルY位置
   const int GameTitleAnimeSpeed = paramMap["title_animespeed"];    //!< ゲームタイトルアニメーション速度
   const int AnyButtonPosX = paramMap["anybutton_x"];               //!< プレスエニイボタンX位置
   const int AnyButtonPosY = paramMap["anybutton_y"];               //!< プレスエニイボタンY位置
   const int AnyButtonAnimeSpeed = paramMap["anybutton_animespeed"];//!< プレスエニイボタンアニメーション速度
   const int GameStartPosX = paramMap["start_x"];                   //!< ゲーム開始セレクトX位置
   const int GameStartPosY = paramMap["start_y"];                   //!< ゲーム開始セレクトY位置
   const int GameStartAnimeSpeed = paramMap["start_animespeed"];    //!< ゲーム開始アニメーション速度
   const int OptionPosX = paramMap["option_x"];                     //!< オプションセレクトX位置
   const int OptionPosY = paramMap["option_y"];                     //!< オプションセレクトY位置
   const int OptionAnimeSpeed = paramMap["option_animespeed"];      //!< オプションアニメーション速度
   const int GameEndPosX = paramMap["end_x"];                       //!< ゲーム終了セレクトX位置
   const int GameEndPosY = paramMap["end_y"];                       //!< ゲーム終了セレクトY位置
   const int GameEndAnimeSpeed = paramMap["end_animespeed"];        //!< ゲーム終了アニメーション速度

   constexpr auto DefaultAnimeSpeed = 1;                            //!< 各画像アニメーション速度
   constexpr auto TitleBgCX = 0;                                    //!< タイトル背景画像X基準点
   constexpr auto TitleBgCY = 0;                                    //!< タイトル背景画像Y基準点
   constexpr auto DefaultGraphScale = 1.0;                          //!< 各画像拡大率
   constexpr auto DefaultGraphAngle = 0.0;                          //!< 各画像角度
   constexpr auto FirstInputFrame = 120;                            //!< エニイボタンから移行後、ゲーム開始を受け付けないフレーム数
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