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
         Register("FadeIn", std::make_shared<ModeFadeIn>(mode->gameBase()));
         Register("FadeOut", std::make_shared<ModeFadeOut>(mode->gameBase()));
         Register(key, mode);
         PushBack(key);           // �ŏ��̃V�[�����v�b�V���o�b�N
         PushBack("FadeIn");      // �ŏ��̃V�[���̏�Ƀt�F�[�h�C�����v�b�V���o�b�N
      }

      void ModeServer::Register(std::string_view key, std::shared_ptr<ModeBaseRoot> mode) {
         if (_modeRegistry.contains(key.data())) {
            _modeRegistry.erase(key.data());       // ���W�X�g���𑖍����A�w��̃L�[������΍폜����
         }
         _modeRegistry.emplace(key, mode);  // �w��̃��[�h�����W�X�g���ɃL�[�ƂƂ��ɓo�^����
         mode->Init();                   // �w��̃��[�h�̏������������s��
      }

      void ModeServer::PushBack(std::string_view key) {
#ifndef _DEBUG
         if (!_modeRegistry.contains(key.data())) {
            return;  // ���W�X�g���𑖍����A�w��̃L�[���Ȃ���ΕԂ�
         }
#else
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
         pushMode->Enter();          // �w��̃��[�h�̓����������s��
         _modeList.push_back(pushMode); // ���X�g�̖����Ɏw��̃��[�h��ǉ�����
      }

      void ModeServer::PopBack() {
#ifndef _DEBUG
         if (_modeList.empty()) {
            return;  // ���X�g�ɉ����Ȃ���ΕԂ�
         }
#else
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------���X�g�����̃��[�h���폜���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _modeList.back()->Exit();  // �����̃��[�h�̏o���������s��
         _modeList.pop_back();      // ���X�g�̖����̃��[�h���폜����
      }

      void ModeServer::GoToMode(std::string_view key, char fadeType) {
         InsertBelowBack(key.data());      // �w��̃��[�h�������[�h�̐^���ɑ}��
         FadeInsertBelowBack(fadeType);    // �t�F�[�h�C���������[�h�̐^���ɑ}���i���ʓI�Ɏw��̃��[�h�̐^��ɑ}�������j
         FadePushBack(fadeType);           // �t�F�[�h�A�E�g�������[�h�̐^��ɑ}��
      }

      void ModeServer::InsertBelowBack(std::string_view key) {
#ifndef _DEBUG
         if (!_modeRegistry.contains(key.data())) {
            return;  // ���W�X�g���𑖍����A�w��̃L�[���Ȃ���ΕԂ�
         }
#else
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
         insertMode->Enter();   // �w��̃��[�h�̓����������s��
         _modeList.insert(std::prev(_modeList.end()), insertMode);   // �w��̃��[�h�����X�g�̖������A����ɐi�񂾈ʒu�ɑ}������
      }

      std::shared_ptr<ModeBaseRoot> ModeServer::GetMode(std::string_view key) {
#ifndef _DEBUG
         if (!_modeRegistry.contains(key.data())) {
            return nullptr;   //�w��̃L�[���Ȃ����null��Ԃ�
         }
#else
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
         return _modeRegistry[key.data()];   //�w��̃L�[�̃��[�h��Ԃ�
      }

      void ModeServer::Input(Input::InputManager& input) {
#ifndef _DEBUG
         if (_modeList.empty()) {
            return;  // ���X�g�ɉ����Ȃ���ΕԂ�
         }
#else
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------���X�g�����̃��[�h�̓��͏��������s���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }

         _fpsCount = clock();
#endif
         _modeList.back()->Input(input);   //���X�g�̖����̃��[�h�̂ݓ��͏�������
      }

      void ModeServer::Update() {
#ifndef _DEBUG
         if (_modeList.empty()) {
            return;  // ���X�g�ɉ����Ȃ���ΕԂ�
         }
#else
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------���X�g�����̃��[�h�̍X�V���������s���悤�Ƃ��܂������A���X�g����ł����B----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _modeList.back()->Update();   //���X�g�̖����̃��[�h�̂ݍX�V��������
         
         // �J�E���g��unsigned int�͈̔͂𒴂���Ȃ�0�ɂ���
         if (_frameCount >= CountMax) {
            _frameCount = 0;
         }
         _frameCount++;   //�Q�[���̃t���[����i�߂�
      }

      void ModeServer::Render() const {
         for (auto&& mode : _modeList) {
            mode->Render();   //���X�g�̑S�Ẵ��[�h�̕`�揈������
         }
#ifdef _DEBUG
         auto nowCount = clock();
         DrawFormatString(DrawFPSPosX, DrawFPSPosY, 
            AppFrame::Math::Utility::GetColorCode(DrawFPSColor, DrawFPSColor, DrawFPSColor), "FPS : %d" , nowCount-_fpsCount);
#endif
      }

      void ModeServer::FadeInsertBelowBack(char fadeType) {
#ifndef _DEBUG
         if (!_modeRegistry.contains("FadeIn")) {
            return;   // ���W�X�g���𑖍����A�w��̃L�[(�t�F�[�h)���Ȃ���ΕԂ�
         }
#else
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
         insertMode->fadeType(fadeType);     // �t�F�[�h���Ԃ̐ݒ�
         insertMode->Enter();                // �w��̃��[�h(�t�F�[�h)�̓�������
         _modeList.insert(std::prev(_modeList.end()), insertMode);  // �w��̃��[�h(�t�F�[�h)�����X�g�̖������A����ɐi�񂾈ʒu�ɑ}������
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
         pushMode->fadeType(fadeType);     // �t�F�[�h���Ԃ̐ݒ�
         pushMode->Enter();                // �w��̃��[�h(�t�F�[�h)��������
         _modeList.push_back(pushMode);    // ���X�g�̖����Ɏw��̃��[�h(�t�F�[�h)��ǉ�����
      }

      std::shared_ptr<ModeBaseRoot> ModeServer::GetNowMode() {
         return _modeList.back();
      }
   }
}