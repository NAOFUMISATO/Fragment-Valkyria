#pragma once
/*****************************************************************//**
 * \file   LoadStageFromJson.h
 * \brief  �X�e�[�W���̊Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include <filesystem>
#include <utility>
#include <unordered_map>
#include "StageData.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria{
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   /**
    * �X�e�[�W�֌W
    */
   namespace Stage {
      /**
       * \class �X�e�[�W���̊Ǘ��N���X
       * \brief �X�e�[�W����Json�ɓǂݍ��݁A�擾����
       */
      class LoadStageFromJson {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���̎Q��
          */
         LoadStageFromJson(Game::GameMain& gameMain);
         /**
          * \brief �f�X�g���N�^
          */
         ~LoadStageFromJson()=default;
         /**
          * \brief �X�e�[�W���̘A�z�z��
          */
         void ClearStageModels();
         /**
          * \brief �X�e�[�W����A�z�z��ɓo�^����
          * \param key �A�z�z��ɓo�^����L�[
          * \param stageData �X�e�[�W���
          */
         void LoadStageModel(std::string_view key,StageData& stageData);
         /**
          * \brief �X�e�[�W����Json�ɓǂݍ���
          * \param jsonName Json�t�@�C����
          */
         void LoadStageModels(const std::filesystem::path jsonName);
         /**
          * \brief �X�e�[�W���̎擾
          * \param key �e�X�e�[�W�Ɋ֘A�t�����C�ӂ̕�����
          * \return �X�e�[�W�p�[�c�̃n���h���ƃX�e�[�W���̃y�A�^�̓��I�z��
          */
         std::vector<std::pair<int ,StageData>> GetStageModels(std::string_view key);

      private:
         Game::GameMain& _gameMain;                                   //!< �Q�[���̎Q��
         std::unordered_map<std::string, 
            std::vector<std::pair<int,StageData>>> _stageModelsMap;   //!< �e�X�e�[�W���̘A�z�z��
      };
   }
}
