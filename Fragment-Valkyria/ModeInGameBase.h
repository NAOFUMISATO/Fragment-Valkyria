#pragma once
/*****************************************************************//**
 * \file   ModeInGameBase.h
 * \brief  �e�C���Q�[���̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeBase.h"
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
          * \brief �C���Q�[����ʂ̎擾
          */
         enum class InGameType {
            Boss   //!< ���[�h�{�X
         };

         /**
          * \brief �R���X�g���N�^
          * \param game �Q�[���{�̂̎Q��
          */
         ModeInGameBase(Game::GameMain& gameMain);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~ModeInGameBase() = default;
         /**
          * \brief ����������
          */
         void Init() override;
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
          * \brief �C���Q�[����ʂ̎擾
          * \return �h����Œ�`
          */
         virtual InGameType GetInGameType() const = 0;
         /**
          * \brief �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[�����C���o�R�Ŏ擾
          * \return �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q��
          */
         Create::ObjectFactory& GetObjFactory() const;

      protected:
         int _lightHandleFirst{ -1 };
         int _lightHandleSecond{ -1 };
         int _lightHandleThird{ -1 };
         int _shadowMapHandleFirst{ -1 };
         int _shadowMapHandleSecond{ -1 };
         int _shadowMapHandleThird{ -1 };
         Vector4 _lightPoint{ 0,40.0,0 };
#ifdef _DEBUG
         void DebugDraw();
         short _padLeftX{ 0 };
         short _padLeftY{ 0 };
         short _padRightX{ 0 };
         short _padRightY{ 0 };
         double _largeEnemyHp{ 0 };
         double _playerHp{ 0 };
         Vector4 _lightFirstPos{ 0,0,0 };
         Vector4 _lightSecondPos{ 0,0,0 };
         Vector4 _lightThirdPos{ 0,0,0 };
        
#endif
      };
   }
}