
/*****************************************************************//**
 * \file   Laser.cpp
 * \brief  レーザーの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Laser.h"
#include "GameMain.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("laser",
      { "radius" });
   const double Radius = paramMap["radius"];
}

using namespace FragmentValkyria::Enemy;

Laser::Laser(Game::GameMain& gameMain) : Object::ObjectBase{ gameMain } {
}

void Laser::Update() {
   _stateServer->Update();
}

void Laser::Draw() {
   _stateServer->Draw();
}

void Laser::StateBase::Draw() {
   auto firstPos = AppFrame::Math::ToDX(_owner._position);
   auto secondPos = AppFrame::Math::ToDX(_owner._end);
   auto radius = static_cast<float>(Radius);
   DrawCapsule3D(firstPos, secondPos, radius, 5, 
      AppFrame::Math::Utility::GetColorCode(255, 255, 0), 
      AppFrame::Math::Utility::GetColorCode(255, 255, 255), TRUE);
}

void Laser::StateIrradiation::Enter() {
   // この状態になった時のゲームのフレームカウントの保存
   _stateCnt = _owner.gameMain().modeServer().frameCount();
}

void Laser::StateIrradiation::Update() {
   auto count = _owner.gameMain().modeServer().frameCount();
   if (count-_stateCnt >= 60 * 3) {
      _owner.SetDead();
   }
}
