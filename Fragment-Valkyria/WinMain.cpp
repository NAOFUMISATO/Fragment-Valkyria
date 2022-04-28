
/*****************************************************************//**
 * \file   WinMain.cpp
 * \brief  �A�v���P�[�V�����̃G���g���[�|�C���g
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
   auto& gameInstance = FragmentValkyria::Game::Game::GetInstance();
   auto success = gameInstance.Initialize(hInstance);
   // ���C�����[�v
   if (success) {
      gameInstance.Run();
   }
   gameInstance.ShutDown();
   return 0;
}