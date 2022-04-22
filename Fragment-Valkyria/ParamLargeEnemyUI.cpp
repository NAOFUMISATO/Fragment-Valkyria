
/*****************************************************************//**
 * \file   ParamLargeEnemyUI.cpp
 * \brief  ボスUIの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamLargeEnemyUI.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamLargeEnemyUI::ParamLargeEnemyUI(std::string_view key) :ParamBase{key } {
   LoadIntParam({ "frontcolor_red" ,"frontcolor_green" ,"frontcolor_blue", 
      "backcolor_red" ,"backcolor_green","backcolor_blue" });
   LoadDoubleParam({ "redbar_speed","max_hp" });
   LoadVecParam({ "hp_pos" });
}
