#pragma once
/*****************************************************************//**
 * \file   SimpleTextureComponent.h
 * \brief  �摜�y�уr���{�[�h�ȈՕ`��֐��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <vector>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameBase;
   }
   namespace Math {
      class Vector4;
   }
   /**
    * \brief �摜�`��֌W
    */
   namespace Texture {
      /**
       * \class �摜�y�уr���{�[�h�ȈՕ`��֐��N���X
       * \brief �摜�̕`��y�уA�j���[�V�������ȈՓI�ɍs��
       */
      class SimpleTextureComponent {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameBase �Q�[���x�[�X�̎Q��
          */
         SimpleTextureComponent(Game::GameBase& gameBase);
         /**
          * \brief �摜�̕`����ȈՓI�ɍs��
          * \param x �摜����X���W
          * \param y �摜����Y���W
          * \param scale �g�嗦
          * \param angle �p�x(�ʓx�@)
          * \param handle �摜�n���h��
          */
         void DrawTexture(int x, int y, double scale, double angle, int handle);
         /**
          * \brief �摜�̃A�j���[�V�����`����ȈՓI�ɍs��
          * \param x �摜����X���W
          * \param y �摜����Y���W
          * \param scale �g�嗦
          * \param angle �p�x(�ʓx�@)
          * \param handles �摜�n���h���̓��I�z��
          * \param animeSpeed �A�j���[�V�����X�s�[�h
          */
         void DrawTexture(int x, int y, double scale, double angle, std::vector<int> handles, int animeSpeed);
         /**
          * \brief �摜�̕`����ڍאݒ���s���A�`�悷��
          * \param x �摜����X���W
          * \param y �摜����Y���W
          * \param cx �摜�̉�]���S�ʒuX���W(�摜���オ0)
          * \param cy �摜�̉�]���S�ʒuY���W(�摜���オ0)
          * \param scale �g�嗦
          * \param angle �p�x(�ʓx�@)
          * \param handle �摜�n���h��
          * \param turnFrag ���]����
          */
         void TransDrawTexture(int x, int y, int cx, int cy, double scale, double angle, int handle, bool turnFrag);
         /**
          * \brief �摜�̃A�j���[�V�����`����ڍאݒ���s���A�`�悷��
          * \param x �摜����X���W
          * \param y �摜����Y���W
          * \param cx �摜�̉�]���S�ʒuX���W(�摜���オ0)
          * \param cy �摜�̉�]���S�ʒuY���W(�摜���オ0)
          * \param scale �g�嗦
          * \param angle �p�x(�ʓx�@)
          * \param handles �摜�n���h���̓��I�z��
          * \param animeSpeed �A�j���[�V�����X�s�[�h
          * \param turnFrag ���]����
          */
         void TransDrawTexture(int x, int y, int cx, int cy, double scale, double angle, std::vector<int> handles, int animeSpeed, bool turnFrag);
         /**
          * \brief �摜�̃r���{�[�h�`����ȈՓI�ɍs��
          * \param pos �`����W(�����)
          * \param size �g�嗦
          * \param angle �p�x(�ʓx�@)
          * \param handle �摜�n���h��
          */
         void DrawBillBoard(Math::Vector4 pos, double size, double angle, int handle);
         /**
          * \brief �摜�̃r���{�[�h�A�j���[�V�����`����ȈՓI�ɍs��
          * \param pos �`����W(�����)
          * \param size �g�嗦
          * \param angle �p�x(�ʓx�@)
          * \param handles �摜�n���h���̓��I�z��
          * \param animeSpeed �A�j���[�V�����X�s�[�h
          */
         void DrawBillBoard(Math::Vector4 pos, double size, double angle, std::vector<int> handles, int animeSpeed);
         /**
          * \brief �摜�̃r���{�[�h�`����ڍאݒ���s���A�`�悷��
          * \param pos �`����W(�����)
          * \param cx ��ʒuX���W
          * \param cy ��ʒuY���W
          * \param size �g�嗦
          * \param angle �p�x(�ʓx�@)
          * \param handle �摜�n���h��
          */
         void TransDrawBillBoard(Math::Vector4 pos, double cx, double cy, double size, double angle, int handle);
         /**
          * \brief �摜�̃r���{�[�h�A�j���[�V�������ڍאݒ���s���A�`�悷��
          * \param pos �`����W(�����)
          * \param cx ��ʒuX���W
          * \param cy ��ʒuY���W
          * \param size �g�嗦
          * \param angle �p�x(�ʓx�@)
          * \param handles �摜�n���h���̓��I�z��
          * \param animeSpeed �A�j���[�V�����X�s�[�h
          */
         void TransDrawBillBoard(Math::Vector4 pos, double cx, double cy, double size, double angle, std::vector<int> handles, int animeSpeed);

      private:
         Game::GameBase& _gameBase;   //!< �Q�[���x�[�X�̎Q��
      };
   }
}