
/*****************************************************************//**
 * \file   ObjectServer.cpp
 * \brief  オブジェクトの一括管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectServer.h"

using namespace FragmentValkyria::Object;

void ObjectServer::Add(std::unique_ptr<ObjectBase> obj) {
   if (_updating) {
      // 更新中は_pendingActorsに追加する
      _pendingObjects.emplace_back(std::move(obj));
   }
   else {
      _runObjects.emplace_back(std::move(obj));
   }
}
/// 入力
void ObjectServer::Input(AppFrame::Input::InputManager& input) {
   _updating = true;
   for (auto&& obj : _runObjects) {
      if (obj->IsActive()) {
         // アクターに入力する
         obj->Input(input);
      }
   }
   _updating = false;
}
/// 更新
void ObjectServer::Update() {
   _updating = true;
   for (auto&& obj : _runObjects) {
      if (obj->IsActive()) {
         // アクターに入力する
         obj->Update();
      }
   }
   _updating = false;

   // 保留中のアクターを_actorsに移動する
   _runObjects.insert(_runObjects.end(),
      make_move_iterator(_pendingObjects.begin()),
      make_move_iterator(_pendingObjects.end()));

   // 保留中のアクターをクリアする
   _pendingObjects.clear();

   // 死んだアクターを削除する
   erase_if(_runObjects, [](auto&& act) { return act->IsDead(); });
}
/// 描画
void ObjectServer::Render() {
   for (auto&& obj : _runObjects) {
      if (obj->IsActive()) {
         obj->Draw();
      }
   }
}
/// 全アクターの削除
void ObjectServer::Clear() {
   _runObjects.clear();
   _pendingObjects.clear();
}
/// アクターの登録
void ObjectServer::Register(std::string_view key, Vector4 vec) {
   if (_objRegistry.contains(key.data())) {
      _objRegistry[key.data()] = vec;
   }
   _objRegistry.emplace(key, vec);
}
/// アクターの一覧の取得
AppFrame::Math::Vector4 ObjectServer::GetPosition(std::string_view key) {
   if (_objRegistry.contains(key.data())) {
      return _objRegistry[key.data()];
   }
   // 未登録
   return { 0, 0, 0 };
}
/// 登録したアクターの位置を得る
AppFrame::Math::Vector4 ObjectServer::GetForward(std::string_view key) {
   if (_objRegistry.contains(key.data())) {
      return _objRegistry[key.data()];
   }
   // 未登録
   return { 0, 0, 0 };
}


