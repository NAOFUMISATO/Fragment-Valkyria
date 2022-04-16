#include "ParamCameraComponent.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamCameraComponent::ParamCameraComponent(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadIntParam({ "dead_zone_divide_value"});
   LoadDoubleParam({ "center_height", "spring_k","divide_t" });
}
