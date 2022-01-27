
/*****************************************************************//**
 * \file   ModelComponent.cpp
 * \brief  モデルの管理
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
   SetUsePixelLighting(_isPixelLighting);
   MV1DrawModel(_modelHandle);
}

int ModelComponent::SetModel(std::string_view key, int no) {
   auto [handle, actualNo] = _owner.gameMain().resServer().GetModel(key, no);
   _modelHandle = handle;
   _key = key;
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

int ModelComponent::FindFrame(std::string_view frameName) {
   auto frame = MV1SearchFrame(_modelHandle, frameName.data());
#ifdef _DEBUG
   try {
      if (frame < 0) {
         std::string message = frameName.data();
         throw std::logic_error("----------指定フレーム名「" + message + "」に一致するフレームは存在しませんでした。----------\n");
      }
   }
   catch (std::logic_error& error) {
      OutputDebugString(error.what());
   }
#endif
   return frame;
}

int ModelComponent::FindFrameChild(std::string_view frameName,std::string_view childName) {
   auto child = MV1SearchFrameChild(_modelHandle, FindFrame(frameName.data()), childName.data());
#ifdef _DEBUG
   try {
      if (child < 0) {
         std::string message = childName.data();
         throw std::logic_error("----------指定の子フレーム名「" + message + "」に一致する子フレームは存在しませんでした。----------\n");
      }
   }
   catch (std::logic_error& error) {
      OutputDebugString(error.what());
   }
#endif
   return child;
}

