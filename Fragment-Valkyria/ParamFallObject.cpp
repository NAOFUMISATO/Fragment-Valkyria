#include "ParamFallObject.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamFallObject::ParamFallObject(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadIntParam({ "fallpoint_animespeed" });
   LoadDoubleParam({ "gravity", "shoot_speed", "up_speed", "rotate_angle","updown_range", "right_hand_up_value",
      "up_value","fallpoint_pos_y","fallpoint_scale"});
}
