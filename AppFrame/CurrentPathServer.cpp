
/*****************************************************************//**
 * \file   CurrentPathServer.cpp
 * \brief  �t�@�C���p�X�̈ꊇ�Ǘ�
 *
 * \author Sato Naofumi
 * \date   December 2021
 *********************************************************************/
#include "CurrentPathServer.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �t�@�C���p�X�֌W
    */
   namespace Path {

      CurrentPathServer::CurrentPathServer() {
         Init();
      }

      void CurrentPathServer::Init() {
         Clear();
      }

      void CurrentPathServer::Clear() {
         _currentPaths.clear();
      }

      void CurrentPathServer::RegistCurrentPath(const CurrentPathMap& pathMap) {
         for (auto&& [key, path] : pathMap) {
            // �o�^���悤�Ƃ����L�[������ΌÂ��L�[�͍폜����
            if (_currentPaths.contains(key.data())) {
               _currentPaths.erase(key.data());
            }
            // �p�X�}�b�v�ɓo�^����
            _currentPaths.emplace(key, path);
         }
      }

      std::filesystem::path CurrentPathServer::GetCurrentPath(std::string_view key) {
#ifndef _DEBUG
         // �w��̃L�[��������΋�̕������Ԃ�
         if (!_currentPaths.contains(key.data())) {
            return "";
         }
#else
         // �w��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_currentPaths.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]���p�X�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // �w��̃L�[�̃t�@�C���p�X��Ԃ�
         return _currentPaths[key.data()];
      }
   }
}
