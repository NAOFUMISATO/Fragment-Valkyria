   
/*****************************************************************//**
 * \file   ParamModeGameOver.cpp
 * \brief  ゲームオーバー画面の値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamModeGameOver.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamModeGameOver::ParamModeGameOver(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadIntParam({ "missionfailed_x","missionfailed_y","missionfailed_animespeed",
      "bg_alpha", "continue_x","continue_y","exit_x","exit_y" });
}