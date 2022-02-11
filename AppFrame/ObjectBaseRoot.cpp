
/*****************************************************************//**
 * \file   ObjectBaseRoot.cpp
 * \brief  オブジェクトの基底の基底
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ObjectBaseRoot.h"
#include "GameBase.h"
#include "ObjectServer.h"
#include "StateServer.h"
#include "DxUtility.h"
#include "ModeServer.h"
#include "TextureComponent.h"

namespace AppFrame {
   namespace Object {

      ObjectBaseRoot::ObjectBaseRoot(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      }

      ObjectBaseRoot::~ObjectBaseRoot() {
      }

      void ObjectBaseRoot::ComputeWorldTransform() {
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

      ObjectServer& ObjectBaseRoot::GetObjServer() const {
         return _gameBase.objServer();
      }

      Effect::EffectServer& ObjectBaseRoot::GetEfcServer() const {
         return _gameBase.efcServer();
      }

      Texture::TextureComponent& ObjectBaseRoot::GetTexComponent() const {
         return _gameBase.texComponent();
      }

      Sound::SoundComponent& ObjectBaseRoot::GetSoundComponent() const {
         return _gameBase.soundComponent();
      }

      AppFrame::Resource::LoadResourceJson& ObjectBaseRoot::GetLoadJson() const{
         return _gameBase.loadresJson();
      }

      void ObjectBaseRoot::stateServer(std::unique_ptr<StateServer> state) {
         _stateServer = std::move(state);
      }
   }
}