
/*****************************************************************//**
 * \file   ModeTitle.cpp
 * \brief  モードタイトル
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

   const int TitleBgWidth = paramMap["bg_width"];         //!< タイトル背景横サイズ
   const int TitleBgHeight = paramMap["bg_height"];       //!< タイトル背景縦サイズ
   const int GameTitleWidth = paramMap["title_width"];    //!< ゲームタイトル横サイズ
   const int GameTitleHeight = paramMap["title_height"];  //!< ゲームタイトル縦サイズ
   const int StartGuideWidth = paramMap["guide_width"];   //!< スタートガイド横サイズ
   const int StartGuideHeight = paramMap["guide_height"]; //!< スタートガイド縦サイズ
   const int StartGuidePositionX = paramMap["guide_x"];   //!< スタートガイドX位置
   const int StartGuidePositionY = paramMap["guide_y"];   //!< スタートガイドY位置
   //仮----------
   auto vecParamMap = AppFrame::Resource::LoadJson::GetVecParamMap("player", { "init_pos" });
   const auto PlayerInitPos = vecParamMap["init_pos"];   //!< プレイヤー初期位置
   //------------
}

ModeTitle::ModeTitle(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeTitle::Init() {
   //仮(値チェック用)-----------
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


