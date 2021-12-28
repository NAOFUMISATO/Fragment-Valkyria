
/*****************************************************************//**
 * \file   ModeTitle.cpp
 * \brief  ���[�h�^�C�g��
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeTitle.h"
#include "GameMain.h"
#include "ResourceLoadJson.h"
using namespace FragmentValkyria::Mode;

namespace {
   constexpr auto TitleBgWidth = 1920;    //!< �^�C�g���w�i���T�C�Y
   constexpr auto TitleBgHeight = 1080;   //!< �^�C�g���w�i�c�T�C�Y
   constexpr auto GameTitleWidth = 1553;  //!< �Q�[���^�C�g�����T�C�Y
   constexpr auto GameTitleHeight = 224;  //!< �Q�[���^�C�g���c�T�C�Y
   constexpr auto StartGuideWidth = 1135; //!< �X�^�[�g�K�C�h���T�C�Y
   constexpr auto StartGuideHeight = 107; //!< �X�^�[�g�K�C�h�c�T�C�Y
   constexpr auto StartGuidePositionX = FragmentValkyria::ScreenWidth / 2 - StartGuideWidth / 2;   //!< �X�^�[�g�K�C�hX�ʒu
   constexpr auto StartGuidePositionY = 700 - StartGuideHeight / 2;              //!< �X�^�[�g�K�C�hY�ʒu
}

ModeTitle::ModeTitle(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeTitle::Init() {

   auto& rj = GetResJson();
   rj.LoadTextures("title");
   rj.LoadSounds("bgm");

   auto& rs = GetResServer();
   _bgGrHandle = rs.GetTexture("TitleBg");
   _titleGrHandle = rs.GetTexture("GameTitle");
   _guideGrHandle = rs.GetTexture("StartGuide");

   auto& ss = GetSoundServer();
   ss.Play("TitleBgm");

}

void ModeTitle::Enter() {
   _alpha = 0;
}

void ModeTitle::Input(AppFrame::Input::InputManager& input) {
   if (input.GetMouse().LeftClick()) {
      GetModeServer().GoToMode("InGame", 'L');
      _alpha = 255;
   }
}

void ModeTitle::Update() {
   _alpha = (_alpha + 8) % 255;
}

void ModeTitle::Render() {
   DrawGraph(0, 0, _bgGrHandle, FALSE);
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
   DrawGraph(StartGuidePositionX, StartGuidePositionY, _guideGrHandle, TRUE);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   DrawGraph(0, 0, _titleGrHandle, TRUE);

}


