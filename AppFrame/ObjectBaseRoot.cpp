
#include "ObjectBaseRoot.h"
#include "GameBase.h"
#include "ObjectServer.h"
#include "StateServer.h"
#include "DxUtility.h"

namespace {
   constexpr auto BillBoardPosX = 0.5f;
   constexpr auto BillBoardPosY = 1.0f;
   constexpr auto BillBoardTransFlag = true;
}
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

      ObjectServer& ObjectBaseRoot::GetObjServer() const {
         return _gameBase.objServer();
      }

      Effect::EffectServer& ObjectBaseRoot::GetEfcServer() const {
         return _gameBase.efcServer();
      }

      AppFrame::Resource::LoadResourceJson& ObjectBaseRoot::GetLoadJson() const{
         return _gameBase.loadJson();
      }

      void ObjectBaseRoot::stateServer(std::unique_ptr<StateServer> state) {
         _stateServer = std::move(state);
      }

      void ObjectBaseRoot::DrawBillBoard(AppFrame::Math::Vector4 pos, double size, double angle, std::vector<int> handle,int animeSpeed) {
         _cnt++;
         auto animeMax = handle.size();
         auto animeNo = (_cnt / animeSpeed) % animeMax;
         DrawBillboard3D(AppFrame::Math::ToDX(pos), BillBoardPosX, BillBoardPosY,
            static_cast<float>(size), static_cast<float>(angle), handle[animeNo], BillBoardTransFlag);
      }
   }
}