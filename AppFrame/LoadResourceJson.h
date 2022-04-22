
/*****************************************************************//**
 * \file   LoadResourceJson.h
 * \brief  ゲーム情報をJsonファイルから読み込むクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <utility>
#include <vector>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   //二重インクルード防止
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
      class LoadResourceJson {
         using Vector4 = Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          */
         LoadResourceJson();
         /**
          * \brief デフォルトデストラクタ
          */
         ~LoadResourceJson() = default;
         /**
          * \brief jsonファイルから画像情報を読み込み、リソースサーバーに登録する
          * \param jsonName jsonファイル名
          */
         void LoadTextures(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルからモデル情報を読み込み、リソースサーバーに登録する
          * \param jsonName jsonファイル名
          */
         void LoadModels(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルから音源情報を読み込み、リソースサーバーに登録する
          * \param jsonName jsonファイル名
          */
         void LoadSounds(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルからエフェクト情報を読み込み、リソースサーバーに登録する
          * \param jsonName jsonファイル名
          */
         void LoadEffects(const std::filesystem::path jsonName);

      private:
         /**
          * \brief jsonファイルを開き、コピーしたオブジェクトを返す
          * \param pathName ゲーム本体側で定義したファイルパスのキー名
          * \param jsonName jsonファイル名
          * \return nlohmann::json型のオブジェクト
          */
         nlohmann::json JsonSetUp(const std::string_view pathName, const std::filesystem::path jsonName);
      };
   }
}
