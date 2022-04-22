#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatling.h
 * \brief  ���u�U���^�C�v�̎G���G�N���X
 * 
 * \author AHMD2000,NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "PoorEnemyBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �G�֌W
    */
   namespace Enemy {
      /**
       * \class ���u�U���^�C�v�̎G���G�N���X
       * \brief ���u�U���^�C�v�̎G���G�̏�������
       */
      class PoorEnemyGatling : public PoorEnemyBase {
         using Vector4 = AppFrame::Math::Vector4;
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         PoorEnemyGatling();
         /**
          * \brief ����������
          */
         void Init()override;

      private:
         /**
          * \brief �K�g�����O�e�̐�������
          */
         void CreateGatling();

         Vector4 _gatlingMoveDirection{ Vector4(0.0, 0.0, 0.0) }; //!< �K�g�����O�e������K���x�N�g��

      public:
         /**
         * \class �ړ���ԃN���X
         * \brief �ړ���Ԃ̏�������
         */
         class StateGatling : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �K�g�����O�U�������Ă���G���G�̎Q��
             */
            StateGatling(PoorEnemyGatling& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;

         private:
            PoorEnemyGatling& _owner;     //!< ���u�U���^�C�v�̎G���G�N���X�̎Q��
            int _remainingGatiling{ 5 };  //!< �K�g�����O�̎c�e��
         };
      };
   }
}
