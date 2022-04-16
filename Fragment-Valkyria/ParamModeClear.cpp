#include "ParamModeClear.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamModeClear::ParamModeClear(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadIntParam({ "s_scoresec","a_scoresec","b_scoresec","c_scoresec",
      "number_diffx","missioncomp_x","missioncomp_y","missioncomp_animespeed","bg_alpha" });
   LoadVecParam({ "time_pos" ,"score_pos" });
}