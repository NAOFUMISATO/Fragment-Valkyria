#pragma once
/*****************************************************************//**
 * \file   LargeEnemyHP.h
 * \brief  �{�X�̗̓o�[�`��N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �G�֌W
    */
   namespace Enemy {
      /**
       * \class �{�X�̗̓o�[�`��N���X
       * \brief �{�X�̗̓o�[�̕`����s��
       */
      class LargeEnemyHP :public Sprite::SpriteBase {
         using Color = AppFrame::Data::Color;
         using Rect = AppFrame::Data::Rect;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         LargeEnemyHP(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
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
          * \return ���:�X�v���C�g
          */
         virtual SpriteType GetSprType() const { return SpriteType::Sprite; }

         private:
            double _hp{ 0.0 };                //!< �̗�
            double _rate{ 0.0 };              //!< �w�ʃo�[�̑������x
            double _oldFrontHP{ 0.0 };        //!< 1�t���[���O�̑O�ʃo�[�̃v���C���[HP
            double _oldBackHP{ 0.0 };         //!< 1�t���[���O�̔w�ʃo�[�̃v���C���[HP
            bool _rateReset{ true };          //!< �w�ʃo�[�̑������x�����Z�b�g���邩
            bool _shake{ false };             //!< HP�o�[��U�������邩
            Rect _frontBar{ 0,0,0,0 };        //!< �O�ʃo�[��`
            Rect _backBar{ 0,0,0,0 };         //!< �w�ʃo�[��`
            Rect _offSet{ 0,0,0,0 };          //!< ��`�`��̃I�t�Z�b�g
            Color _backColor{ 255,255,255 };  //!< �w�ʃo�[�̃J���[
            Color _frontColor{ 255,255,255 }; //!< �O�ʃo�[�̃J���[
      };
   }
}
