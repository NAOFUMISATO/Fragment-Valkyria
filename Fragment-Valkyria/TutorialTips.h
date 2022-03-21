#pragma once
/*****************************************************************//**
 * \file   TutorialTips.h
 * \brief  �`���[�g���A���\���e�L�X�g�N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   /**
    * \brief �`���[�g���A���֌W
    */
   namespace Tutorial {
      /**
       * \class �`���[�g���A���\���e�L�X�g�N���X
       * \brief �`���[�g���A���̕\�����s��
       */
      class TutorialTips :public Sprite::SpriteBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param tipsName �\������TIPS�̉摜�Ɋ֘A�t����������
          */
         TutorialTips(Game::GameMain& gameMain,std::string_view tipsName);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Draw()override;
         /**
          * \brief �X�v���C�g��ʂ̎擾
          * \return ���:�`���[�g���A���\���e�L�X�g
          */
         virtual SpriteType GetSprType() const { return SpriteType::TutorialTips; }
         /**
          * \brief �o���������ǂ�����Ԃ�
          * \return �o�������Ȃ�true,�łȂ��Ȃ��false
          */
         bool isAppear() { return _isAppear; }
         /**
          * \brief TIPS���̃N���A�����Ԃ�
          * \param key TIPS�Ɋ֘A�t����������
          * \return �֘A�t���������񖈂̃N���A����
          */
         bool IsTipsClear(std::string_view key) { return _tipsClear[key.data()]; }

      private:
         int _width{ 0 };                                             //!< �摜���T�C�Y
         bool _isAppear{ false };                                     //!< TIPS�o���t���O
         std::string _tipsName{ "" };                                 //!< TIPS�̉摜�Ɋ֘A�t����������
         std::unordered_map<std::string, int> _tipsNum;               //!< TIPS�Ɋ֘A�t���镶����Ɣԍ��̘A�z�z��
         std::unordered_map<std::string, bool> _tipsClear;            //!< �֘A�t���������񖈂̒B���t���O
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< ��Ԉꊇ�Ǘ��N���X�̃|�C���^

      public:
         /**
          * \class ��Ԃ̊��N���X
          * \brief �e��Ԃ͂��̃N���X��h������`����
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �`���[�g���A���\���e�L�X�g�N���X�̎Q��
             */
            StateBase(TutorialTips& owner) : _owner{ owner } {};
            /**
             * \brief �`�揈��
             */
            void Draw() override;

         protected:
            TutorialTips& _owner;   //!< ���[�h�^�C�g���̎Q��
         };
         /**
          * \class TIPS�̃t�F�[�h�C����ԃN���X
          * \brief �t�F�[�h�C��������
          */
         class StateFadeIn : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �`���[�g���A���\���e�L�X�g�N���X�̎Q��
             */
            StateFadeIn(TutorialTips& owner) :StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update()override;
            /**
             * \brief �o������
             */
            void Exit()override;
         };
         /**
          * \class �N���A������s����ԃN���X
          * \brief �N���A������s��
          */
         class StateJudge : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �`���[�g���A���\���e�L�X�g�N���X�̎Q��
             */
            StateJudge(TutorialTips& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;

         private:
            /**
             * \brief �ړ��̃N���A������s��
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void MoveJudge(InputManager& input);
            /**
             * \brief �J�����ړ��̃N���A������s��
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void CameraJudge(InputManager& input);
            /**
             * \brief ��U���̃N���A������s��
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void WeakShootJudge(InputManager& input);
            /**
             * \brief �����[�h�̃N���A������s��
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void ReloadJudge(InputManager& input);
            /**
             * \brief �I�u�W�F�N�g���˂̃N���A������s��
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void ObjectShootJudge(InputManager& input);
            /**
             * \brief �񕜂̃N���A������s��
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void HealJudge(InputManager& input);
            bool _isWeakReady{ false };   //!< ��U���\����Ԃ��N���A������
            bool _isObjectReady{ false }; //!< �I�u�W�F�N�g���ˍU���\����Ԃ��N���A������
         };
         /**
          * \class TIPS�̃t�F�[�h�A�E�g��ԃN���X
          * \brief �t�F�[�h�A�E�g������
          */
         class StateFadeOut : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �`���[�g���A���\���e�L�X�g�N���X�̎Q��
             */
            StateFadeOut(TutorialTips& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update()override;
         };
      };
   }
}
