
/*****************************************************************//**
 * \file   EffectBase.cpp
 * \brief  エフェクトの基底
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBase::EffectBase(Game::GameMain& gameMain,std::string_view key) : _gameMain{gameMain} {
}

void EffectBase::Draw() {
   SetPosition();
   SetRotation();
   SetScale();
   SetSpeed();
   SetEfcColor();
}

void EffectBase::SetEffectLoadHandle(std::string_view key) {
   auto handle = _gameMain.resServer().GetEffectHandle(key);
   _loadHandle = handle;
}

void EffectBase::PlayEffect() {
   _playHandle = PlayEffekseer3DEffect(_loadHandle);
}

bool EffectBase::IsPlaying() {
   auto nowPlaying = IsEffekseer3DEffectPlaying(_playHandle);
   if (nowPlaying != -1) {
      return true;
   }
   return false;
}

void EffectBase::StopEffect() {
   StopEffekseer3DEffect(_playHandle);
}

void EffectBase::SetPosition() {
   auto [x, y, z] = _position.GetVec3();
   auto cx = static_cast<float>(x);
   auto cy = static_cast<float>(y);
   auto cz = static_cast<float>(z);
   SetPosPlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
}

void EffectBase::SetRotation() {
   auto [x, y, z] = _rotation.GetVec3();
   auto cx = static_cast<float>(x);
   auto cy = static_cast<float>(y);
   auto cz = static_cast<float>(z);
   SetRotationPlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
}

void EffectBase::SetScale() {
   auto [x, y, z] = _scale.GetVec3();
   auto cx = static_cast<float>(x);
   auto cy = static_cast<float>(y);
   auto cz = static_cast<float>(z);
   SetScalePlayingEffekseer3DEffect(_playHandle, cx, cy, cz);
}

void EffectBase::SetSpeed() {
   auto cSpeed = static_cast<float>(_speed);
   SetSpeedPlayingEffekseer3DEffect(_playHandle, cSpeed);
}

void EffectBase::SetEfcColor() {
   auto [red, green, blue, alpha] = _color.GetColorA();
   SetColorPlayingEffekseer3DEffect(_playHandle, red, green, blue, alpha);
}