#pragma once
/*****************************************************************//**
 * \file   StageData.h
 * \brief  �X�e�[�W���N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <tuple>
#include <string>
#include "Vector4.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �X�e�[�W�֌W
    */
   namespace Stage {
      /**
       * \class �X�e�[�W���N���X
       * \brief �e�X�e�[�W�p�[�c����o�^����
       */
      class StageData {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param fileName �t�@�C����
          * \param position �ʒu
          * \param rotation ��]�p
          * \param scale �g�嗦
          */
         StageData(std::string_view fileName, Vector4 position, Vector4 rotation, Vector4 scale) :
            _fileName(fileName), _position(position), _rotation(rotation), _scale(scale) {
         };
         /**
          * \brief �R���X�g���N�^
          */
         StageData() {};
         /**
          * \brief �t�@�C�����̎擾
          * \return �t�@�C����
          */
         std::string_view fileName() const { return _fileName; }
         /**
          * \brief �X�e�[�W���̎擾
          * \return �ʒu�A��]�p�A�g�嗦��Tuple�^
          */
         std::tuple<Vector4, Vector4, Vector4> GetStageParams() const { return { _position,_rotation,_scale }; }

      private:
         std::string _fileName;    //!< �t�@�C����
         Vector4 _position;        //!< �ʒu
         Vector4 _rotation;        //!< ��]�p
         Vector4 _scale;           //!< �g�嗦
      };
   }
}
