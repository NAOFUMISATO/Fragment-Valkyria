
/*****************************************************************//**
 * \file   ParamCollision.cpp
 * \brief  当たり判定クラスの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamCollision.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamCollision::ParamCollision(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadDoubleParam({ "fallobject_range","ply_radius","ply_capsule_pos1","ply_capsule_pos2" ,
      "gatling_radius","fallobject_drum_capsule_pos1","fallobject_drum_capsule_pos2",
      "fallobject_drum_radius","laser_radius","bullet_radius","weak_object_damage",
      "normal_object_damage","weak_bullet_damage","normal_bullet_damage",
      "poorenemy_object_damage","poorenemy_bullet_damage","player_object_damage",
      "player_laser_damage","player_largeenemy_damage","player_gatling_damage",
      "player_poorenemy_damage" });
}
