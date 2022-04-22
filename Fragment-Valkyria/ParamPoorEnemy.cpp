
/*****************************************************************//**
 * \file   ParamPoorEnemy.cpp
 * \brief  G‹›“G‚Ì’lŠÇ—ƒNƒ‰ƒX
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamPoorEnemy.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamPoorEnemy::ParamPoorEnemy(std::string_view key) :ParamBase{ key } {
   LoadIntParam({"max_wave","rush_frame","max_gatling" ,"gatling_rate","white_frame" });
   LoadDoubleParam({ "gravity","rotate_speed","step_distance","step_speed","rush_speed",
      "idle_animespeed","walk_animespeed", "rush_animespeed" ,"gatling_animespeed" ,
      "fall_animespeed" ,"die_animespeed" });
}
