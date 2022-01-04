#include "EffectServer.h"

namespace AppFrame {
   namespace Effect {
      void EffectServer::Add(std::unique_ptr<EffectBaseRoot> efc) {
         if (_updating) {
            // 更新中は保留中の動的配列に追加する
            _pendingEffects.emplace_back(std::move(efc));
         }
         else {
            //更新中でなければ処理を回す用の動的配列に追加する
            _runEffects.emplace_back(std::move(efc));
         }
      }
      
      void EffectServer::Input(AppFrame::Input::InputManager& input){
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
         // _runEffectsに登録されているActive状態のオブジェクトの更新処理を回す
         for (auto&& efc : _runEffects) {
            if (efc->IsActive()) {
               efc->Update();
            }
         }
         _updating = false;

         // 保留中のオブジェクトを_runEffectsに移動する
         _runEffects.insert(_runEffects.end(),
            make_move_iterator(_pendingEffects.begin()),
            make_move_iterator(_pendingEffects.end()));

         // 保留中のオブジェクトを削除する
         _pendingEffects.clear();

         // 死んだアクターを削除する
         erase_if(_runEffects, [](auto&& act) { return act->IsDead(); });
      }

      void EffectServer::Render() {
         for (auto&& efc : _runEffects) {
            // _runObjectsに登録されているActive状態のオブジェクトの描画処理を回す
            if (efc->IsActive()) {
               efc->Draw();
            }
         }
      }

      void EffectServer::Clear() {
         // 全ての動的配列内の要素を削除する
         _runEffects.clear();
         _pendingEffects.clear();
      }
   }
}
