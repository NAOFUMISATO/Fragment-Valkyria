
/*****************************************************************//**
 * \file   ObjectServer.cpp
 * \brief  �I�u�W�F�N�g�̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectServer.h"

namespace FragmentValkyria {
   namespace Object {
      void ObjectServer::Add(std::unique_ptr<ObjectBase> obj) {
         obj->Init();
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

         // ���񂾃I�u�W�F�N�g���폜����
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

      void ObjectServer::RegistVector(std::string_view key, Vector4 vecData) {
         if (_objVecData.contains(key.data())) {
            _objVecData[key.data()] = vecData;
         }
         _objVecData.emplace(key, vecData);
      }

      void ObjectServer::RegistDouble(std::string_view key, double doubleData) {
         if (_objDoubleData.contains(key.data())) {
            _objDoubleData[key.data()] = doubleData;
         }
         _objDoubleData.emplace(key, doubleData);
      }

      AppFrame::Math::Vector4 ObjectServer::GetVecData(std::string_view key) {
#ifndef _DEBUG
         if (!_objVecData.contains(key.data())) {
            return{ 0,0,0 };
         }
#else
         try {
            if (!_objVecData.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]���I�u�W�F�N�g���W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _objVecData[key.data()];
      }

      double ObjectServer::GetDoubleData(std::string_view key) {
#ifndef _DEBUG
         if (!_objDoubleData.contains(key.data())) {
            return 0.0;
         }
#else
         try {
            if (!_objDoubleData.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]���I�u�W�F�N�g���W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _objDoubleData[key.data()];
      }
   }
}

