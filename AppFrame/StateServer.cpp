
/*****************************************************************//**
 * \file   StateServer.cpp
 * \brief  �e��Ԉꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "StateServer.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
#include "StateBaseRoot.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ��Ԋ֌W
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
#ifndef _DEBUG
         if (!_stateRegistry.contains(key.data())) {
            return;   // �L�[�����o�^
         }
#else
         try {
            if (!_stateRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]����ԃ��W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto pushScene = _stateRegistry[key.data()];
         pushScene->Enter();
         _stateList.push_back(pushScene);
      }

      void StateServer::PopBack() {
#ifndef _DEBUG
         if (_stateList.empty()) {
            return;
         }
#else
         try {
            if (_stateList.empty()) {
               throw std::logic_error("----------���X�g�����̏�Ԃ��폜���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _stateList.back()->Exit();
         _stateList.pop_back();

#ifndef _DEBUG
         if (_stateList.empty()) {
            return;
         }
#else
         try {
            if (_stateList.empty()) {
               throw std::logic_error("----------���X�g�����̏�Ԃ̓������������s���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _stateList.back()->Enter();
      }

      void StateServer::GoToState(std::string_view key) {
         InsertBelowBack(key.data());
         PopBack();
      }

      void StateServer::InsertBelowBack(std::string_view key) {
#ifndef _DEBUG
         if (!_stateRegistry.contains(key.data())) {
            return;   // �L�[�����o�^
         }
#else
         try {
            if (!_stateRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]����ԃ��W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto insertState = _stateRegistry[key.data()];
         _stateList.insert(std::prev(_stateList.end()), insertState);
      }

      void StateServer::Input(Input::InputManager& input) {
#ifndef _DEBUG
         if (_stateList.empty()) {
            return;
         }
#else
         try {
            if (_stateList.empty()) {
               throw std::logic_error("----------���X�g�����̏�Ԃ̓��͏��������s���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _stateList.back()->Input(input);
      }

      void StateServer::Update() {
#ifndef _DEBUG
         if (_stateList.empty()) {
            return;
         }
#else
         try {
            if (_stateList.empty()) {
               throw std::logic_error("----------���X�g�����̏�Ԃ̍X�V���������s���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _stateList.back()->Update();
      }

      void StateServer::Draw() const {
#ifndef _DEBUG
         if (_stateList.empty()) {
            return;
         }
#else
         try {
            if (_stateList.empty()) {
               throw std::logic_error("----------���X�g�����̏�Ԃ̕`�揈�������s���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _stateList.back()->Draw();
      }
   }
}