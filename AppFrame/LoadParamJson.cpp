#include "LoadParamJson.h"
#include <string_view>
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif
#include <fstream>
#include "Vector4.h"
namespace AppFrame {

   namespace Resource {

      std::unordered_map<std::string, nlohmann::json> LoadParamJson::GetParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> paramNames) {
         // �e�f�[�^���i�[���Ă���json�t�H���_�ւ̃p�X���擾����
         auto jsonDirectory = GetCurrentPath();
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
         auto paramArray = value[jsonName.generic_string()];
         // �e�f�[�^���i�[����A�z�z���nlohmann::json�^�Œ�`����
         std::unordered_map<std::string, nlohmann::json> paramMap;
         // �����Ɏw�肵��������̓��I�z���S�ĉ�
         for (auto i = 0; i < paramNames.size(); i++) {
            // �g�b�v���x���̔z���S�ĉ񂵁A�����̕�����Ɉ�v����i�[���Ă���e�f�[�^��S�Ď��o��
            for (auto& paramData : paramArray) {
               const auto param = paramData[paramNames[i].data()];
               // �e�f�[�^�����o�����ۂ̕�������L�[�A���o����nlohmann::json�^�f�[�^��A�z�z��ɓo�^����
               paramMap.emplace(paramNames[i], param);
            }
         }
         // �e�f�[�^��o�^�����A�z�z���Ԃ�
         return paramMap;
      }

      std::unordered_map<std::string, Math::Vector4> LoadParamJson::GetVecParamMap(const std::filesystem::path jsonName, const std::vector<std::string_view> vecParamNames) {
         // �eVector4�f�[�^���i�[���Ă���json�t�H���_�ւ̃p�X���擾����
         auto jsonDirectory = GetCurrentPath();
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
         auto vecArray = value[jsonName.generic_string() + "_vec"];
         // �eVector4�f�[�^���i�[����A�z�z����`����
         std::unordered_map<std::string, Math::Vector4> vecParamMap;
         // �����Ɏw�肵��������̓��I�z���S�ĉ�
         for (auto i = 0; i < vecParamNames.size(); i++) {
            // �g�b�v���x���̔z���S�ĉ񂵁A�����̕�����Ɉ�v����i�[���Ă���eVector4�f�[�^��S�Ď��o��
            for (auto& vecData : vecArray) {
               auto vecParamArray = vecData[vecParamNames[i].data()];
               // ���o����Vector4�f�[�^�̊e�v�f�����o��
               for (auto& vecParam : vecParamArray) {
                  const auto x = vecParam["x"];   // x����
                  const auto y = vecParam["y"];   // y����
                  const auto z = vecParam["z"];   // z����
                  // Vector4�N���X�𐶐����AVector4�f�[�^��o�^
                  auto vec = Math::Vector4(x, y, z);
                  // Vector4�f�[�^�����o�����ۂ̕�������L�[�A���o����Vector4�f�[�^��A�z�z��ɓo�^����
                  vecParamMap.emplace(vecParamNames[i], vec);
               }
            }
         }
         // �eVector4�f�[�^��o�^�����A�z�z���Ԃ�
         return vecParamMap;
      }
   }
}