
/*****************************************************************//**
 * \file   LoadResourceJson.h
 * \brief  �Q�[������Json�t�@�C������ǂݍ��ރN���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <utility>
#include <vector>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   //��d�C���N���[�h�h�~
   namespace Math {
      class Vector4;
   }
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {
      //��d�C���N���[�h�h�~
      class Texture;
      /**
       * \class �Q�[������Json�t�@�C������ǂݍ��ރN���X
       * \brief ���\�[�X�T�[�o�[�ɓo�^����f�ޏ��y�ъe�f�[�^��json�t�@�C������ǂݍ���
       */
      class LoadResourceJson {
         using Vector4 = Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         LoadResourceJson();
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~LoadResourceJson() = default;
         /**
          * \brief json�t�@�C������摜����ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonName json�t�@�C����
          */
         void LoadTextures(const std::filesystem::path jsonName);
         /**
          * \brief json�t�@�C�����烂�f������ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonName json�t�@�C����
          */
         void LoadModels(const std::filesystem::path jsonName);
         /**
          * \brief json�t�@�C�����特������ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonName json�t�@�C����
          */
         void LoadSounds(const std::filesystem::path jsonName);
         /**
          * \brief json�t�@�C������G�t�F�N�g����ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonName json�t�@�C����
          */
         void LoadEffects(const std::filesystem::path jsonName);

      private:
         /**
          * \brief json�t�@�C�����J���A�R�s�[�����I�u�W�F�N�g��Ԃ�
          * \param pathName �Q�[���{�̑��Œ�`�����t�@�C���p�X�̃L�[��
          * \param jsonName json�t�@�C����
          * \return nlohmann::json�^�̃I�u�W�F�N�g
          */
         nlohmann::json JsonSetUp(const std::string_view pathName, const std::filesystem::path jsonName);
      };
   }
}
