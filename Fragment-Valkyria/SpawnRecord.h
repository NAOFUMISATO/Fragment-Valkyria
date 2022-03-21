#pragma once
/*****************************************************************//**
 * \file   SpawnRecord.h
 * \brief  �������̏��N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include <string_view>
#include "Vector4.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �����֌W
    */
   namespace Create {
      /**
       * \class �������̏��N���X
       * \brief �������̏���o�^����
       */
      class SpawnRecord {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         SpawnRecord() {};
         /**
          * \brief �R���X�g���N�^
          * \param prograss �i��
          * \param type ��������I�u�W�F�N�g�̎��
          * \param position �ʒu
          * \param rotation ��]�p
          */
         SpawnRecord(unsigned int prograss, std::string type, Vector4 position, Vector4 rotation) :
            _progress{ prograss }, _type{ type }, _position{ position }, _rotation{ rotation }{
         }
         /**
          * \brief �������̏����擾����
          * \return �������̏��
          */
         std::tuple<unsigned int, std::string, Vector4, Vector4> GetSpawnParam() const { 
            return { _progress ,_type,_position,_rotation }; 
         }

      private:
         unsigned int _progress{ 0 };  //!< �i��
         std::string _type{ "" };      //!< ��������I�u�W�F�N�g�̎�ʕ�����
         Vector4 _position{ 0,0,0 };   //!< �ʒu
         Vector4 _rotation{ 0,0,0 };   //!< ��]�p
      };
   }
}
