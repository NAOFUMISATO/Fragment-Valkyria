
/*****************************************************************//**
 * \file   LoadResourceJson.cpp
 * \brief  素材情報をJsonファイルから読み込む
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "LoadResourceJson.h"
#include <tuple>
#include <string_view>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
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

      LoadResourceJson::LoadResourceJson() {
      };

      void LoadResourceJson::LoadTextures(const std::filesystem::path jsonName) {
         // jsonファイルを開き、オブジェクトを取り出す
         auto textureArray = JsonSetUp("TextureJson", jsonName);
         // 画像を格納しているフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto& pathInstance = Path::CurrentPathServer::GetInstance();
         auto textureDirectory = pathInstance.GetCurrentPath("Texture") / jsonName;
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
            auto& resServerInstance = ResourceServer::GetInstance();
            resServerInstance.LoadTexture(keyName, tex);
         }
      }

      void LoadResourceJson::LoadModels(const std::filesystem::path jsonName) {
         // jsonファイルを開き、オブジェクトを取り出す
         auto modelArray = JsonSetUp("ModelJson", jsonName);
         // トップレベルの配列を全て回し、格納しているモデル情報を全て取り出す
         for (auto& modelData : modelArray) {
            const auto keyName = modelData["keyname"];   //!< キー(モデルフォルダへのファイルパスと兼用)
            const auto fileName = modelData["filename"]; //!< ファイル名
            // モデルを格納しているフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
            auto& pathInstance = Path::CurrentPathServer::GetInstance();
            auto modelDirectory = pathInstance.GetCurrentPath("Model") / keyName;
            // ファイルへのパスを形成
            const auto modelPath = (modelDirectory / fileName).generic_string();
            // 取り出したキーとモデルへのファイルパスをResourceServerに登録する
            auto& resServerInstance = ResourceServer::GetInstance();
            resServerInstance.LoadModel(keyName, modelPath);
         }
      }

      void LoadResourceJson::LoadSounds(const std::filesystem::path jsonName) {
         // jsonファイルを開き、オブジェクトを取り出す
         auto soundArray = JsonSetUp("SoundJson", jsonName);
         // 音源を格納しているフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto& pathInstance = Path::CurrentPathServer::GetInstance();
         auto soundDirectory = pathInstance.GetCurrentPath("Sound") / jsonName;
         // トップレベルの配列を全て回し、格納している音源情報を全て取り出す
         for (auto& soundParam : soundArray) {
            const auto keyName = soundParam["keyname"];    // キー
            const auto fileName = soundParam["filename"];  // ファイル名
            const auto isLoad = soundParam["isload"];      // 事前読み込みの有無
            const auto volume = soundParam["volume"];      // 初期音量
            const auto is3Dsound = soundParam["is3Dsound"];// 3Dサウンド設定にするか
            const auto radius = soundParam["radius"];      // 3Dサウンドの聞こえる距離
            // ファイルへのパスを形成
            const auto soundPath = (soundDirectory / fileName).generic_string();
            // 音源データを作成
            auto soundData = SoundData(soundPath, volume, is3Dsound, radius);
            // 取り出したキー及び音源データと事前読み込みの有無のペアをResourceServerに登録する
            auto& resServerInstance = ResourceServer::GetInstance();
            resServerInstance.LoadSound(keyName, std::make_pair(soundData, isLoad));
         }
      }

      void LoadResourceJson::LoadEffects(const std::filesystem::path jsonName) {
         // jsonファイルを開き、オブジェクトを取り出す
         auto effectArray = JsonSetUp("EffectJson", jsonName);
         // トップレベルの配列を全て回し、格納しているエフェクト情報を全て取り出す
         for (auto& effectData : effectArray) {
            const auto keyName = effectData["keyname"];    // キー
            const auto fileName = effectData["filename"];  // ファイル名
            const auto scale = effectData["scale"];        // 初期拡大率
            const auto speed = effectData["speed"];        // 初期再生速度
            // エフェクトを格納しているフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
            auto& pathInstance = Path::CurrentPathServer::GetInstance();
            auto effectDirectory = pathInstance.GetCurrentPath("Effect") / keyName;
            // ファイルへのパスを形成
            const auto effectPath = (effectDirectory / fileName).generic_string();
            // 取り出したキー及びエフェクトへのファイルパスと初期拡大率と初期再生速度のtuple型をResourceServerに登録する
            auto& resServerInstance = ResourceServer::GetInstance();
            resServerInstance.LoadEffect(keyName, std::make_tuple(effectPath, scale, speed));
         }
      }

      nlohmann::json LoadResourceJson::JsonSetUp(const std::string_view pathName, const std::filesystem::path jsonName) {
         // 値情報を格納しているjsonフォルダへのパスを、ゲーム本体側で定義したパスサーバーから取得する
         auto& pathInstance = Path::CurrentPathServer::GetInstance();
         auto jsonDirectory = pathInstance.GetCurrentPath(pathName);
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
         // トップレベルの配列のキーを引数から指定し、jsonオブジェクトとして返す
         return value[jsonName.generic_string()];
      }
   }
}