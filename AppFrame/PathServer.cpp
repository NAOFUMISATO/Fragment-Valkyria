
/*****************************************************************//**
 * \file   PathServer.cpp
 * \brief  ファイルパスの一括管理
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
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief ファイルパス関係
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
               throw std::logic_error("----------キー[" + message + "]がパスコンテナに存在しませんでした。----------\n");
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
