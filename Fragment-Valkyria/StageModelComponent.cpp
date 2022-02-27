
/*****************************************************************//**
 * \file   StageModelComponent.cpp
 * \brief  ステージモデルを一括管理する
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "StageModelComponent.h"
#include "Stage.h"
#include "StageData.h"
#include "LoadStageFromJson.h"
#include "GameMain.h"
#include <utility>

using namespace FragmentValkyria::Stage;
StageModelComponent::StageModelComponent(Object::ObjectBase& owner) : _owner{ owner } {
}

void StageModelComponent::Init() {

}

void StageModelComponent::Update() {
   for (auto& modelHandle : _modelHandles) {
      SetMatrix(modelHandle, _owner.worldTransform());
   }
}

void StageModelComponent::Draw() {
   SetUseLighting(_isLighting);
   SetUsePixelLighting(_isPixelLighting);
   for (auto& modelHandle : _modelHandles) {
      MV1DrawModel(modelHandle);
   }
}

void StageModelComponent::SetModels(std::string_view key) {
   auto stageModels = _owner.gameMain().loadStage().GetStageModels(key);
   for (auto& stageModel : stageModels) {
      auto [handle, stageData] = stageModel;
      auto [position, rotation, scale] = stageData.GetStageParams();
      SetPosition(handle, position);
      SetRotation(handle, rotation);
      SetScale(handle, scale);
      _modelHandles.emplace_back(handle);
   }
}

void StageModelComponent::SetDifColor(std::string_view key, std::string_view fileName, int index, float red, float green, float blue, float alpha) {
   auto stageModels = _owner.gameMain().loadStage().GetStageModels(key);
   for (auto& stageModel : stageModels) {
      auto [handle, stageData] = stageModel;
      auto stageName = stageData.fileName();
      if (fileName == stageName) {
         MV1SetMaterialDifColor(handle, index, GetColorF(red, green, blue, alpha));
      }
   }
}

void StageModelComponent::SetSpcColor(std::string_view key, std::string_view fileName, int index, float red, float green, float blue, float alpha) {
   auto stageModels = _owner.gameMain().loadStage().GetStageModels(key);
   for (auto& stageModel : stageModels) {
      auto [handle, stageData] = stageModel;
      auto stageName = stageData.fileName();
      if (fileName == stageName) {
         MV1SetMaterialSpcColor(handle, index, GetColorF(red, green, blue, alpha));
      }
   }
}

void StageModelComponent::SetEmiColor(std::string_view key, std::string_view fileName, int index, float red, float green, float blue, float alpha) {
   auto stageModels = _owner.gameMain().loadStage().GetStageModels(key);
   for (auto& stageModel : stageModels) {
      auto [handle, stageData] = stageModel;
      auto stageName = stageData.fileName();
      if (fileName == stageName) {
         MV1SetMaterialEmiColor(handle, index, GetColorF(red, green, blue, alpha));
      }
   }
}

void StageModelComponent::SetAmbColor(std::string_view key, std::string_view fileName, int index, float red, float green, float blue, float alpha) {
   auto stageModels = _owner.gameMain().loadStage().GetStageModels(key);
   for (auto& stageModel : stageModels) {
      auto [handle, stageData] = stageModel;
      auto stageName = stageData.fileName();
      if (fileName == stageName) {
         MV1SetMaterialAmbColor(handle, index, GetColorF(red, green, blue, alpha));
      }
   }
}

void StageModelComponent::SetSpcPower(std::string_view key, std::string_view fileName, int index, float power) {
   auto stageModels = _owner.gameMain().loadStage().GetStageModels(key);
   for (auto& stageModel : stageModels) {
      auto [handle, stageData] = stageModel;
      auto stageName = stageData.fileName();
      if (fileName == stageName) {
         MV1SetMaterialSpcPower(handle, index, power);
      }
   }
}

std::pair<int, int> StageModelComponent::GetHandleAndFrameNum(std::string_view frameName) {
   for (int i = 0; i < _modelHandles.size(); i++) {
      auto handle = _modelHandles[i];
      auto frameNum = MV1SearchFrame(handle, frameName.data());
      if (frameNum >= 0) {
         return std::make_pair(handle, frameNum);
      }
   }
   return std::make_pair(0, 0);
}

void StageModelComponent::SetPosition(int handle, Vector4 position) {
   MV1SetPosition(handle, AppFrame::Math::ToDX(position));
}

void StageModelComponent::SetRotation(int handle, Vector4 rotation) {
   MV1SetRotationXYZ(handle, AppFrame::Math::ToDX(rotation));
}

void StageModelComponent::SetScale(int handle, Vector4  scale) {
   MV1SetScale(handle, AppFrame::Math::ToDX(scale));
}

void StageModelComponent::SetMatrix(int handle, Matrix44& world) {
   MV1SetMatrix(handle, AppFrame::Math::ToDX(world));
}