
/*****************************************************************//**
 * \file   ParamBase.cpp
 * \brief  各値の管理を行う基底
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamBase.h"
#include "GameBase.h"
#include "CurrentPathServer.h"

namespace AppFrame {
   /**
    * \brief 値管理関係
    */
   namespace Param {
      ParamBase::ParamBase(Game::GameBase& gameBase, std::string_view key) :
         _gameBase{ gameBase }, _key{ key } {
      }

      int ParamBase::GetIntParam(std::string_view paramName) {
#ifndef _DEBUG
         // 指定のキーが無ければ空の文字列を返す
         if (!_intMap.contains(paramName.data())) {
            return "";
         }
#else
         // 指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_intMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------キー[" + message + "]がパスコンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _intMap[paramName.data()];
      }

      float ParamBase::GetFloatParam(std::string_view paramName) {
#ifndef _DEBUG
         // 指定のキーが無ければ空の文字列を返す
         if (!_floatMap.contains(paramName.data())) {
            return "";
         }
#else
         // 指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_floatMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------キー[" + message + "]が値コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _floatMap[paramName.data()];
      }

      double ParamBase::GetDoubleParam(std::string_view paramName) {
#ifndef _DEBUG
         // 指定のキーが無ければ空の文字列を返す
         if (!_doubleMap.contains(paramName.data())) {
            return "";
         }
#else
         // 指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_doubleMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------キー[" + message + "]が値コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _doubleMap[paramName.data()];
      }

      bool ParamBase::GetBoolParam(std::string_view paramName) {
#ifndef _DEBUG
         // 指定のキーが無ければ空の文字列を返す
         if (!_boolMap.contains(paramName.data())) {
            return "";
         }
#else
         // 指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_boolMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------キー[" + message + "]が値コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _boolMap[paramName.data()];
      }

      Math::Vector4 ParamBase::GetVecParam(std::string_view paramName) {
#ifndef _DEBUG
         // 指定のキーが無ければ空の文字列を返す
         if (!_vecMap.contains(paramName.data())) {
            return "";
         }
#else
         // 指定のキーが無ければ、エラーメッセージを出力する
         try {
            if (!_vecMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------キー[" + message + "]が値コンテナに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _vecMap[paramName.data()];
      }

      void ParamBase::LoadIntParam(std::vector<std::string_view> paramNames) {
         // jsonファイルを開き、オブジェクトを取り出す
         auto paramArray = JsonSetUp();
         // 引数に指定した文字列の動的配列を全て回す
         for (auto i = 0; i < paramNames.size(); i++) {
            // トップレベルの配列を全て回し、引数の文字列に一致する格納している各データを全て取り出す
            for (auto& paramData : paramArray) {
               const int param = paramData[paramNames[i].data()];
               // 登録しようとしたキーがあれば古いキーは削除する
               if (_intMap.contains(paramNames[i].data())) {
                  _intMap.erase(paramNames[i].data());
               }
               // 各データを取り出した際の文字列をキー、取り出したint型データを連想配列に登録する
               _intMap.emplace(paramNames[i], param);
            }
         }
      }

      void ParamBase::LoadFloatParam(std::vector<std::string_view> paramNames) {
         // jsonファイルを開き、オブジェクトを取り出す
         auto paramArray = JsonSetUp();
         // 引数に指定した文字列の動的配列を全て回す
         for (auto i = 0; i < paramNames.size(); i++) {
            // トップレベルの配列を全て回し、引数の文字列に一致する格納している各データを全て取り出す
            for (auto& paramData : paramArray) {
               const float param = paramData[paramNames[i].data()];
               // 登録しようとしたキーがあれば古いキーは削除する
               if (_floatMap.contains(paramNames[i].data())) {
                  _floatMap.erase(paramNames[i].data());
               }
               // 各データを取り出した際の文字列をキー、取り出したfloat型データを連想配列に登録する
               _floatMap.emplace(paramNames[i], param);
            }
         }
      }

      void ParamBase::LoadDoubleParam(std::vector<std::string_view> paramNames) {
         // jsonファイルを開き、オブジェクトを取り出す
         auto paramArray = JsonSetUp();
         // 引数に指定した文字列の動的配列を全て回す
         for (auto i = 0; i < paramNames.size(); i++) {
            // トップレベルの配列を全て回し、引数の文字列に一致する格納している各データを全て取り出す
            for (auto& paramData : paramArray) {
               const double param = paramData[paramNames[i].data()];
               // 登録しようとしたキーがあれば古いキーは削除する
               if (_doubleMap.contains(paramNames[i].data())) {
                  _doubleMap.erase(paramNames[i].data());
               }
               // 各データを取り出した際の文字列をキー、取り出したdouble型データを連想配列に登録する
               _doubleMap.emplace(paramNames[i], param);
            }
         }
      }

      void ParamBase::LoadBoolParam(std::vector<std::string_view> paramNames) {
         // jsonファイルを開き、オブジェクトを取り出す
         auto paramArray = JsonSetUp();
         // 引数に指定した文字列の動的配列を全て回す
         for (auto i = 0; i < paramNames.size(); i++) {
            // トップレベルの配列を全て回し、引数の文字列に一致する格納している各データを全て取り出す
            for (auto& paramData : paramArray) {
               const bool param = paramData[paramNames[i].data()];
               // 登録しようとしたキーがあれば古いキーは削除する
               if (_boolMap.contains(paramNames[i].data())) {
                  _boolMap.erase(paramNames[i].data());
               }
               // 各データを取り出した際の文字列をキー、取り出したbool型データを連想配列に登録する
               _boolMap.emplace(paramNames[i], param);
            }
         }
      }

      void ParamBase::LoadVecParam(std::vector<std::string_view> paramNames) {
         // 値情報を格納しているjsonフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ParamJson");
         auto jsonPath = (jsonDirectory / _key).generic_string() + ".json";
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
         // トップレベルの配列のキーを引数から指定し、jsonオブジェクトとして返す
         auto vecArray = value[_key + "_vec"];
         // 引数に指定した文字列の動的配列を全て回す
         for (auto i = 0; i < paramNames.size(); i++) {
            // トップレベルの配列を全て回し、引数の文字列に一致する格納している各Vector4データを全て取り出す
            for (auto& vecData : vecArray) {
               auto vecParamArray = vecData[paramNames[i].data()];
               // 取り出したVector4データの各要素を取り出す
               for (auto& vecParam : vecParamArray) {
                  const double x = vecParam["x"];   // x成分
                  const double y = vecParam["y"];   // y成分
                  const double z = vecParam["z"];   // z成分
                  // Vector4クラスを生成し、Vector4データを登録
                  auto vec = Math::Vector4(x, y, z);
                  // 登録しようとしたキーがあれば古いキーは削除する
                  if (_vecMap.contains(paramNames[i].data())) {
                     _vecMap.erase(paramNames[i].data());
                  }
                  // Vector4データを取り出した際の文字列をキー、取り出したVector4データを連想配列に登録する
                  _vecMap.emplace(paramNames[i], vec);
               }
            }
         }
      }

      nlohmann::json ParamBase::JsonSetUp() {
         // 値情報を格納しているjsonフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ParamJson");
         auto jsonPath = (jsonDirectory / _key).generic_string() + ".json";
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
         // トップレベルの配列のキーを引数から指定し、jsonオブジェクトとして返す
         return value[_key];
      }
   }
}
