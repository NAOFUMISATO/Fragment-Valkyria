#pragma once
/*****************************************************************//**
 * \file   ModeInGameBase.h
 * \brief  �e�C���Q�[���̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeBase.h"
#include "LightAndShadow.h"
#include "Stage.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   namespace Create {
      class ObjectFactory;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�C���Q�[���N���X
       * \brief �C���Q�[���̏�������
       */
      class ModeInGameBase : public ModeBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param game �Q�[���{�̂̎Q��
          */
         ModeInGameBase(Game::GameMain& gameMain);
         /**
          * \brief ��������
          */
         void Enter() override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Render() override;
         /**
          * \brief �o������
          */
         void Exit() override;
         /**
          * \brief �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[�����C���o�R�Ŏ擾
          * \return �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q��
          */
         Create::ObjectFactory& GetObjFactory() const;
         /**
          * \brief �X�e�[�W�̎Q�Ƃ̎擾
          * \return �X�e�[�W�̎Q��
          */
         inline Stage::Stage& GetStage() { return *_stage; }
         void IndividualEffectClear();
      protected:
         std::unique_ptr<Stage::Stage> _stage;                 //!< �X�e�[�W�N���X�̃��j�[�N�|�C���^
         std::unique_ptr<Lighting::LightAndShadow> _lighting;  //!< ���C�e�B���O�N���X�̃��j�[�N�|�C���^
#ifdef _DEBUG
         /**
          * \brief �f�o�b�O�`�揈��
          */
         void DebugDraw();
         short _padLeftX{ 0 };   //!< ���X�e�B�b�NX�����͗�
         short _padLeftY{ 0 };   //!< ���X�e�B�b�NY�����͗�
         short _padRightX{ 0 };  //!< �E�X�e�B�b�NX�����͗�
         short _padRightY{ 0 };  //!< �E�X�e�B�b�NY�����͗�
#endif
      };
   }
}