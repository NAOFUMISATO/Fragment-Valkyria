#pragma once
/*****************************************************************//**
 * \file   SoundServer.h
 * \brief  �����̈ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <string_view>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   //��d�C���N���[�h�h�~
   namespace Game {
      class GameBase;
   }
   /**
    * \brief �����֌W
    */
   namespace Sound {
      /**
       * \class �����̈ꊇ�Ǘ��N���X
       * \brief �����̍Đ��A���ʕύX���s��
       */
      class SoundServer {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param appBase �A�v���P�[�V�����̊��N���X�̎Q��
          */
         SoundServer(Game::GameBase& appBase) :_gameBase{ appBase } {};
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~SoundServer() = default;
         /**
          * \brief �C�ӂ̕����񂩂特�����w�肵�A�o�b�N�O���E���h�Đ����s��
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          */
         void Play(std::string_view key);
         /**
          * \brief �C�ӂ̕����񂩂特�����w�肵�A���[�v�Đ����s��
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          */
         void PlayLoop(std::string_view key);
         /**
          * \brief ���ʂ̕ύX���s��
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          * \param changeVolume �ύX��̉���
          */
         void ChangeVolume(std::string_view key, int changeVolume);

      protected:
         /**
          * \brief ���ʂ̕ύX���s������A�v���C�^�C�v���w�肵�Đ�����
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          * \param playType �Đ��̎��
          */
         void Play(std::string_view key, int playType);

      private:
         Game::GameBase& _gameBase;   //!< �Q�[���x�[�X�̎Q��
      };
   }
}