#pragma once
/*****************************************************************//**
 * \file   StateBaseRoot.h
 * \brief  �e��Ԋ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ��Ԋ֌W
    */
   // ��d�C���N���[�h�h�~
   namespace Input {
      class InputManager;
   }
   namespace State {
      /**
       * \class �e��Ԋ��N���X
       * \brief StateServer�ɓo�^����e��Ԃ̊��N���X
       *        ���̃N���X���p�����ď�Ԃ��`����
       */
      class StateBaseRoot {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         StateBaseRoot() {};
         /**
          * \brief �������� 1�x�����Ă΂��
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
          * \brief �`�揈��
          */
         virtual void Draw() {};
         /**
          * \brief �o������
          */
         virtual void Exit() {};
      };
   }
}

