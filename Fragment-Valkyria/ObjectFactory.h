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
      class GameMain;
   }
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief �I�u�W�F�N�g�����֌W
    */
   namespace Create {
       class SpawnRecord;
       using Vector4 = AppFrame::Math::Vector4;
       //using SpawnRecord = std::tuple<unsigned int/*�i����*/, std::string/*�Ăт����N���G�C�^�[�̕�����*/, Vector4/*�ʒu*/, Vector4/*��]*/>;
       //using SpawnTable = std::vector<SpawnRecord>;
      /**
       * \class �I�u�W�F�N�g�����̈ꊇ�Ǘ��N���X
       * \brief �e�I�u�W�F�N�g�̐����Ǘ��N���X��o�^���Ďg�p����
       */
      class ObjectFactory {
         using SpawnTable = std::vector<SpawnRecord>;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ObjectFactory(Game::GameMain& gameMain);
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

         void SetSpawnTable(std::string_view key);

         void UpdateSpawn();

         void LoadSpawnTables(const std::filesystem::path jsonName, const std::vector<std::string_view> tableNames);

         void LoadSpawnTable(std::string_view key, SpawnTable& spawnTables);

         SpawnTable GetSpawnTable(std::string_view key);

      private:
         Game::GameMain& _gameMain;   //!< �Q�[���{�̃N���X�̎Q��
         std::unordered_map<std::string, std::unique_ptr<CreatorBase>> _creatorMap;   //!< �I�u�W�F�N�g�����Ǘ��N���X��o�^����A�z�z��
         std::unordered_map<std::string, SpawnTable> _spawnTableMap;
         SpawnTable _spawnTable;
         unsigned int _progress{ 0 };     //!< �i��
         unsigned int _spawnProgress{ 0 };//!< �X�|�[��������
      };
   }
}
