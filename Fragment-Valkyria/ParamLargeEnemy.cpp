
/*****************************************************************//**
 * \file   ParamLargeEnemy.cpp
 * \brief  ボスの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamLargeEnemy.h"

using namespace FragmentValkyria::Param;

ParamLargeEnemy::ParamLargeEnemy(std::string_view key) :ParamBase{key } {
   LoadIntParam({ "gatling_frame",  "consecutive_fall_object_frame", 
      "consecutive_num", "action_cooltime", "crasheffect_count",
      "object_max_num","crasheffect_frame",
      "max_wave","max_poorenemy" });
   LoadDoubleParam({  "max_stun", "stun_decrease", "object_stun_value",
      "bullet_stun_value", "hp", "gravity","crasheffect_distance" });
}
