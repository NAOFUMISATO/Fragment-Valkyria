
/*****************************************************************//**
 * \file   ModelAnimeComponent.cpp
 * \brief  モデルのアニメーション管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModelAnimeComponent.h"
using namespace FragmentValkyria::Model;

ModelAnimeComponent::ModelAnimeComponent(Object::ObjectBase& owner) : ModelComponent{ owner } {

}

ModelAnimeComponent::~ModelAnimeComponent() {
   MV1DetachAnim(_modelHandle, _attachIndex);
}

void ModelAnimeComponent::Init() {
}

void ModelAnimeComponent::Register(std::string_view key, int animIndex) {
   if (_animeRegistry.contains(key.data())) {
      _animeRegistry.erase(key.data());
   }
   _animeRegistry.emplace(key, animIndex);
}

void ModelAnimeComponent::Update() {
   // 再生時間をセットする
   MV1SetAttachAnimTime(_modelHandle, _attachIndex, _playTime);

   // ワールド行列更新
   ModelComponent::Update();

   // 再生時間を進める
   _playTime += (1.f * _timeRate);

   if (_playTime >= _totalTime) {
      if (_repeate) {
         // リピート再生あり
         _playTime = 0.0f;
         ++_repeatedCount;
      }
      else {
         // リピート再生なし
         _repeatedCount = 1;
         _playTime = _totalTime;
      }
   }
}

void ModelAnimeComponent::Draw() {
   MV1DrawModel(_modelHandle);
}

void ModelAnimeComponent::ChangeAnime(std::string_view key, bool repeate) {
   auto newAnimIndex = _animeRegistry[key.data()];
   if (_animIndex == newAnimIndex) {
      return;
   }
   _animIndex = newAnimIndex;
   // アニメーションをデタッチする
   MV1DetachAnim(_modelHandle, _attachIndex);
   // 新しいアニメーションをアタッチする
   _attachIndex = MV1AttachAnim(_modelHandle, newAnimIndex, -1, FALSE);
   // アタッチしたアニメーションの総再生時間を取得する
   _totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _attachIndex);

   _playTime = 0.0f;
   _repeatedCount = 0;
   _repeate = repeate;
}
