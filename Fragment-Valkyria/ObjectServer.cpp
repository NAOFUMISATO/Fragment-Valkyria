
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
      // �X�V����_pendingActors�ɒǉ�����
      _pendingObjects.emplace_back(std::move(obj));
   }
   else {
      _runObjects.emplace_back(std::move(obj));
   }
}
/// ����
void ObjectServer::Input(AppFrame::Input::InputManager& input) {
   _updating = true;
   for (auto&& obj : _runObjects) {
      if (obj->IsActive()) {
         // �A�N�^�[�ɓ��͂���
         obj->Input(input);
      }
   }
   _updating = false;
}
/// �X�V
void ObjectServer::Update() {
   _updating = true;
   for (auto&& obj : _runObjects) {
      if (obj->IsActive()) {
         // �A�N�^�[�ɓ��͂���
         obj->Update();
      }
   }
   _updating = false;

   // �ۗ����̃A�N�^�[��_actors�Ɉړ�����
   _runObjects.insert(_runObjects.end(),
      make_move_iterator(_pendingObjects.begin()),
      make_move_iterator(_pendingObjects.end()));

   // �ۗ����̃A�N�^�[���N���A����
   _pendingObjects.clear();

   // ���񂾃A�N�^�[���폜����
   erase_if(_runObjects, [](auto&& act) { return act->IsDead(); });
}
/// �`��
void ObjectServer::Render() {
   for (auto&& obj : _runObjects) {
      if (obj->IsActive()) {
         obj->Draw();
      }
   }
}
/// �S�A�N�^�[�̍폜
void ObjectServer::Clear() {
   _runObjects.clear();
   _pendingObjects.clear();
}
/// �A�N�^�[�̓o�^
void ObjectServer::Register(std::string_view key, Vector4 vec) {
   if (_objRegistry.contains(key.data())) {
      _objRegistry[key.data()] = vec;
   }
   _objRegistry.emplace(key, vec);
}
/// �A�N�^�[�̈ꗗ�̎擾
AppFrame::Math::Vector4 ObjectServer::GetPosition(std::string_view key) {
   if (_objRegistry.contains(key.data())) {
      return _objRegistry[key.data()];
   }
   // ���o�^
   return { 0, 0, 0 };
}
/// �o�^�����A�N�^�[�̈ʒu�𓾂�
AppFrame::Math::Vector4 ObjectServer::GetForward(std::string_view key) {
   if (_objRegistry.contains(key.data())) {
      return _objRegistry[key.data()];
   }
   // ���o�^
   return { 0, 0, 0 };
}


