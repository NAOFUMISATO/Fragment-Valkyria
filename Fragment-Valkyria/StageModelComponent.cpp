#include "StageModelComponent.h"
#include "Stage.h"
#include "StageData.h"
#include "LoadStageFromJson.h"
#include "GameMain.h"

using namespace FragmentValkyria::Stage;
StageModelComponent::StageModelComponent(Stage& stage) : _stage{ stage } {
}

StageModelComponent::~StageModelComponent() {
}

void StageModelComponent::Init() {

}

void StageModelComponent::Update() {
   for (auto& modelHandle : _modelHandles) {
      SetMatrix(modelHandle, _stage.worldTransform());
   }
}

void StageModelComponent::Draw() {
   SetUseLighting(_isLighting);
   for (auto& modelHandle : _modelHandles) {
      MV1DrawModel(modelHandle);
   }
}

void StageModelComponent::SetModels(std::string_view key) {
   auto stageModels = _stage.gameMain().loadStage().GetStageModels(key);
   for (auto stageModel : stageModels) {
      auto [handle, stageData] = stageModel;
      auto [position,rotation,scale] = stageData.GetStageParams();
      SetPosition(handle, position);
      SetRotation(handle,rotation);
      SetScale(handle, scale);
      _modelHandles.emplace_back(handle);
   }
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