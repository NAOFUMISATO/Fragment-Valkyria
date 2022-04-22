
/*****************************************************************//**
 * \file   ParamLightShadow.cpp
 * \brief  ライティング及び落ち影の処理を行うクラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamLightShadow.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamLightShadow::ParamLightShadow(std::string_view key) :ParamBase{key } {
   LoadIntParam({ "fog_red","fog_green","fog_blue","shadow_resolution" });
   LoadFloatParam({ "fog_start","global_ambred","global_ambgreen","global_ambblue",
      "global_ambalpha","alllight_difred","alllight_difgreen","alllight_difblue",
      "alllight_difalpha","fog_end" ,"fixedlight_area" ,"fixedlight_atten_first" ,
      "fixedlight_atten_second","fixedlight_atten_third","alllight_spcred","alllight_spcgreen",
      "alllight_spcblue","alllight_spcalpha","alllight_ambred","alllight_ambgreen","alllight_ambblue",
      "alllight_ambalpha","fixedlight_difred","fixedlight_difgreen","fixedlight_difblue",
      "fixedlight_difalpha","fixedlight_spcred","fixedlight_spcgreen","fixedlight_spcblue",
      "fixedlight_spcalpha","fixedlight_ambred","fixedlight_ambgreen","fixedlight_ambblue",
      "fixedlight_ambalpha" });
   LoadBoolParam({ "fog_enuble","alllight_enuble"});
   LoadVecParam({ "fixedlight_pos" ,"shadow_minarea" ,"shadow_maxarea" });
}
