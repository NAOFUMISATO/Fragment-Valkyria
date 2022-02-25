#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatling.h
 * \brief  �K�g�����O�U�������Ă���G���G�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "PoorEnemyBase.h"
namespace FragmentValkyria {
   namespace Enemy {

      class PoorEnemyGatling : public PoorEnemyBase {
            using Vector4 = AppFrame::Math::Vector4;
            using InputManager = AppFrame::Input::InputManager;
        public:
            PoorEnemyGatling(Game::GameMain& gameMain);
            void Init()override;
        private:
            void CreateGatling();

            Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };
            Vector4 _gatlingMoveDirection{ Vector4(0.0, 0.0, 0.0) };

      public:
            /**
            * \class �ړ���ԃN���X
            * \brief �ړ���Ԃ̏�������
            */
            class StateGatling : public StateBase{
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
               PoorEnemyGatling& _owner;
               int _gatlingCnt{ 5 };
            };
      };
   }

}
