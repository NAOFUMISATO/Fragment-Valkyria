
/*****************************************************************//**
 * \file   ModelComponent.cpp
 * \brief  ÉÇÉfÉãÇÃä«óù
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModelComponent.h"
#include "ObjectBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Model;

ModelComponent::ModelComponent(Object::ObjectBase& owner) : _owner{ owner } {
}

ModelComponent::~ModelComponent() {
}

void ModelComponent::Init() {

}

void ModelComponent::Update() {
   MV1SetMatrix(_modelHandle, AppFrame::Math::ToDX(_owner.worldTransform()));
}

void ModelComponent::Draw() {
   SetUseLighting(_isLighting);
   MV1DrawModel(_modelHandle);
}

int ModelComponent::SetModel(std::string_view key, int no) {
   auto [handle, actualNo] = _owner.gameMain().resServer().GetModel(key, no);
   _modelHandle = handle;
   return actualNo;
}

void ModelComponent::SetPosition(Vector4 position) {
   MV1SetPosition(_modelHandle, AppFrame::Math::ToDX(position));
}

void ModelComponent::SetRotation(Vector4 rotation) {
   MV1SetRotationXYZ(_modelHandle, AppFrame::Math::ToDX(rotation));
}

void ModelComponent::SetScale(Vector4  scale) {
   MV1SetScale(_modelHandle, AppFrame::Math::ToDX(scale));
}

void ModelComponent::SetMatrix(Matrix44& world) {
   MV1SetMatrix(_modelHandle, AppFrame::Math::ToDX(world));
}

void ModelComponent::SetEmiColor(int index, float r, float g, float b) {
   MV1SetMaterialEmiColor(_modelHandle, index, GetColorF(r, g, b, 0.f));
}