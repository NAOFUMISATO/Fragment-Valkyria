
/*****************************************************************//**
 * \file   ModeOption.cpp
 * \brief  モードオプション
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeOption.h"
#include "GameMain.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("option",{"default_camera_sens","default_aim_sens",
      "default_deadzone","sensivity_min","sensivity_max","deadzone_min","deadzone_max","aimsens_x" ,"aimsens_y" ,
      "aimbar_x","aimbar_y","camerasens_x","camerasens_y","camerabar_x","camerabar_y","deadzone_x" ,"deadzone_y" ,
      "deadzonebar_x","deadzonebar_y","return_x" ,"return_y" });
   const int DefaultCameraSens = paramMap["default_camera_sens"];
   const int DefaultAimSens = paramMap["default_aim_sens"];
   const int DefaultDeadZone = paramMap["default_deadzone"];
   const int SensivityMin = paramMap["sensivity_min"];
   const int SensivityMax= paramMap["sensivity_max"];
   const int DeadZoneMin = paramMap["deadzone_min"];
   const int DeadZoneMax = paramMap["deadzone_max"];
   const int CameraSensPosX = paramMap["camerasens_x"];
   const int CameraSensPosY = paramMap["camerasens_y"];
   const int CameraBarPosX = paramMap["camerabar_x"];
   const int CameraBarPosY = paramMap["camerabar_y"];
   const int AimSensPosX = paramMap["aimsens_x"];
   const int AimSensPosY = paramMap["aimsens_y"];
   const int AimBarPosX = paramMap["aimbar_x"];
   const int AimBarPosY = paramMap["aimbar_y"];
   const int DeadZonePosX = paramMap["deadzone_x"];
   const int DeadZonePosY = paramMap["deadzone_y"];
   const int DeadZoneBarPosX = paramMap["deadzonebar_x"];
   const int DeadZoneBarPosY = paramMap["deadzonebar_y"];
   const int ReturnPosX = paramMap["return_x"];
   const int ReturnPosY = paramMap["return_y"];

   constexpr auto BgAlpha = 150;
   constexpr auto BoxWidth = 1920;      //!< DxLib::DrawBox横サイズ
   constexpr auto BoxHeight = 1080;     //!< DxLib::DrawBox縦サイズ
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
   const int SensitivityDivNum = SensivityMax / SensivityMin - 1;
   const int DeadZoneDivNum = DeadZoneMax / DeadZoneMin - 1;
}

using namespace FragmentValkyria::Mode;

ModeOption::ModeOption(Game::GameMain& gameMain) : ModeBase { gameMain }{
}

void ModeOption::Init() {
   GetLoadJson().LoadTextures("option");
   auto& resServer = GetResServer();
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
   
   _cameraSens = static_cast<double>(DefaultCameraSens);
   _aimSens = static_cast<double>(DefaultAimSens);
   _deadZone = DefaultDeadZone;
   _gameMain.sensitivity(_cameraSens, _aimSens, _deadZone);
}

void ModeOption::Enter() {
   _stateServer->PushBack("ReturnSelect");
}

void ModeOption::Input(AppFrame::Input::InputManager& input) {
   _stateServer->Input(input);
}

void ModeOption::Update() {
   _gameMain.sensitivity(_cameraSens, _aimSens, _deadZone);
   _stateServer->Update();
}

void ModeOption::Render() {
   _stateServer->Draw();
}

void ModeOption::StateBase::Update() {
   auto [cameraSencivity, aimSencivity, deadZone] = _owner._gameMain.sensitivity();
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
   auto cameraCusorX = (CameraBarPosX + (sensDivOne * (cameraSencivity / SensivityMin)) - sensDivOne)+ cusorOffsetX;
   auto cameraCusorY = (CameraBarPosY + ajustHeight / 2) + cusorOffsetY;
   auto aimCusorX = (AimBarPosX + (sensDivOne * (aimSencivity / SensivityMin)) - sensDivOne)+ cusorOffsetX;
   auto aimCusorY = (AimBarPosY + ajustHeight / 2) + cusorOffsetY;
   auto deadZoneCusorX = (DeadZoneBarPosX + (deadDivOne * (deadZone / DeadZoneMin)) - deadDivOne)+ cusorOffsetX;
   auto deadZoneCusorY = (DeadZoneBarPosY + ajustHeight / 2) + cusorOffsetY;
   _owner._cameraCusorPos = { static_cast<int>(cameraCusorX) , static_cast<int>(cameraCusorY) };
   _owner._aimCusorPos = { static_cast<int>(aimCusorX) , static_cast<int>(aimCusorY) };
   _owner._deadzoneCusorPos = { static_cast<int>(deadZoneCusorX) , static_cast<int>(deadZoneCusorY) };
}

void ModeOption::StateBase::Draw() {
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, BgAlpha);
   DrawBox(0, 0, BoxWidth, BoxHeight, AppFrame::Math::Utility::GetColorCode(0, 0, 0), TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   auto handleMap = _owner._handleMap;
   _owner.GetTexComponent().DrawTexture(CameraSensPosX, CameraSensPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["CameraSensivity"]);
   _owner.GetTexComponent().DrawTexture(CameraBarPosX, CameraBarPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["AdjustmentBar"]);
   _owner.GetTexComponent().DrawTexture(AimSensPosX, AimSensPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["AimSensivity"]);
   _owner.GetTexComponent().DrawTexture(AimBarPosX, AimBarPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["AdjustmentBar"]);
   _owner.GetTexComponent().DrawTexture(DeadZonePosX, DeadZonePosY, DefaultGraphScale, DefaultGraphAngle, handleMap["DeadZone"]);
   _owner.GetTexComponent().DrawTexture(DeadZoneBarPosX, DeadZoneBarPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["AdjustmentBar"]);
   _owner.GetTexComponent().DrawTexture(ReturnPosX, ReturnPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["Return"]);
   auto [cameraCusorPosX, cameraCusorPosY] = _owner._cameraCusorPos;
   auto [aimCusorPosX, aimCusorPosY] = _owner._aimCusorPos;
   auto [deadzoneCusorPosX, deadzoneCusorPosY] = _owner._deadzoneCusorPos;
   _owner.GetTexComponent().DrawTexture(cameraCusorPosX, cameraCusorPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["BarCusor"]);
   _owner.GetTexComponent().DrawTexture(aimCusorPosX, aimCusorPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["BarCusor"]);
   _owner.GetTexComponent().DrawTexture(deadzoneCusorPosX, deadzoneCusorPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["BarCusor"]);
   auto [selectCusorPosX, selectCusorPosY] = _owner._selectCusorPos;
   _owner.GetTexComponent().DrawTexture(selectCusorPosX, selectCusorPosY, DefaultGraphScale, DefaultGraphAngle, handleMap["OptionCusor"]);
}

void ModeOption::StateCameraSencivitySelect::Enter() {
   _owner._selectCusorPos = { CameraSensPosX ,CameraSensPosY };
}

void ModeOption::StateCameraSencivitySelect::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("ReturnSelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("AimSencivitySelect");
   }
   if (input.GetXJoypad().DRightClick() && _owner._cameraSens < SensivityMax) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._cameraSens += SensivityMin;
   }
   if (input.GetXJoypad().DLeftClick() && _owner._cameraSens > SensivityMin) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._cameraSens -= SensivityMin;
   }
}

void ModeOption::StateAimSencivitySelect::Enter() {
   _owner._selectCusorPos = { AimSensPosX ,AimSensPosY };
}

void ModeOption::StateAimSencivitySelect::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("CameraSencivitySelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("DeadZoneSelect");
   }
   if (input.GetXJoypad().DRightClick() && _owner._aimSens < SensivityMax) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._aimSens += SensivityMin;
   }
   if (input.GetXJoypad().DLeftClick() && _owner._aimSens > SensivityMin) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._aimSens -= SensivityMin;
   }
}

void ModeOption::StateDeadZoneSelect::Enter() {
   _owner._selectCusorPos = { DeadZonePosX ,DeadZonePosY };
}

void ModeOption::StateDeadZoneSelect::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("AimSencivitySelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("ReturnSelect");
   }
   if (input.GetXJoypad().DRightClick() && _owner._deadZone < DeadZoneMax) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._deadZone += DeadZoneMin;
   }
   if (input.GetXJoypad().DLeftClick() && _owner._deadZone > DeadZoneMin) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._deadZone -= DeadZoneMin;
   }
}

void ModeOption::StateReturnSelect::Enter() {
   _owner._selectCusorPos = { ReturnPosX ,ReturnPosY };
}

void ModeOption::StateReturnSelect::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().DUpClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("DeadZoneSelect");
   }
   if (input.GetXJoypad().DDownClick()) {
      _owner.GetSoundComponent().Play("SystemSelect");
      _owner._stateServer->GoToState("CameraSencivitySelect");
   }
   if (input.GetXJoypad().AClick()) {
      _owner.GetSoundComponent().Play("SystemDecision");
      _owner.GetModeServer().PopBack();
   }
}
