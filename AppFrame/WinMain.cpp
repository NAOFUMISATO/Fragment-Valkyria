
/*****************************************************************//**
 * \file   WinMain.cpp
 * \brief  �A�v���P�[�V�����̃G���g���[�|�C���g
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "GameBase.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

   auto gameBase = AppFrame::Game::GameBase::gameInstance();
   auto success = gameBase->Initialize(hInstance);
   // ���C�����[�v
   if (success) {
      gameBase->Run();
   }
   gameBase->ShutDown();
   return 0;
}
