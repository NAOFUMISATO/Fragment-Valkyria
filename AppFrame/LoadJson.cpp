
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
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("TextureJson");
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
         auto textureArray = value[jsonName.generic_string()];
         auto textureDirectory = _gameBase.pathServer().GetCurrentPath("Texture") / jsonName;
         for (auto& textureData : textureArray) {
            const auto keyName = textureData["keyname"];
            const auto fileName = textureData["filename"];
            const auto allNum = textureData["allnum"];
            const auto xNum = textureData["xnum"];
            const auto yNum = textureData["ynum"];
            const auto xSize = textureData["xsize"];
            const auto ySize = textureData["ysize"];
            const auto texturePath = (textureDirectory / fileName).generic_string();
            Texture tex = Texture(texturePath, allNum, xNum, yNum, xSize, ySize);
            _gameBase.resServer().LoadTexture(keyName, tex);
         }
      }

      void LoadJson::LoadModels(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ModelJson");
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
         auto modelArray = value[jsonName.generic_string()];
         auto modelDirectory = _gameBase.pathServer().GetCurrentPath("Model") / jsonName;
         for (auto& modelData : modelArray) {
            const auto keyName = modelData["keyname"];
            const auto fileName = modelData["filename"];
            const auto modelPath = (modelDirectory / fileName).generic_string();
            _gameBase.resServer().LoadModel(keyName, modelPath);
         }
      }

      void LoadJson::LoadSounds(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("SoundJson");
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
         auto soundArray = value[jsonName.generic_string()];
         auto soundDirectory = _gameBase.pathServer().GetCurrentPath("Sound") / jsonName;
         for (auto& soundData : soundArray) {
            const auto keyName = soundData["keyname"];
            const auto fileName = soundData["filename"];
            const auto isLoad = soundData["isload"];
            const auto volume = soundData["volume"];
            const auto soundPath = (soundDirectory / fileName).generic_string();
            _gameBase.resServer().LoadSound(keyName, std::make_tuple(soundPath, isLoad, volume));
         }
      }

      void LoadJson::LoadEffects(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("EffectJson");
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
         auto effectArray = value[jsonName.generic_string()];
         auto effectDirectory = _gameBase.pathServer().GetCurrentPath("Effect") / jsonName;
         for (auto& effectData : effectArray) {
            const auto keyName = effectData["keyname"];
            const auto fileName = effectData["filename"];
            const auto scale = effectData["scale"];
            const auto effectPath = (effectDirectory / fileName).generic_string();
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
         auto jsonDirectory = _paramJsonCurrentPath;
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
         std::unordered_map<std::string, nlohmann::json> paramMap;
         for (auto i = 0; i < paramNames.size(); i++) {
            for (auto& paramData : paramArray) {
               const auto param = paramData[paramNames[i].data()];
               paramMap.emplace(paramNames[i], param);
            }
         }
         return paramMap;
      }

      std::unordered_map<std::string, Math::Vector4> LoadJson::GetVecParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> vecParamNames) {
         auto jsonDirectory = _paramJsonCurrentPath;
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
         auto vecArray = value[jsonName.generic_string()+"_vec"];
         std::unordered_map<std::string, Vector4> vecParamMap;
         for (auto i = 0; i < vecParamNames.size(); i++) {
            for (auto& vecData : vecArray) {
               auto vecParamArray = vecData[vecParamNames[i].data()];
               for (auto& vecParam : vecParamArray) {
                  const auto x = vecParam["x"];
                  const auto y = vecParam["y"];
                  const auto z = vecParam["z"];
                  Vector4 vec = Vector4(x, y, z);
                  vecParamMap.emplace(vecParamNames[i],vec);
               }
            }
         }
         return vecParamMap;
      }
   }
}