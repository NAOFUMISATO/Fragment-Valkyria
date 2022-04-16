
/*****************************************************************//**
 * \file   ClearScore.cpp
 * \brief  �N���A�X�R�A��\������
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ClearScore.h"
#include "GameMain.h"
#include "ParamModeClear.h"

namespace {
   constexpr auto OneSecFrame = 60;
}

using namespace FragmentValkyria::Clear;

ClearScore::ClearScore(Game::GameMain& gameMain) :SpriteBase{ gameMain } {
   _param = std::make_unique < Param::ParamModeClear >(_gameMain,"clear");
}

void ClearScore::Init() {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   _position = _param->GetVecParam("score_pos");
   auto handles = GetResServer().GetTextures("ClearScore");
   auto timer = static_cast<int>(_gameMain.ingameTimer());
   const auto CScoreSec = _IntParam("c_scoresec");
   const auto BScoreSec = _IntParam("b_scoresec");
   const auto AScoreSec = _IntParam("a_scoresec");
   const auto SScoreSec = _IntParam("s_scoresec");
   if (timer >= OneSecFrame * CScoreSec) {
      _grHandle = handles[3];
   }
   else if (timer >= OneSecFrame * BScoreSec && OneSecFrame * CScoreSec > timer) {
      _grHandle = handles[2];
   }
   else if (timer >= OneSecFrame * AScoreSec && OneSecFrame * BScoreSec > timer) {
      _grHandle = handles[1];
   }
   else if (timer >= OneSecFrame * SScoreSec && OneSecFrame * AScoreSec > timer) {
      _grHandle = handles[0];
   }
   else if (OneSecFrame * SScoreSec > timer) {
      _grHandle = handles[4];
   }
}

void ClearScore::Draw() {
   SpriteBase::Draw();
}
