#pragma once
/*****************************************************************//**
 * \file   GameBase.h
 * \brief  �Q�[���{�̂̊��N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <memory>
#include <tuple>
#include <DxLib.h>
#include "Singleton.h"
#include "ModeServer.h"
#include "InputManager.h"
#include "ResourceServer.h"
#include "SoundComponent.h"
#include "CurrentPathServer.h"
#include "LoadResourceJson.h"
#include "TextureComponent.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   //��d�C���N���[�h�h�~
   namespace Input {
      class InputManager;
   }
   /**
    * \brief �Q�[���x�[�X
    */
   namespace Game {
      /**
       * \class �A�v���P�[�V�����̊��N���X
       * \brief �Q�[�����ƂɌp�����Ē�`����
       */
      class GameBase {
      protected:
         /**
          * \brief �R���X�g���N�^
          */
         GameBase() {};
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~GameBase() {};
      public:
         friend class GameBase;
         /**
          * \brief �Q�[���̏�ԗ�
          */
         enum class GameState {
            Play,    //!< ���s��
            Paused,  //!< �ꎞ��~
            Quit     //!< �I��
         };
         /**
          * \brief ����������
          * \param hInstance WinMain�̑�����
          * \return ����������������
          */
         virtual bool Initialize(HINSTANCE hInstance);
         /**
          * \brief ���C�����[�v�����s����
          */
         virtual void Run();
         /**
          * \brief ���C�����[�v���I������
          */
         virtual void ShutDown();
         /**
          * \brief ���C�����[�v���Ŏ��s������͏���
          */
         virtual void Input();
         /**
          * \brief ���C�����[�v���Ŏ��s����X�V����
          */
         virtual void Update();
         /**
          * \brief ���C�����[�v���Ŏ��s����`�揈��
          */
         virtual void Render();

         static GameBase& GetInstance() {
            static GameBase instance;
            return instance;
         }
         /**
          * \brief ���[�h�T�[�o�[�̎Q�Ƃ��擾
          * \return ���[�h�T�[�o�[�̃|�C���^
          */
         inline Mode::ModeServer& modeServer() const { return *_modeServer; }
         /**
          * \brief ���\�[�X�T�[�o�[�̎Q�Ƃ��擾
          * \return ���\�[�X�T�[�o�[�̃|�C���^
          */
         inline Resource::ResourceServer& resServer() const { return *_resServer; }
         /**
          * \brief ���͂̈ꊇ�Ǘ��N���X�̎Q�Ƃ��擾
          * \return ���͂̈ꊇ�Ǘ��N���X�̃|�C���^
          */
         inline Input::InputManager& inputManager() const { return *_inputManager; }
         /**
          * \brief �p�X�T�[�o�[�̎Q�Ƃ��擾
          * \return �p�X�T�[�o�[�̃|�C���^
          */
         inline Path::CurrentPathServer& pathServer() const { return *_pathServer; }
         /**
          * \brief �T�E���h�Ǘ��T�[�o�[�̎Q�Ƃ��擾
          * \return �T�E���h�Ǘ��T�[�o�[�̃|�C���^
          */
         inline Sound::SoundComponent& soundComponent() const { return *_soundComponent; }
         /**
          * \brief json�t�@�C���Ǘ��̎Q�Ƃ��擾
          * \return json�t�@�C���Ǘ��̃|�C���^
          */
         inline Resource::LoadResourceJson& loadresJson() const { return *_loadresJson; }
         /**
          * \brief �摜�̊ȈՕ`��N���X�̎Q�Ƃ��擾
          * \return �摜�̊ȈՕ`��N���X�̃|�C���^
          */
         inline Texture::TextureComponent& texComponent() const { return *_texComponent; }
         /**
          * \brief �E�B���h�E���[�h�ɐݒ肷�邩
          * \param windowMode TRUE�ŃE�B���h�E���[�h�Ŏ��s�A�łȂ���΃t���X�N���[�����[�h�Ŏ��s
          */
         inline virtual void AppWindowMode(bool windowMode) { ChangeWindowMode(windowMode); }
         /**
          * \brief �f�o�b�O���O���o�͂��邩
          * \param _window_mode TRUE�Ńf�o�b�O���O�o��
          */
         inline virtual void OutputDebugLog(bool windowMode) { SetOutApplicationLogValidFlag(windowMode); };
         /**
          * \brief ��ʐݒ�̒l��Ԃ�
          * \return ��ʉ��T�C�Y�A��ʏc�T�C�Y�A��ʃr�b�g��
          */
         inline virtual std::tuple<int, int, int> GraphSize() { return { 1280,1024,32 }; }

      protected:
         GameState _gState{ GameState::Play };                     //!< �Q�[�����
         std::unique_ptr<Mode::ModeServer> _modeServer;            //!< ���[�h�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Resource::ResourceServer> _resServer;     //!< ���\�[�X�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Input::InputManager> _inputManager;       //!< ���͂̈ꊇ�Ǘ��N���X
         std::unique_ptr<Path::CurrentPathServer> _pathServer;     //!< �p�X�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Sound::SoundComponent> _soundComponent;   //!< �T�E���h�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Resource::LoadResourceJson> _loadresJson; //!< json�t�@�C���Ǘ��N���X
         std::unique_ptr<Texture::TextureComponent> _texComponent; //!< �摜�̊ȈՕ`��N���X
      };
   }
}