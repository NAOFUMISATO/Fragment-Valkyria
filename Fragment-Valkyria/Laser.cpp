
/*****************************************************************//**
 * \file   Laser.cpp
 * \brief  レーザーの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Laser.h"

using namespace FragmentValkyria::Enemy;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("laser",
		{ "radius" });

	const double Radius = paramMap["radius"];
}

Laser::Laser(Game::GameMain& gameMain) : Object::ObjectBase{ gameMain } {


}

void Laser::Init() {

}

void Laser::Update() {
	_stateServer->Update();
}

void Laser::Draw() {
	/*auto [x, y, z] = _position.GetVec3();
	auto firstPos = Vector4(x, 0.0, y);*/
	auto firstPos = AppFrame::Math::ToDX(_position);
	auto secondPos = AppFrame::Math::ToDX(_end);
	auto radius = static_cast<float>(Radius);
	DrawCapsule3D(firstPos, secondPos, radius, 5, AppFrame::Math::Utility::GetColorCode(255, 255, 0), AppFrame::Math::Utility::GetColorCode(255, 255, 255), TRUE);
}

void Laser::StateBase::Draw() {

}

void Laser::StateIdle::Enter() {
	_owner._stateCnt = 0;
}

void Laser::StateIdle::Update() {
	if (_owner._stateCnt >= 60 * 10) {
		_owner.SetDead();
	}

	++_owner._stateCnt;
}
