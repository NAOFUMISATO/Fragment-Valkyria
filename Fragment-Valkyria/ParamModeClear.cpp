
/*****************************************************************//**
 * \file   ParamModeClear.cpp
 * \brief  クリア画面の値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamModeClear.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamModeClear::ParamModeClear(std::string_view key) :ParamBase{key } {
   LoadIntParam({ "s_scoresec","a_scoresec","b_scoresec","c_scoresec",
      "number_diffx","missioncomp_x","missioncomp_y","missioncomp_animespeed","bg_alpha" });
   LoadVecParam({ "time_pos" ,"score_pos" });
}