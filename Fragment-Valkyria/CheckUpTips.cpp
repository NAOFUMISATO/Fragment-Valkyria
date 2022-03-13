#include "CheckUpTips.h"

using namespace FragmentValkyria::Clear;

CheckUpTips::CheckUpTips(Game::GameMain& gameMain) :Sprite::SpriteBase{gameMain} {
}

void CheckUpTips::Init() {

   _grHandle = GetResServer().GetTexture("");
   _stateServer = std::make_unique<AppFrame::State::StateServer>("FadeIn", std::make_shared <StateFadeIn>(*this));
   _stateServer->Register("Stop", std::make_shared<StateJudge>(*this));
   _stateServer->Register("FadeOut", std::make_shared<StateFadeOut>(*this));

   int height;
   GetGraphSize(_grHandle, &_width, &height);
   _position = { -static_cast<double>(_width),700.0,0 };
   _alpha = 0;
   _isAppear = false;
}

void CheckUpTips::Update() {

}

void CheckUpTips::Draw() {

}

void CheckUpTips::StateBase::Draw() {

}

void CheckUpTips::StateFadeIn::Enter() {

}

void CheckUpTips::StateFadeIn::Update() {

}

void CheckUpTips::StateFadeIn::Exit() {

}

void CheckUpTips::StateJudge::Enter() {

}

void CheckUpTips::StateJudge::Input(InputManager& input) {

}

void CheckUpTips::StateFadeOut::Enter() {

}

void CheckUpTips::StateFadeOut::Update() {

}
