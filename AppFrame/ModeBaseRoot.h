#pragma once
/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  �e���[�h�̊��N���X
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <memory>
#include "GameBase.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   namespace Game {
      class GameBase;
   }
   namespace Resource {
      class ResourceServer;
      class LoadJson;
   }
   namespace Input {
      class InputManager;
   }
   namespace Sound {
      class SoundServer;
   }
   namespace Object {
      class ObjectServer;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      //��d�C���N���[�h�h�~
      class ModeServer;
      /**
       * \class �e���[�h�̊��N���X
       * \brief ModeServer�ɓo�^���郂�[�h�̊��N���X
       *        ���̃N���X���p�����ă��[�h���`����
       */
      class ModeBaseRoot {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param appBase �Q�[���x�[�X�̎Q��
          */
         ModeBaseRoot(Game::GameBase& gameBase);
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~ModeBaseRoot();
         /**
          * \brief ����������
          */
         virtual void Init() = 0;
         /**
          * \brief ��������
          */
         virtual void Enter() {};
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         virtual void Input(Input::InputManager& input) {};
         /**
          * \brief �X�V����
          */
         virtual void Update() {};
         /**
          * \brief �X�V����
          */
         virtual void Render() {};
         /**
          * \brief �o������
          */
         virtual void Exit() {};

         /**
          * \brief �Q�[���x�[�X�̎Q�Ƃ��擾
          * \return �Q�[���x�[�X�̎Q��
          */
         inline Game::GameBase& GetGameBase() const { return _gameBase; }
         /**
          * \brief ���[�h�ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return ���[�h�Ǘ��N���X�̎Q��
          */
         inline ModeServer& GetModeServer() const { return _gameBase.modeServer(); };
         /**
          * \brief ���\�[�X�ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return ���\�[�X�ꊇ�Ǘ��N���X�̎Q��
          */
         inline Resource::ResourceServer& GetResServer() const { return _gameBase.resServer(); }
         /**
          * \brief �����ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return
          */
         inline Sound::SoundServer& GetSoundServer() const { return _gameBase.soundServer(); }
         /**
          * \brief json�t�@�C���Ǘ��N���X�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return json�t�@�C���Ǘ��N���X�̎Q��
          */
         inline Resource::LoadJson& GetLoadJson() const { return _gameBase.loadJson(); }
         inline Object::ObjectServer& GetObjServer() const { return _gameBase.objServer(); }
         /**
          * \brief �t�F�[�h���Ԃ̐ݒ�
          * \param fadeType �t�F�[�h���Ԃ��w�肷�镶��
          */
         inline void fadeType(char fadeType) { _fadeType = fadeType; };

      protected:
         Game::GameBase& _gameBase;   //!< �Q�[���x�[�X�̎Q��
         char _fadeType{ 'M' };       //!< �t�F�[�h���Ԃ��w�肷�镶��
      };
   }
}