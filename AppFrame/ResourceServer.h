#pragma once
/*****************************************************************//**
 * \file   ResourceServer.h
 * \brief  ���\�[�X�̈ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <utility>
#include <tuple>
#include "Texture.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   //��d�C���N���[�h�h�~
   namespace Game {
      class GameBase;
   }
   /**
    * @brief ���\�[�X�֌W
    */
   namespace Resource {
      /**
       * \class ���\�[�X�̈ꊇ�Ǘ��N���X
       * \brief ���\�[�X��ǂݍ��݁A�ꊇ�Ǘ����s��
       */
      class ResourceServer {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameBase �Q�[���x�[�X�N���X�̎Q��
          */
         ResourceServer(Game::GameBase& gameBase);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         virtual ~ResourceServer() = default;
         /**
          * \brief ���\�[�X������
          */
         virtual void Init();
         /**
          * \brief ���\�[�X���
          */
         virtual void Release();

         /*----------2D�֌W----------*/

         /**
          * \brief �摜�R���e�i�̉��
          */
         virtual void ClearTextures();
         /**
          * \brief �摜�̓ǂݍ���
          * \param key �L�[�ƂȂ�C�ӂ̕�����
          * \param texture DxLib::LoadDivGraph�p�N���X
          */
         virtual void LoadTexture(std::string_view key, Texture& texture);
         /**
          * \brief �摜�n���h���̎擾
          * \param key �摜���Ɋ֘A�t�����C�ӂ̕�����
          * \param no �n���h���ԍ�
          * \return �摜�n���h����Ԃ��A�����ꍇ��-1��Ԃ�
          */
         virtual int GetTexture(const std::string_view key, int no = 0);
         /**
          * \brief �A�j���[�V�����摜�n���h���Q�̎擾
          * \param key �摜���Ɋ֘A�t�����C�ӂ̕�����
          * \return �摜�n���h���̓��I�z��
          */
         virtual std::vector<int> GetTextures(std::string_view key);
         /**
          * \brief �摜���̎擾
          * \param key �摜���Ɋ֘A�t�����C�ӂ̕�����
          * \return �摜���
          */
         virtual Texture GetTextureInfo(std::string_view key);


         /*----------3D�֌W----------*/

         /**
          * \brief ���f���R���e�i�̉��
          */
         virtual void ClearModels();
         /**
          * \brief �����������f�������폜���ăN���A����
          */
         virtual void DeleteDuplicateModels();
         /**
          * \brief ���f���̓ǂݍ���
          * \param key �L�[�ƂȂ�C�ӂ̕�����
          * \param filename ���f���t�@�C����
          * \return ���f���n���h��
          */
         virtual int LoadModel(std::string_view key, const std::string_view filename);
         /**
          * \brief ���f���n���h��
          * \param key �L�[�ƂȂ�C�ӂ̕�����
          * \param no �n���h���ԍ�
          * \return ���f���n���h����Ԃ��A�����ꍇ��-1��Ԃ�
          */
         virtual std::pair<int, int> GetModel(std::string_view key, int no = 0);
         /**
          * \brief �w�胂�f���̎w��A�j���[�V�����ԍ��̎擾
          * \param key ���f���̃L�[�ƂȂ�C�ӂ̕�����
          * \param animName ���f�����̃A�j���[�V������
          * \return �w��̃A�j���[�V�����̃A�j���ԍ�
          */
         virtual int GetModelAnimIndex(std::string_view key, std::string_view animName);

         /*----------�����֌W----------*/


         /**
          * \brief �T�E���h�R���e�i�̉��
          */
         virtual void ClearSounds();
         /**
          * \brief ���t�@�C���̓ǂݍ���
          * \param key �L�[�ƂȂ�C�ӂ̕�����
          * \param filename_isLoad �t�@�C�����Ǝ��O�ǂݍ��ݗL���̃y�A
          */
         virtual void LoadSound(std::string_view key, std::tuple<std::string, bool, int> soundInfo);
         /**
          * \brief ���t�@�C�����̎擾
          * \param key �L�[�ƂȂ�C�ӂ̕�����
          * \return ���t�@�C����, �n���h��
          */
         virtual std::tuple<std::string, int, int> GetSoundInfo(std::string_view key);


         /*----------�G�t�F�N�g�֌W----------*/


         /**
          * \brief �G�t�F�N�g�R���e�i�̉��
          */
         virtual void ClearEffects();
         /**
          * \brief �G�t�F�N�g��A�z�z��ɓo�^����
          * \param key �L�[�ƂȂ�C�ӂ̕�����
          * \param effectInfo �t�@�C�����Ə����g�嗦�̃y�A
          */
         virtual void LoadEffect(std::string_view key, std::pair<std::string, double> effectInfo);
         /**
          * \brief �A�z�z��ɓo�^�����G�t�F�N�g�̃n���h��
          * \param key �n���h���Ɋ֘A�t�����C�ӂ̕�����
          * \return �G�t�F�N�g�n���h��
          */
         virtual int GetEffectHandle(std::string_view key);

      private:
         Game::GameBase& _gameBase;                                                        //!< �Q�[���x�[�X�̎Q��
         std::unordered_map<std::string, std::pair<Texture, std::vector<int>>> _textures;  //!< �C�ӂ̕�������L�[�ɂ���DivGraph�Ɖ摜�n���h���̃y�A���Ǘ�
         std::unordered_map<std::string, std::pair<std::vector<int>,
            std::unordered_map<std::string, int>>> _models;                                //!< �C�ӂ̕�������L�[�ɂ��ăn���h���ƃA�j���}�b�v�̃y�A���Ǘ�
         std::unordered_map<std::string, std::tuple<std::string, int, int>> _sounds;       //!< �C�ӂ̕�������L�[�ɂ��ĉ��t�@�C�����ƃn���h���Ə������ʂ��Ǘ�
         std::unordered_map<std::string, int> _effects;                                    //!< �C�ӂ̕�������L�[�ɂ��ăG�t�F�N�g�n���h�����Ǘ�
      };
   }
}