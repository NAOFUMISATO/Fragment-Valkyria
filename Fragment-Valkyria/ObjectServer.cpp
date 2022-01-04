
/*****************************************************************//**
 * \file   ObjectServer.cpp
 * \brief  �I�u�W�F�N�g�̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectServer.h"

using namespace FragmentValkyria::Object;

void ObjectServer::Add(std::unique_ptr<ObjectBase> obj) {
   if (_updating) {
      // �X�V���͕ۗ����̓��I�z��ɒǉ�����
      _pendingObjects.emplace_back(std::move(obj));
   }
   else {
      //�X�V���łȂ���Ώ������񂷗p�̓��I�z��ɒǉ�����
      _runObjects.emplace_back(std::move(obj));
   }
}

void ObjectServer::Input(AppFrame::Input::InputManager& input) {
   _updating = true;
   // _runObjects�ɓo�^����Ă���Active��Ԃ̃I�u�W�F�N�g�̓��͏�������
   for (auto&& obj : _runObjects) {
      if (obj->IsActive()) {
         obj->Input(input);
      }
   }
   _updating = false;
}
/// �X�V
void ObjectServer::Update() {
   _updating = true;
   // _runObjects�ɓo�^����Ă���Active��Ԃ̃I�u�W�F�N�g�̍X�V��������
   for (auto&& obj : _runObjects) {
      if (obj->IsActive()) {
         obj->Update();
      }
   }
   _updating = false;

   // �ۗ����̃I�u�W�F�N�g��_runObjects�Ɉړ�����
   _runObjects.insert(_runObjects.end(),
      make_move_iterator(_pendingObjects.begin()),
      make_move_iterator(_pendingObjects.end()));

   // �ۗ����̃I�u�W�F�N�g���폜����
   _pendingObjects.clear();

   // ���񂾃A�N�^�[���폜����
   erase_if(_runObjects, [](auto&& act) { return act->IsDead(); });
}

void ObjectServer::Render() {
   for (auto&& obj : _runObjects) {
      // _runObjects�ɓo�^����Ă���Active��Ԃ̃I�u�W�F�N�g�̕`�揈������
      if (obj->IsActive()) {
         obj->Draw();
      }
   }
}

void ObjectServer::Clear() {
   // �S�Ă̓��I�z����̗v�f���폜����
   _runObjects.clear();
   _pendingObjects.clear();
}

void ObjectServer::Register(std::string_view key, Vector4 vec) {
   if (_objRegistry.contains(key.data())) {
      _objRegistry[key.data()] = vec;
   }
   _objRegistry.emplace(key, vec);
}

AppFrame::Math::Vector4 ObjectServer::GetPosition(std::string_view key) {
   if (_objRegistry.contains(key.data())) {
      return _objRegistry[key.data()];
   }
   return { 0, 0, 0 };
}

AppFrame::Math::Vector4 ObjectServer::GetForward(std::string_view key) {
   if (_objRegistry.contains(key.data())) {
      return _objRegistry[key.data()];
   }
   return { 0, 0, 0 };
}


