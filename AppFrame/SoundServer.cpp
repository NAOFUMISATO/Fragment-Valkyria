
/*****************************************************************//**
 * \file   SoundServer.cpp
 * \brief  音源の一括管理
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
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 音源関係
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
            // 読み込み有り
            ChangeVolumeSoundMem(changeVolume, handle);
         }
         else {
            // 読み込み無し
            SetVolumeMusic(changeVolume);
         }
      }

      void SoundServer::Play(std::string_view key, int playType) {

         auto res = _gameBase.resServer();
         auto&& [filename, handle, volume] = res.GetSoundInfo(key);

         if (handle != -1) {
            // 読み込み有り
            PlaySoundMem(handle, playType, TRUE);
            ChangeVolumeSoundMem(volume, handle);
         }
         else {
            // 読み込み無し
            PlayMusic(filename.c_str(), playType);
            SetVolumeMusic(volume);
         }
      }
   }
}
