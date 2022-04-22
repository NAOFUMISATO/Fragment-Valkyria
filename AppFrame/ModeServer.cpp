#pragma once
/*****************************************************************//**
 * \file   ModeServer.cpp
 * \brief  ���[�h�̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeServer.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#include <time.h>
#include <DxLib.h>
#include "Utility.h"
#endif
#include "ModeBaseRoot.h"
#include "ModeFadeIn.h"
#include "ModeFadeOut.h"

namespace {
   constexpr auto CountMax = 4294967295;   //!< _frameCount�̌^unsigned int�͈̔�
#ifdef _DEBUG
   constexpr auto DrawFPSPosX = 0;         //!< �f�o�b�O�`�悷��FPS��X���W
   constexpr auto DrawFPSPosY = 0;         //!< �f�o�b�O�`�悷��FPS��Y���W
   constexpr auto DrawFPSColor = 255;      //!< �f�o�b�O�`�悷��FPS�̐F
#endif
}
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      ModeServer::ModeServer(std::string_view key, std::shared_ptr<ModeBaseRoot> mode) {
         // �t�F�[�h�C���A�t�F�[�h�A�E�g���[�h��o�^����
         Register("FadeIn", std::make_shared<ModeFadeIn>());
         Register("FadeOut", std::make_shared<ModeFadeOut>());
         // �ŏ��̃��[�h��o�^����
         Register(key, mode);
         // �ŏ��̃��[�h��}��
         PushBack(key);
         // �ŏ��̃��[�h�̏�Ƀt�F�[�h�C����}��
         PushBack("FadeIn");
      }

      void ModeServer::Register(std::string_view key, std::shared_ptr<ModeBaseRoot> mode) {
         // ���W�X�g���𑖍����A�w��̃L�[������΍폜����
         if (_modeRegistry.contains(key.data())) {
            _modeRegistry.erase(key.data()); 
         }
         // �w��̃��[�h�����W�X�g���ɃL�[�ƂƂ��ɓo�^����
         _modeRegistry.emplace(key, mode);
         // �w��̃��[�h�̏������������s��
         mode->Init();
      }

      void ModeServer::PushBack(std::string_view key) {
#ifndef _DEBUG
           // ���W�X�g���𑖍����A�w��̃L�[���Ȃ���ΕԂ�
         if (!_modeRegistry.contains(key.data())) {
            return;
         }
#else
         // ���W�X�g���𑖍����A�w��̃L�[���Ȃ���΃G���[���b�Z�[�W���o�͂���
         try {
            if (!_modeRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]�����[�h���W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto pushMode = _modeRegistry[key.data()];
         // �w��̃��[�h�̓����������s��
         pushMode->Enter();
         // ���X�g�̖����Ɏw��̃��[�h��ǉ�����
         _modeList.push_back(pushMode);
      }

      void ModeServer::PopBack() {
#ifndef _DEBUG
         // ���X�g�ɉ����Ȃ���ΕԂ�
         if (_modeList.empty()) {
            return;
         }
#else
         // ���X�g�ɉ����Ȃ���΃G���[���b�Z�[�W���o�͂���
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------���X�g�����̃��[�h���폜���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // �����̃��[�h�̏o���������s��
         _modeList.back()->Exit();
         // ���X�g�̖����̃��[�h���폜����
         _modeList.pop_back();
      }

      void ModeServer::GoToMode(std::string_view key, char fadeType) {
         // �w��̃��[�h�������[�h�̐^���ɑ}��
         InsertBelowBack(key.data());
         // �t�F�[�h�C���������[�h�̐^���ɑ}���i���ʓI�Ɏw��̃��[�h�̐^��ɑ}�������j
         FadeInsertBelowBack(fadeType);
         // �t�F�[�h�A�E�g�������[�h�̐^��ɑ}��
         FadePushBack(fadeType);
      }

      void ModeServer::InsertBelowBack(std::string_view key) {
#ifndef _DEBUG
         // ���W�X�g���𑖍����A�w��̃L�[���Ȃ���ΕԂ�
         if (!_modeRegistry.contains(key.data())) {
            return;
         }
#else
         // ���W�X�g���𑖍����A�w��̃L�[���Ȃ���΃G���[���b�Z�[�W���o�͂���
         try {
            if (!_modeRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]�����[�h���W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto insertMode = _modeRegistry[key.data()];
         // �w��̃��[�h�̓����������s��
         insertMode->Enter();
         // �w��̃��[�h�����X�g�̖������A����ɐi�񂾈ʒu�ɑ}������
         _modeList.insert(std::prev(_modeList.end()), insertMode);
      }

      std::shared_ptr<ModeBaseRoot> ModeServer::GetMode(std::string_view key) {
#ifndef _DEBUG
         //�w��̃L�[���Ȃ����null��Ԃ�
         if (!_modeRegistry.contains(key.data())) {
            return nullptr;
         }
#else
         //�w��̃L�[���Ȃ���΃G���[���b�Z�[�W���o�͂���
         try {
            if (!_modeRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------�L�[[" + message + "]�����[�h���W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         //�w��̃L�[�̃��[�h��Ԃ�
         return _modeRegistry[key.data()];
      }

      void ModeServer::Input(Input::InputManager& input) {
#ifndef _DEBUG
         // ���X�g�ɉ����Ȃ���ΕԂ�
         if (_modeList.empty()) {
            return;
         }
#else
         // ���X�g�ɉ����Ȃ���΃G���[���b�Z�[�W���o�͂���
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------���X�g�����̃��[�h�̓��͏��������s���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
         // FPS���~���b�P�ʂŌv�����邽�ߎn�_
         _fpsCount = clock();
#endif
         //���X�g�̖����̃��[�h�̂ݓ��͏�������
         _modeList.back()->Input(input);
      }

      void ModeServer::Update() {
#ifndef _DEBUG
         // ���X�g�ɉ����Ȃ���ΕԂ�
         if (_modeList.empty()) {
            return;
         }
#else
         // ���X�g�ɉ����Ȃ���΃G���[���b�Z�[�W���o�͂���
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------���X�g�����̃��[�h�̍X�V���������s���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         //���X�g�̖����̃��[�h�̂ݍX�V��������
         _modeList.back()->Update();
         
         // �J�E���g��unsigned int�͈̔͂𒴂���Ȃ�0�ɂ���
         if (_frameCount >= CountMax) {
            _frameCount = 0;
         }
         _frameCount++;   //�Q�[���̃t���[����i�߂�
      }

      void ModeServer::Render() const {
         //���X�g�̑S�Ẵ��[�h�̕`�揈������
         for (auto&& mode : _modeList) {
            mode->Render();
         }
#ifdef _DEBUG
         // FPS���~���b�P�ʂŌv�����邽�ߏI�_
         auto nowCount = clock();
         // FPS���f�o�b�O�`�悷��
         DrawFormatString(DrawFPSPosX, DrawFPSPosY, 
            AppFrame::Math::Utility::GetColorCode(DrawFPSColor, DrawFPSColor, DrawFPSColor), "FPS : %d" , nowCount-_fpsCount);
#endif
      }

      void ModeServer::FadeInsertBelowBack(char fadeType) {
#ifndef _DEBUG
         // ���W�X�g���𑖍����A�w��̃L�[(�t�F�[�h)���Ȃ���ΕԂ�
         if (!_modeRegistry.contains("FadeIn")) {
            return;
         }
#else
         // ���W�X�g���𑖍����A�w��̃L�[(�t�F�[�h)���Ȃ���΃G���[���b�Z�[�W���o�͂���
         try {
            if (!_modeRegistry.contains("FadeIn")) {
               std::string message = "FadeIn";
               throw std::logic_error("----------�L�[[" + message + "]�����[�h���W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto insertMode = _modeRegistry["FadeIn"];
         // �t�F�[�h���Ԃ̐ݒ�
         insertMode->fadeType(fadeType);
         // �w��̃��[�h(�t�F�[�h)�̓�������
         insertMode->Enter();
         // �w��̃��[�h(�t�F�[�h)�����X�g�̖������A����ɐi�񂾈ʒu�ɑ}������
         _modeList.insert(std::prev(_modeList.end()), insertMode);
      }

      void ModeServer::FadePushBack(char fadeType) {
#ifndef _DEBUG
         if (!_modeRegistry.contains("FadeOut")) {
            return;   // ���W�X�g���𑖍����A�w��̃L�[(�t�F�[�h)���Ȃ���ΕԂ�
         }
#else
         try {
            if (!_modeRegistry.contains("FadeOut")) {
               std::string message = "FadeOut";
               throw std::logic_error("----------�L�[[" + message + "]�����[�h���W�X�g���ɑ��݂��܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto pushMode = _modeRegistry["FadeOut"];
         // �t�F�[�h���Ԃ̐ݒ�
         pushMode->fadeType(fadeType);
         // �w��̃��[�h(�t�F�[�h)��������
         pushMode->Enter();
         // ���X�g�̖����Ɏw��̃��[�h(�t�F�[�h)��ǉ�����
         _modeList.push_back(pushMode);
      }

      std::shared_ptr<ModeBaseRoot> ModeServer::GetNowMode() {
         // �����̃��[�h���擾����
         return _modeList.back();
      }
   }
}