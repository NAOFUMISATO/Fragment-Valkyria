#pragma once
/*****************************************************************//**
 * \file   Singleton.h
 * \brief  �V���O���g���N���X�e���v���[�g
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <memory>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   /**
    * \brief �e���v���[�g�֌W
    */
   namespace Temp {
      template<class T>
      /**
       * \class �V���O���g���N���X�e���v���[�g
       * \brief �v���W�F�N�g��1�̂ݑ��݂���N���X�͂�����p������
       */
      class Singleton {
      protected:
         /**
          * \brief �R���X�g���N�^(�O���ł̃C���X�^���X�����͋֎~)
          */
         Singleton() {}
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~Singleton() {}

      public:
         /**
          * \brief �C���X�^���X�̐���
          * \return �h���N���X�̃C���X�^���X
          */
         static inline T& GetInstance()
         {
            static T instance;
            return instance;
         }

      private:
         /**
          * \brief ������Z�q�֎~
          */
         void operator=(const Singleton& obj) {}
         /**
          * \brief �R�s�[�R���X�g���N�^�֎~
          */
         Singleton(const Singleton& obj) {}
      };
   }
}