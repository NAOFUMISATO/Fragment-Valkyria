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
   //��d�C���N���[�h�h�~
   namespace Input {
      class InputManager;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /** ��d�C���N���[�h�h�~ */
      class ModeBaseRoot;
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
         ModeServer(std::string_view key, std::shared_ptr<ModeBaseRoot> mode);
         /**
          * \brief ���[�h�̓o�^
          * \param key ���[�h�̃C���X�^���X�Ɋ֘A�t����C�ӂ̕�����
          * \param mode ���[�h�̃C���X�^���X
          */
         void Register(std::string_view key, std::shared_ptr<ModeBaseRoot> mode);
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
         std::shared_ptr<ModeBaseRoot> GetMode(std::string_view key);
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
         /**
          * \brief �Q�[���̃t���[�����擾
          * \return �Q�[���̃t���[��
          */
         unsigned int frameCount() { return _frameCount; }
         std::shared_ptr<ModeBaseRoot > GetNowMode();
      private:
         /**
          * \brief ���[�h���X�g�̈�Ԍ��̐^���Ƀt�F�[�h(�A�E�gor�C��)���[�h�}�����A�t�F�[�h���Ԃ�ݒ肷��
          * \param fadeType �t�F�[�h���Ԃ̎��
          */
         void FadeInsertBelowBack(char fadeType);
         /**
          * \brief ���[�h���X�g�̈�Ԍ��Ƀ��[�h�}��
          * \param fadeType �t�F�[�h���Ԃ̎��
          */
         void FadePushBack(char fadeType);

         unsigned int _frameCount{ 0 };                                                 //!< �Q�[���̃t���[�����J�E���g
         std::unordered_map<std::string, std::shared_ptr<ModeBaseRoot>> _modeRegistry;  //!< ���[�h��o�^����A�z�z��
         std::list<std::shared_ptr<ModeBaseRoot>> _modeList;                            //!< ���[�h�̏������񂷑o�����z��
#ifdef _DEBUG
         unsigned int _fpsCount{ 0 };                                                   //!< �Q�[�����������x�J�E���g
#endif
      };
   }
}