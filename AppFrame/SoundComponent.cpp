
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
         // �T�E���h�Đ��ɕK�v�ȃf�[�^�̎擾
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         auto&& [soundData, handle] = gameInstance.resServer().GetSoundInfo(key);
         // ���O�ǂݍ��݂ɂ��n���h�����쐬���Ă��邩
         if (handle != -1) {
            ChangeVolumeSoundMem(changeVolume, handle);  // ���ʂ̕ύX
         }
         else {
            SetVolumeMusic(changeVolume);                // ���ʂ̕ύX
         }
      }

      void SoundComponent::Stop(std::string_view key) {
         // �T�E���h�Đ��ɕK�v�ȃf�[�^�̎擾
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         auto&& [soundData, handle] = gameInstance.resServer().GetSoundInfo(key);
         // ���O�ǂݍ��݂ɂ��n���h�����쐬���Ă��邩
         if (handle != -1) {
            StopSoundMem(handle);   // �����̒�~
         }
         else {
            StopMusic();            // �����̒�~
         }
      }

      void SoundComponent::Play(std::string_view key, int playType, Math::Vector4 pos) {
         // �T�E���h�Đ��ɕK�v�ȃf�[�^�̎擾
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         auto&& [soundData, handle] = gameInstance.resServer().GetSoundInfo(key);
         auto fileName = soundData.fileName();
         auto [volume, is3Dsound, radius] = soundData.GetSoundParams();
         // ���O�ǂݍ��݂ɂ��n���h�����쐬���Ă��邩
         if (handle != -1) {
            // 3D�T�E���h�œǂݍ���ł��邩
            if (is3Dsound) {
               Set3DPositionSoundMem(Math::ToDX(pos), handle);  // 3D�T�E���h���Đ�����ʒu��ݒ�
               Set3DRadiusSoundMem(radius, handle);             // 3D�T�E���h���Đ��������̕������鋗����ݒ�
            }
            PlaySoundMem(handle, playType, TRUE);               // �������Đ�
            ChangeVolumeSoundMem(volume, handle);               // ���ʂ̕ύX
         }
         else {
            PlayMusic(fileName.data(), playType);               // �������Đ�
            SetVolumeMusic(volume);                             // ���ʂ̕ύX
         }
      }
   }
}
