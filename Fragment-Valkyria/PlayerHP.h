#pragma once
/*****************************************************************//**
 * \file   PlayerHP.h
 * \brief  �v���C���[HP�̕`��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �v���C���[�֌W
    */
   namespace Player {
      /**
       * \class �v���C���[HP�̕`��N���X
       * \brief �v���C���[HP�̃o�[�̑��������Ǘ�����
       */
      class PlayerHP :public Sprite::SpriteBase {
         using Color = AppFrame::Data::Color;
         using Rect = AppFrame::Data::Rect;
         using Triangle = AppFrame::Data::Triangle;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         PlayerHP(Game::GameMain& gameMain);
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
          * \brief �X�v���C�g���:UI�̎擾
          * \return �X�v���C�g���:UI
          */
         virtual SpriteType GetSprType() const { return SpriteType::UI; }

      private:
         /**
          * \brief HP�o�[�U���̏���
          */
         void BarShake(unsigned int count);
         double _hp{ 100.0 };                          //!< �v���C���[HP
         double _hpRange{ 100.0 };                     //!< �v���C���[�ő�HP
         double _rate{ 0.0 };                          //!< �w�ʃo�[�̑������x
         double _oldFrontHP{ 0.0 };                    //!< 1�t���[���O�̑O�ʃo�[�̃v���C���[HP
         double _oldBackHP{ 0.0 };                     //!< 1�t���[���O�̔w�ʃo�[�̃v���C���[HP
         bool _rateReset{ true };                      //!< �w�ʃo�[�̑������x�����Z�b�g���邩
         bool _shake{ false };                         //!< HP�o�[��U�������邩
         unsigned int _shakeCnt{0};                    //!< HP�o�[�U���t���[���̕ۑ�
         Color _frontColor{ 255,255,255 };             //!< �O�ʃo�[�̃J���[
         Rect _frontBar{ 0,0,0,0 };                    //!< �O�ʃo�[��`
         Triangle _frontTriangle{ {0,0},{0,0},{0,0} }; //!< �O�ʃo�[�O�p�`
         Color _backColor{ 255,255,255 };              //!< �w�ʃo�[�̃J���[
         Rect _backBar{ 0,0,0,0 };                     //!< �w�ʃo�[��`
         Triangle _backTriangle{ {0,0},{0,0},{0,0} };  //!< �w�ʃo�[�O�p�`
         Rect _offSet{ 0,0,0,0 };                      //!< ��`�`��̃I�t�Z�b�g
      };
   }
}
