#include "ParamGatling.h"
#include "GameMain.h"

using namespace FragmentValkyria::Param;

ParamGatling::ParamGatling(Game::GameMain& gameMain, std::string_view key) :ParamBase{ gameMain,key } {
   LoadDoubleParam({ "speed" });
}
