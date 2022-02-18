
/*****************************************************************//**
 * \file   SoundConponent.cpp
 * \brief  �����̈ꊇ�Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "SoundComponent.h"
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

      void SoundComponent::Play(std::string_view key) {
         Play(key, DX_PLAYTYPE_BACK);
      }
      void SoundComponent::PlayLoop(std::string_view key) {
         Play(key, DX_PLAYTYPE_LOOP);
      }

      void SoundComponent::ChangeVolume(std::string_view key, int changeVolume) {
         auto&& [filename, handle, volume] = _gameBase.resServer().GetSoundInfo(key);

         if (handle != -1) {
            // �ǂݍ��ݗL��
            ChangeVolumeSoundMem(changeVolume, handle);
         }
         else {
            // �ǂݍ��ݖ���
            SetVolumeMusic(changeVolume);
         }
      }

      void SoundComponent::Stop(std::string_view key) {
         auto&& [filename, handle, volume] = _gameBase.resServer().GetSoundInfo(key);
         if (handle != -1) {
            // �ǂݍ��ݗL��
            StopSoundMem(handle);
         }
         else {
            // �ǂݍ��ݖ���
            StopMusic();
         }
      }

      void SoundComponent::Play(std::string_view key, int playType) {
         auto&& [filename, handle, volume] = _gameBase.resServer().GetSoundInfo(key);

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
