#pragma once
/*****************************************************************//**
 * \file   SoundData.h
 * \brief  ���f�[�^�N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
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
       * \class ���f�[�^�N���X
       * \brief ���f�[�^��o�^����
       */
      class SoundData {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param fileName �t�@�C����
          * \param initVolume ��������
          * \param is3Dsound 3D�T�E���h�ɂ��邩
          * \param radius �����������鋗��
          */
         SoundData(std::string_view fileName, int initVolume, bool is3Dsound, float radius) :
            _fileName{ fileName }, _volume{ initVolume }, _is3Dsound{ is3Dsound }, _radius{radius}{
         };
         /**
          * \brief ��̃R���X�g���N�^
          */
         SoundData() {};
         /**
          * \brief �t�@�C�����̎擾
          * \return �t�@�C����
          */
         inline std::string_view fileName() { return _fileName; }
         /**
          * \brief ���f�[�^�̎擾
          * \return ���ʁA3D�T�E���h���ǂ����A���̕������鋗����Tuple�^
          */
         inline std::tuple<int, bool, float> GetSoundParams() { return { _volume ,_is3Dsound,_radius }; }

      private:
         std::string _fileName{ "" };  //!< �t�@�C����
         int _volume{ 0 };             //!< ����
         bool _is3Dsound{ false };     //!< 3D�T�E���h���ǂ���
         float _radius{ 0.0f };        //!< ���̕������鋗��
      };
   }
}
