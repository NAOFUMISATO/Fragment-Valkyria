
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
   constexpr auto TitleBgWidth = 1920;    //!< タイトル背景横サイズ
   constexpr auto TitleBgHeight = 1080;   //!< タイトル背景縦サイズ
   constexpr auto GameTitleWidth = 1553;  //!< ゲームタイトル横サイズ
   constexpr auto GameTitleHeight = 224;  //!< ゲームタイトル縦サイズ
   constexpr auto StartGuideWidth = 1135; //!< スタートガイド横サイズ
   constexpr auto StartGuideHeight = 107; //!< スタートガイド縦サイズ
   constexpr auto StartGuidePositionX = FragmentValkyria::ScreenWidth / 2 - StartGuideWidth / 2;   //!< スタートガイドX位置
   constexpr auto StartGuidePositionY = 700 - StartGuideHeight / 2;              //!< スタートガイドY位置
}

ModeTitle::ModeTitle(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeTitle::Init() {

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


