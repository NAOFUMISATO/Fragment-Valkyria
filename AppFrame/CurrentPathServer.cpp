
/*****************************************************************//**
 * \file   CurrentPathServer.cpp
 * \brief  ファイルパスの一括管理
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
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief ファイルパス関係
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
            // 登録しようとしたキーがあれば古いキーは削除する
            if (_currentPaths.contains(key.data())) {
               _currentPaths.erase(key.data());
            }
            // パスマップに登録する
            _currentPaths.emplace(key, path);
         }
      }

      std::filesystem::path CurrentPathServer::GetCurrentPath(std::string_view key) {
#ifndef _DEBUG
         // 指定のキーが無ければ空の文字列を返す
         if (!_currentPaths.contains(key.data())) {
            return "";
         }
#else
         // 指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_currentPaths.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]がパスコンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // 指定のキーのファイルパスを返す
         return _currentPaths[key.data()];
      }
   }
}
