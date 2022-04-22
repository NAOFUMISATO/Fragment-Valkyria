
/*****************************************************************//**
 * \file   LoadStageFromJson.cpp
 * \brief  ステージの読み込みを行う
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "LoadStageFromJson.h"
#include <nlohmann/json.hpp>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
#include "GameMain.h"
#include "StageData.h"
#include <utility>

using namespace FragmentValkyria::Stage;
using namespace FragmentValkyria;

LoadStageFromJson::LoadStageFromJson() {
   ClearStageModels();
}

void LoadStageFromJson::ClearStageModels() {
   for (auto& [key,stageModels] : _stageModelsMap) {
      for (auto& [handle,stageData] : stageModels) {
         MV1DeleteModel(handle);
      }
      stageModels.clear();
   }
   _stageModelsMap.clear();
}

void LoadStageFromJson::LoadStageModel(std::string_view key, StageData& stageData) {
   if (_stageModelsMap.contains(key.data())) {
      for (auto& stagemodel : _stageModelsMap[key.data()]) {
         auto [originalHandle, Data] = stagemodel;
         if (stageData.fileName() == Data.fileName()) {
            auto copyHandle = MV1DuplicateModel(originalHandle);
            _stageModelsMap[key.data()].emplace_back(copyHandle, stageData);
            return;
         }
      }
      auto filename = stageData.fileName().data();
      auto handle = MV1LoadModel(filename);
      _stageModelsMap[key.data()].emplace_back(handle, stageData);
   }
   else {
      auto filename = stageData.fileName().data();
      auto handle = MV1LoadModel(filename);
      std::vector<std::pair<int, StageData>> stageDatas;
      stageDatas.emplace_back(handle, stageData);
      _stageModelsMap.emplace(key, stageDatas);
   }
}

void LoadStageFromJson::LoadStageModels(const std::filesystem::path jsonName) {
   namespace AppMath = AppFrame::Math;
   auto gameInstance = Game::GameMain::GetInstance();
   auto jsonDirectory = gameInstance->pathServer().GetCurrentPath("StageJson");
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
   auto stageArray = value[jsonName.generic_string()];
   auto stageDirectory = gameInstance->pathServer().GetCurrentPath("Stage") / jsonName;
   for (auto& stageParam : stageArray) {
      const auto fileName = stageParam["filename"];
      const auto tx = stageParam["tx"];
      const auto ty = stageParam["ty"];
      const auto tz = stageParam["tz"];
      const auto rx = stageParam["rx"];
      const auto ry = stageParam["ry"];
      const auto rz = stageParam["rz"];
      const auto sx = stageParam["sx"];
      const auto sy = stageParam["sy"];
      const auto sz = stageParam["sz"];
      const auto stagePath = (stageDirectory / fileName).generic_string()+".mv1";
      AppMath::Vector4 position = AppMath::Vector4(tx, ty, tz);
      AppMath::Vector4 rotation = AppMath::Vector4(rx, ry, rz);
      AppMath::Vector4 scale = AppMath::Vector4(sx, sy, sz);
      StageData stageData = StageData(stagePath, position, rotation, scale);
      LoadStageModel(jsonName.generic_string(),stageData);
   }
}

std::vector<std::pair<int, StageData>> LoadStageFromJson::GetStageModels(std::string_view key) {
#ifndef _DEBUG
   if (!_stageModelsMap.contains(key.data())) {
      auto  stageData = { std::make_pair(-1,StageData()) };
      return stageData;
   }
#else
   try {
      if (!_stageModelsMap.contains(key.data())) {
         std::string message = key.data();
         throw std::logic_error("----------キー[" + message + "]がステージ情報コンテナに存在しませんでした。----------\n");
      }
   }
   catch (std::logic_error& error) {
      OutputDebugString(error.what());
   }
#endif
   auto stageModels = _stageModelsMap[key.data()];
   return stageModels;
}
