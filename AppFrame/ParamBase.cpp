
/*****************************************************************//**
 * \file   ParamBase.cpp
 * \brief  �e�l�̊Ǘ����s�����
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "ParamBase.h"
#include "GameBase.h"
#include "CurrentPathServer.h"

namespace AppFrame {
   /**
    * \brief �l�Ǘ��֌W
    */
   namespace Param {
      ParamBase::ParamBase(Game::GameBase& gameBase, std::string_view key) :
         _gameBase{ gameBase }, _key{ key } {
      }

      int ParamBase::GetIntParam(std::string_view paramName) {
#ifndef _DEBUG
         // �w��̃L�[��������΋�̕������Ԃ�
         if (!_intMap.contains(paramName.data())) {
            return "";
         }
#else
         // �w��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_intMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------�L�[[" + message + "]���p�X�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _intMap[paramName.data()];
      }

      float ParamBase::GetFloatParam(std::string_view paramName) {
#ifndef _DEBUG
         // �w��̃L�[��������΋�̕������Ԃ�
         if (!_floatMap.contains(paramName.data())) {
            return "";
         }
#else
         // �w��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_floatMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------�L�[[" + message + "]���l�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _floatMap[paramName.data()];
      }

      double ParamBase::GetDoubleParam(std::string_view paramName) {
#ifndef _DEBUG
         // �w��̃L�[��������΋�̕������Ԃ�
         if (!_doubleMap.contains(paramName.data())) {
            return "";
         }
#else
         // �w��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_doubleMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------�L�[[" + message + "]���l�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _doubleMap[paramName.data()];
      }

      bool ParamBase::GetBoolParam(std::string_view paramName) {
#ifndef _DEBUG
         // �w��̃L�[��������΋�̕������Ԃ�
         if (!_boolMap.contains(paramName.data())) {
            return "";
         }
#else
         // �w��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_boolMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------�L�[[" + message + "]���l�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _boolMap[paramName.data()];
      }

      Math::Vector4 ParamBase::GetVecParam(std::string_view paramName) {
#ifndef _DEBUG
         // �w��̃L�[��������΋�̕������Ԃ�
         if (!_vecMap.contains(paramName.data())) {
            return "";
         }
#else
         // �w��̃L�[��������΁A�G���[���b�Z�[�W���o�͂���
         try {
            if (!_vecMap.contains(paramName.data())) {
               std::string message = paramName.data();
               throw std::logic_error("----------�L�[[" + message + "]���l�R���e�i�ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _vecMap[paramName.data()];
      }

      void ParamBase::LoadIntParam(std::vector<std::string_view> paramNames) {
         // json�t�@�C�����J���A�I�u�W�F�N�g�����o��
         auto paramArray = JsonSetUp();
         // �����Ɏw�肵��������̓��I�z���S�ĉ�
         for (auto i = 0; i < paramNames.size(); i++) {
            // �g�b�v���x���̔z���S�ĉ񂵁A�����̕�����Ɉ�v����i�[���Ă���e�f�[�^��S�Ď��o��
            for (auto& paramData : paramArray) {
               const int param = paramData[paramNames[i].data()];
               // �o�^���悤�Ƃ����L�[������ΌÂ��L�[�͍폜����
               if (_intMap.contains(paramNames[i].data())) {
                  _intMap.erase(paramNames[i].data());
               }
               // �e�f�[�^�����o�����ۂ̕�������L�[�A���o����int�^�f�[�^��A�z�z��ɓo�^����
               _intMap.emplace(paramNames[i], param);
            }
         }
      }

      void ParamBase::LoadFloatParam(std::vector<std::string_view> paramNames) {
         // json�t�@�C�����J���A�I�u�W�F�N�g�����o��
         auto paramArray = JsonSetUp();
         // �����Ɏw�肵��������̓��I�z���S�ĉ�
         for (auto i = 0; i < paramNames.size(); i++) {
            // �g�b�v���x���̔z���S�ĉ񂵁A�����̕�����Ɉ�v����i�[���Ă���e�f�[�^��S�Ď��o��
            for (auto& paramData : paramArray) {
               const float param = paramData[paramNames[i].data()];
               // �o�^���悤�Ƃ����L�[������ΌÂ��L�[�͍폜����
               if (_floatMap.contains(paramNames[i].data())) {
                  _floatMap.erase(paramNames[i].data());
               }
               // �e�f�[�^�����o�����ۂ̕�������L�[�A���o����float�^�f�[�^��A�z�z��ɓo�^����
               _floatMap.emplace(paramNames[i], param);
            }
         }
      }

      void ParamBase::LoadDoubleParam(std::vector<std::string_view> paramNames) {
         // json�t�@�C�����J���A�I�u�W�F�N�g�����o��
         auto paramArray = JsonSetUp();
         // �����Ɏw�肵��������̓��I�z���S�ĉ�
         for (auto i = 0; i < paramNames.size(); i++) {
            // �g�b�v���x���̔z���S�ĉ񂵁A�����̕�����Ɉ�v����i�[���Ă���e�f�[�^��S�Ď��o��
            for (auto& paramData : paramArray) {
               const double param = paramData[paramNames[i].data()];
               // �o�^���悤�Ƃ����L�[������ΌÂ��L�[�͍폜����
               if (_doubleMap.contains(paramNames[i].data())) {
                  _doubleMap.erase(paramNames[i].data());
               }
               // �e�f�[�^�����o�����ۂ̕�������L�[�A���o����double�^�f�[�^��A�z�z��ɓo�^����
               _doubleMap.emplace(paramNames[i], param);
            }
         }
      }

      void ParamBase::LoadBoolParam(std::vector<std::string_view> paramNames) {
         // json�t�@�C�����J���A�I�u�W�F�N�g�����o��
         auto paramArray = JsonSetUp();
         // �����Ɏw�肵��������̓��I�z���S�ĉ�
         for (auto i = 0; i < paramNames.size(); i++) {
            // �g�b�v���x���̔z���S�ĉ񂵁A�����̕�����Ɉ�v����i�[���Ă���e�f�[�^��S�Ď��o��
            for (auto& paramData : paramArray) {
               const bool param = paramData[paramNames[i].data()];
               // �o�^���悤�Ƃ����L�[������ΌÂ��L�[�͍폜����
               if (_boolMap.contains(paramNames[i].data())) {
                  _boolMap.erase(paramNames[i].data());
               }
               // �e�f�[�^�����o�����ۂ̕�������L�[�A���o����bool�^�f�[�^��A�z�z��ɓo�^����
               _boolMap.emplace(paramNames[i], param);
            }
         }
      }

      void ParamBase::LoadVecParam(std::vector<std::string_view> paramNames) {
         // �l�����i�[���Ă���json�t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ParamJson");
         auto jsonPath = (jsonDirectory / _key).generic_string() + ".json";
         // �w�肵��json�t�@�C����ǂݎ���p�ŊJ��
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         // �w�肵���t�@�C�����J���̂Ɏ��s�����Ȃ�logic_error�����o���A�f�o�b�K�ɏo�͂���
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "�t�@�C�����J���܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         // �t�@�C���̒��g�����o��
         reading >> value;
         // �t�@�C�������
         reading.close();
         // �g�b�v���x���̔z��̃L�[����������w�肵�Ajson�I�u�W�F�N�g�Ƃ��ĕԂ�
         auto vecArray = value[_key + "_vec"];
         // �����Ɏw�肵��������̓��I�z���S�ĉ�
         for (auto i = 0; i < paramNames.size(); i++) {
            // �g�b�v���x���̔z���S�ĉ񂵁A�����̕�����Ɉ�v����i�[���Ă���eVector4�f�[�^��S�Ď��o��
            for (auto& vecData : vecArray) {
               auto vecParamArray = vecData[paramNames[i].data()];
               // ���o����Vector4�f�[�^�̊e�v�f�����o��
               for (auto& vecParam : vecParamArray) {
                  const double x = vecParam["x"];   // x����
                  const double y = vecParam["y"];   // y����
                  const double z = vecParam["z"];   // z����
                  // Vector4�N���X�𐶐����AVector4�f�[�^��o�^
                  auto vec = Math::Vector4(x, y, z);
                  // �o�^���悤�Ƃ����L�[������ΌÂ��L�[�͍폜����
                  if (_vecMap.contains(paramNames[i].data())) {
                     _vecMap.erase(paramNames[i].data());
                  }
                  // Vector4�f�[�^�����o�����ۂ̕�������L�[�A���o����Vector4�f�[�^��A�z�z��ɓo�^����
                  _vecMap.emplace(paramNames[i], vec);
               }
            }
         }
      }

      nlohmann::json ParamBase::JsonSetUp() {
         // �l�����i�[���Ă���json�t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ParamJson");
         auto jsonPath = (jsonDirectory / _key).generic_string() + ".json";
         // �w�肵��json�t�@�C����ǂݎ���p�ŊJ��
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         // �w�肵���t�@�C�����J���̂Ɏ��s�����Ȃ�logic_error�����o���A�f�o�b�K�ɏo�͂���
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "�t�@�C�����J���܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         // �t�@�C���̒��g�����o��
         reading >> value;
         // �t�@�C�������
         reading.close();
         // �g�b�v���x���̔z��̃L�[����������w�肵�Ajson�I�u�W�F�N�g�Ƃ��ĕԂ�
         return value[_key];
      }
   }
}
