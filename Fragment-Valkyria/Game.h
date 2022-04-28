#pragma once
/*****************************************************************//**
 * \file   GameMain.h
 * \brief  �Q�[���{�̃N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "LoadStageFromJson.h"
#include "ObjectServer.h"
#include "SpriteServer.h"
#include "EffectServer.h"
#include "ObjectFactory.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief �Q�[���{��
    */
   namespace Game {
      /**
       * \class �Q�[���{�̃N���X
       * \brief �Q�[���̃��C����������
       */
      class Game : public AppFrame::Temp::Singleton<Game>{
      private:
         /**
          * \brief �R���X�g���N�^
          */
         Game() {};
         /**
          * \brief �f�X�g���N�^
          */
         ~Game() {};
      public:
         friend class AppFrame::Temp::Singleton<Game>;
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
          * \return �������ɐ���������true�A���s������false��Ԃ�
          */
         bool Initialize(HINSTANCE hInstance);
         /**
          * \brief ���s
          */
         void Run();
         /**
          * \brief ��~
          */
         void ShutDown();
         /**
          * \brief ���͏���
          */
         void Input();
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Render();
         /**
          * \brief ���͂̈ꊇ�Ǘ��N���X�̎Q�Ƃ��擾
          * \return ���͂̈ꊇ�Ǘ��N���X�̃|�C���^
          */
         inline AppFrame::Input::InputManager& inputManager() const { return *_inputManager; }
         /**
          * \brief �T�E���h�Ǘ��T�[�o�[�̎Q�Ƃ��擾
          * \return �T�E���h�Ǘ��T�[�o�[�̃|�C���^
          */
         inline AppFrame::Sound::SoundComponent& soundComponent() const { return *_soundComponent; }
         /**
          * \brief json�t�@�C���Ǘ��̎Q�Ƃ��擾
          * \return json�t�@�C���Ǘ��̃|�C���^
          */
         inline AppFrame::Resource::LoadResourceJson& loadresJson() const { return *_loadresJson; }
         /**
          * \brief �摜�̊ȈՕ`��N���X�̎Q�Ƃ��擾
          * \return �摜�̊ȈՕ`��N���X�̃|�C���^
          */
         inline AppFrame::Texture::TextureComponent& texComponent() const { return *_texComponent; }
         /**
          * \brief �G������N���A�������̔����Ԃ�
          * \return �N���A���Ă����true,�łȂ����false
          */
         inline bool isPoorClear() { return _isPoorClear; }
         /**
          * \brief �G������N���A�������̔����ݒ�
          * \param poorClear �G������N���A�������̔���
          */
         inline void isPoorClear(bool poorClear) { _isPoorClear = poorClear; }
         /**
          * \brief �`���[�g���A���̃N���A�����Ԃ�
          * \return �N���A���Ă����true�A�łȂ����false
          */
         inline bool isTutorialClear() { return _isTutorialClear; }
         /**
          * \brief �`���[�g���A���̃N���A�����ݒ肷��
          * \param tutorialClear �ݒ肷��N���A����
          */
         inline void isTutorialClear(bool tutorialClear) { _isTutorialClear = tutorialClear; }
         /**
          * \brief �v�����Ԃ̎擾
          * \return �C���Q�[���Ōv�����Ă��鎞��
          */
         inline unsigned int ingameTimer() const { return _ingameTimer; }
         /**
          * \brief �v�����Ԃ̐ݒ�
          * \param timer �ݒ肷�鎞��
          */
         inline void ingameTimer(unsigned int timer) { _ingameTimer = timer; }
         /**
          * \brief �v�����Ԃ�i�߂�
          */
         inline void IngameTimerCount() { _ingameTimer++; }
         /**
          * \brief �Q�[�������x�̎擾
          * \return �Q�[�������x
          */
         inline std::tuple<double, double, int> sensitivity() const { return _sensitivity; }
         /**
          * \brief �Q�[�������x�̐ݒ�
          * \param cameraSens �J�������x
          * \param aimSens �G�C�����x
          * \param deadZone �f�b�h�]�[��
          */
         inline void sensitivity(double cameraSens, double aimSens, int deadZone) { _sensitivity = std::make_tuple(cameraSens, aimSens, deadZone); }
         /**
          * \brief �v���C���[�̃X�e�[�^�X�̎擾
          * \return �v���C���[�̃X�e�[�^�X
          */
         inline std::tuple<double, int, int> playerStatus() const { return _playerStatus; }
         /**
          * \brief �v���C���[�̃X�e�[�^�X�ݒ�
          * \param hp �̗�
          * \param bullet �c�e
          * \param portion �c�񕜃A�C�e����
          */
         inline void playerStatus(double hp, int bullet, int portion) { _playerStatus = std::make_tuple(hp, bullet, portion); }
         /**
          * \brief �v���C���[�̗͂̎擾
          * \return �v���C���[�̗�
          */
         inline double playerHp()const { return std::get<0>(_playerStatus); }
         /**
          * \brief �v���C���[�̗͂̐ݒ�
          * \param hp �ݒ肷��̗�
          */
         inline void playerHp(double hp) {
            auto [oldHp, bullet, portion] = _playerStatus;
            _playerStatus = std::make_tuple(hp, bullet, portion);
         }
         /**
          * \brief �v���C���[�̎c�e���̎擾
          * \return �v���C���[�̎c�e��
          */
         inline int playerBullet() const { return std::get<1>(_playerStatus); }
         /**
          * \brief �v���C���[�̎c�e���̐ݒ�
          * \param bullet �ݒ肷��v���C���[�̎c�e��
          */
         inline void playerBullet(int bullet) {
            auto [hp, oldBullet, portion] = _playerStatus;
            _playerStatus = std::make_tuple(hp, bullet, portion);
         }
         /**
          * \brief �v���C���[�̎c�񕜃A�C�e�����̎擾
          * \return �v���C���[�̎c�񕜃A�C�e����
          */
         inline int playerPortion()const { return std::get<2>(_playerStatus); }
         /**
          * \brief �v���C���[�̎c�񕜃A�C�e�����̐ݒ�
          * \param portion �ݒ肷��v���C���[�̎c�񕜃A�C�e����
          */
         inline void playerPortion(int portion) {
            auto [hp, bullet, oldPortion] = _playerStatus;
            _playerStatus = std::make_tuple(hp, bullet, portion);
         }
         /**
          * \brief �I�u�W�F�N�g�̐����ꊇ�Ǘ��N���X�̎擾
          * \return �I�u�W�F�N�g�̐����ꊇ�Ǘ��N���X�̎Q��
          */
         Create::ObjectFactory& objFactory() const { return *_objFactory; }
         /**
          * \brief �X�e�[�W���Ǘ��N���X�̎擾
          * \return �X�e�[�W���Ǘ��N���X�̎Q��
          */
         Stage::LoadStageFromJson& loadStage() const { return *_loadStage; }
         /**
          * \brief �I�u�W�F�N�g�T�[�o�[�̎Q�Ƃ��擾
          * \return �I�u�W�F�N�g�T�[�o�[�̎Q��
          */
         inline Object::ObjectServer& objServer() const { return *_objServer; }
         /**
          * \brief �X�v���C�g�T�[�o�[�̎Q�Ƃ��擾
          * \return �X�v���C�g�T�[�o�[�̎Q��
          */
         inline Sprite::SpriteServer& sprServer() const { return *_sprServer; }
         /**
          * \brief �G�t�F�N�g�T�[�o�[�̎Q�Ƃ��擾
          * \return �G�t�F�N�g�T�[�o�[�̃|�C���^
          */
         inline Effect::EffectServer& efcServer() const { return *_efcServer; }

      private:
         /**
          * \brief �t�@�C���ւ̃J�����g�p�X��o�^����
          */
         void CurrentPathRegist();
         /**
          * \brief ���[�h��o�^����
          */
         void ModeRegist();

         GameState _gState{ GameState::Play };                               //!< �Q�[�����
         bool _isPoorClear{ false };                                         //!< �G������N���A������
         bool _isTutorialClear{ false };                                     //!< �`���[�g���A�����I���܂��̓X�L�b�v������
         unsigned int _ingameTimer{ 0 };                                     //!< �Q�[�����^�C�}�[
         std::tuple<double, double, int> _sensitivity;                       //!< �Q�[�������x�y�уf�b�h�]�[���l��Tuple�^
         std::tuple<double, int, int> _playerStatus;                         //!< �v���C���[�̃X�e�[�^�X��Tuple�^
         std::unique_ptr<AppFrame::Input::InputManager> _inputManager;       //!< ���͂̈ꊇ�Ǘ��N���X
         std::unique_ptr<AppFrame::Sound::SoundComponent> _soundComponent;   //!< �T�E���h�̈ꊇ�Ǘ��N���X
         std::unique_ptr<AppFrame::Resource::LoadResourceJson> _loadresJson; //!< json�t�@�C���Ǘ��N���X
         std::unique_ptr<AppFrame::Texture::TextureComponent> _texComponent; //!< �摜�̊ȈՕ`��N���X
         std::unique_ptr<Object::ObjectServer> _objServer;                   //!< �I�u�W�F�N�g�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Sprite::SpriteServer> _sprServer;                   //!< �X�v���C�g�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Effect::EffectServer> _efcServer;                   //!< �G�t�F�N�g�̈ꊇ�Ǘ��N���X
         std::unique_ptr<Create::ObjectFactory> _objFactory;                 //!< �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̃��j�[�N�|�C���^
         std::unique_ptr<Stage::LoadStageFromJson> _loadStage;               //!< �X�e�[�W���Ǘ��N���X�̃��j�[�N�|�C���^
      };
   }
}
