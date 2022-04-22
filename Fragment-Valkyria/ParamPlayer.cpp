
/*****************************************************************//**
 * \file   ParamPlayer.cpp
 * \brief  プレイヤーの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamPlayer.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamPlayer::ParamPlayer( std::string_view key) :ParamBase{ key } {
   LoadIntParam({ "max_bullet", "max_portion","walk_dead_zone_range",
      "wait_frame", "invincible_frame","blinking_frame", "cooltime" });
   LoadDoubleParam({ "idle_animespeed", "walk_animespeed", "run_animespeed", 
      "shootready_animespeed", "shoot_animespeed","knockback_animespeed", 
      "run_speed", "recovery_rate", "max_hp","rotate_rate", "walk_speed" });
}
