
/*****************************************************************//**
 * \file   ModeTitle.cpp
 * \brief  モードタイトル
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeTitle.h"
#include "ParamModeTitle.h"
#include "GameMain.h"
using namespace FragmentValkyria::Mode;

namespace {
   constexpr auto DefaultAnimeSpeed = 1;   //!< 各画像アニメーション速度
   constexpr auto TitleBgCX = 0;           //!< タイトル背景画像X基準点
   constexpr auto TitleBgCY = 0;           //!< タイトル背景画像Y基準点
   constexpr auto DefaultGraphScale = 1.0; //!< 各画像拡大率
   constexpr auto DefaultGraphAngle = 0.0; //!< 各画像角度
   constexpr auto FirstInputFrame = 120;   //!< エニイボタンから移行後、ゲーム開始を受け付けないフレーム数
}

ModeTitle::ModeTitle() {
   _param = std::make_unique<Param::ParamModeTitle>("title");
}

void ModeTitle::Init() {

   GetLoadJson().LoadSounds("outgame");

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
   GetSoundComponent().PlayLoop("TitleBgm");
   _stateServer->PushBack("AnyButton");
   _logoHandle = _handleMap["TitleLogo"][0];
   _cntInit = false;
   auto gameInstance = Game::GameMain::GetInstance();
   gameInstance->isTutorialClear(false);
   gameInstance->isPoorClear(false);
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
   auto gameInstance = Game::GameMain::GetInstance();
   if (!_cntInit) {
      GetSoundComponent().Play("TitleVoice");
      _logoCnt = gameInstance->modeServer().frameCount();
      _cntInit = true;
   }
   auto gameCount = static_cast<int>(gameInstance->modeServer().frameCount());
   auto frame = gameCount - _logoCnt;
   auto allNum = std::get<0>(GetResServer().GetTextureInfo("TitleLogo").GetDivParams());
   const int TitleAnimeSpeed = _param->GetIntParam("title_animespeed");
   if (frame < allNum * TitleAnimeSpeed) {
      _logoHandle = _handleMap["TitleLogo"][(frame / TitleAnimeSpeed) % allNum];
   }
}

void ModeTitle::StateBase::Draw() {
   auto handleMap = _owner._handleMap;
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };

   _owner.GetTexComponent().TransDrawTexture(_IntParam("bg_x"), _IntParam("bg_y"), TitleBgCX, TitleBgCY,
      DefaultGraphScale, DefaultGraphAngle, handleMap["TitleBg"], DefaultAnimeSpeed, false, false);
   DrawGraph(_IntParam("title_x"), _IntParam("title_y"), _owner._logoHandle, true);
   if (!_owner._pushAnyButton) {
      _owner.GetTexComponent().DrawTexture(_IntParam("anybutton_x"), _IntParam("anybutton_y"), 
         DefaultGraphScale, DefaultGraphAngle, handleMap["AnyButton"], _IntParam("anybutton_animespeed"));
   }
   if (_owner._pushAnyButton) {
      _owner.GetTexComponent().DrawTexture(_IntParam("start_x"), _IntParam("start_y"), 
         DefaultGraphScale, DefaultGraphAngle, _owner._startDrawHandles, _IntParam("start_animespeed"));
      _owner.GetTexComponent().DrawTexture(_IntParam("option_x"), _IntParam("option_y"), 
         DefaultGraphScale, DefaultGraphAngle, _owner._optionDrawHandles, _IntParam("option_animespeed"));
      _owner.GetTexComponent().DrawTexture(_IntParam("end_x"), _IntParam("end_y"), 
         DefaultGraphScale, DefaultGraphAngle, _owner._endDrawHandles, _IntParam("end_animespeed"));
   }
}

void ModeTitle::StateAnyButton::Enter() {
   _owner._pushAnyButton = false;
}

void ModeTitle::StateAnyButton::Input(InputManager& input){
   /**
    * \brief 入力情報の有無を返す
    * \param inputArray 入力情報の配列
    * \return 入力情報配列が一つでもtrueならtrueを返す
    */
   auto _IsInput = [](std::vector<bool> inputArray) {
      for (auto&& iArray : inputArray) {
         if (iArray) {
            return true;
         }
      }
      return  false;
   };
   auto joypad = input.GetXJoypad();      // XInput対応ジョイパッドの入力管理クラスの参照
   auto keyboard = input.GetKeyboard();   // キーボードの入力管理クラスの参照

   if (_IsInput({ joypad.XClick(), joypad.YClick(),joypad.AClick(),
      joypad.BClick(),keyboard.UpClick(),keyboard.DownClick(),
      keyboard.RightClick(),keyboard.LeftClick(),keyboard.SpaceClick() })) {
      _owner.GetSoundComponent().Play("SystemDecision");
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
   /**
    * \brief 入力情報の有無を返す
    * \param a 入力情報1
    * \param b 入力情報2
    * \return 入力したならtrue,でないならばfalse
    */
   auto _IsInput = [](bool a, bool b) {
      return a || b;
   };
   auto joypad = input.GetXJoypad();      // XInput対応ジョイパッドの入力管理クラスの参照
   auto keyboard = input.GetKeyboard();   // キーボードの入力管理クラスの参照

   if (_IsInput(joypad.DDownClick(), keyboard.DownClick())) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("OptionSelect");
   }
   auto frameCount = _owner.GetModeServer().frameCount() - _owner._firstInputCnt;
   if (frameCount > FirstInputFrame) {
      if (_IsInput(joypad.AClick(), keyboard.SpaceClick())) {
         _owner.GetSoundComponent().Play("SystemDecision");
         _owner.GetModeServer().GoToMode("TutorialSelect", 'S');
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
   /**
    * \brief 入力情報の有無を返す
    * \param a 入力情報1
    * \param b 入力情報2
    * \return 入力したならtrue,でないならばfalse
    */
   auto _IsInput = [](bool a, bool b) {
      return a || b;
   };
   auto joypad = input.GetXJoypad();      // XInput対応ジョイパッドの入力管理クラスの参照
   auto keyboard = input.GetKeyboard();   // キーボードの入力管理クラスの参照

   if (_IsInput(joypad.DUpClick(), keyboard.UpClick())) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("StartSelect");
   }
   if (_IsInput(joypad.DDownClick(), keyboard.DownClick())) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("EndSelect");
   }
   if (_IsInput(joypad.AClick(), keyboard.SpaceClick())) {
      _owner.GetSoundComponent().Play("SystemSelect");
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
   /**
    * \brief 入力情報の有無を返す
    * \param a 入力情報1
    * \param b 入力情報2
    * \return 入力したならtrue,でないならばfalse
    */
   auto _IsInput = [](bool a, bool b) {
      return a || b;
   };
   auto joypad = input.GetXJoypad();      // XInput対応ジョイパッドの入力管理クラスの参照
   auto keyboard = input.GetKeyboard();   // キーボードの入力管理クラスの参照

   if (_IsInput(joypad.DUpClick(), keyboard.UpClick())) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("OptionSelect");
   }
   if (_IsInput(joypad.AClick(), keyboard.SpaceClick())) {
      _owner.GetSoundComponent().Play("SystemDecision");
      auto gameInstance = Game::GameMain::GetInstance();
      gameInstance->ShutDown();
   }
}

void ModeTitle::StateEndSelect::Exit() {
   _owner._endDrawHandles = _owner._handleMap["GameEndNoSelect"];
}