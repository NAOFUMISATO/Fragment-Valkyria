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
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   namespace {
      constexpr auto ScreenWidth = 1920;   //!< ����ʃT�C�Y
      constexpr auto ScreenHeight = 1080;  //!< �c��ʃT�C�Y
      constexpr auto ScreenDepth = 32;     //!< �r�b�g��
   }
   //��d�C���N���[�h�h�~
   namespace Mode {
      class ModeServer;
   }
   namespace Resource {
      class ResourceServer;
      class LoadJson;
   }
   namespace Input {
      class InputManager;
   }
   namespace Path {
      class PathServer;
   }
   namespace Sound {
      class SoundServer;
   }
   namespace Effect {
      class EffectServer;
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
      public:
         /**
          * \brief �Q�[���̏�ԗ�
          */
         enum class GameState {
            Play,    //!< ���s��
            Paused,  //!< �ꎞ��~
            Quit     //!< �I��
         };
         /**
          * \brief �R���X�g���N�^
          */
         GameBase();
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~GameBase();
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

         /**
          * \brief �Q�[���̊��N���X�̃C���X�^���X���擾
          * \return �Q�[���̊��N���X�̃C���X�^���X
          */
         inline static GameBase* gameInstance() { return _gameInstance; }
         /**
          * \brief ���[�h�T�[�o�[�̎Q�Ƃ��擾
          * \return ���[�h�T�[�o�[�̎Q��
          */
         inline Mode::ModeServer& modeServer() const { return *_modeServer; }
         /**
          * \brief ���\�[�X�T�[�o�[�̎Q�Ƃ��擾
          * \return ���\�[�X�T�[�o�[�̎Q��
          */
         inline Resource::ResourceServer& resServer() const { return *_resServer; }
         /**
          * \brief ���͂̈ꊇ�Ǘ��N���X�̎Q�Ƃ��擾
          * \return ���͂̈ꊇ�Ǘ��N���X�̎Q��
          */
         inline Input::InputManager& inputManager() const { return *_inputManager; }
         /**
          * \brief �p�X�T�[�o�[�̎Q�Ƃ��擾
          * \return �p�X�T�[�o�[�̎Q��
          */
         inline Path::PathServer& pathServer() const { return *_pathServer; }
         /**
          * \brief �T�E���h�Ǘ��T�[�o�[�̎Q�Ƃ��擾
          * \return �T�E���h�Ǘ��T�[�o�[�̎Q��
          */
         inline Sound::SoundServer& soundServer() const { return *_soundServer; }
         /**
          * \brief json�t�@�C���Ǘ��̎Q�Ƃ��擾
          * \return json�t�@�C���Ǘ��̎Q��
          */
         inline Resource::LoadJson& loadJson() const { return *_loadJson; }
         /**
          * \brief �G�t�F�N�g�T�[�o�[�̎Q�Ƃ��擾
          * \return �G�t�F�N�g�T�[�o�[�̎Q��
          */
         inline Effect::EffectServer& efcServer() const { return *_efcServer; }
         /**
          * \brief �E�B���h�E���[�h�ɐݒ肷�邩
          * \param _window_mode TRUE�ŃE�B���h�E���[�h�Ŏ��s�A�łȂ���΃t���X�N���[�����[�h�Ŏ��s
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
         inline virtual std::tuple<int, int, int> GraphSize() { return { ScreenWidth,ScreenHeight,ScreenDepth }; }

      protected:
         static GameBase* _gameInstance;        //!< �Q�[���̃C���X�^���X
         GameState _gState{ GameState::Play };                  //!< �Q�[�����
         std::unique_ptr<Mode::ModeServer> _modeServer;         //!< ���[�h�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Resource::ResourceServer> _resServer;  //!< ���\�[�X�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Input::InputManager> _inputManager;    //!< ���͂̈ꊇ�Ǘ��N���X
         std::unique_ptr<Path::PathServer> _pathServer;         //!< �p�X�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Sound::SoundServer> _soundServer;      //!< �T�E���h�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Resource::LoadJson> _loadJson;         //!< json�t�@�C���Ǘ��N���X
         std::unique_ptr<Effect::EffectServer> _efcServer;      //!< �G�t�F�N�g�̈ꊇ�Ǘ��N���X
      };
   }
}