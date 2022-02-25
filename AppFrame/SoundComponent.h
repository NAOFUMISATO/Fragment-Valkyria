#pragma once
/*****************************************************************//**
 * \file   SoundComponent.h
 * \brief  �����̈ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <string_view>
#include "Vector4.h"
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
      class SoundComponent {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param appBase �A�v���P�[�V�����̊��N���X�̎Q��
          */
         SoundComponent(Game::GameBase& appBase) :_gameBase{ appBase } {};
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~SoundComponent() = default;
         /**
          * \brief �C�ӂ̕����񂩂特�����w�肵�A�o�b�N�O���E���h�Đ����s��
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          */
         void Play(std::string_view key, Math::Vector4 pos = {0,0,0});
         /**
          * \brief �C�ӂ̕����񂩂特�����w�肵�A���[�v�Đ����s��
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          */
         void PlayLoop(std::string_view key, Math::Vector4 pos = { 0,0,0 });
         /**
          * \brief ���ʂ̕ύX���s��
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          * \param changeVolume �ύX��̉���
          */
         void ChangeVolume(std::string_view key, int changeVolume);
         /**
          * \brief �������~����
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          */
         void Stop(std::string_view key);
      protected:
         /**
          * \brief ���ʂ̕ύX���s������A�v���C�^�C�v���w�肵�Đ�����
          * \param key ResourceServer�œo�^�ς݂̉������֘A�t�����C�ӂ̕�����
          * \param playType �Đ��̎��
          */
         void Play(std::string_view key, int playType,Math::Vector4 pos);

      private:
         Game::GameBase& _gameBase;   //!< �Q�[���x�[�X�̎Q��
      };
   }
}