#include "ParamLargeEnemyUI.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamLargeEnemyUI::ParamLargeEnemyUI(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadIntParam({ "frontcolor_red" ,"frontcolor_green" ,"frontcolor_blue", 
      "backcolor_red" ,"backcolor_green","backcolor_blue" });
   LoadDoubleParam({ "redbar_speed","max_hp" });
   LoadVecParam({ "hp_pos" });
}
