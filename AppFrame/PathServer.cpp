
/*****************************************************************//**
 * \file   PathServer.cpp
 * \brief  �t�@�C���p�X�̈ꊇ�Ǘ�
 *
 * \author Sato Naofumi
 * \date   December 2021
 *********************************************************************/
#include "PathServer.h"
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
         if (!_paths.contains(key.data())) {
            return "";
         }
         return _paths[key.data()];
      }
   }
}
