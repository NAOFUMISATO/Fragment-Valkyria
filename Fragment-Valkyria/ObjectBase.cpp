
/*****************************************************************//**
 * \file   ObjectBase.cpp
 * \brief  オブジェクトの基底
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectBase.h"
#include "ModelAnimeComponent.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"

using namespace FragmentValkyria::Object;

ObjectBase::ObjectBase() {
   _collisionComponent = std::make_unique<Collision::CollisionComponent>(*this);
}

ObjectBase::~ObjectBase() {
}

void ObjectBase::ComputeWorldTransform() {
   auto [sx, sy, sz] = _scale.GetVec3();
   auto [rx, ry, rz] = _rotation.GetVec3();
   auto [px, py, pz] = _position.GetVec3();
   auto world = Matrix44();
   world.Scale(sx, sy, sz, true);
   world.RotateZ(rz, false);
   world.RotateX(rx, false);
   world.RotateY(ry, false);
   world.Transfer(px, py, pz, false);
   _worldTransform = world;
}

void ObjectBase::modelAnimeComponent(std::unique_ptr<Model::ModelAnimeComponent> model) {
   _modelAnimeComponent = std::move(model);
}

void ObjectBase::cameraComponent(std::shared_ptr<Camera::CameraComponent> camera) {
   _cameraComponent = camera;
}

void ObjectBase::stateServer(std::unique_ptr<StateServer> state) {
   _stateServer = std::move(state);
}