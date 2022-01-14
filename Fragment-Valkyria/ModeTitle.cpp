
/*****************************************************************//**
 * \file   ModeTitle.cpp
 * \brief  ���[�h�^�C�g��
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeTitle.h"
#include "GameMain.h"
#include "LoadJson.h"
using namespace FragmentValkyria::Mode;

namespace {
   auto paramMap = AppFrame::Resource::LoadJson::GetParamMap("title",
      { "bg_width","bg_height" ,"title_width" ,"title_height" ,
      "guide_width" ,"guide_height", "guide_x" ,"guide_y" });

   const int TitleBgWidth = paramMap["bg_width"];         //!< �^�C�g���w�i���T�C�Y
   const int TitleBgHeight = paramMap["bg_height"];       //!< �^�C�g���w�i�c�T�C�Y
   const int GameTitleWidth = paramMap["title_width"];    //!< �Q�[���^�C�g�����T�C�Y
   const int GameTitleHeight = paramMap["title_height"];  //!< �Q�[���^�C�g���c�T�C�Y
   const int StartGuideWidth = paramMap["guide_width"];   //!< �X�^�[�g�K�C�h���T�C�Y
   const int StartGuideHeight = paramMap["guide_height"]; //!< �X�^�[�g�K�C�h�c�T�C�Y
   const int StartGuidePositionX = paramMap["guide_x"];   //!< �X�^�[�g�K�C�hX�ʒu
   const int StartGuidePositionY = paramMap["guide_y"];   //!< �X�^�[�g�K�C�hY�ʒu
   //��----------
   auto vecParamMap = AppFrame::Resource::LoadJson::GetVecParamMap("player", { "init_pos" });
   const auto PlayerInitPos = vecParamMap["init_pos"];   //!< �v���C���[�����ʒu
   //------------
}

ModeTitle::ModeTitle(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeTitle::Init() {
   //��(�l�`�F�b�N�p)-----------
   auto vec = PlayerInitPos;
   //---------------------------
   auto& loadJson = GetLoadJson();
   loadJson.LoadTextures("title");
   loadJson.LoadSounds("bgm");

   auto& resServer = GetResServer();
   _bgGrHandle = resServer.GetTexture("TitleBg");
   _titleGrHandle = resServer.GetTexture("GameTitle");
   _guideGrHandle = resServer.GetTexture("StartGuide");

   auto& soundServer = GetSoundServer();
   soundServer.Play("TitleBgm");

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


