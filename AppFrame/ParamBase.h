#pragma once
/*****************************************************************//**
 * \file   ParamBase.h
 * \brief  �e�l�̊Ǘ����s�����N���X
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include <unordered_map>
#include <utility>
#include <vector>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Vector4.h"
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameBase;
   }
   namespace Math {
      class Vector4;
   }
   /**
    * \brief �l�Ǘ��֌W
    */
   namespace Param {
      /**
       * \class �e�l�̊Ǘ����s�����N���X
       * \brief �e�l��json�t�@�C������ǂݍ��݁A�擾����
       */
      class ParamBase {
         public:
         /**
          * \brief �R���X�g���N�^
          * \param gameBase �Q�[���x�[�X�̎Q��
          */
         ParamBase(Game::GameBase& gameBase,std::string_view key);
         /**
          * \brief int�^�̒l��A�z�z�񂩂�擾����
          * \param paramName �l��
          * \return int�^�̒l
          */
         int GetIntParam(std::string_view paramName);
         /**
          * \brief float�^�̒l��A�z�z�񂩂�擾����
          * \param paramName �l��
          * \return float�^�̒l
          */
         float GetFloatParam(std::string_view paramName);
         /**
          * \brief double�^�̒l��A�z�z�񂩂�擾����
          * \param paramName �l��
          * \return double�^�̒l
          */
         double GetDoubleParam(std::string_view paramName);
         /**
          * \brief bool�^�̒l��A�z�z�񂩂�擾����
          * \param paramName �l��
          * \return bool�^�̒l
          */
         bool GetBoolParam(std::string_view paramName);
         /**
          * \brief Vector4�^�̒l��A�z�z�񂩂�擾����
          * \param paramName �l��
          * \return Vector4�^�̒l
          */
         Math::Vector4 GetVecParam(std::string_view paramName);

      protected:
         /**
          * \brief int�^�̒l��json�t�@�C������ǂݍ��݁A�A�z�z��ɓo�^����
          * \param paramNames �l��
          */
         void LoadIntParam(std::vector<std::string_view> paramNames);
         /**
          * \brief float�^�̒l��json�t�@�C������ǂݍ��݁A�A�z�z��ɓo�^����
          * \param paramNames �l��
          */
         void LoadFloatParam(std::vector<std::string_view> paramNames);
         /**
          * \brief double�^�̒l��json�t�@�C������ǂݍ��݁A�A�z�z��ɓo�^����
          * \param paramNames �l��
          */
         void LoadDoubleParam(std::vector<std::string_view> paramNames);
         /**
          * \brief bool�̒l��json�t�@�C������ǂݍ��݁A�A�z�z��ɓo�^����
          * \param paramNames �l��
          */
         void LoadBoolParam(std::vector<std::string_view> paramNames);
         /**
          * \brief Vector4�^�̒l��json�t�@�C������ǂݍ��݁A�A�z�z��ɓo�^����
          * \param paramNames �l��
          */
         void LoadVecParam(std::vector<std::string_view> paramNames);
         /**
          * \brief json�t�@�C�����J���A�R�s�[�����I�u�W�F�N�g��Ԃ�
          * \return nlohmann::json�^�̃I�u�W�F�N�g
          */
         nlohmann::json JsonSetUp();

         Game::GameBase& _gameBase;                               //!< �Q�[���x�[�X�̎Q��
         std::string _key;                                        //!< json�t�@�C����
         std::unordered_map<std::string, int> _intMap;            //!< int�^�̒l��o�^����A�z�z��
         std::unordered_map<std::string, float> _floatMap;        //!< float�^�̒l��o�^����A�z�z��
         std::unordered_map<std::string, double> _doubleMap;      //!< double�^�̒l��o�^����A�z�z��
         std::unordered_map<std::string, bool> _boolMap;          //!< bool�^�̒l��o�^����A�z�z��
         std::unordered_map<std::string, Math::Vector4> _vecMap;  //!< Vector4�^�̒l��o�^����A�z�z��
      };
   }
}