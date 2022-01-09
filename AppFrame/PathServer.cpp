
/*****************************************************************//**
 * \file   PathServer.cpp
 * \brief  �t�@�C���p�X�̈ꊇ�Ǘ�
 *
 * \author Sato Naofumi
 * \date   December 2021
 *********************************************************************/
#include "PathServer.h"
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

      PathServer::PathServer() {
         Init();
      }

      void PathServer::Init() {
         Clear();
      }

      void PathServer::Clear() {
         _paths.clear();
      }

      void PathServer::RegistPath(const PathMap& pathMap) {
         for (auto&& [key, path] : pathMap) {
            if (_paths.contains(key.data())) {
               _paths.erase(key.data());
            }
            _paths.emplace(key, path);
         }
      }

      std::filesystem::path PathServer::GetPath(std::string_view key) {
#ifndef _DEBUG
         if (!_paths.contains(key.data())) {
            return "";
         }
#else
         try {
            if (!_paths.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]���p�X�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _paths[key.data()];
      }
   }
}
