#pragma once
/*****************************************************************//**
 * \file   ModeTeam.h
 * \brief  ���[�h�`�[�����S�N���X
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
       * \class ���[�h�`�[�����S�N���X
       * \brief �`�[�����S�̕`����s��
       */
      class ModeTeam :public ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ModeTeam();
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
