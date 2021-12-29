#pragma once
/*****************************************************************//**
 * \file   PathServer.h
 * \brief  �t�@�C���p�X�ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <utility>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �t�@�C���p�X�֌W
    */
   namespace Path {
      /**
       * \class �t�@�C���p�X�ꊇ�Ǘ��N���X
       * \brief �t�@�C���p�X�̓o�^�A�擾���s��
       */
      class PathServer {
      public:
         /**
          * \brief �C�ӂ̕�������L�[�ɂ��ăt�@�C���p�X���Ǘ�
          */
         using PathMap = std::unordered_map<std::string, std::filesystem::path>;
         /**
          * \brief �R���X�g���N�^
          */
         PathServer();
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~PathServer() = default;
         /**
          * \brief ����������
          */
         virtual void Init();
         /**
          * \brief �R���e�i�̉��
          */
         virtual void Clear();
         /**
          * \brief �t�@�C���p�X�̈ꊇ�o�^
          * \param pathMap �t�@�C���p�X�}�b�v
          */
         void RegistPath(const PathMap& pathMap);
         /**
          * \brief �t�@�C���p�X�̎擾
          * \param key �L�[�ƂȂ�C�ӂ̕�����
          * \return �t�@�C���p�X
          */
         std::filesystem::path GetPath(std::string_view key);

      private:
         std::unordered_map < std::string, std::filesystem::path > _paths;   //!< �t�@�C���p�X�R���e�i
      };
   }
}