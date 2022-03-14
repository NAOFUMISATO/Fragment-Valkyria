#include "CheckUpTips.h"
#include "GameMain.h"
#include "ObjectServer.h"
#include "LargeEnemy.h"
namespace {
   constexpr auto MaxAlpha = 255;
   constexpr auto MoveSpeed = 4;
}

using namespace FragmentValkyria::Clear;

CheckUpTips::CheckUpTips(Game::GameMain& gameMain) :Sprite::SpriteBase{gameMain} {
}

void CheckUpTips::Init() {

   _grHandle = GetResServer().GetTexture("CheckUp");
   _stateServer = std::make_unique<AppFrame::State::StateServer>("FadeIn", std::make_shared <StateFadeIn>(*this));
   _stateServer->Register("Judge", std::make_shared<StateJudge>(*this));
   _stateServer->Register("FadeOut", std::make_shared<StateFadeOut>(*this));

   int height;
   GetGraphSize(_grHandle, &_width, &height);
   _position = { -static_cast<double>(_width),700.0,0 };
   _alpha = 0;
   _isAppear = false;
}

void CheckUpTips::Input(InputManager& input) {
   _stateServer->Input(input);
}

void CheckUpTips::Update() {
   _stateServer->Update();
}

void CheckUpTips::Draw() {
   _stateServer->Draw();
}

void CheckUpTips::StateBase::Draw() {
   _owner.SpriteBase::Draw();
}

void CheckUpTips::StateFadeIn::Enter() {
   _owner._alpha = 0;
}

void CheckUpTips::StateFadeIn::Update() {
   auto [x, y, z] = _owner._position.GetVec3();
   if (x < 0) {
      auto delta = static_cast<float>(MaxAlpha) / static_cast<float>(_owner._width);
      _owner._alpha += static_cast<int>(delta) * MoveSpeed;
      _owner._position = AppFrame::Math::Vector4(x + MoveSpeed, y, z);
   }
   else {
      _owner._stateServer->GoToState("Judge");
   }
}

void CheckUpTips::StateFadeIn::Exit() {
   _owner._alpha = MaxAlpha;
}

void CheckUpTips::StateJudge::Update() {
   for (auto& object : _owner._gameMain.objServer().runObjects()) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::LargeEnemy) {
         auto& largeEnemy = dynamic_cast<Enemy::LargeEnemy&>(*object);
         if (!largeEnemy.ClearObjectHitCheckFromPlayer()) {
            _owner._stateServer->GoToState("FadeOut");
         }
      }
   }
}

void CheckUpTips::StateFadeOut::Enter() {
   _owner._alpha = MaxAlpha;
}

void CheckUpTips::StateFadeOut::Update() {
   auto [x, y, z] = _owner._position.GetVec3();
   if (x > -static_cast<double>(_owner._width)) {
      auto delta = MaxAlpha / static_cast<float>(_owner._width);
      _owner._alpha -= static_cast<int>(delta) * MoveSpeed;
      _owner._position = AppFrame::Math::Vector4(x - MoveSpeed, y, z);
   }
   else {
      _owner.SetDead();
   }
}
