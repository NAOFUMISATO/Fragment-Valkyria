
/*****************************************************************//**
 * \file   GameBase.cpp
 * \brief  �Q�[���{�̂̊��
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "GameBase.h"
#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "ModeServer.h"
#include "InputManager.h"
#include "ResourceServer.h"
#include "SoundComponent.h"
#include "CurrentPathServer.h"
#include "LoadResourceJson.h"
#include "EffectServer.h"
#include "ObjectServer.h"
#include "TextureComponent.h"
#include "SpriteServer.h"
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
#ifndef _DEBUG
         OutputDebugLog(false);
         AppWindowMode(false);
#else
         OutputDebugLog(true);
         AppWindowMode(true);
#endif
         //��ʃT�C�Y�ݒ�
         auto [width, height, depth] = GraphSize();
         SetGraphMode(width, height, depth);

         // DirectX11���g�p��(Effekseer���g�p�����)
         SetUseDirect3DVersion(DX_DIRECT3D_11);
        
         //�G���[���N�����璼���ɏI��
         if (DxLib_Init() == -1) {
            return false;
         }

         SetBackgroundColor(0, 0, 255);
         //�`���𗠉�ʂɃZ�b�g
         SetDrawScreen(DX_SCREEN_BACK);

         // Effekseer�̏�����
         if (Effekseer_Init(8000) == -1)
         {
            DxLib_End();
            return -1;
         }

         // �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h��
         SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
         // DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N�̐ݒ�
         Effekseer_SetGraphicsDeviceLostCallbackFunctions();

         // �y�o�b�t�@��L���ɂ���
         SetUseZBuffer3D(TRUE);
         // �y�o�b�t�@�ւ̏������݂�L���ɂ���
         SetWriteZBuffer3D(TRUE);

         _inputManager = std::make_unique<Input::InputManager>();

         _resServer = std::make_unique<Resource::ResourceServer>(*this);

         _soundComponent = std::make_unique<Sound::SoundComponent>(*this);

         _pathServer = std::make_unique<Path::CurrentPathServer>();

         _loadresJson = std::make_unique<Resource::LoadResourceJson>(*this);

         _efcServer = std::make_unique<Effect::EffectServer>();

         _objServer = std::make_unique<Object::ObjectServer>();

         _texComponent = std::make_unique<Texture::TextureComponent>(*this);

         _sprServer = std::make_unique <Sprite::SpriteServer>();

         return true;
      }

      void GameBase::Run() {
         //���C�����[�v
         while (_gState != GameState::Quit) {
            Input();  // ����
            Update(); // �X�V
            Render(); // �`��
         }
      }

      void GameBase::ShutDown() {
         Effkseer_End();
         DxLib_End();
      }


      void GameBase::Input() {
         if (ProcessMessage() == -1) {
            _gState = GameState::Quit;  // -1 ���Ԃ��Ă����̂ŃQ�[�����I������
         }
         // ���͏�Ԃ̍X�V
         _inputManager->Update();
         if (_inputManager->GetKeyboard().EscClick()) {
            _gState = GameState::Quit;  // ESC�����ꂽ�̂ŃQ�[�����I������
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