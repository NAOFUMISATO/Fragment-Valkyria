
/*****************************************************************//**
 * \file   LoadJson.h
 * \brief  �Q�[������Json�t�@�C������ǂݍ��ރN���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#pragma once
#include <DxLib.h>
#include <unordered_map>
#include <string_view>
#include <filesystem>
#include <nlohmann/json.hpp>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   //��d�C���N���[�h�h�~
   namespace Game {
      class GameBase;
   }
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
      class LoadJson {
         using Vector4 = Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameBase �Q�[���x�[�X�̎Q��
          */
         LoadJson(Game::GameBase& gameBase);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~LoadJson() = default;
         /**
          * \brief json�t�@�C������摜����ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonFileName json�t�@�C����
          */
         void LoadTextures(const std::filesystem::path jsonName);
         /**
          * \brief json�t�@�C�����烂�f������ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonFileName json�t�@�C����
          */
         void LoadModels(const std::filesystem::path jsonName);
         /**
          * \brief json�t�@�C�����特������ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonFileName json�t�@�C����
          */
         void LoadSounds(const std::filesystem::path jsonName);
         /**
          * \brief json�t�@�C������f�[�^���擾����
          * \param jsonName json�t�@�C����
          * \param paramName �f�[�^��
          * \return �f�[�^
          */
         nlohmann::json GetParam(const std::filesystem::path jsonName, const std::string_view paramName);
         Vector4 GetVecParam(const std::filesystem::path jsonName,const std::string_view vecName);

      private:
         Game::GameBase& _gameBase;   //!< �Q�[���x�[�X�̎Q��
      };
   }
}
