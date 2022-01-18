#pragma once
/*****************************************************************//**
 * \file   LoadParamJson.h
 * \brief  json�t�@�C������f�[�^��ǂݍ��݁A�擾����N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "Singleton.h"
#include <nlohmann/json.hpp>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   // ��d�C���N���[�h�h�~
   namespace Math {
      class Vector4;
   }
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {
      /**
       * \class json�t�@�C������f�[�^��ǂݍ��݁A�擾����N���X
       * \brief json����f�[�^���擾���A�߂�l�Ƀf�[�^�̘A�z�z���Ԃ�
       */
      class LoadParamJson :public Temp::Singleton<LoadParamJson> {
      private:
         /**
          * \brief �R���X�g���N�^(�O���ŃC���X�^���X�����֎~)
          */
         LoadParamJson() {};
         /**
          * \brief �f�X�g���N�^
          */
         ~LoadParamJson()override {};

      public:
         // �V���O���g���N���X�e���v���[�g�ł̃C���X�^���X�����͋���
         friend class Singleton<LoadParamJson>;
         /**
          * \brief json�t�@�C������e�f�[�^��A�z�z��ɂ��ĕԂ�
          * \param jsonName json�t�@�C����
          * \param paramNames �f�[�^���Q
          * \return �f�[�^��nlohmann::json�^�œo�^�����A�z�z��
          */
         static std::unordered_map<std::string, nlohmann::json> GetParamMap(
            const std::filesystem::path jsonName, const std::vector<std::string_view> paramNames);
         /**
          * \brief json�t�@�C������Vector4�f�[�^��A�z�z��ɂ��ĕԂ�
          * \param jsonName json�t�@�C����
          * \param vecParamNames Vector4�f�[�^���Q
          * \return Vector4�f�[�^��o�^�����A�z�z��
          */
         static std::unordered_map<std::string, Math::Vector4> GetVecParamMap(
            const std::filesystem::path jsonName, const std::vector<std::string_view> vecParamNames);

      private:
         /**
          * \brief json�̃J�����g�p�X��Ԃ�
          * \return json�̃J�����g�p�X
          */
         static std::filesystem::path GetCurrentPath() {
            auto currentPath = "resource/json/param";
            return currentPath;
         }
      };
   }
}
