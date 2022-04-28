#pragma once
/*****************************************************************//**
 * \file   ObjectFactory.h
 * \brief  �I�u�W�F�N�g�����̈ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "CreatorBase.h"
#include "SpawnRecord.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class Game;
   }
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief �I�u�W�F�N�g�����֌W
    */
   namespace Create {
       class SpawnRecord;
      /**
       * \class �I�u�W�F�N�g�����̈ꊇ�Ǘ��N���X
       * \brief �e�I�u�W�F�N�g�̐����Ǘ��N���X��o�^���Ďg�p����
       */
      class ObjectFactory {
         using Vector4 = AppFrame::Math::Vector4;
         using SpawnTable = std::vector<SpawnRecord>;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ObjectFactory();
         /**
          * \brief �I�u�W�F�N�g�̐����Ǘ��N���X�̓o�^
          * \param type �����Ǘ��N���X���֘A�t����ۂɓo�^����C�ӂ̕�����
          * \param creator �o�^���鐶���Ǘ��N���X�̃C���X�^���X
          * \return �o�^�ł����Ȃ�true,�łȂ����false��Ԃ�
          */
         bool Register(std::string_view type, std::unique_ptr<CreatorBase> creator);
         /**
          * \brief �e�I�u�W�F�N�g�̐����Ǘ��N���X�̐������\�b�h���Ăяo��
          * \param type �����Ǘ��N���X���֘A�t�����C�ӂ̕�����
          * \return ���������C���X�^���X�̃|�C���^
          */
         std::unique_ptr<Object::ObjectBase> Create(std::string_view type);
         /**
          * \brief �R���e�i�̑S�v�f�폜
          */
         void Clear();
         /**
          * \brief �X�|�[���e�[�u�����Z�b�g����
          * \param key �Z�b�g����X�|�[���e�[�u����
          */
         void SetSpawnTable(std::string_view key);
         /**
          * \brief �X�|�[���e�[�u���̍X�V���s��
          */
         void UpdateSpawn();
         /**
          * \brief �X�|�[���e�[�u������json�t�@�C������ǂݍ���
          * \param jsonName json�t�@�C����
          * \param tableNames �X�|�[���e�[�u����
          */
         void LoadSpawnTables(const std::filesystem::path jsonName, const std::vector<std::string> tableNames);
         /**
          * \brief �X�|�[���e�[�u������A�z�z��ɓo�^����
          * \param key �X�|�[���e�[�u����
          * \param spawnTable �X�|�[���e�[�u��
          */
         void LoadSpawnTable(std::string_view key, SpawnTable& spawnTable);
         /**
          * \brief �X�|�[���e�[�u����A�z�z�񂩂�擾����
          * \param key �擾����X�|�[���e�[�u����
          * \return �X�|�[���e�[�u��
          */
         SpawnTable GetSpawnTable(std::string_view key);

      private:
         std::unordered_map<std::string, std::unique_ptr<CreatorBase>> _creatorMap; //!< �I�u�W�F�N�g�����Ǘ��N���X��o�^����A�z�z��
         std::unordered_map<std::string, SpawnTable> _spawnTableMap;                //!< �X�|�[���e�[�u�����̘A�z�z��
         SpawnTable _spawnTable{ SpawnRecord() };                                   //!< �X�|�[���e�[�u�����N���X
         unsigned int _progress{ 0 };                                               //!< �i��
         unsigned int _spawnProgress{ 0 };                                          //!< �X�|�[���̐i��
      };
   }
}
