#pragma once
/*****************************************************************//**
 * \file   EffectBase.h
 * \brief  �G�t�F�N�g�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   //��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   /**
    * \brief �G�t�F�N�g�֌W
    */
   namespace Effect {
      /**
       * \class �G�t�F�N�g�̊��N���X
       * \brief �e�G�t�F�N�g�͂��̃N���X��h�����Ē�`����
       */
      class EffectBase :public AppFrame::Effect::EffectBaseRoot {
      public:
         /**
          * \brief �G�t�F�N�g�̎��
          */
         enum class EffectType {
            Effect=0,   //!< �G�t�F�N�g
            PlayerShot  //!< �v���C���[�ˌ�
         };
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         EffectBase(Game::GameMain& gameMain);
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~EffectBase() override ;

         /**
          * \brief �������y��_loadHandle�ɃG�t�F�N�g�n���h����o�^����
          * \param key ResourceServer�ɓo�^�ς݂̔C�ӂ̕�����
          */
         void Init(std::string_view key) override ;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input) override {};
         /**
          * \brief �X�V����
          */
         void Update() override {};
         /**
          * \brief �`�揈��
          */
         void Draw() override ;
         /**
          * \brief �G�t�F�N�g�̎�ʂ̎擾
          * \return �h����Œ�`
          */
         virtual EffectType GetEfcType() const = 0;

      protected:
         Game::GameMain& _gameMain;   //!< �Q�[���{�̃N���X�̎Q��
      };
}
}
