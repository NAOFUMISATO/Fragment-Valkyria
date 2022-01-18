#pragma once
/*****************************************************************//**
 * \file   LoadParamJson.h
 * \brief  jsonファイルからデータを読み込み、取得するクラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "Singleton.h"
#include <nlohmann/json.hpp>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   // 二重インクルード防止
   namespace Math {
      class Vector4;
   }
   /**
    * \brief リソース関係
    */
   namespace Resource {
      /**
       * \class jsonファイルからデータを読み込み、取得するクラス
       * \brief jsonからデータを取得し、戻り値にデータの連想配列を返す
       */
      class LoadParamJson :public Temp::Singleton<LoadParamJson> {
      private:
         /**
          * \brief コンストラクタ(外部でインスタンス生成禁止)
          */
         LoadParamJson() {};
         /**
          * \brief デストラクタ
          */
         ~LoadParamJson()override {};

      public:
         // シングルトンクラステンプレートでのインスタンス生成は許可
         friend class Singleton<LoadParamJson>;
         /**
          * \brief jsonファイルから各データを連想配列にして返す
          * \param jsonName jsonファイル名
          * \param paramNames データ名群
          * \return データをnlohmann::json型で登録した連想配列
          */
         static std::unordered_map<std::string, nlohmann::json> GetParamMap(
            const std::filesystem::path jsonName, const std::vector<std::string_view> paramNames);
         /**
          * \brief jsonファイルからVector4データを連想配列にして返す
          * \param jsonName jsonファイル名
          * \param vecParamNames Vector4データ名群
          * \return Vector4データを登録した連想配列
          */
         static std::unordered_map<std::string, Math::Vector4> GetVecParamMap(
            const std::filesystem::path jsonName, const std::vector<std::string_view> vecParamNames);

      private:
         /**
          * \brief jsonのカレントパスを返す
          * \return jsonのカレントパス
          */
         static std::filesystem::path GetCurrentPath() {
            auto currentPath = "resource/json/param";
            return currentPath;
         }
      };
   }
}
