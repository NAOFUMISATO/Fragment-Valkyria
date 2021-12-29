
/*****************************************************************//**
 * \file   SoundServer.cpp
 * \brief  �����̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "SoundServer.h"
#include <tuple>
#include <DxLib.h>
#include "ResourceServer.h"
#include "GameBase.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �����֌W
    */
   namespace Sound {
      namespace {
         constexpr auto HandleNo = 1;
      }

      void SoundServer::Play(std::string_view key) {
         Play(key, DX_PLAYTYPE_BACK);
      }
      void SoundServer::PlayLoop(std::string_view key) {
         Play(key, DX_PLAYTYPE_LOOP);
      }

      void SoundServer::ChangeVolume(std::string_view key, int changeVolume) {
         auto soundinfo = _gameBase.resServer().GetSoundInfo(key);
         auto& handle = std::get<HandleNo>(soundinfo);

         if (handle != -1) {
            // �ǂݍ��ݗL��
            ChangeVolumeSoundMem(changeVolume, handle);
         }
         else {
            // �ǂݍ��ݖ���
            SetVolumeMusic(changeVolume);
         }
      }

      void SoundServer::Play(std::string_view key, int playType) {

         auto res = _gameBase.resServer();
         auto&& [filename, handle, volume] = res.GetSoundInfo(key);

         if (handle != -1) {
            // �ǂݍ��ݗL��
            PlaySoundMem(handle, playType, TRUE);
            ChangeVolumeSoundMem(volume, handle);
         }
         else {
            // �ǂݍ��ݖ���
            PlayMusic(filename.c_str(), playType);
            SetVolumeMusic(volume);
         }
      }
   }
}
