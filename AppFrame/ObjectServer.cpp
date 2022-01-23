
/*****************************************************************//**
 * \file   ObjectServer.cpp
 * \brief  オブジェクトの一括管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectServer.h"
#include <DxLib.h>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

namespace AppFrame {
   namespace Object {
      void ObjectServer::Add(std::unique_ptr<ObjectBaseRoot> obj) {
         if (_updating) {
            // 更新中は保留中の動的配列に追加する
            _pendingObjects.emplace_back(std::move(obj));
         }
         else {
            //更新中でなければ処理を回す用の動的配列に追加する
            _runObjects.emplace_back(std::move(obj));
         }
      }

      void ObjectServer::Input(AppFrame::Input::InputManager& input) {
         _updating = true;
         // _runObjectsに登録されているActive状態のオブジェクトの入力処理を回す
         for (auto&& obj : _runObjects) {
            if (obj->IsActive()) {
               obj->Input(input);
            }
         }
         _updating = false;
      }
      /// 更新
      void ObjectServer::Update() {
         _updating = true;
         // _runObjectsに登録されているActive状態のオブジェクトの更新処理を回す
         for (auto&& obj : _runObjects) {
            if (obj->IsActive()) {
               obj->Update();
            }
         }
         _updating = false;

         // 保留中のオブジェクトを_runObjectsに移動する
         _runObjects.insert(_runObjects.end(),
            make_move_iterator(_pendingObjects.begin()),
            make_move_iterator(_pendingObjects.end()));

         // 保留中のオブジェクトを削除する
         _pendingObjects.clear();

         // 死んだアクターを削除する
         erase_if(_runObjects, [](auto&& act) { return act->IsDead(); });
      }

      void ObjectServer::Render() {
         for (auto&& obj : _runObjects) {
            // _runObjectsに登録されているActive状態のオブジェクトの描画処理を回す
            if (obj->IsActive()) {
               obj->Draw();
            }
         }
      }

      void ObjectServer::Clear() {
         // 全ての動的配列内の要素を削除する
         _runObjects.clear();
         _pendingObjects.clear();
      }

      void ObjectServer::Register(std::string_view key, Vector4 vec) {
         if (_objRegistry.contains(key.data())) {
            _objRegistry[key.data()] = vec;
         }
         _objRegistry.emplace(key, vec);
      }

      AppFrame::Math::Vector4 ObjectServer::GetVecData(std::string_view key) {
#ifndef _DEBUG
         if (!_objRegistry.contains(key.data())) {
            return{ 0,0,0 };
         }
#else
         try {
            if (!_objRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]がオブジェクトレジストリに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _objRegistry[key.data()];
      }
   }
}

