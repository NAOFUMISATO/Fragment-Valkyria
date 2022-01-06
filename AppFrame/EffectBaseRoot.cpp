#include "EffectBaseRoot.h"
#include "GameBase.h"
#include "ResourceServer.h"
namespace AppFrame {
   namespace Effect {
      EffectBaseRoot::EffectBaseRoot(Game::GameBase& gameBase) :_gameBase{gameBase} {
      }

      EffectBaseRoot::~EffectBaseRoot() {
      };

      void EffectBaseRoot::SetEffectLoadHandle(std::string_view key) {
         auto handle = _gameBase.resServer().GetEffectHandle(key);
         _loadHandle = handle;
      }

      void EffectBaseRoot::PlayEffect() {
         _playHandle = PlayEffekseer3DEffect(_loadHandle);
      }

      void EffectBaseRoot::SetPosition(Math::Vector4 pos){
         _position = pos;
         auto [x, y, z] = _position.GetXYZ();
         auto cx = static_cast<float>(x);
         auto cy = static_cast<float>(y);
         auto cz = static_cast<float>(z);
         SetPosPlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
      }

      void EffectBaseRoot::SetRotation(Math::Vector4 rot) {
         _rotation = rot;
         auto [x, y, z] = _rotation.GetXYZ();
         auto cx = static_cast<float>(x);
         auto cy = static_cast<float>(y);
         auto cz = static_cast<float>(z);
         SetRotationPlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
      }

      void EffectBaseRoot::SetScale(Math::Vector4 sca) {
         _scale = sca;
         auto [x, y, z] = _scale.GetXYZ();
         auto cx = static_cast<float>(x);
         auto cy = static_cast<float>(y);
         auto cz = static_cast<float>(z);
         SetScalePlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
      }

      double EffectBaseRoot::GetSpeed() {
         auto speed = GetSpeedPlayingEffekseer3DEffect(_playHandle);
         auto cSpeed = static_cast<double>(speed);
         return cSpeed;
      }

      void EffectBaseRoot::SetSpeed(double speed) {
         auto cSpeed = static_cast<float>(speed);
         SetSpeedPlayingEffekseer3DEffect(_playHandle, cSpeed);
      }

      void EffectBaseRoot::SetEfcColor(std::tuple<int, int, int, int> color){
         _color = color;
         auto [red, green, blue, alpha] = _color;
         SetColorPlayingEffekseer3DEffect(_playHandle, red, green, blue, alpha);
      }
   }
}
