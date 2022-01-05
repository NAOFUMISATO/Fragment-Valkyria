
#include "ObjectBaseRoot.h"
#include "GameBase.h"
#include "ObjectServer.h"

namespace AppFrame {
   namespace Object {

      ObjectBaseRoot::ObjectBaseRoot(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      }

      ObjectBaseRoot::~ObjectBaseRoot() {
      }

      void ObjectBaseRoot::ComputeWorldTransform() {
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

      ObjectServer& ObjectBaseRoot::GetObjServer() {
         return _gameBase.objServer();
      }

      AppFrame::Resource::LoadJson& ObjectBaseRoot::GetLoadJson() {
         return _gameBase.loadJson();
      }

      void ObjectBaseRoot::stateServer(std::unique_ptr<StateServer> state) {
         _stateServer = std::move(state);
      }
   }
}