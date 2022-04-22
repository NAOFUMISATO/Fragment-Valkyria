#pragma once
/*****************************************************************//**
 * \file   ModeAmg.h
 * \brief  ���[�hAMG���S�N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
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
       * \class ���[�hAMG���S�N���X
       * \brief AMG���S�̕`����s��
       */
      class ModeAmg :public ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ModeAmg();
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ��������
          */
         void Enter()override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Render() override;

      private:
         int _grHandle{ -1 };    //!< �摜�n���h��
         int _fadeCnt{ -1 };     //!< �t���[���J�E���g�ۑ��p
         bool _cntInit{ false }; //!< �t���[���J�E���g�������t���O
      };
   }
}
