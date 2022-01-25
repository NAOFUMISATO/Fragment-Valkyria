
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
#include <fstream>
#include "GameBase.h"
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

      LoadResourceJson::LoadResourceJson(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      };

      void LoadResourceJson::LoadTextures(const std::filesystem::path jsonName) {
         // �摜�����i�[���Ă���json�t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("TextureJson");
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
         // �g�b�v���x���̔z��̃L�[���A��������w�肵���o��
         auto textureArray = value[jsonName.generic_string()];
         // �摜���i�[���Ă���t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto textureDirectory = _gameBase.pathServer().GetCurrentPath("Texture") / jsonName;
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
            _gameBase.resServer().LoadTexture(keyName, tex);
         }
      }

      void LoadResourceJson::LoadModels(const std::filesystem::path jsonName) {
         // ���f�������i�[���Ă���json�t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("ModelJson");
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
         // �g�b�v���x���̔z��̃L�[���A��������w�肵���o��
         auto modelArray = value[jsonName.generic_string()];
         // �g�b�v���x���̔z���S�ĉ񂵁A�i�[���Ă��郂�f������S�Ď��o��
         for (auto& modelData : modelArray) {
            const auto keyName = modelData["keyname"];   //!< �L�[(���f���t�H���_�ւ̃t�@�C���p�X�ƌ��p)
            const auto fileName = modelData["filename"]; //!< �t�@�C����
            // ���f�����i�[���Ă���t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
            auto modelDirectory = _gameBase.pathServer().GetCurrentPath("Model") / keyName;
            // �t�@�C���ւ̃p�X���`��
            const auto modelPath = (modelDirectory / fileName).generic_string();
            // ���o�����L�[�ƃ��f���ւ̃t�@�C���p�X��ResourceServer�ɓo�^����
            _gameBase.resServer().LoadModel(keyName, modelPath);
         }
      }

      void LoadResourceJson::LoadSounds(const std::filesystem::path jsonName) {
         // ���������i�[���Ă���json�t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("SoundJson");
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
         // �g�b�v���x���̔z��̃L�[���A��������w�肵���o��
         auto soundArray = value[jsonName.generic_string()];
         // �������i�[���Ă���t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto soundDirectory = _gameBase.pathServer().GetCurrentPath("Sound") / jsonName;
         // �g�b�v���x���̔z���S�ĉ񂵁A�i�[���Ă��鉹������S�Ď��o��
         for (auto& soundData : soundArray) {
            const auto keyName = soundData["keyname"];   // �L�[
            const auto fileName = soundData["filename"]; // �t�@�C����
            const auto isLoad = soundData["isload"];     // ���O�ǂݍ��݂̗L��
            const auto volume = soundData["volume"];     // ��������
            // �t�@�C���ւ̃p�X���`��
            const auto soundPath = (soundDirectory / fileName).generic_string();
            // ���o�����L�[�y�щ����ւ̃t�@�C���p�X�Ǝ��O�ǂݍ��݂̗L���Ə������ʂ�tuple�^��ResourceServer�ɓo�^����
            _gameBase.resServer().LoadSound(keyName, std::make_tuple(soundPath, isLoad, volume));
         }
      }

      void LoadResourceJson::LoadEffects(const std::filesystem::path jsonName) {
         // �G�t�F�N�g�����i�[���Ă���json�t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto jsonDirectory = _gameBase.pathServer().GetCurrentPath("EffectJson");
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
         // �g�b�v���x���̔z��̃L�[���A��������w�肵���o��
         auto effectArray = value[jsonName.generic_string()];
         // �G�t�F�N�g���i�[���Ă���t�H���_�ւ̃p�X���A�Q�[���{�̑��Œ�`�����p�X�T�[�o�[����擾����
         auto effectDirectory = _gameBase.pathServer().GetCurrentPath("Effect") / jsonName;
         // �g�b�v���x���̔z���S�ĉ񂵁A�i�[���Ă���G�t�F�N�g����S�Ď��o��
         for (auto& effectData : effectArray) {
            const auto keyName = effectData["keyname"];    // �L�[
            const auto fileName = effectData["filename"];  // �t�@�C����
            const auto scale = effectData["scale"];        // �����g�嗦
            // �t�@�C���ւ̃p�X���`��
            const auto effectPath = (effectDirectory / fileName).generic_string();
            // ���o�����L�[�y�уG�t�F�N�g�ւ̃t�@�C���p�X�Ə����g�嗦��pair�^��ResourceServer�ɓo�^����
            _gameBase.resServer().LoadEffect(keyName, std::make_pair(effectPath, scale));
         }
      }
   }
}