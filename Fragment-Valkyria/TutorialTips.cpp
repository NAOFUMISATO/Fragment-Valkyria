
/*****************************************************************//**
 * \file   TutorialTips.cpp
 * \brief  チュートリアルのテキスト表示
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "TutorialTips.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include "CollisionComponent.h"

namespace {
   constexpr auto MaxAlpha = 255;
   constexpr auto MoveSpeed = 3;
}
using namespace FragmentValkyria::Tutorial;

TutorialTips::TutorialTips(std::string_view tipsName) : _tipsName{ tipsName }{
}

void TutorialTips::Init() {
   _tipsNum = {
      {"MovePromotion",       1 },
      {"CameraPromotion",     2 },
      {"WeakBulletPromotion", 3 },
      {"ReloadPromotion",     4 },
      {"ObjectShootPromotion",5 },
      {"HealPromotion",       6 }
   };
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _grHandle = resServer.GetTexture(_tipsName);
   _tipsClear = {
      {"MoveClear",       false },
      {"CameraClear",     false },
      {"WeakBulletClear", false },
      {"ReloadClear",     false },
      {"ObjectShootClear",false },
      {"HealClear",       false }
   };

   _stateServer = std::make_unique<AppFrame::State::StateServer>("FadeIn", std::make_shared <StateFadeIn>(*this));
   _stateServer->Register("Judge", std::make_shared<StateJudge>(*this));
   _stateServer->Register("FadeOut", std::make_shared<StateFadeOut>(*this));

   int height;
   GetGraphSize(_grHandle, &_width, &height);
   _position = { -static_cast<double>(_width),700.0,0 };
   _alpha = 0;
   _isAppear = false;
}

void TutorialTips::Input(InputManager& input) {
   _stateServer->Input(input);
};

void TutorialTips::Update() {
   _stateServer->Update();
}

void TutorialTips::Draw() {
   _stateServer->Draw();
}

void TutorialTips::StateBase::Draw() {
   _owner.SpriteBase::Draw();
}

void TutorialTips::StateFadeIn::Enter() {
   _owner._alpha = 0;
}

void TutorialTips::StateFadeIn::Update() {
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

void TutorialTips::StateFadeIn::Exit() {
   _owner._alpha = MaxAlpha;
}

void TutorialTips::StateJudge::Enter() {
   _owner._isAppear = true;
   _isWeakReady = false;
   _isObjectReady = false;
   if (_owner._tipsNum[_owner._tipsName] == 6) {
      auto& gameInstance = Game::Game::GetInstance();
      gameInstance.playerHp(50.0);
   }
}

void TutorialTips::StateJudge::Input(InputManager& input) {
   switch (_owner._tipsNum[_owner._tipsName]) {
   case 1:
      MoveJudge(input);
      break;
   case 2:
      CameraJudge(input);
      break;
   case 3:
      WeakShootJudge(input);
      break;
   case 4:
      ReloadJudge(input);
      break;
   case 5:
      ObjectShootJudge(input);
      break;
   case 6:
      HealJudge(input);
      break;
   }
}

void TutorialTips::StateFadeOut::Enter() {
   _owner._alpha = MaxAlpha;
}

void TutorialTips::StateFadeOut::Update() {
   auto [x, y, z] = _owner._position.GetVec3();
   if (x > -static_cast<double>(_owner._width)) {
      auto delta = MaxAlpha / static_cast<float>(_owner._width);
      _owner._alpha -= static_cast<int>(delta) * MoveSpeed;
      _owner._position = AppFrame::Math::Vector4(x - MoveSpeed, y, z);
   }
   else {
      _owner.SetDead();
      if (_owner._tipsNum[_owner._tipsName] == 6) {
         auto& gameInstance = Game::Game::GetInstance();
         gameInstance.isTutorialClear(true);
         auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
         modeServer.GoToMode("Loading", 'S');
      }
   }
}

void TutorialTips::StateJudge::MoveJudge(InputManager& input) {
   auto& gameInstance = Game::Game::GetInstance();
   auto [cameraSencivity, aimSencivity, deadZone] = gameInstance.sensitivity();
   auto joypad = input.GetXJoypad();
   if (joypad.LeftStickX() > deadZone || joypad.LeftStickX() < -deadZone ||
      joypad.LeftStickY() < -deadZone || joypad.LeftStickY() > deadZone) {
      _owner._tipsClear["MoveClear"] = true;
      _owner._stateServer->GoToState("FadeOut");
   }
}

void TutorialTips::StateJudge::CameraJudge(InputManager& input) {
   auto& gameInstance = Game::Game::GetInstance();
   auto [cameraSencivity, aimSencivity, deadZone] = gameInstance.sensitivity();
   if (input.GetXJoypad().RightStickX() > deadZone || input.GetXJoypad().RightStickX() < -deadZone ||
      input.GetXJoypad().RightStickY() < -deadZone || input.GetXJoypad().RightStickY() > deadZone) {
      _owner._tipsClear["CameraClear"] = true;
      _owner._stateServer->GoToState("FadeOut");
   }
}

void TutorialTips::StateJudge::WeakShootJudge(InputManager& input) {
   if (input.GetXJoypad().LBClick()) {
      _isWeakReady = true;
   }
   if (input.GetXJoypad().RBClick() && _isWeakReady) {
      _owner._tipsClear["WeakBulletClear"] = true;
      _owner._stateServer->GoToState("FadeOut");
   }
}

void TutorialTips::StateJudge::ReloadJudge(InputManager& input) {
   if (input.GetXJoypad().XClick()) {
      _owner._tipsClear["ReloadClear"] = true;
      _owner._stateServer->GoToState("FadeOut");
   }
}

void TutorialTips::StateJudge::ObjectShootJudge(InputManager& input) {
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto& object : runObjects) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player::Player&>(*object);
         if (player.objectShoot()) {
            _isObjectReady = true;
         }
      }
   }
   if (input.GetXJoypad().RBClick()  && _isObjectReady) {
      _owner._tipsClear["ObjectShootClear"] = true;
      _owner._stateServer->GoToState("FadeOut");
   }
}

void TutorialTips::StateJudge::HealJudge(InputManager& input) {
   if (input.GetXJoypad().YClick()) {
      _owner._tipsClear["HealClear"] = true;
      _owner._stateServer->GoToState("FadeOut");
   }
}
