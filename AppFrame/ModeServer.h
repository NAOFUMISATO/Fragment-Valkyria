#pragma once
/*****************************************************************//**
 * \file   ModeServer.h
 * \brief  ���[�h�̈ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <memory>
#include <unordered_map>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   /**
    * \brief ���[�h�֌W
    */
   //��d�C���N���[�h�h�~
   namespace Input {
      class InputManager;
   }
   namespace Mode {
      /** ��d�C���N���[�h�h�~ */
      class ModeBase;
      /**
       * \class ���[�h�̈ꊇ�Ǘ��N���X
       * \brief ���[�h��o�^���A�ꊇ�Ǘ����s��
       */
      class ModeServer {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key ���[�h�̃C���X�^���X���֘A�t����C�ӂ̕�����
          * \param mode ���[�h�̃C���X�^���X
          */
         ModeServer(std::string_view key, std::shared_ptr<ModeBase> mode);
         /**
          * \brief ���[�h�̓o�^
          * \param key ���[�h�̃C���X�^���X�Ɋ֘A�t����C�ӂ̕�����
          * \param mode ���[�h�̃C���X�^���X
          */
         void Register(std::string_view key, std::shared_ptr<ModeBase> mode);

         /**
          * \brief ���[�h���X�g�̈�Ԍ��Ƀ��[�h�}��
          * \param key �o�^�ς݂̃��[�h�Ɋ֘A�t����������
          */
         void PushBack(std::string_view key);
         /**
          * \brief ���[�h���X�g�̈�Ԍ��̃��[�h���폜
          */
         void PopBack();
         /**
          * \brief ���[�h�̑J��
          * \param key �o�^�ς݂̃��[�h�Ɋ֘A�t����������
          * \param fadeType �t�F�[�h���Ԃ̎��
          */
         void GoToMode(std::string_view key, char fadeType= 'M' );
         /**
          * \brief ���[�h���X�g�̈�Ԍ��̐^���Ƀ��[�h�}��
          * \param key �o�^�ς݂̃��[�h�Ɋ֘A�t����������
          */
         void InsertBelowBack(std::string_view key);
         /**
          * \brief ���[�h�̃C���X�^���X��Ԃ�
          * \param key �o�^�ς݂̃��[�h�Ɋ֘A�t����������
          * \return ���[�h�̃C���X�^���X
          */
         std::shared_ptr<ModeBase> GetMode(std::string_view key);
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(Input::InputManager& input);
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Render() const;

      private:
         /**
          * \brief ���[�h���X�g�̈�Ԍ��̐^���Ƀt�F�[�h(�A�E�gor�C��)���[�h�}�����A�t�F�[�h���Ԃ�ݒ肷��
          * \param key �o�^�ς݂̃��[�h�Ɋ֘A�t����������
          * \param fadeType �t�F�[�h���Ԃ̎��
          */
         void FadeInsertBelowBack(std::string_view key, char fadeType);
         /**
          * \brief ���[�h���X�g�̈�Ԍ��Ƀ��[�h�}��
          * \param key �o�^�ς݂̃��[�h�Ɋ֘A�t����������
          * \param fadeType �t�F�[�h���Ԃ̎��
          */
         void FadePushBack(std::string_view key, char fadeType);

         std::unordered_map<std::string, std::shared_ptr<ModeBase>> _modeRegistry;  //!< ���[�h��o�^����A�z�z��
         std::list<std::shared_ptr<ModeBase>> _modeList;                            //!< ���[�h�̏������񂷑o�����z��
      };
   }
}