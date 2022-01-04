
/*****************************************************************//**
 * \file   LoadJson.h
 * \brief  ゲーム情報をJsonファイルから読み込むクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#pragma once
#include <DxLib.h>
#include <unordered_map>
#include <string_view>
#include <filesystem>
#include <nlohmann/json.hpp>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   //二重インクルード防止
   namespace Game {
      class GameBase;
   }
   namespace Math {
      class Vector4;
   }
   /**
    * \brief リソース関係
    */
   namespace Resource {
      //二重インクルード防止
      class Texture;
      /**
       * \class ゲーム情報をJsonファイルから読み込むクラス
       * \brief リソースサーバーに登録する素材情報及び各データをjsonファイルから読み込む
       */
      class LoadJson {
         using Vector4 = Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param gameBase ゲームベースの参照
          */
         LoadJson(Game::GameBase& gameBase);
         /**
          * \brief デフォルトデストラクタ
          */
         ~LoadJson() = default;
         /**
          * \brief jsonファイルから画像情報を読み込み、リソースサーバーに登録する
          * \param jsonFileName jsonファイル名
          */
         void LoadTextures(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルからモデル情報を読み込み、リソースサーバーに登録する
          * \param jsonFileName jsonファイル名
          */
         void LoadModels(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルから音源情報を読み込み、リソースサーバーに登録する
          * \param jsonFileName jsonファイル名
          */
         void LoadSounds(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルからデータを取得する
          * \param jsonName jsonファイル名
          * \param paramName データ名
          * \return データ
          */
         nlohmann::json GetParam(const std::filesystem::path jsonName, const std::string_view paramName);
         /**
          * \brief jsonファイルからVecotor4データを取得する
          * \param jsonName jsonファイル名
          * \param vecName データ名
          * \return データ
          */
         Vector4 GetVecParam(const std::filesystem::path jsonName,const std::string_view vecName);

      private:
         Game::GameBase& _gameBase;   //!< ゲームベースの参照
      };
   }
}
