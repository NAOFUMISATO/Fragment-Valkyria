
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
      { "bg_x","bg_y","title_x","title_y","guide_x" ,"guide_y" });
   const int TitleBgPosX = paramMap["bg_x"];        //!< タイトル背景X位置
   const int TitleBgPosY = paramMap["bg_y"];        //!< タイトル背景Y位置
   const int GameTitlePosX = paramMap["title_x"];   //!< ゲームタイトルX位置
   const int GameTitlePosY = paramMap["title_y"];   //!< ゲームタイトルY位置
   const int StartGuidePosX = paramMap["guide_x"];  //!< スタートガイドX位置
   const int StartGuidePosY = paramMap["guide_y"];  //!< スタートガイドY位置
   //仮----------
   auto vecParamMap = AppFrame::Resource::LoadParamJson::GetVecParamMap("player", { "init_pos" });
   const auto PlayerInitPos = vecParamMap["init_pos"];   //!< プレイヤー初期位置
   //------------
   constexpr auto DefaultGraphScale = 1.0;
   constexpr auto DefaultGraphAngle = 0.0;
}

ModeTitle::ModeTitle(Game::GameMain& gameMain) :ModeBase{ gameMain } {
}

void ModeTitle::Init() {
   //仮(値チェック用)-----------
   auto vec = PlayerInitPos;
   //---------------------------
   auto& loadJson = GetLoadJson();
   loadJson.LoadTextures("title");
   loadJson.LoadSounds("title");

   auto& resServer = GetResServer();
   
   _grHandles = {
      resServer.GetTexture("TitleBg"),
      resServer.GetTexture("StartGuide"),
      resServer.GetTexture("GameTitle")
   };

   auto& soundServer = GetSoundServer();
   soundServer.Play("TitleBgm");

}

void ModeTitle::Enter() {
   _alpha = 0;
}

void ModeTitle::Input(AppFrame::Input::InputManager& input) {
   if (input.GetMouse().LeftClick()) {
      GetModeServer().GoToMode("Boss", 'L');
      _alpha = 255;
   }
}

void ModeTitle::Update() {
   _alpha = (_alpha + 8) % 255;
}

void ModeTitle::Render() {
   auto [bgGrHandle, guideGrHandle, titleGrHandle] = _grHandles;
   GetSimpTexComponent().DrawTexture(TitleBgPosX, TitleBgPosY, DefaultGraphScale, DefaultGraphAngle, bgGrHandle);
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
   GetSimpTexComponent().DrawTexture(StartGuidePosX, StartGuidePosY, DefaultGraphScale, DefaultGraphAngle, guideGrHandle);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   GetSimpTexComponent().DrawTexture(GameTitlePosX, GameTitlePosY, DefaultGraphScale, DefaultGraphAngle, titleGrHandle);

}


