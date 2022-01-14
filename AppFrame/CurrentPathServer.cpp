
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
            if (_currentPaths.contains(key.data())) {
               _currentPaths.erase(key.data());
            }
            _currentPaths.emplace(key, path);
         }
      }

      std::filesystem::path CurrentPathServer::GetCurrentPath(std::string_view key) {
#ifndef _DEBUG
         if (!_paths.contains(key.data())) {
            return "";
         }
#else
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
         return _currentPaths[key.data()];
      }
   }
}
