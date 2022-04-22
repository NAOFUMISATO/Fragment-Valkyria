
/*****************************************************************//**
 * \file   ParamFallObject.cpp
 * \brief  落下オブジェクトの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamFallObject.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamFallObject::ParamFallObject(std::string_view key) :ParamBase{key } {
   LoadIntParam({ "fallpoint_animespeed" });
   LoadDoubleParam({ "gravity", "shoot_speed", "up_speed", "rotate_angle","updown_range", "right_hand_up_value",
      "up_value","fallpoint_pos_y","fallpoint_scale"});
}
