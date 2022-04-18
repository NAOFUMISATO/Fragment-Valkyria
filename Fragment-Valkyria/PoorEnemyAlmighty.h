#pragma once
/*****************************************************************//**
 * \file   PoorEnemyAlmighty.h
 * \brief  �S�Ă̍U�����s���G���G�N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
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
       * \class �S�Ă̍U�����s���G���G�N���X
       * \brief �S�Ă̍U�����s���G���G�̏������s��
       */
      class PoorEnemyAlmighty :public PoorEnemyBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         PoorEnemyAlmighty(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;

      private:
         /**
          * \brief �ːi�U���̏���
          * \param moved �ːi��������̐��K���x�N�g��
          */
         void Rush(const Vector4& moved);
         /**
          * \brief �K�g�����O�e�̐�������
          */
         void CreateGatling();

         Vector4 _gatlingMoveDirection{ Vector4(0.0, 0.0, 0.0) }; //!< �K�g�����O�e������K���x�N�g��

      public:
         /**
          * \class �ːi�U����ԃN���X
          * \brief �ːi�U����Ԃ̏������s��
          */
         class StateRush : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �ߐڍU���^�C�v�̎G���G�N���X�̎Q��
             */
            StateRush(PoorEnemyAlmighty& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;

         private:
            PoorEnemyAlmighty& _owner;    //!< �ߐڍU���^�C�v�̎G���G�N���X�̎Q��
            Vector4 _moved{ 0,0,0 };   //!< �ːi��������̐��K���x�N�g��
         };
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
            StateGatling(PoorEnemyAlmighty& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;

         private:
            PoorEnemyAlmighty& _owner;     //!< ���u�U���^�C�v�̎G���G�N���X�̎Q��
            int _remainingGatiling{ 5 };   //!< �K�g�����O�̎c�e��
         };
      };
   }
}
