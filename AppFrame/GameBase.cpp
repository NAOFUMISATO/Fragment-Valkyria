
/*****************************************************************//**
 * \file   GameBase.cpp
 * \brief  �Q�[���{�̂̊��
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "GameBase.h"
#include <DxLib.h>
#include "ModeServer.h"
#include "InputManager.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "PathServer.h"
#include "LoadJson.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �Q�[���x�[�X
    */
   namespace Game {
      GameBase* GameBase::_gameInstance = nullptr;

      GameBase::GameBase() {
         _gameInstance = this;
      };

      GameBase::~GameBase() {
      };
      bool GameBase::Initialize(HINSTANCE hinstance) {
         //�f�o�b�O���[�h�Ȃ�E�C���h�E���[�h�ɐݒ�y�сA�f�o�b�O���O�̏o�͂��s��
#ifdef _DEBUG
         AppWindowMode(true);
         OutputDebugLog(true);
#else
         AppWindowMode(false);
         OutputDebugLog(false);
#endif
         //��ʃT�C�Y�ݒ�
         auto [width, height, depth] = GraphSize();
         SetGraphMode(width, height, depth);
         //�G���[���N�����璼���ɏI��
         if (DxLib_Init() == -1) {
            return false;
         }
         SetBackgroundColor(0, 0, 255);
         //�`���𗠉�ʂɃZ�b�g
         SetDrawScreen(DX_SCREEN_BACK);
         // �y�o�b�t�@��L���ɂ���
         SetUseZBuffer3D(TRUE);
         // �y�o�b�t�@�ւ̏������݂�L���ɂ���
         SetWriteZBuffer3D(TRUE);

         _inputManager = std::make_unique<Input::InputManager>();

         _resServer = std::make_unique<Resource::ResourceServer>(*this);

         _soundServer = std::make_unique<Sound::SoundServer>(*this);

         _pathServer = std::make_unique<Path::PathServer>();

         _loadJson = std::make_unique<Resource::LoadJson>(*this);

         return true;
      }

      void GameBase::Run() {
         //���C�����[�v
         while (_gState != GAME_STATE::QUIT) {
            Input();  // ����
            Update(); // �X�V
            Render(); // �`��
         }
      }

      void GameBase::ShutDown() {
         DxLib_End();
      }


      void GameBase::Input() {
         if (ProcessMessage() == -1) {
            _gState = GAME_STATE::QUIT;  // -1 ���Ԃ��Ă����̂ŃQ�[�����I������
         }
         // ���͏�Ԃ̍X�V
         _inputManager->Update();
         if (_inputManager->GetKeyboard().EscClick()) {
            _gState = GAME_STATE::QUIT;  // ESC�����ꂽ�̂ŃQ�[�����I������
         }
         _modeServer->Input(*_inputManager);    // ���[�h�T�[�o�[�̓��͏��������s
      }

      void GameBase::Update() {
         //���[�h�̍X�V����
         _modeServer->Update();
      }

      void GameBase::Render() {
         //���[�h�̕`�揈��
         ClearDrawScreen();
         _modeServer->Render();
         ScreenFlip();
      }
   }
}