#include "EffectPlayerShot.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectPlayerShot::EffectPlayerShot(Game::GameMain& gameMain) :EffectBase{gameMain} {
   Init();
}

EffectPlayerShot::~EffectPlayerShot() {
}

void EffectPlayerShot::Init() {
   SetEffectLoadHandle("Shot");
   PlayEffect();
   SetPosition(_gameMain.objServer().GetPosition("PlayerPos"));
   SetRotation(_gameMain.objServer().GetForward("PlayerRot"));
   SetSpeed(5.0);
   SetEfcColor(std::make_tuple(12, 255, 255,255));
}

void EffectPlayerShot::Input(AppFrame::Input::InputManager& input) {
}

void EffectPlayerShot::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
}

void EffectPlayerShot::Draw() {
}
