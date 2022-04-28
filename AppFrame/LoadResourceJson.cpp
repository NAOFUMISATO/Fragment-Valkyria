
/*****************************************************************//**
 * \file   LoadResourceJson.cpp
 * \brief  �f�ޏ���Json�t�@�C������ǂݍ���
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "LoadResourceJson.h"
#include <tuple>
#include <string_view>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
#include "CurrentPathServer.h"
#include "ResourceServer.h"
#include "Vector4.h"
#include "Texture.h"


 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {

      LoadResourceJson::LoadResourceJson() {
      };

      void LoadResourceJson::LoadTextures(const std::filesystem::path jsonName) {
         // json�t�@�C�����J���A�I�u�W�F�N�g�����o��
         auto textureArray = JsonSetUp("TextureJson", jsonName);
         // �摜���i�[���Ă���t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto& pathInstance = Path::CurrentPathServer::GetInstance();
         auto textureDirectory = pathInstance.GetCurrentPath("Texture") / jsonName;
         // �g�b�v���x���̔z���S�ĉ񂵁A�i�[���Ă���摜����S�Ď��o��
         for (auto& textureData : textureArray) {
            const auto keyName = textureData["keyname"];    // �L�[
            const auto fileName = textureData["filename"];  // �t�@�C����
            const auto allNum = textureData["allnum"];      // �摜��������
            const auto xNum = textureData["xnum"];          // �摜��������
            const auto yNum = textureData["ynum"];          // �摜�c������
            const auto xSize = textureData["xsize"];        // ���摜�T�C�Y
            const auto ySize = textureData["ysize"];        // �c�摜�T�C�Y
            // �t�@�C���ւ̃p�X���`��
            const auto texturePath = (textureDirectory / fileName).generic_string();
            // Texture�N���X�𐶐����A�摜����o�^
            Texture tex = Texture(texturePath, allNum, xNum, yNum, xSize, ySize);
            // ���o�����L�[�Ɖ摜����o�^����Texture�N���X��ResourceServer�ɓo�^����
            auto& resServerInstance = ResourceServer::GetInstance();
            resServerInstance.LoadTexture(keyName, tex);
         }
      }

      void LoadResourceJson::LoadModels(const std::filesystem::path jsonName) {
         // json�t�@�C�����J���A�I�u�W�F�N�g�����o��
         auto modelArray = JsonSetUp("ModelJson", jsonName);
         // �g�b�v���x���̔z���S�ĉ񂵁A�i�[���Ă��郂�f������S�Ď��o��
         for (auto& modelData : modelArray) {
            const auto keyName = modelData["keyname"];   //!< �L�[(���f���t�H���_�ւ̃t�@�C���p�X�ƌ��p)
            const auto fileName = modelData["filename"]; //!< �t�@�C����
            // ���f�����i�[���Ă���t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
            auto& pathInstance = Path::CurrentPathServer::GetInstance();
            auto modelDirectory = pathInstance.GetCurrentPath("Model") / keyName;
            // �t�@�C���ւ̃p�X���`��
            const auto modelPath = (modelDirectory / fileName).generic_string();
            // ���o�����L�[�ƃ��f���ւ̃t�@�C���p�X��ResourceServer�ɓo�^����
            auto& resServerInstance = ResourceServer::GetInstance();
            resServerInstance.LoadModel(keyName, modelPath);
         }
      }

      void LoadResourceJson::LoadSounds(const std::filesystem::path jsonName) {
         // json�t�@�C�����J���A�I�u�W�F�N�g�����o��
         auto soundArray = JsonSetUp("SoundJson", jsonName);
         // �������i�[���Ă���t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto& pathInstance = Path::CurrentPathServer::GetInstance();
         auto soundDirectory = pathInstance.GetCurrentPath("Sound") / jsonName;
         // �g�b�v���x���̔z���S�ĉ񂵁A�i�[���Ă��鉹������S�Ď��o��
         for (auto& soundParam : soundArray) {
            const auto keyName = soundParam["keyname"];    // �L�[
            const auto fileName = soundParam["filename"];  // �t�@�C����
            const auto isLoad = soundParam["isload"];      // ���O�ǂݍ��݂̗L��
            const auto volume = soundParam["volume"];      // ��������
            const auto is3Dsound = soundParam["is3Dsound"];// 3D�T�E���h�ݒ�ɂ��邩
            const auto radius = soundParam["radius"];      // 3D�T�E���h�̕������鋗��
            // �t�@�C���ւ̃p�X���`��
            const auto soundPath = (soundDirectory / fileName).generic_string();
            // �����f�[�^���쐬
            auto soundData = SoundData(soundPath, volume, is3Dsound, radius);
            // ���o�����L�[�y�щ����f�[�^�Ǝ��O�ǂݍ��݂̗L���̃y�A��ResourceServer�ɓo�^����
            auto& resServerInstance = ResourceServer::GetInstance();
            resServerInstance.LoadSound(keyName, std::make_pair(soundData, isLoad));
         }
      }

      void LoadResourceJson::LoadEffects(const std::filesystem::path jsonName) {
         // json�t�@�C�����J���A�I�u�W�F�N�g�����o��
         auto effectArray = JsonSetUp("EffectJson", jsonName);
         // �g�b�v���x���̔z���S�ĉ񂵁A�i�[���Ă���G�t�F�N�g����S�Ď��o��
         for (auto& effectData : effectArray) {
            const auto keyName = effectData["keyname"];    // �L�[
            const auto fileName = effectData["filename"];  // �t�@�C����
            const auto scale = effectData["scale"];        // �����g�嗦
            const auto speed = effectData["speed"];        // �����Đ����x
            // �G�t�F�N�g���i�[���Ă���t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
            auto& pathInstance = Path::CurrentPathServer::GetInstance();
            auto effectDirectory = pathInstance.GetCurrentPath("Effect") / keyName;
            // �t�@�C���ւ̃p�X���`��
            const auto effectPath = (effectDirectory / fileName).generic_string();
            // ���o�����L�[�y�уG�t�F�N�g�ւ̃t�@�C���p�X�Ə����g�嗦�Ə����Đ����x��tuple�^��ResourceServer�ɓo�^����
            auto& resServerInstance = ResourceServer::GetInstance();
            resServerInstance.LoadEffect(keyName, std::make_tuple(effectPath, scale, speed));
         }
      }

      nlohmann::json LoadResourceJson::JsonSetUp(const std::string_view pathName, const std::filesystem::path jsonName) {
         // �l�����i�[���Ă���json�t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto& pathInstance = Path::CurrentPathServer::GetInstance();
         auto jsonDirectory = pathInstance.GetCurrentPath(pathName);
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         // �w�肵��json�t�@�C����ǂݎ���p�ŊJ��
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         // �w�肵���t�@�C�����J���̂Ɏ��s�����Ȃ�logic_error�����o���A�f�o�b�K�ɏo�͂���
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "�t�@�C�����J���܂���ł����B----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         // �t�@�C���̒��g�����o��
         reading >> value;
         // �t�@�C�������
         reading.close();
         // �g�b�v���x���̔z��̃L�[����������w�肵�Ajson�I�u�W�F�N�g�Ƃ��ĕԂ�
         return value[jsonName.generic_string()];
      }
   }
}