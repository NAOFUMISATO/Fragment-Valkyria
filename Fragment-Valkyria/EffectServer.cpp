
/*****************************************************************//**
 * \file   EffectServer.cpp
 * \brief  エフェクトサーバー
 *
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "EffectServer.h"
#include <EffekseerForDxLib.h>

using namespace FragmentValkyria::Effect;

void EffectServer::Add(std::unique_ptr<EffectBase> efc) {
   efc->Init();
   if (_updating) {
      // 更新中は保留中の動的配列に追加する
      _pendingEffects.emplace_back(std::move(efc));
   }
   else {
      //更新中でなければ処理を回す用の動的配列に追加する
      _runEffects.emplace_back(std::move(efc));
   }
}

void EffectServer::Input(AppFrame::Input::InputManager& input) {
   _updating = true;
   // _runEffectsに登録されているActive状態のエフェクトの入力処理を回す
   for (auto&& efc : _runEffects) {
      if (efc->IsActive()) {
         efc->Input(input);
      }
   }
   _updating = false;
}

void EffectServer::Update() {
   _updating = true;
   // _runEffectsに登録されているActive状態のエフェクトの更新処理を回す
   for (auto&& efc : _runEffects) {
      if (efc->IsActive()) {
         efc->Update();
      }
   }
   Effekseer_Sync3DSetting();
   UpdateEffekseer3D();
   _updating = false;

   // 保留中のエフェクトを_runEffectsに移動する
   _runEffects.insert(_runEffects.end(),
      make_move_iterator(_pendingEffects.begin()),
      make_move_iterator(_pendingEffects.end()));

   // 保留中のエフェクトを削除する
   _pendingEffects.clear();

   // 死んだエフェクトを削除する
   erase_if(_runEffects, [](auto&& act) { return act->IsDead(); });
}

void EffectServer::Render() {
   for (auto&& efc : _runEffects) {
      // _runEffectsに登録されているActive状態のエフェクトの描画処理を回す
      if (efc->IsActive()) {
         efc->Draw();
      }
   }
   DrawEffekseer3D();
}

void EffectServer::Clear() {
   // 全ての動的配列内の要素を削除する
   for (auto& effect : _runEffects) {
      effect->StopEffect();
   }
   _runEffects.clear();
   _pendingEffects.clear();
}