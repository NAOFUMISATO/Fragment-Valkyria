
/*****************************************************************//**
 * \file   ModeMissionCompleted.cpp
 * \brief  モードミッション完遂
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeMissionCompleted.h"
#include "ParamModeClear.h"

namespace {
   constexpr auto BoxWidth = 1920;
   constexpr auto BoxHeight = 1080;
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeMissionCompleted::ModeMissionCompleted() {
   _param = std::make_unique<Param::ParamModeClear>("clear");
}

void ModeMissionCompleted::Init() {
   GetLoadJson().LoadTextures("clear");
   _grHandles = GetResServer().GetTextures("MissionCompleted");
}

void ModeMissionCompleted::Enter() {
   GetSoundComponent().Play("GameClearVoice");
   GetSoundComponent().Play("ClearResult");
}

void ModeMissionCompleted::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().AClick()) {
      GetModeServer().PopBack();
      GetModeServer().GoToMode("ClearResult", 'S');
   }
}

void ModeMissionCompleted::Render() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _IntParam("bg_alpha"));
   DrawBox(0, 0, BoxWidth, BoxHeight, AppFrame::Math::Utility::GetColorCode(0, 0, 0), TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   GetTexComponent().DrawTexture(_IntParam("missioncomp_x"), _IntParam("missioncomp_y"), 
      DefaultGraphScale, DefaultGraphAngle, _grHandles, _IntParam("missioncomp_animespeed"));
}