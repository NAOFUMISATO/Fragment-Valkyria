#pragma once
/*****************************************************************//**
 * \file   PoorEnemyMelee.h
 * \brief  �ߐڍU���^�C�v�̎G���G�N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
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
       * \class �ߐڍU���^�C�v�̎G���G�N���X
       * \brief �ߐڍU���^�C�v�̎G���G�̏������s��
       */
      class PoorEnemyMelee :public PoorEnemyBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         PoorEnemyMelee(Game::GameMain& gameMain);
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
            StateRush(PoorEnemyMelee& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;

         private:
            PoorEnemyMelee& _owner;    //!< �ߐڍU���^�C�v�̎G���G�N���X�̎Q��
            Vector4 _moved{ 0,0,0 };   //!< �ːi��������̐��K���x�N�g��
         };
      };
   }
}
