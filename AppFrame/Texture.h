#pragma once
/*****************************************************************//**
 * \file   Texture.h
 * \brief  �摜�f�[�^�N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <string_view>
#include <vector>
#include <tuple>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {
      /**
       * \class �摜�f�[�^�N���X
       * \brief DxLib::LoadDivGraph�֐��ɕK�v�ȉ摜�����R�s�[����
       */
      class Texture {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param textureName �t�@�C����
          * \param allNum �S�Ẳ摜����
          * \param xNum �摜�̉�������
          * \param yNum �摜�̉�������
          * \param xSize �������ꂽ�摜��̉��T�C�Y
          * \param ySize �������ꂽ�摜��̏c�T�C�Y
          */
         Texture(std::string_view textureName, int allNum, int xNum, int yNum, int xSize, int ySize) :
            _textureName(textureName),_allNum(allNum), _xNum(xNum), _yNum(yNum), _xSize(xSize), _ySize(ySize) {
         };
         /**
          * \brief �R���X�g���N�^
          */
         Texture() {};
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~Texture() = default;
         /**
          * \brief  �t�@�C�����̎擾
          * \return �摜�t�@�C����
          */
         inline std::string_view textureName() const { return _textureName; }
         /**
          * \brief �t�@�C�����̐ݒ�
          * \param textureName
          */
         inline void textureName(std::string_view textureName) { _textureName = textureName; }
         /**
          * \brief  DxLib::LoadDivGraph�ɕK�v�ȏ���Ԃ�
          * \return �摜�̑�������,�摜�̉�������,�摜�̏c������,�������ꂽ�摜��̉��T�C�Y,�������ꂽ�摜��̏c�T�C�Y
          */
         inline std::tuple<int, int, int, int, int> GetDivParams() const { return { _allNum, _xNum,_yNum, _xSize, _ySize }; }
         /**
          * \brief DxLib::LoadDivGraph�ɕK�v�ȏ��̐ݒ�
          * \param divParams �摜�̑�������,�摜�̉�������,�摜�̏c������,�������ꂽ�摜��̉��T�C�Y,�������ꂽ�摜��̏c�T�C�Y
          */
         inline void SetDivParams(std::tuple<int, int, int, int, int> divParams) {
            auto [allNum, xNum, yNum, xSize, ySize] = divParams; {
               _allNum = allNum; _xNum = xNum; _yNum = yNum; _xSize = xSize; _ySize = ySize;
            }
         }

      private:
         std::string _textureName{ "" };//!< �t�@�C����
         int _allNum{ 0 };              //!< �摜�̑�������
         int _xNum{ 0 };                //!< �摜�̉�������
         int _yNum{ 0 };                //!< �摜�̏c������
         int _xSize{ 0 };               //!< �������ꂽ�摜��̉��T�C�Y
         int _ySize{ 0 };               //!< �������ꂽ�摜��̏c�T�C�Y
      };
   }
}
