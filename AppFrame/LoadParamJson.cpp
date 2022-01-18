#include "LoadParamJson.h"
#include <string_view>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
#include <fstream>
#include "Vector4.h"
namespace AppFrame {

   namespace Resource {

      std::unordered_map<std::string, nlohmann::json> LoadParamJson::GetParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> paramNames) {
         // 各データを格納しているjsonフォルダへのパスを取得する
         auto jsonDirectory = GetCurrentPath();
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         // 指定したjsonファイルを読み取り専用で開く
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         // 指定したファイルを開くのに失敗したならlogic_errorを検出し、デバッガに出力する
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "ファイルが開けませんでした。----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         // ファイルの中身を取り出す
         reading >> value;
         // ファイルを閉じる
         reading.close();
         // トップレベルの配列のキーを、引数から指定し取り出す
         auto paramArray = value[jsonName.generic_string()];
         // 各データを格納する連想配列をnlohmann::json型で定義する
         std::unordered_map<std::string, nlohmann::json> paramMap;
         // 引数に指定した文字列の動的配列を全て回す
         for (auto i = 0; i < paramNames.size(); i++) {
            // トップレベルの配列を全て回し、引数の文字列に一致する格納している各データを全て取り出す
            for (auto& paramData : paramArray) {
               const auto param = paramData[paramNames[i].data()];
               // 各データを取り出した際の文字列をキー、取り出したnlohmann::json型データを連想配列に登録する
               paramMap.emplace(paramNames[i], param);
            }
         }
         // 各データを登録した連想配列を返す
         return paramMap;
      }

      std::unordered_map<std::string, Math::Vector4> LoadParamJson::GetVecParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> vecParamNames) {
         // 各Vector4データを格納しているjsonフォルダへのパスを取得する
         auto jsonDirectory = GetCurrentPath();
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         // 指定したjsonファイルを読み取り専用で開く
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         // 指定したファイルを開くのに失敗したならlogic_errorを検出し、デバッガに出力する
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "ファイルが開けませんでした。----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         // ファイルの中身を取り出す
         reading >> value;
         // ファイルを閉じる
         reading.close();
         // トップレベルの配列のキーを、引数から指定し取り出す
         auto vecArray = value[jsonName.generic_string() + "_vec"];
         // 各Vector4データを格納する連想配列を定義する
         std::unordered_map<std::string, Math::Vector4> vecParamMap;
         // 引数に指定した文字列の動的配列を全て回す
         for (auto i = 0; i < vecParamNames.size(); i++) {
            // トップレベルの配列を全て回し、引数の文字列に一致する格納している各Vector4データを全て取り出す
            for (auto& vecData : vecArray) {
               auto vecParamArray = vecData[vecParamNames[i].data()];
               // 取り出したVector4データの各要素を取り出す
               for (auto& vecParam : vecParamArray) {
                  const auto x = vecParam["x"];   // x成分
                  const auto y = vecParam["y"];   // y成分
                  const auto z = vecParam["z"];   // z成分
                  // Vector4クラスを生成し、Vector4データを登録
                  auto vec = Math::Vector4(x, y, z);
                  // Vector4データを取り出した際の文字列をキー、取り出したVector4データを連想配列に登録する
                  vecParamMap.emplace(vecParamNames[i], vec);
               }
            }
         }
         // 各Vector4データを登録した連想配列を返す
         return vecParamMap;
      }
   }
}