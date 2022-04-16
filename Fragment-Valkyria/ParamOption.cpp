#include "ParamOption.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamOption::ParamOption(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadIntParam({"default_deadzone","deadzone_min","deadzone_max","aimsens_x" ,"aimsens_y" ,
      "aimbar_x","aimbar_y","camerasens_x","camerasens_y","camerabar_x","camerabar_y","deadzone_x" ,"deadzone_y" ,
      "deadzonebar_x","deadzonebar_y","return_x" ,"return_y" ,"bg_alpha" });
   LoadDoubleParam({ "default_camera_sens","default_aim_sens", "sensivity_min","sensivity_max" });
}
