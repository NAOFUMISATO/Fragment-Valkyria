
/*****************************************************************//**
 * \file   ObjectBase.cpp
 * \brief  オブジェクトの基底
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectBase.h"
#include "GameMain.h"
#include "ModelAnimeComponent.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Object;

ObjectBase::ObjectBase(Game::GameMain& gameMain) :ObjectBaseRoot{ gameMain }, _gameMain{gameMain} {
   _collisionComponent = std::make_unique<Collision::CollisionComponent>(*this);
}

ObjectBase::~ObjectBase() {
}

void ObjectBase::modelAnimeComponent(std::unique_ptr<Model::ModelAnimeComponent> model) {
   _modelAnimeComponent = std::move(model);
}

void ObjectBase::cameraComponent(std::shared_ptr<Camera::CameraComponent> camera) {
   _cameraComponent = camera;
}