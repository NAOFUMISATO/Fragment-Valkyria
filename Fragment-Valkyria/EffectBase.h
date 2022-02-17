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
            Effect = 0    //!< �G�t�F�N�g
         };
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key ResourceServer�ɓo�^�����C�ӂ̕�����̃G�t�F�N�g�L�[
          */
         EffectBase(Game::GameMain& gameMain,std::string_view key);

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
