
/*****************************************************************//**
 * \file   EffectBaseRoot.cpp
 * \brief  エフェクトの基底の基底
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectBaseRoot.h"
#include "GameBase.h"
#include "ResourceServer.h"
namespace AppFrame {
   namespace Effect {
      EffectBaseRoot::EffectBaseRoot(Game::GameBase& gameBase, std::string_view key) :_gameBase{gameBase} {
      }

      void EffectBaseRoot::Draw() {
         SetPosition(_position);
         SetRotation(_rotation);
         SetScale(_scale);
         SetSpeed(_speed);
         SetEfcColor(_color);
      }

      void EffectBaseRoot::SetEffectLoadHandle(std::string_view key) {
         auto handle = _gameBase.resServer().GetEffectHandle(key);
         _loadHandle = handle;
      }

      void EffectBaseRoot::PlayEffect() {
         _playHandle = PlayEffekseer3DEffect(_loadHandle);
      }

      bool EffectBaseRoot::IsPlaying() {
       auto nowPlaying = IsEffekseer3DEffectPlaying(_playHandle);
       if (nowPlaying != -1) {
          return true;
       }
       return false;
      }

      void EffectBaseRoot::StopEffect() {
         StopEffekseer3DEffect(_playHandle);
      }

      void EffectBaseRoot::SetPosition(Math::Vector4 pos){
         _position = pos;
         auto [x, y, z] = _position.GetVec3();
         auto cx = static_cast<float>(x);
         auto cy = static_cast<float>(y);
         auto cz = static_cast<float>(z);
         SetPosPlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
      }

      void EffectBaseRoot::SetRotation(Math::Vector4 rot) {
         _rotation = rot;
         auto [x, y, z] = _rotation.GetVec3();
         auto cx = static_cast<float>(x);
         auto cy = static_cast<float>(y);
         auto cz = static_cast<float>(z);
         SetRotationPlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
      }

      void EffectBaseRoot::SetScale(Math::Vector4 sca) {
         _scale = sca;
         auto [x, y, z] = _scale.GetVec3();
         auto cx = static_cast<float>(x);
         auto cy = static_cast<float>(y);
         auto cz = static_cast<float>(z);
         SetScalePlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
      }

      double EffectBaseRoot::GetSpeed() {
         auto speed = GetSpeedPlayingEffekseer3DEffect(_playHandle);
         auto cSpeed = static_cast<double>(speed);
         _speed = cSpeed;
         return _speed;
      }

      void EffectBaseRoot::SetSpeed(double speed) {
         _speed = speed;
         auto cSpeed = static_cast<float>(_speed);
         SetSpeedPlayingEffekseer3DEffect(_playHandle, cSpeed);
      }

      void EffectBaseRoot::SetEfcColor(Data::Color color){
         _color = color;
         auto [red, green, blue, alpha] = _color.GetColorA();
         SetColorPlayingEffekseer3DEffect(_playHandle, red, green, blue, alpha);
      }
   }
}
