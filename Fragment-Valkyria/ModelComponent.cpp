
/*****************************************************************//**
 * \file   ModelComponent.cpp
 * \brief  ���f���̊Ǘ�
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
   auto gameInstance = Game::GameMain::GetInstance();
   auto [handle, actualNo] = gameInstance->resServer().GetModel(key, no);
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

void ModelComponent::SetDifColor(int index, float red, float green, float blue) {
   MV1SetMaterialDifColor(_modelHandle, index, GetColorF(red, green, blue, 1.0f));
}

void ModelComponent::SetSpcColor(int index, float red, float green, float blue) {
   MV1SetMaterialSpcColor(_modelHandle, index, GetColorF(red, green, blue, 1.0f));
}

void ModelComponent::SetEmiColor(int index, float red, float green, float blue) {
   MV1SetMaterialEmiColor(_modelHandle, index, GetColorF(red, green, blue, 1.0f));
}

void ModelComponent::SetAmbColor(int index, float red, float green, float blue) {
   MV1SetMaterialAmbColor(_modelHandle, index, GetColorF(red, green, blue, 1.0f));
}

void ModelComponent::SetSpcPower(int index, float power) {
   MV1SetMaterialSpcPower(_modelHandle, index, power);
}

void ModelComponent::SetBlendModeAdd(int index) {
   MV1SetMaterialDrawBlendMode(_modelHandle, index, DX_BLENDMODE_ADD);
}

void ModelComponent::SetBlendModeAlpha(int index) {
   MV1SetMaterialDrawBlendMode(_modelHandle, index, DX_BLENDMODE_ALPHA);
}

void ModelComponent::SetBlendModeReset(int index) {
   MV1SetMaterialDrawBlendMode(_modelHandle, index, DX_BLENDMODE_NOBLEND);
}

void ModelComponent::SetBlendParam(int index,int param) {
   MV1SetMaterialDrawBlendParam(_modelHandle, index, param);
}

int ModelComponent::FindFrame(std::string_view frameName) {
   auto frame = MV1SearchFrame(_modelHandle, frameName.data());
#ifdef _DEBUG
   try {
      if (frame < 0) {
         std::string message = frameName.data();
         throw std::logic_error("----------�w��t���[�����u" + message + "�v�Ɉ�v����t���[���͑��݂��܂���ł����B----------\n");
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
         throw std::logic_error("----------�w��̎q�t���[�����u" + message + "�v�Ɉ�v����q�t���[���͑��݂��܂���ł����B----------\n");
      }
   }
   catch (std::logic_error& error) {
      OutputDebugString(error.what());
   }
#endif
   return child;
}

AppFrame::Math::Vector4 ModelComponent::GetFramePosion(std::string_view frameName) {
   auto frame = MV1SearchFrame(_modelHandle, frameName.data());
#ifdef _DEBUG
   try {
      if (frame < 0) {
         std::string message = frameName.data();
         throw std::logic_error("----------�t���[���ʒu���擾���悤�Ƃ����Ƃ���A�w��t���[�����u" + message + "�v�Ɉ�v����t���[���͑��݂��܂���ł����B----------\n");
      }
   }
   catch (std::logic_error& error) {
      OutputDebugString(error.what());
   }
#endif
   auto pos = MV1GetFramePosition(_modelHandle, frame);
   return AppFrame::Math::ToMath(pos);
}

AppFrame::Math::Vector4 ModelComponent::GetFrameChildPosion(std::string_view frameName, std::string_view childName) {
   auto child = MV1SearchFrameChild(_modelHandle, FindFrame(frameName.data()), childName.data());
#ifdef _DEBUG
   try {
      if (child < 0) {
         std::string message = childName.data();
         throw std::logic_error("----------�q�t���[���ʒu���擾���悤�Ƃ����Ƃ���A�w��̎q�t���[�����u" + message + "�v�Ɉ�v����q�t���[���͑��݂��܂���ł����B----------\n");
      }
   }
   catch (std::logic_error& error) {
      OutputDebugString(error.what());
   }
#endif
   auto pos = MV1GetFramePosition(_modelHandle, child);
   return AppFrame::Math::ToMath(pos);
}
