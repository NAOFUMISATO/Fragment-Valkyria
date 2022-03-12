#pragma once
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
      class TutorialTips :public Sprite::SpriteBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
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
         
         bool IsTipsClear(std::string_view key) { return _tipsClear[key.data()]; }

      private:
         int _width{ 0 };
         bool _isAppear{ false };
         std::string _tipsName{ "" };
         std::unordered_map<std::string, int> _tipsNum;
         std::unordered_map<std::string, bool> _tipsClear;
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< ��Ԉꊇ�Ǘ��N���X�̃|�C���^ 

      public:
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            
            StateBase(TutorialTips& owner) : _owner{ owner } {};
            /**
             * \brief �`�揈��
             */
            void Draw() override;

         protected:
            TutorialTips& _owner;   //!< ���[�h�^�C�g���̎Q��
         };

         class StateFadeIn : public StateBase {
         public:
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

         class StateJudge : public StateBase {
         public:
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
            void MoveJudge(InputManager& input);
            void CameraJudge(InputManager& input);
            void WeakShootJudge(InputManager& input);
            void ReloadJudge(InputManager& input);
            void ObjectShootJudge(InputManager& input);
            void HealJudge(InputManager& input);
            bool _isWeakReady{ false };
            bool _isObjectReady{ false };
         };

         class StateFadeOut : public StateBase {
         public:
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
