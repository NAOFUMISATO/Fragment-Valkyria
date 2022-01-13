#include "LoadStageFromJson.h"
#include <nlohmann/json.hpp>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
#include <fstream>
#include "GameMain.h"
#include "StageData.h"
#include <utility>

using namespace FragmentValkyria::Stage;
using namespace FragmentValkyria;

LoadStageFromJson::LoadStageFromJson(Game::GameMain& gameMain) :_gameMain{gameMain} {
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
         if (stageData.stageName() == Data.stageName()) {
            auto copyHandle = MV1DuplicateModel(originalHandle);
            _stageModelsMap[key.data()].emplace_back(copyHandle, stageData);
            return;
         }
      }
      auto filename = stageData.stageName().data();
      auto handle = MV1LoadModel(filename);
      _stageModelsMap[key.data()].emplace_back(handle, stageData);
   }
   else {
      auto filename = stageData.stageName().data();
      auto handle = MV1LoadModel(filename);
      std::vector<std::pair<int, StageData>> stageDatas;
      stageDatas.emplace_back(handle, stageData);
      _stageModelsMap.emplace(key, stageDatas);
   }
}

void LoadStageFromJson::LoadStageModels(const std::filesystem::path jsonName) {
   namespace AppMath = AppFrame::Math;
   auto jsonDirectory = _gameMain.pathServer().GetPath("StageJson");
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
   auto stageDirectory = _gameMain.pathServer().GetPath("Stage") / jsonName;
   for (auto& stageParam : stageArray) {
      auto fileName = stageParam["filename"];
      auto tx = stageParam["tx"];
      auto ty = stageParam["ty"];
      auto tz = stageParam["tz"];
      auto rx = stageParam["rx"];
      auto ry = stageParam["ry"];
      auto rz = stageParam["rz"];
      auto sx = stageParam["sx"];
      auto sy = stageParam["sy"];
      auto sz = stageParam["sz"];
      auto stagePath = (stageDirectory / fileName).generic_string();
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
      std::vector<std::pair<int, StageData>> vs = { std::make_pair(-1,StageData()) };
      return vs;
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
