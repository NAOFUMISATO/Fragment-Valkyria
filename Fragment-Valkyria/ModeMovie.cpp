#include "ModeMovie.h"
#include "GameMain.h"

namespace {
   constexpr auto FileName = "PoorToBoss.mp4";
   constexpr auto DefaultScale = 1.0;
   constexpr auto DefaultAngle = 0.0;
}

using namespace FragmentValkyria::Mode;

ModeMovie::ModeMovie(Game::GameMain& gameMain) :ModeBase{gameMain} {
}

void ModeMovie::Init() {
   auto currentPath = _gameMain.pathServer().GetCurrentPath("Movie");
   auto filePath = currentPath/FileName;
   _mvHandle = LoadGraph(filePath.generic_string().c_str());
}

void ModeMovie::Enter() {
   SeekMovieToGraph(_mvHandle, 0);
   PlayMovieToGraph(_mvHandle);
}

void ModeMovie::Input(AppFrame::Input::InputManager& input) {
   if (input.GetXJoypad().StartClick()) {
      PauseMovieToGraph(_mvHandle);
   }
}

void ModeMovie::Update() {
   if (!GetMovieStateToGraph(_mvHandle)) {
      GetModeServer().GoToMode("Boss");
   }
}

void ModeMovie::Render() {
   GetTexComponent().DrawTexture(0,0, DefaultScale, DefaultAngle,_mvHandle);
}
