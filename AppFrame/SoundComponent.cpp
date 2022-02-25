
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
#include "DxUtility.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �����֌W
    */
   namespace Sound {

      void SoundComponent::Play(std::string_view key, Math::Vector4 pos) {
         Play(key, DX_PLAYTYPE_BACK,pos);
      }
      void SoundComponent::PlayLoop(std::string_view key, Math::Vector4 pos) {
         Play(key, DX_PLAYTYPE_LOOP, pos);
      }

      void SoundComponent::ChangeVolume(std::string_view key, int changeVolume) {
         auto&& [soundData, handle] = _gameBase.resServer().GetSoundInfo(key);

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
         auto&& [soundData, handle] = _gameBase.resServer().GetSoundInfo(key);
         if (handle != -1) {
            // �ǂݍ��ݗL��
            StopSoundMem(handle);
         }
         else {
            // �ǂݍ��ݖ���
            StopMusic();
         }
      }

      void SoundComponent::Play(std::string_view key, int playType, Math::Vector4 pos) {
         auto&& [soundData, handle] = _gameBase.resServer().GetSoundInfo(key);
         auto fileName = soundData.fileName();
         auto [volume, is3Dsound, radius] = soundData.GetSoundParams();
         if (handle != -1) {
            // �ǂݍ��ݗL��
            if (is3Dsound) {
               // 3D�T�E���h�Ȃ�΍Đ�����ʒu��ݒ�
               Set3DPositionSoundMem(Math::ToDX(pos), handle);
            }
            PlaySoundMem(handle, playType, TRUE);
            ChangeVolumeSoundMem(volume, handle);
         }
         else {
            // �ǂݍ��ݖ���
            PlayMusic(fileName.data(), playType);
            SetVolumeMusic(volume);
         }
      }
   }
}
