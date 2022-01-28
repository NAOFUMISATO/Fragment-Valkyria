
/*****************************************************************//**
 * \file   ModelAnimeComponent.cpp
 * \brief  モデルのアニメーション管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModelAnimeComponent.h"
#include "GameMain.h"
#include "ObjectBase.h"
using namespace FragmentValkyria::Model;

ModelAnimeComponent::ModelAnimeComponent(Object::ObjectBase& owner) : ModelComponent{ owner } {

}

ModelAnimeComponent::~ModelAnimeComponent() {
   MV1DetachAnim(_modelHandle, _attachNum);
}

void ModelAnimeComponent::Init() {
}

void ModelAnimeComponent::Update() {

   // 再生時間をセットする
   MV1SetAttachAnimTime(_modelHandle, _attachNum, _playTime);

   // ワールド行列更新
   ModelComponent::Update();

   // 再生時間を進める
   _playTime += (1.f * static_cast<float>(_timeRate));

   // 再生時間が総再生時間以上か
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

   // アニメーションブレンドの処理
   if (_blending) {
      _blendRate += 0.1f;  // ブレンド率の変化
      _repeatedCount = 0;  // ブレンド中はアニメーション回数を上昇させない
      // 現在のアニメーションのブレンド率を徐々に減少させる
      MV1SetAttachAnimBlendRate(_modelHandle, _attachNum, 1.f - _blendRate);
      // 次のアニメーションのブレンド率を徐々に上昇させる
      MV1SetAttachAnimBlendRate(_modelHandle, _nextAttachNum, _blendRate);
      // ブレンド率が上限に達したなら、アニメーションブレンドの終了処理を行う
      if (_blendRate >= 1.0f) {
         _animIndex = _newAnimIndex;  // アニメ番号の更新
         MV1DetachAnim(_modelHandle, _attachNum);     // 現在のアニメーションをデタッチ
         MV1DetachAnim(_modelHandle, _nextAttachNum); // 次のアニメーションをデタッチ
         // 新しいアニメーションにアタッチする
         _attachNum = MV1AttachAnim(_modelHandle, _newAnimIndex, -1, FALSE);
         // 総再生時間の取得
         _totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _attachNum);
         _playTime = 0.0f;  // 再生時間の初期化
         _blending = false; // ブレンド中かの判定をfalse
      }
   }
}

//void ModelAnimeComponent::Draw() {
//   MV1DrawModel(_modelHandle);
//}

void ModelAnimeComponent::ChangeAnime(std::string_view animeName, bool repeate,double animeSpeed) {
   // アニメーションブレンド中に、ChangeAnime関数を呼んだならブレンドの終了処理を行う
   if (_blending) {
      _animIndex = _newAnimIndex;  // アニメ番号の更新
      MV1DetachAnim(_modelHandle, _attachNum);     // 現在のアニメーションをデタッチ
      MV1DetachAnim(_modelHandle, _nextAttachNum); // 次のアニメーションをデタッチ
      // 新しいアニメーションにアタッチする
      _attachNum = MV1AttachAnim(_modelHandle, _newAnimIndex, -1, FALSE);
      // 総再生時間の取得
      _totalTime = MV1GetAttachAnimTotalTime(_modelHandle, _attachNum);
      _playTime = 0.0f;  // 再生時間の初期化
      _blending = false; // ブレンド中かの判定をfalse
   }
   // 新しくアタッチするアニメ番号をResourceServerから取得する
   _newAnimIndex = _owner.gameMain().resServer().GetModelAnimIndex(_key, animeName);
   // 同一のアニメーションに切り替えようとしたなら返す
   if (_animIndex == _newAnimIndex) {
      return;
   }
   // アニメーションブレンドのために新しいアニメーションにアタッチしておく
   _nextAttachNum = MV1AttachAnim(_modelHandle, _newAnimIndex,-1,FALSE);
   _blendRate = 0.f;    // ブレンド率の初期化
   _blending = true;    // ブレンド中かの判定をtrue
   _repeate = repeate;  // 引数からアニメーションを繰り返すかの判定を取得
   _timeRate = animeSpeed;
   _repeatedCount = 0;  // アニメーション回数の初期化
}
