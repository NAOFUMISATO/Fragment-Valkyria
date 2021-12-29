#pragma once
/*****************************************************************//**
 * \file   ModeServer.cpp
 * \brief  ���[�h�̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeServer.h"
#include "ModeBase.h"
#include "ModeFadeIn.h"
#include "ModeFadeOut.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      ModeServer::ModeServer(std::string_view key, std::shared_ptr<ModeBase> mode) {
         Register("FadeIn", std::make_shared<ModeFadeIn>(mode->GetGameBase()));
         Register("FadeOut", std::make_shared<ModeFadeOut>(mode->GetGameBase()));
         Register(key, mode);
         PushBack(key);           // �ŏ��̃V�[�����v�b�V���o�b�N
         PushBack("FadeIn");      // �ŏ��̃V�[���̏�Ƀt�F�[�h�C�����v�b�V���o�b�N
      }

      void ModeServer::Register(std::string_view key, std::shared_ptr<ModeBase> mode) {
         if (_modeRegistry.contains(key.data())) {
            _modeRegistry.erase(key.data());       // ���W�X�g���𑖍����A�w��̃L�[������΍폜����
         }
         _modeRegistry.emplace(key, mode);  // �w��̃��[�h�����W�X�g���ɃL�[�ƂƂ��ɓo�^����
         mode->Init();                   // �w��̃��[�h�̏������������s��
      }

      void ModeServer::PushBack(std::string_view key) {
         if (!_modeRegistry.contains(key.data())) {
            return;  // ���W�X�g���𑖍����A�w��̃L�[���Ȃ���ΕԂ�
         }
         auto pushmode = _modeRegistry[key.data()];
         pushmode->Enter();          // �w��̃��[�h�̓����������s��
         _modeList.push_back(pushmode); // ���X�g�̖����Ɏw��̃��[�h��ǉ�����
      }

      void ModeServer::PopBack() {
         if (_modeList.empty()) {
            return;  // ���X�g�ɉ����Ȃ���ΕԂ�
         }
         _modeList.back()->Exit();  // �����̃��[�h�̏o���������s��
         _modeList.pop_back();      // ���X�g�̖����̃��[�h���폜����
      }

      void ModeServer::GoToMode(std::string_view key, char fadeType) {
         InsertBelowBack(key.data());  // �w��̃��[�h�������[�h�̐^���ɑ}��
         FadeInsertBelowBack("FadeIn", fadeType);    // �t�F�[�h�C���������[�h�̐^���ɑ}���i���ʓI�Ɏw��̃��[�h�̐^��ɑ}�������j
         FadePushBack("FadeOut", fadeType);          // �t�F�[�h�A�E�g�������[�h�̐^��ɑ}��
      }

      void ModeServer::InsertBelowBack(std::string_view key) {
         if (!_modeRegistry.contains(key.data())) {
            return;   // ���W�X�g���𑖍����A�w��̃L�[���Ȃ���ΕԂ�
         }
         auto insertmode = _modeRegistry[key.data()];
         insertmode->Enter();   // �w��̃��[�h�̓����������s��
         _modeList.insert(std::prev(_modeList.end()), insertmode);   // �w��̃��[�h�����X�g�̖������A����ɐi�񂾈ʒu�ɑ}������
      }

      std::shared_ptr<ModeBase> ModeServer::GetMode(std::string_view key) {
         if (!_modeRegistry.contains(key.data())) {
            return nullptr;   //�w��̃L�[���Ȃ����null��Ԃ�
         }
         return _modeRegistry[key.data()];   //�w��̃L�[�̃��[�h��Ԃ�
      }

      void ModeServer::Input(Input::InputManager& input) {
         if (_modeList.empty()) {
            return;  // ���X�g�ɉ����Ȃ���ΕԂ�
         }
         _modeList.back()->Input(input);   //���X�g�̖����̃��[�h�̂ݓ��͏�������
      }

      void ModeServer::Update() {
         if (_modeList.empty()) {
            return;  // ���X�g�ɉ����Ȃ���ΕԂ�
         }
         _modeList.back()->Update();   //���X�g�̖����̃��[�h�̂ݍX�V��������
      }

      void ModeServer::Render() const {
         for (auto&& mode : _modeList) {
            mode->Render();   //���X�g�̑S�Ẵ��[�h�̕`�揈������
         }
      }

      void ModeServer::FadeInsertBelowBack(std::string_view key, char fadeType) {
         if (!_modeRegistry.contains(key.data())) {
            return;   // ���W�X�g���𑖍����A�w��̃L�[(�t�F�[�h)���Ȃ���ΕԂ�
         }
         auto insertmode = _modeRegistry[key.data()];
         insertmode->fadeType(fadeType);  // �t�F�[�h���Ԃ̐ݒ�
         insertmode->Enter();                // �w��̃��[�h(�t�F�[�h)�̓�������
         _modeList.insert(std::prev(_modeList.end()), insertmode);  // �w��̃��[�h(�t�F�[�h)�����X�g�̖������A����ɐi�񂾈ʒu�ɑ}������
      }

      void ModeServer::FadePushBack(std::string_view key, char fadeType) {
         if (!_modeRegistry.contains(key.data())) {
            return;   // ���W�X�g���𑖍����A�w��̃L�[(�t�F�[�h)���Ȃ���ΕԂ�
         }
         auto pushmode = _modeRegistry[key.data()];
         pushmode->fadeType(fadeType);  // �t�F�[�h���Ԃ̐ݒ�
         pushmode->Enter();                // �w��̃��[�h(�t�F�[�h)��������
         _modeList.push_back(pushmode);       // ���X�g�̖����Ɏw��̃��[�h(�t�F�[�h)��ǉ�����
      }
   }
}