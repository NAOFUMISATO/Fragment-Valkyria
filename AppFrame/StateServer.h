#pragma once
/*****************************************************************//**
 * \file   StateServer.h
 * \brief  �e��Ԉꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <string_view>
#include <unordered_map>
#include <memory>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ��Ԋ֌W
    */
    //��d�C���N���[�h�h�~
   namespace Input {
      class InputManager;
   }
   namespace State {
      class StateBaseRoot;
      /**
       * \class �e��Ԉꊇ�Ǘ��N���X
       * \brief StateBaseRoot���p������`������ԃN���X�̓o�^�A�Ǘ����s��
       */
      class StateServer {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key �ŏ��̏�ԂɊ֘A�t����C�ӂ̕�����
          * \param state �ŏ��̏�Ԃ̃C���X�^���X
          */
         StateServer(std::string_view key, std::shared_ptr<StateBaseRoot> state);
         /**
          * \brief ��Ԃ̓o�^
          * \param key ��ԂɊ֘A�t���邽�߂̔C�ӂ̕�����
          * \param state ��Ԃ̃C���X�^���X
          */
         void Register(std::string_view key, std::shared_ptr<StateBaseRoot> state);
         /**
          * \brief ���X�g�̈�Ԍ��(�őO��)�ɏ�Ԃ�ǉ�����
          * \param key �o�^�ς݂̏�ԂɊ֘A�t����������
          */
         void PushBack(std::string_view key);
         /**
          * \brief ���X�g�̈�Ԍ��(�őO��)�̏�Ԃ����O����
          */
         void PopBack();
         /**
          * \brief ��Ԃ̑J��
          * \param key �o�^�ς݂̏�ԂɊ֘A�t����������
          */
         void GoToState(std::string_view key);
         void InsertBelowBack(std::string_view key);
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(Input::InputManager& input);
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Draw() const;

      private:
         std::unordered_map<std::string, std::shared_ptr<StateBaseRoot>> _stateRegistry; //!< ��Ԃ�o�^����A�z�z��
         std::list<std::shared_ptr<StateBaseRoot>> _stateList;                           //!< ��Ԃ̏������񂷑o�����z��
      };
   }
}

