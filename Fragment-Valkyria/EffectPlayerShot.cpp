#include "EffectPlayerShot.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectPlayerShot::EffectPlayerShot(Game::GameMain& gameMain) :EffectBase{gameMain} {
}

EffectPlayerShot::~EffectPlayerShot() {
}

void EffectPlayerShot::Init() {
   SetEffectLoadHandle("Shot");
   PlayEffect();
   SetPosition({0,0,0});
}

void EffectPlayerShot::Input(AppFrame::Input::InputManager& input) {
}

void EffectPlayerShot::Update() {
}

void EffectPlayerShot::Draw() {
}
