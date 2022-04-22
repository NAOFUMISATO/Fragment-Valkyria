#pragma once
/*****************************************************************//**
 * \file   LightAndShadow.h
 * \brief  ���C�e�B���O�y�ї����e�̏������s���N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
#include "ParamLightShadow.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
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
          */
         LightAndShadow();
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Render();
         /**
          * \brief ���C�e�B���O�̊g�U���F�̐ݒ�
          * \param red ��
          * \param green ��
          * \param blue ��
          */
         void SetDifColor(float red,float green,float blue);
         /**
          * \brief ���C�e�B���O�̊����F�̐ݒ�
          * \param red ��
          * \param green ��
          * \param blue ��
          */
         void SetAmbColor(float red, float green, float blue);

      private:
         std::unique_ptr<Param::ParamLightShadow> _param;//!< ���C�e�B���O�������s���N���X�̒l�Ǘ��N���X
         int _lightHandle{ -1 };                         //!< �����n���h���̃y�A�^
         int _shadowHandle{ -1 };                        //!< �����e�n���h��
         Vector4 _lightPosition{ 0,0,0 };                //!< �����n���h���ʒu�̃y�A�^
      };
   }

}
