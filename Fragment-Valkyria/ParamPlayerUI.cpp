#include "ParamPlayerUI.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamPlayerUI::ParamPlayerUI(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadIntParam({"frontcolor_red","frontcolor_green","frontcolor_blue", 
      "backcolor_red" ,"backcolor_green" ,"backcolor_blue","shake_frame" ,
      "bullet_diff_x" ,"portion_diff_x","reticle_animespeed" });
   LoadDoubleParam({ "shake_width","redbar_speed" });
   LoadVecParam({ "hp_pos" ,"bullet_pos" ,"portion_pos" ,"reticle_pos" });
}