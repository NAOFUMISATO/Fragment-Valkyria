
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
#include <utility>
#include <vector>
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
         /**
          * \brief json�t�@�C������f�[�^���擾����
          * \param jsonName json�t�@�C����
          * \param paramName �f�[�^��
          * \return �f�[�^
          */
         nlohmann::json GetParam(const std::filesystem::path jsonName, const std::string_view paramName);
         /**
          * \brief json�t�@�C������Vecotor4�f�[�^���擾����
          * \param jsonName json�t�@�C����
          * \param vecName �f�[�^��
          * \return �f�[�^
          */
         Vector4 GetVecParam(const std::filesystem::path jsonName,const std::string_view vecName);
         /**
          * \brief json�t�@�C������e�f�[�^��A�z�z��ɂ��ĕԂ�
          * \param jsonName json�t�@�C����
          * \param paramNames �f�[�^���Q
          * \return �f�[�^��nlohmann::json�^�œo�^�����A�z�z��
          */
         static std::unordered_map<std::string, nlohmann::json> GetParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> paramNames);
         /**
          * \brief json�t�@�C������Vector4�f�[�^��A�z�z��ɂ��ĕԂ�
          * \param jsonName json�t�@�C����
          * \param vecParamNames Vector4�f�[�^���Q
          * \return Vector4�f�[�^��o�^�����A�z�z��
          */
         static std::unordered_map<std::string, Vector4> GetVecParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> vecParamNames);

      private:
         Game::GameBase& _gameBase;   //!< �Q�[���x�[�X�̎Q��
         static const std::filesystem::path _paramJsonCurrentPath;   //!< �e�f�[�^���i�[���Ă���json�t�@�C���̃p�X
      };
   }
}
