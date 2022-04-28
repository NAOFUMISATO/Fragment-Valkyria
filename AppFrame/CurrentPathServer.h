#pragma once
/*****************************************************************//**
 * \file   CurrentPathServer.h
 * \brief  ファイルパス一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <utility>
#include "Singleton.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief ファイルパス関係
    */
   namespace Path {
      /**
       * \class ファイルパス一括管理クラス
       * \brief ファイルパスの登録、取得を行う
       */
      class CurrentPathServer :public Temp::Singleton<CurrentPathServer> {
      private:
         /**
          * \brief コンストラクタ
          */
         CurrentPathServer();
         /**
          * \brief デストラクタ
          */
         virtual ~CurrentPathServer() = default;
      public:
         friend class Temp::Singleton<CurrentPathServer>;
         using CurrentPathMap = std::unordered_map<std::string, std::filesystem::path>;
         /**
          * \brief 初期化処理
          */
         void Init();
         /**
          * \brief コンテナの解放
          */
         void Clear();
         /**
          * \brief ファイルパスの一括登録
          * \param pathMap ファイルパスマップ
          */
         void RegistCurrentPath(const CurrentPathMap& pathMap);
         /**
          * \brief ファイルパスの取得
          * \param key キーとなる任意の文字列
          * \return ファイルパス
          */
         std::filesystem::path GetCurrentPath(std::string_view key);

      private:
         std::unordered_map < std::string, std::filesystem::path > _currentPaths;   //!< ファイルパスコンテナ
      };
   }
}