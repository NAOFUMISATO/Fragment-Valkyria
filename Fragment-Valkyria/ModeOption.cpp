
/*****************************************************************//**
 * \file   ModeOption.cpp
 * \brief  モードオプション
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeOption.h"
#include "Game.h"
#include "ParamModeOption.h"

namespace {
   constexpr auto BoxWidth = 1920;      //!< DxLib::DrawBox横サイズ
   constexpr auto BoxHeight = 1080;     //!< DxLib::DrawBox縦サイズ
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
   constexpr auto SensivityMax = 10.0;
   constexpr auto DeadZoneMax = 100.0;
   constexpr auto SensivityMin = 1.0;
   constexpr auto DeadZoneMin = 10;
   constexpr auto SensitivityDivNum = SensivityMax / SensivityMin - 1;
   constexpr auto DeadZoneDivNum = DeadZoneMax / DeadZoneMin - 1;
}

using namespace FragmentValkyria::Mode;

ModeOption::ModeOption() {
   _param = std::make_unique<Param::ParamModeOption>("option");
}

void ModeOption::Init() {
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   auto& loadresJson = Game::Game::GetLoadresJson();
   loadresJson.LoadTextures("option");

   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _handleMap = {
      {"AdjustmentBar",  resServer.GetTexture("AdjustmentBar")},
      {"AimSensivity",   resServer.GetTexture("AimSensivity")},
      {"BarCusor",       resServer.GetTexture("BarCusor")},
      {"CameraSensivity",resServer.GetTexture("CameraSensivity")},
      {"OptionCusor",    resServer.GetTexture("OptionCusor")},
      {"DeadZone",       resServer.GetTexture("DeadZone")},
      {"Return",         resServer.GetTexture("Return")}
   };

   _stateServer = std::make_unique<AppFrame::State::StateServer>("CameraSencivitySelect", std::make_shared <StateCameraSencivitySelect>(*this));
   _stateServer->Register("AimSencivitySelect", std::make_shared<StateAimSencivitySelect>(*this));
   _stateServer->Register("DeadZoneSelect", std::make_shared<StateDeadZoneSelect>(*this));
   _stateServer->Register("ReturnSelect", std::make_shared<StateReturnSelect>(*this));
   
   _cameraSens = _DoubleParam("default_camera_sens");
   _aimSens = _DoubleParam("default_aim_sens");
   _deadZone =_param->GetIntParam("default_deadzone");
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.sensitivity(_cameraSens, _aimSens, _deadZone);
}

void ModeOption::Enter() {
   _stateServer->PushBack("ReturnSelect");
}

void ModeOption::Input(AppFrame::Input::InputManager& input) {
   _stateServer->Input(input);
}

void ModeOption::Update() {
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.sensitivity(_cameraSens, _aimSens, _deadZone);
   _stateServer->Update();
}

void ModeOption::Render() {
   _stateServer->Draw();
}

void ModeOption::StateBase::Update() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   auto& gameInstance = Game::Game::GetInstance();
   auto [cameraSencivity, aimSencivity, deadZone] = gameInstance.sensitivity();
   auto& ajustHandle = _owner._handleMap["AdjustmentBar"];
   auto& cusorHandle = _owner._handleMap["BarCusor"];
   int ajustWidth, ajustHeight;
   int cusorWidth, cusorHeight;
   GetGraphSize(ajustHandle, &ajustWidth, &ajustHeight);
   GetGraphSize(cusorHandle, &cusorWidth, &cusorHeight);
   auto cusorOffsetX = -cusorWidth / 2;
   auto cusorOffsetY = -cusorHeight / 2;
   auto sensDivOne = (static_cast<double>(ajustWidth) / SensitivityDivNum);
   auto deadDivOne = ajustWidth / DeadZoneDivNum;
   auto cameraCusorX = (_IntParam("camerabar_x") + (sensDivOne * (cameraSencivity / SensivityMin)) - sensDivOne)+ cusorOffsetX;
   auto cameraCusorY = (_IntParam("camerabar_y") + ajustHeight / 2) + cusorOffsetY;
   auto aimCusorX = (_IntParam("aimbar_x") + (sensDivOne * (aimSencivity / SensivityMin)) - sensDivOne)+ cusorOffsetX;
   auto aimCusorY = (_IntParam("aimbar_y") + ajustHeight / 2) + cusorOffsetY;
   auto deadZoneCusorX = (_IntParam("deadzonebar_x") + (deadDivOne * (deadZone / DeadZoneMin)) - deadDivOne) + cusorOffsetX;
   auto deadZoneCusorY = (_IntParam("deadzonebar_y") + ajustHeight / 2) + cusorOffsetY;
   _owner._cameraCusorPos = { static_cast<int>(cameraCusorX) , static_cast<int>(cameraCusorY) };
   _owner._aimCusorPos = { static_cast<int>(aimCusorX) , static_cast<int>(aimCusorY) };
   _owner._deadzoneCusorPos = { static_cast<int>(deadZoneCusorX) , static_cast<int>(deadZoneCusorY) };
}

void ModeOption::StateBase::Draw() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _IntParam("bg_alpha"));
   DrawBox(0, 0, BoxWidth, BoxHeight, AppFrame::Math::Utility::GetColorCode(0, 0, 0), TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   auto handleMap = _owner._handleMap;
   auto& texComponent = Game::Game::GetTexComponent();
   texComponent.DrawTexture(_IntParam("camerasens_x"), _IntParam("camerasens_y"),
      DefaultGraphScale, DefaultGraphAngle, handleMap["CameraSensivity"]);
   texComponent.DrawTexture(_IntParam("camerabar_x"), _IntParam("camerabar_y"),
      DefaultGraphScale, DefaultGraphAngle, handleMap["AdjustmentBar"]);
   texComponent.DrawTexture(_IntParam("aimsens_x"), _IntParam("aimsens_y"),
      DefaultGraphScale, DefaultGraphAngle, handleMap["AimSensivity"]);
   texComponent.DrawTexture(_IntParam("aimbar_x"), _IntParam("aimbar_y"),
      DefaultGraphScale, DefaultGraphAngle, handleMap["AdjustmentBar"]);
   texComponent.DrawTexture(_IntParam("deadzone_x"), _IntParam("deadzone_y"),
      DefaultGraphScale, DefaultGraphAngle, handleMap["DeadZone"]);
   texComponent.DrawTexture(_IntParam("deadzonebar_x"), _IntParam("deadzonebar_y"),
      DefaultGraphScale, DefaultGraphAngle, handleMap["AdjustmentBar"]);
   texComponent.DrawTexture(_IntParam("return_x"), _IntParam("return_y"),
      DefaultGraphScale, DefaultGraphAngle, handleMap["Return"]);
   auto [cameraCusorPosX, cameraCusorPosY] = _owner._cameraCusorPos;
   auto [aimCusorPosX, aimCusorPosY] = _owner._aimCusorPos;
   auto [deadzoneCusorPosX, deadzoneCusorPosY] = _owner._deadzoneCusorPos;
   texComponent.DrawTexture(cameraCusorPosX, cameraCusorPosY,
      DefaultGraphScale, DefaultGraphAngle, handleMap["BarCusor"]);
   texComponent.DrawTexture(aimCusorPosX, aimCusorPosY,
      DefaultGraphScale, DefaultGraphAngle, handleMap["BarCusor"]);
   texComponent.DrawTexture(deadzoneCusorPosX, deadzoneCusorPosY,
      DefaultGraphScale, DefaultGraphAngle, handleMap["BarCusor"]);
   auto [selectCusorPosX, selectCusorPosY] = _owner._selectCusorPos;
   texComponent.DrawTexture(selectCusorPosX, selectCusorPosY,
      DefaultGraphScale, DefaultGraphAngle, handleMap["OptionCusor"]);
}

void ModeOption::StateCameraSencivitySelect::Enter() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   _owner._selectCusorPos = { _IntParam("camerasens_x") ,_IntParam("camerasens_y") };
}

void ModeOption::StateCameraSencivitySelect::Input(AppFrame::Input::InputManager& input) {
   auto& soundComponent = Game::Game::GetSoundComponent();
   if (input.GetXJoypad().DUpClick()) {
      soundComponent.Play("SystemSelect");
      _owner._stateServer->GoToState("ReturnSelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      soundComponent.Play("SystemSelect");
      _owner._stateServer->GoToState("AimSencivitySelect");
   }
   if (input.GetXJoypad().DRightClick() && _owner._cameraSens < SensivityMax) {
      soundComponent.Play("SystemSelect");
      _owner._cameraSens += SensivityMin;
   }
   if (input.GetXJoypad().DLeftClick() && _owner._cameraSens > SensivityMin) {
      soundComponent.Play("SystemSelect");
      _owner._cameraSens -= SensivityMin;
   }
}

void ModeOption::StateAimSencivitySelect::Enter() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   _owner._selectCusorPos = { _IntParam("aimsens_x") ,_IntParam("aimsens_y") };
}

void ModeOption::StateAimSencivitySelect::Input(AppFrame::Input::InputManager& input) {
   auto& soundComponent = Game::Game::GetSoundComponent();
   if (input.GetXJoypad().DUpClick()) {
      soundComponent.Play("SystemSelect");
      _owner._stateServer->GoToState("CameraSencivitySelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      soundComponent.Play("SystemSelect");
      _owner._stateServer->GoToState("DeadZoneSelect");
   }
   if (input.GetXJoypad().DRightClick() && _owner._aimSens < SensivityMax) {
      soundComponent.Play("SystemSelect");
      _owner._aimSens += SensivityMin;
   }
   if (input.GetXJoypad().DLeftClick() && _owner._aimSens > SensivityMin) {
      soundComponent.Play("SystemSelect");
      _owner._aimSens -= SensivityMin;
   }
}

void ModeOption::StateDeadZoneSelect::Enter() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   _owner._selectCusorPos = { _IntParam("deadzone_x") ,_IntParam("deadzone_y") };
}

void ModeOption::StateDeadZoneSelect::Input(AppFrame::Input::InputManager& input) {
   auto& soundComponent = Game::Game::GetSoundComponent();
   if (input.GetXJoypad().DUpClick()) {
      soundComponent.Play("SystemSelect");
      _owner._stateServer->GoToState("AimSencivitySelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      soundComponent.Play("SystemSelect");
      _owner._stateServer->GoToState("ReturnSelect");
   }
   if (input.GetXJoypad().DRightClick() && _owner._deadZone < DeadZoneMax) {
      soundComponent.Play("SystemSelect");
      _owner._deadZone += DeadZoneMin;
   }
   if (input.GetXJoypad().DLeftClick() && _owner._deadZone > DeadZoneMin) {
      soundComponent.Play("SystemSelect");
      _owner._deadZone -= DeadZoneMin;
   }
}

void ModeOption::StateReturnSelect::Enter() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   _owner._selectCusorPos = { _IntParam("return_x") ,_IntParam("return_y") };
}

void ModeOption::StateReturnSelect::Input(AppFrame::Input::InputManager& input) {
   auto& soundComponent = Game::Game::GetSoundComponent();
   if (input.GetXJoypad().DUpClick()) {
      soundComponent.Play("SystemSelect");
      _owner._stateServer->GoToState("DeadZoneSelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      soundComponent.Play("SystemSelect");
      _owner._stateServer->GoToState("CameraSencivitySelect");
   }
   if (input.GetXJoypad().AClick()) {
      soundComponent.Play("SystemDecision");
      auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
      modeServer.PopBack();
   }
}
