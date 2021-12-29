
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
#include "ObjectServer.h"

using namespace FragmentValkyria::Object;

ObjectBase::ObjectBase(Game::GameMain& gameMain) :_gameMain{ gameMain } {
}

ObjectBase::~ObjectBase() {
}

void ObjectBase::ComputeWorldTransform() {
   auto [sx, sy, sz] = _scale.GetXYZ();
   auto [rx, ry, rz] = _rotation.GetXYZ();
   auto [px, py, pz] = _position.GetXYZ();
   auto world = Matrix44();
   world.Scale(sx, sy, sz, true);
   world.RotateZ(rz, false);
   world.RotateX(rx, false);
   world.RotateY(ry, false);
   world.Transfer(px, py, pz, false);
   _worldTransform = world;
}

ObjectServer& ObjectBase::GetObjServer() {
   return _gameMain.objServer();
}

AppFrame::Resource::LoadJson& ObjectBase::GetLoadJson() {
   return _gameMain.loadJson();
}

void ObjectBase::stateServer(std::unique_ptr<StateServer> state) {
   _stateServer = std::move(state);
}

void ObjectBase::modelAnimeComponent(std::unique_ptr<Model::ModelAnimeComponent> model) {
   _modelAnimeComponent = std::move(model);
}

void ObjectBase::cameraComponent(std::shared_ptr<Camera::CameraComponent> camera) {
   _cameraComponent = camera;
}