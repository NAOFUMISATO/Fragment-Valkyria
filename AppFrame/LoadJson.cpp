
/*****************************************************************//**
 * \file   LoadJson.cpp
 * \brief  �f�ޏ���Json�t�@�C������ǂݍ���
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
#include "PathServer.h"
#include "ResourceServer.h"
#include "Vector4.h"
#include "Texture.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {

      LoadJson::LoadJson(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      };

      void LoadJson::LoadTextures(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("TextureJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "�t�@�C���͌�����܂���ł����B----------\n");
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
         auto textureDirectory = _gameBase.pathServer().GetPath("Texture") / jsonName;
         for (auto& textureData : textureArray) {
            auto keyName = textureData["keyname"];
            auto fileName = textureData["filename"];
            auto allNum = textureData["allnum"];
            auto xNum = textureData["xnum"];
            auto yNum = textureData["ynum"];
            auto xSize = textureData["xsize"];
            auto ySize = textureData["ysize"];
            Texture tex = Texture();
            auto texturePath = (textureDirectory / fileName).generic_string();
            tex.SetTextureName(texturePath);
            tex.SetDivParams(std::make_tuple(allNum, xNum, yNum, xSize, ySize));
            _gameBase.resServer().LoadTexture(keyName, tex);
         }
      }

      void LoadJson::LoadModels(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("ModelJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "�t�@�C�����J���܂���ł����B----------\n");
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
         auto modelDirectory = _gameBase.pathServer().GetPath("Model") / jsonName;
         for (auto& modelData : modelArray) {
            auto keyName = modelData["keyname"];
            auto fileName = modelData["filename"];
            auto modelPath = (modelDirectory / fileName).generic_string();
            _gameBase.resServer().LoadModel(keyName, modelPath);
         }
      }

      void LoadJson::LoadSounds(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("SoundJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "�t�@�C�����J���܂���ł����B----------\n");
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
         auto soundDirectory = _gameBase.pathServer().GetPath("Sound") / jsonName;
         for (auto& soundData : soundArray) {
            auto keyName = soundData["keyname"];
            auto fileName = soundData["filename"];
            auto isLoad = soundData["isload"];
            auto volume = soundData["volume"];
            auto soundPath = (soundDirectory / fileName).generic_string();
            _gameBase.resServer().LoadSound(keyName, std::make_tuple(soundPath, isLoad, volume));
         }
      }

      nlohmann::json LoadJson::GetParam(const std::filesystem::path jsonName, const std::string_view paramName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("ParamJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "�t�@�C�����J���܂���ł����B----------\n");
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
            auto param = paramData[paramName.data()];
            return param;
         }
         return -1;
      }

      void LoadJson::LoadVecParam(const std::string_view jsonName, const std::string_view key,Math::Vector4 vec) {
         if (_vecParams.contains(jsonName.data())) {
            _vecParams.erase(jsonName.data());
         }
         if (_vecParams[jsonName.data()].contains(key.data())) {
            _vecParams[jsonName.data()].erase(key.data());
         }
         auto vecMap = _vecParams[jsonName.data()].emplace(key,vec);
         _vecParams.emplace(jsonName, vecMap);
      }

      void LoadJson::LoadVecParams(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("ParamJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "�t�@�C�����J���܂���ł����B----------\n");
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
            auto keyName = vecData["keyname"];
            auto x = vecData["x"];
            auto y = vecData["y"];
            auto z = vecData["z"];
            Vector4 vec = Vector4();
            vec.SetXYZ(std::make_tuple(x,y,z));
            LoadVecParam(jsonName.generic_string(),keyName, vec);
         }
      }

      Math::Vector4 LoadJson::GetVecParam(const std::string_view jsonName ,const std::string_view key) {
#ifndef _DEBUG
         if (!_vecParams.contains(jsonName.data())) {
            return Vector4();
         }
         if (!_vecParams[jsonName.data()].contains(key.data())) {
            return Vector4();
         }
#else
         try {
            if (!_vecParams.contains(jsonName.data())) {
               std::string message = jsonName.data();
               throw std::logic_error("----------�t�@�C���L�[[" + message +"]��Vector4�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
            if (!_vecParams[jsonName.data()].contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�l�L�[[" + message + "]��Vector4�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto vec = _vecParams[jsonName.data()][key.data()];
         return vec;
      }
   }
}