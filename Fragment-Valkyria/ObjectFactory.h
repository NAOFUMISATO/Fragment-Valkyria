#pragma once
/*****************************************************************//**
 * \file   ObjectFactory.h
 * \brief  オブジェクト生成の一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "CreatorBase.h"
#include "SpawnRecord.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class Game;
   }
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief オブジェクト生成関係
    */
   namespace Create {
       class SpawnRecord;
      /**
       * \class オブジェクト生成の一括管理クラス
       * \brief 各オブジェクトの生成管理クラスを登録して使用する
       */
      class ObjectFactory {
         using Vector4 = AppFrame::Math::Vector4;
         using SpawnTable = std::vector<SpawnRecord>;
      public:
         /**
          * \brief コンストラクタ
          */
         ObjectFactory();
         /**
          * \brief オブジェクトの生成管理クラスの登録
          * \param type 生成管理クラスを関連付ける際に登録する任意の文字列
          * \param creator 登録する生成管理クラスのインスタンス
          * \return 登録できたならtrue,でなければfalseを返す
          */
         bool Register(std::string_view type, std::unique_ptr<CreatorBase> creator);
         /**
          * \brief 各オブジェクトの生成管理クラスの生成メソッドを呼び出す
          * \param type 生成管理クラスを関連付けた任意の文字列
          * \return 生成したインスタンスのポインタ
          */
         std::unique_ptr<Object::ObjectBase> Create(std::string_view type);
         /**
          * \brief コンテナの全要素削除
          */
         void Clear();
         /**
          * \brief スポーンテーブルをセットする
          * \param key セットするスポーンテーブル名
          */
         void SetSpawnTable(std::string_view key);
         /**
          * \brief スポーンテーブルの更新を行う
          */
         void UpdateSpawn();
         /**
          * \brief スポーンテーブル情報をjsonファイルから読み込む
          * \param jsonName jsonファイル名
          * \param tableNames スポーンテーブル名
          */
         void LoadSpawnTables(const std::filesystem::path jsonName, const std::vector<std::string> tableNames);
         /**
          * \brief スポーンテーブル情報を連想配列に登録する
          * \param key スポーンテーブル名
          * \param spawnTable スポーンテーブル
          */
         void LoadSpawnTable(std::string_view key, SpawnTable& spawnTable);
         /**
          * \brief スポーンテーブルを連想配列から取得する
          * \param key 取得するスポーンテーブル名
          * \return スポーンテーブル
          */
         SpawnTable GetSpawnTable(std::string_view key);

      private:
         std::unordered_map<std::string, std::unique_ptr<CreatorBase>> _creatorMap; //!< オブジェクト生成管理クラスを登録する連想配列
         std::unordered_map<std::string, SpawnTable> _spawnTableMap;                //!< スポーンテーブル情報の連想配列
         SpawnTable _spawnTable{ SpawnRecord() };                                   //!< スポーンテーブル情報クラス
         unsigned int _progress{ 0 };                                               //!< 進捗
         unsigned int _spawnProgress{ 0 };                                          //!< スポーンの進捗
      };
   }
}
