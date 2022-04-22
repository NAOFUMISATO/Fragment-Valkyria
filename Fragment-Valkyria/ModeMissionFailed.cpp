
/*****************************************************************//**
 * \file   ModeMissionFailed.cpp
 * \brief  ���[�h�~�b�V�������s
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeMissionFailed.h"
#include "GameMain.h"
#include "ParamModeGameOver.h"

namespace {
   constexpr auto BoxWidth = 1920;
   constexpr auto BoxHeight = 1080;
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeMissionFailed::ModeMissionFailed()  {
   _param = std::make_unique<Param::ParamModeGameOver>("over");
}

void ModeMissionFailed::Init() {
   GetLoadJson().LoadTextures("over");
   _grHandles = GetResServer().GetTextures("MissionFailed");
}

void ModeMissionFailed::Enter() {
   _animeNo = 0;
   _cntInit = false;
}

void ModeMissionFailed::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().AClick()) {
      GetModeServer().PopBack();
      GetModeServer().GoToMode("GameOver", 'S');
   }
}

void ModeMissionFailed::Update() {
   auto gameInstance = Game::GameMain::GetInstance();
   if (!_cntInit) {
      _animeCnt = gameInstance->modeServer().frameCount();
      _cntInit = true;
      GetSoundComponent().Play("GameOverVoice");
      GetSoundComponent().Play("GameOver");
   }
   auto gameCount = static_cast<int>(gameInstance->modeServer().frameCount());
   auto frame = gameCount - _animeCnt;
   auto allNum = std::get<0>(GetResServer().GetTextureInfo("MissionFailed").GetDivParams());
   const auto FailedAnimeSpeed = _param->GetIntParam("missionfailed_animespeed");
   if (frame < allNum * FailedAnimeSpeed) {
      _animeNo = frame / FailedAnimeSpeed % allNum;
   }
}

void ModeMissionFailed::Render() {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _IntParam("bg_alpha"));
   DrawBox(0, 0, BoxWidth, BoxHeight, AppFrame::Math::Utility::GetColorCode(0, 0, 0), TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   DrawGraph(_IntParam("missionfailed_x"), _IntParam("missionfailed_y"),_grHandles[_animeNo], true);
}