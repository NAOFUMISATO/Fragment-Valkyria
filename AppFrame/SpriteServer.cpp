
/*****************************************************************//**
 * \file   SpriteServer.cpp
 * \brief  スプライトサーバー
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteServer.h"
#include <DxLib.h>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

namespace AppFrame {
   namespace Sprite {
      void SpriteServer::Add(std::unique_ptr<SpriteBaseRoot> spr) {
         spr->Init();
         if (_updating) {
            // 更新中は保留中の動的配列に追加する
            _pendingSprites.emplace_back(std::move(spr));
         }
         else {
            //更新中でなければ処理を回す用の動的配列に追加する
            _runSprites.emplace_back(std::move(spr));
         }
      }

      void SpriteServer::Input(AppFrame::Input::InputManager& input) {
         _updating = true;
         // _runSpritesに登録されているActive状態のスプライトの入力処理を回す
         for (auto&& spr : _runSprites) {
            if (spr->IsActive()) {
               spr->Input(input);
            }
         }
         _updating = false;
      }
      /// 更新
      void SpriteServer::Update() {
         _updating = true;
         // _runSpritesに登録されているActive状態のスプライトの更新処理を回す
         for (auto&& spr : _runSprites) {
            if (spr->IsActive()) {
               spr->Update();
            }
         }
         _updating = false;

         // 保留中のスプライトを_runSpritesに移動する
         _runSprites.insert(_runSprites.end(),
            make_move_iterator(_pendingSprites.begin()),
            make_move_iterator(_pendingSprites.end()));

         // 保留中のスプライトを削除する
         _pendingSprites.clear();

         // 死んだスプライトを削除する
         erase_if(_runSprites, [](auto&& act) { return act->IsDead(); });
      }

      void SpriteServer::Render() {
         for (auto&& spr : _runSprites) {
            // _runSpritesに登録されているActive状態のスプライトの描画処理を回す
            if (spr->IsActive()) {
               spr->Draw();
            }
         }
      }

      void SpriteServer::Clear() {
         // 全ての動的配列内の要素を削除する
         _runSprites.clear();
         _pendingSprites.clear();
      }
   }
}

