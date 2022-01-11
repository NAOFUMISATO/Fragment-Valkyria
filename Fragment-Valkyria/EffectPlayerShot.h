#pragma once
/*****************************************************************//**
 * \file   EffectPlayerShot.h
 * \brief  �v���C���[�ˌ��G�t�F�N�g�N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �G�t�F�N�g�֌W
    */
   namespace Effect {
      /**
       * \class �v���C���[�ˌ��G�t�F�N�g�N���X
       * \brief �ˌ��G�t�F�N�g�̔���
       */
      class EffectPlayerShot :public EffectBase{
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         EffectPlayerShot(Game::GameMain& gameMain);
         /**
          * \brief �f�X�g���N�^
          */
         ~EffectPlayerShot()override;

         /**
          * \brief �������y��_loadHandle�ɃG�t�F�N�g�n���h����o�^����
          * \param key ResourceServer�ɓo�^�ς݂̔C�ӂ̕�����
          */
         void Init(std::string_view key)override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input)override {};
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Draw()override;
         /**
          * \brief �I�u�W�F�N�g�̎�ʂ̎擾
          * \return �v���C���[�ˌ��G�t�F�N�g
          */
         EffectType GetEfcType() const override { return EffectType::PlayerShot; }
      };
   }

}
