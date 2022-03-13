#pragma once
/*****************************************************************//**
 * \file   LightAndShadow.h
 * \brief  ���C�e�B���O�y�ї����e�̏������s���N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   /**
    * \brief ���C�e�B���O�֌W
    */
   namespace Lighting {
      /**
       * \class ���C�e�B���O�y�ї����e�̏������s���N���X
       * \brief ���������y�уV���h�E�}�b�v�̕`����Ǘ�����
       */
      class LightAndShadow {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         LightAndShadow(Game::GameMain& gameMain);
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Render();
         void SetDifColor(float red,float green,float blue);
         void SetAmbColor(float red, float green, float blue);
      private:
         Game::GameMain& _gameMain;       //!< �Q�[���{�̃N���X�̎Q��
         int _lightHandle{ -1 };          //!< �����n���h���̃y�A�^
         Vector4 _lightPosition{ 0,0,0 }; //!< �����n���h���ʒu�̃y�A�^
         int _shadowHandle{ -1 };         //!< �����e�n���h��
      };
   }

}
