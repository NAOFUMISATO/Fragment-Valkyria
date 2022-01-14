
/*****************************************************************//**
 * \file   LoadJson.cpp
 * \brief  素材情報をJsonファイルから読み込む
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "LoadJson.h"
#include <tuple>
#include <string_view>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
#include <fstream>
#include "GameBase.h"
#include "CurrentPathServer.h"
#include "ResourceServer.h"
#include "Vector4.h"
#include "Texture.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief リソース関係
    */
   namespace Resource {

      LoadJson::LoadJson(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      };

      void LoadJson::LoadTextures(const std::filesystem::path jsonName) {
         // 画像情報を格納しているjsonフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("TextureJson");
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
         auto textureArray = value[jsonName.generic_string()];
         // 画像を格納しているフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto textureDirectory = _gameBase.pathServer().GetCurrentPath("Texture") / jsonName;
         // トップレベルの配列を全て回し、格納している画像情報を全て取り出す
         for (auto& textureData : textureArray) {
            const auto keyName = textureData["keyname"];    // キー
            const auto fileName = textureData["filename"];  // ファイル名
            const auto allNum = textureData["allnum"];      // 画像分割総数
            const auto xNum = textureData["xnum"];          // 画像横分割数
            const auto yNum = textureData["ynum"];          // 画像縦分割数
            const auto xSize = textureData["xsize"];        // 横画像サイズ
            const auto ySize = textureData["ysize"];        // 縦画像サイズ
            // ファイルへのパスを形成
            const auto texturePath = (textureDirectory / fileName).generic_string();
            // Textureクラスを生成し、画像情報を登録
            Texture tex = Texture(texturePath, allNum, xNum, yNum, xSize, ySize);
            // 取り出したキーと画像情報を登録したTextureクラスをResourceServerに登録する
            _gameBase.resServer().LoadTexture(keyName, tex);
         }
      }

      void LoadJson::LoadModels(const std::filesystem::path jsonName) {
         // モデル情報を格納しているjsonフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ModelJson");
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
         auto modelArray = value[jsonName.generic_string()];
         // トップレベルの配列を全て回し、格納しているモデル情報を全て取り出す
         for (auto& modelData : modelArray) {
            const auto keyName = modelData["keyname"];   //!< キー(モデルフォルダへのファイルパスと兼用)
            const auto fileName = modelData["filename"]; //!< ファイル名
            // モデルを格納しているフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
            auto modelDirectory = _gameBase.pathServer().GetCurrentPath("Model") / keyName;
            // ファイルへのパスを形成
            const auto modelPath = (modelDirectory / fileName).generic_string();
            // 取り出したキーとモデルへのファイルパスをResourceServerに登録する
            _gameBase.resServer().LoadModel(keyName, modelPath);
         }
      }

      void LoadJson::LoadSounds(const std::filesystem::path jsonName) {
         // 音源情報を格納しているjsonフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("SoundJson");
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
         auto soundArray = value[jsonName.generic_string()];
         // 音源を格納しているフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto soundDirectory = _gameBase.pathServer().GetCurrentPath("Sound") / jsonName;
         // トップレベルの配列を全て回し、格納している音源情報を全て取り出す
         for (auto& soundData : soundArray) {
            const auto keyName = soundData["keyname"];   // キー
            const auto fileName = soundData["filename"]; // ファイル名
            const auto isLoad = soundData["isload"];     // 事前読み込みの有無
            const auto volume = soundData["volume"];     // 初期音量
            // ファイルへのパスを形成
            const auto soundPath = (soundDirectory / fileName).generic_string();
            // 取り出したキー及び音源へのファイルパスと事前読み込みの有無と初期音量のtuple型をResourceServerに登録する
            _gameBase.resServer().LoadSound(keyName, std::make_tuple(soundPath, isLoad, volume));
         }
      }

      void LoadJson::LoadEffects(const std::filesystem::path jsonName) {
         // エフェクト情報を格納しているjsonフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("EffectJson");
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
         auto effectArray = value[jsonName.generic_string()];
         // エフェクトを格納しているフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto effectDirectory = _gameBase.pathServer().GetCurrentPath("Effect") / jsonName;
         // トップレベルの配列を全て回し、格納しているエフェクト情報を全て取り出す
         for (auto& effectData : effectArray) {
            const auto keyName = effectData["keyname"];    // キー
            const auto fileName = effectData["filename"];  // ファイル名
            const auto scale = effectData["scale"];        // 初期拡大率
            // ファイルへのパスを形成
            const auto effectPath = (effectDirectory / fileName).generic_string();
            // 取り出したキー及びエフェクトへのファイルパスと初期拡大率のpair型をResourceServerに登録する
            _gameBase.resServer().LoadEffect(keyName, std::make_pair(effectPath, scale));
         }
      }

      nlohmann::json LoadJson::GetParam(const std::filesystem::path jsonName, const std::string_view paramName) {
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ParamJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
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
         reading >> value;
         reading.close();
         auto paramArray = value[jsonName.generic_string()];
         for (auto& paramData : paramArray) {
            const auto param = paramData[paramName.data()];
            return param;
         }
         return -1;
      }

      Math::Vector4 LoadJson::GetVecParam(const std::filesystem::path jsonName,const std::string_view vecName) {
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ParamJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
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
         reading >> value;
         reading.close();
         auto vecArray = value[jsonName.generic_string()];
         for (auto& vecData : vecArray) {
            auto paramArray = vecData[vecName.data()];
            for (auto& vecParam : paramArray) {
               const auto x = vecParam["x"];
               const auto y = vecParam["y"];
               const auto z = vecParam["z"];
               Vector4 vec = Vector4(x,y,z);
               return vec;
            }
         }
         return {0,0,0};
      }

      std::unordered_map<std::string, nlohmann::json> LoadJson::GetParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> paramNames) {
         // 各データを格納しているjsonフォルダへのパスを、ゲーム本体側で実体を定義した静的メンバから取得する
         auto jsonDirectory = _paramJsonCurrentPath;
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

      std::unordered_map<std::string, Math::Vector4> LoadJson::GetVecParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> vecParamNames) {
         // 各Vector4データを格納しているjsonフォルダへのパスを、ゲーム本体側で実体を定義した静的メンバから取得する
         auto jsonDirectory = _paramJsonCurrentPath;
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
         auto vecArray = value[jsonName.generic_string()+"_vec"];
         // 各Vector4データを格納する連想配列を定義する
         std::unordered_map<std::string, Vector4> vecParamMap;
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
                  Vector4 vec = Vector4(x, y, z);
                  // Vector4データを取り出した際の文字列をキー、取り出したVector4データを連想配列に登録する
                  vecParamMap.emplace(vecParamNames[i],vec);
               }
            }
         }
         // 各Vector4データを登録した連想配列を返す
         return vecParamMap;
      }
   }
}