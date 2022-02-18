
/*****************************************************************//**
 * \file   Bullet.cpp
 * \brief  �v���C���[�̉��u��U���̒e�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Bullet.h"
#include "CollisionComponent.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Player;

namespace {
	constexpr double Radius = 20.0;
	constexpr double Speed = 50.0;
}

Bullet::Bullet(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void Bullet::Init() {
	_moved = GetObjServer().GetVecData("CamTarget") - _position;
	_moved.Normalized();
}

void Bullet::Input(InputManager& input) {

}

void Bullet::Update() {
	// ��Ԃ̍X�V
	_stateServer->Update();
	// ���[���h�s��̍X�V
	ComputeWorldTransform();
	//// ���f���̍X�V
	//_modelAnimeComponent->Update();

}

void Bullet::Draw() {
	_stateServer->Draw();
}

void Bullet::Move() {
	_position = _position + _moved * Speed;

}

void Bullet::HitCheckFromLargeEnemy() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
		_stateServer->GoToState("Die");
	}
}

void Bullet::HitCheckFromPoorEnemyGatling() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemyGatling) {
	
		_stateServer->GoToState("Die");
	}
}

void Bullet::OutCheckFromStage() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {

		_stateServer->GoToState("Die");
	}
}

void Bullet::StateBase::Draw() {
	auto pos = AppFrame::Math::ToDX(_owner._position);
	auto radius = static_cast<float>(Radius);
	DrawSphere3D(pos, radius, 20, GetColor(0, 0, 255), GetColor(0, 0, 0), TRUE);
}

void Bullet::StateShoot::Enter() {

}

void Bullet::StateShoot::Input(InputManager& input) {

}

void Bullet::StateShoot::Update() {
	_owner.Move();
	_owner._collisionComponent->LargeEnemyFromBullet();
	_owner._collisionComponent->OutStage();

	_owner.HitCheckFromLargeEnemy();
	_owner.HitCheckFromPoorEnemyGatling();
	_owner.OutCheckFromStage();
}

void Bullet::StateDie::Enter() {

}

void Bullet::StateDie::Input(InputManager& input) {

}

void Bullet::StateDie::Update() {
	_owner.SetDead();
}


