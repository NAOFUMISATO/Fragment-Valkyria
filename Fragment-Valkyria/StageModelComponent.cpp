
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
   for (auto stageModel : stageModels) {
      auto [handle, stageData] = stageModel;
      auto [position,rotation,scale] = stageData.GetStageParams();
      SetPosition(handle, position);
      SetRotation(handle,rotation);
      SetScale(handle, scale);
      _modelHandles.emplace_back(handle);
   }
}

std::pair<int, int> StageModelComponent::GetHandleAndCollNum(std::string_view collName) {
   for (int i = 0; i < _modelHandles.size(); i++) {
      auto handle = _modelHandles[i];
      auto collNum = MV1SearchFrame(handle, collName.data());
      if (collNum >= 0) {
        return std::make_pair(handle, collNum);
      }
   }
   return std::make_pair(0,0);
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

void StageModelComponent::SetEmiColor(int index, float r, float g, float b) {
   for (auto& modelHandle : _modelHandles) {
      MV1SetMaterialEmiColor(modelHandle, index, GetColorF(r, g, b, 0.f));
   }
}