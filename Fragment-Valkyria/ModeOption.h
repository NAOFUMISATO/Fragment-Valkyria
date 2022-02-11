#pragma once
/*****************************************************************//**
 * \file   ModeOption.h
 * \brief  ���[�h�I�v�V�����N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�I�v�V�����N���X
       * \brief �I�v�V������ʂ�\�����A�e�ݒ���s��
       */
      class ModeOption :public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ModeOption(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ��������
          */
         void Enter()override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input)override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Render()override;

      private:
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;//!< ��Ԃ̈ꊇ�Ǘ��N���X�̃|�C���^
         double _cameraSens{ 0 };                                   //!< �J�������x
         double _aimSens{ 0 };                                      //!< �G�C�����x
         int _deadZone{ 0 };                                        //!< �f�b�h�]�[��
         std::pair<int, int> _cameraCusorPos{ 0,0 };                //!< �J�������x�o�[�J�[�\�����W
         std::pair<int, int> _aimCusorPos{ 0,0 };                   //!< �G�C�����x�o�[�J�[�\�����W
         std::pair<int, int> _deadzoneCusorPos{ 0,0 };              //!< �f�b�h�]�[���o�[�J�[�\�����W
         std::pair<int, int> _selectCusorPos{ 0,0 };                //!< �I���G�t�F�N�g�摜�̍��W
         std::tuple<int, int, int, int, int, int, int> _grHandles{  //!< �摜�n���h���Q
            -1,-1,-1,-1,-1,-1,-1
         };

      public:
         /**
          * \class ���[�h�I�v�V�����̏�Ԋ��N���X
          * \brief �e��Ԃ͂��̃N���X��h�����Ē�`����
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�I�v�V�����̎Q��
             */
            StateBase(ModeOption& owner) : _owner{ owner } {};
            /**
             * \brief �X�V����
             */
            void Update()override;
            /**
             * \brief �`�揈��
             */
            void Draw() override;

         protected:
            ModeOption& _owner;   //!< ���[�h�I�v�V�����̎Q��
         };

         /**
          * \class �J�������x�ݒ�I����ԃN���X
          * \brief �J�������x�̐ݒ���s��
          */
         class StateCameraSencivitySelect : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�I�v�V�����̎Q��
             */
            StateCameraSencivitySelect(ModeOption& owner) :StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;
         };

         /**
          * \class �G�C�����x�ݒ�I����ԃN���X
          * \brief �G�C�����x�̐ݒ���s��
          */
         class StateAimSencivitySelect : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�I�v�V�����̎Q��
             */
            StateAimSencivitySelect(ModeOption& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;
         };

         /**
          * \class �f�b�h�]�[���ݒ�I����ԃN���X
          * \brief �f�b�h�]�[���̐ݒ���s��
          */
         class StateDeadZoneSelect : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�I�v�V�����̎Q��
             */
            StateDeadZoneSelect(ModeOption& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;
         };

         /**
          * \class ���[�h�I�v�V�����폜�I����ԃN���X
          * \brief ���[�h�I�v�V�����폜���s��
          */
         class StateReturnSelect : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�I�v�V�����̎Q��
             */
            StateReturnSelect(ModeOption& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;

         };
      };
   }
}