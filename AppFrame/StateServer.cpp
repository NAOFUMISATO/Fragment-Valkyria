
/*****************************************************************//**
 * \file   StateServer.cpp
 * \brief  各状態一括管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "StateServer.h"
#include "StateBaseRoot.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 状態関係
    */
   namespace State {
      StateServer::StateServer(std::string_view key, std::shared_ptr<StateBaseRoot> state) {
         Register(key, state);
         PushBack(key);
      }

      void StateServer::Register(std::string_view key, std::shared_ptr<StateBaseRoot> state) {
         if (_stateRegistry.contains(key.data())) {
            _stateRegistry.erase(key.data());
         }
         _stateRegistry.emplace(key, state);
      }

      void StateServer::PushBack(std::string_view key) {
         if (!_stateRegistry.contains(key.data())) {
            return;   // キーが未登録
         }
         auto pushScene = _stateRegistry[key.data()];
         pushScene->Enter();
         _stateList.push_back(pushScene);
      }

      void StateServer::PopBack() {
         if (_stateList.empty()) {
            return;
         }
         _stateList.pop_back();
         if (_stateList.empty()) {
            return;
         }
         _stateList.back()->Enter();
      }

      void StateServer::GoToState(std::string_view key) {
         PopBack();
         PushBack(key.data());
      }

      void StateServer::Input(Input::InputManager& input) {
         if (_stateList.empty()) {
            return;
         }
         _stateList.back()->Input(input);
      }

      void StateServer::Update() {
         if (_stateList.empty()) {
            return;
         }
         _stateList.back()->Update();
      }

      void StateServer::Draw() const {
         if (_stateList.empty()) {
            return;
         }
         _stateList.back()->Draw();
      }
   }
}