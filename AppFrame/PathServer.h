#pragma once
/*****************************************************************//**
 * \file   PathServer.h
 * \brief  ファイルパス一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <utility>
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
      class PathServer {
      public:
         /**
          * \brief 任意の文字列をキーにしてファイルパスを管理
          */
         using PathMap = std::unordered_map<std::string, std::filesystem::path>;
         /**
          * \brief コンストラクタ
          */
         PathServer();
         /**
          * \brief デストラクタ
          */
         virtual ~PathServer() = default;
         /**
          * \brief 初期化処理
          */
         virtual void Init();
         /**
          * \brief コンテナの解放
          */
         virtual void Clear();
         /**
          * \brief ファイルパスの一括登録
          * \param pathMap ファイルパスマップ
          */
         void RegistPath(const PathMap& pathMap);
         /**
          * \brief ファイルパスの取得
          * \param key キーとなる任意の文字列
          * \return ファイルパス
          */
         std::filesystem::path GetPath(std::string_view key);

      private:
         std::unordered_map < std::string, std::filesystem::path > _paths;   //!< ファイルパスコンテナ
      };
   }
}