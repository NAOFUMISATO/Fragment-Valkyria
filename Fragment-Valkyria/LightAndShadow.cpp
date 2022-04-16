
/*****************************************************************//**
 * \file   LightAndShadow.cpp
 * \brief  �����y�ї����e�̏������s��
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "LightAndShadow.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
#ifdef _DEBUG
   constexpr auto MatDifRed = 0.0f;           //!< �}�e���A���̊g�U���ˌ�(��)
   constexpr auto MatDifGreen = 0.0f;         //!< �}�e���A���̊g�U���ˌ�(��)
   constexpr auto MatDifBlue = 0.0f;          //!< �}�e���A���̊g�U���ˌ�(��)
   constexpr auto MatDifAlpha = 1.0f;         //!< �}�e���A���̊g�U���ˌ�(�����x)
   constexpr auto MatSpeRed = 0.0f;           //!< �}�e���A���̋��ʔ��ˌ�(��)
   constexpr auto MatSpeGreen = 0.0f;         //!< �}�e���A���̋��ʔ��ˌ�(��)
   constexpr auto MatSpeBlue = 0.0f;          //!< �}�e���A���̋��ʔ��ˌ�(��)
   constexpr auto MatSpeAlpha = 0.0f;         //!< �}�e���A���̋��ʔ��ˌ�(�����x)
   constexpr auto MatAmbRed = 0.0f;           //!< �}�e���A���̊���(��)
   constexpr auto MatAmbGreen = 0.0f;         //!< �}�e���A���̊���(��)
   constexpr auto MatAmbBlue = 0.0f;          //!< �}�e���A���̊���(��)
   constexpr auto MatAmbAlpha = 0.0f;         //!< �}�e���A���̊���(�����x)
   constexpr auto MatEmiRed = 0.0f;           //!< �}�e���A���̕��ˌ�(��)
   constexpr auto MatEmiGreen = 0.0f;         //!< �}�e���A���̕��ˌ�(��)
   constexpr auto MatEmiBlue = 0.5f;          //!< �}�e���A���̕��ˌ�(��)
   constexpr auto MatEmiAlpha = 0.0f;         //!< �}�e���A���̕��ˌ�(�����x)
   constexpr auto MatPower = 20.0f;           //!< �}�e���A���̋��ʔ��ˋ��x
   constexpr auto LightSphereDiffY = 100.0;   //!< �������̂̌��������Y����
   constexpr auto LightSphereRadius = 80.0f;  //!< �������̔��a
   constexpr auto LightSphereDiv = 32;        //!< �������̕�����
   constexpr auto LightSphereRed = 0;         //!< �������̐F(��)
   constexpr auto LightSphereGreen = 128;     //!< �������̐F(��)
   constexpr auto LightSphereBlue = 200;      //!< �������̐F(��)
#endif
}

using namespace FragmentValkyria::Lighting;

LightAndShadow::LightAndShadow(Game::GameMain& gameMain) :_gameMain{gameMain} {
   _param = std::make_unique<Param::ParamLightShadow>(_gameMain, "lightshadow");
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   /**
    * \brief float�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _FloatParam = [&](std::string paramName) {
      return _param->GetFloatParam(paramName);
   };
   /**
    * \brief bool�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _BoolParam = [&](std::string paramName) {
      return _param->GetBoolParam(paramName);
   };
   /**
    * \brief Vector4�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _VecParam = [&](std::string paramName) {
      return _param->GetVecParam(paramName);
   };
   namespace AppMath = AppFrame::Math;
#ifdef _DEBUG
   // �}�e���A���̃��C�e�B���O���̐ݒ�
   MATERIALPARAM material;
   material.Diffuse = GetColorF(MatDifRed, MatDifGreen, MatDifBlue, MatDifAlpha);
   material.Specular = GetColorF(MatSpeRed, MatSpeGreen, MatSpeBlue, MatSpeAlpha);
   material.Ambient = GetColorF(MatAmbRed, MatAmbGreen, MatAmbBlue, MatAmbAlpha);
   material.Emissive = GetColorF(MatEmiRed, MatEmiGreen, MatEmiBlue, MatEmiAlpha);
   material.Power = MatPower;
   SetMaterialParam(material);
#endif
   // �t�H�O��L���ɂ���
   SetFogEnable(_BoolParam("fog_enuble"));
   // �t�H�O�̐F�̐ݒ�
   SetFogColor(_IntParam("fog_red"), _IntParam("fog_green"), _IntParam("fog_blue"));
   // �t�H�O�̎n�_�ƏI�_�̐ݒ�
   SetFogStartEnd(_FloatParam("fog_start"), _FloatParam("fog_end"));
   // �W��������L���ɂ��邩
   SetLightEnable(_BoolParam("alllight_enuble"));
   // �S�Ẵ��f���ɓK�����������̐ݒ�
   SetGlobalAmbientLight(GetColorF(_FloatParam("global_ambred"), _FloatParam("global_ambgreen"), 
      _FloatParam("global_ambblue"), _FloatParam("global_ambalpha")));
   // �W�������̊g�U���ˌ���ݒ�
   SetLightDifColor(GetColorF(_FloatParam("alllight_difred"), _FloatParam("alllight_difgreen"),
      _FloatParam("alllight_difblue"), _FloatParam("alllight_difalpha")));
   // �W�������̋��ʔ��ˌ���ݒ�
   SetLightSpcColor(GetColorF(_FloatParam("alllight_spcred"), _FloatParam("alllight_spcgreen"),
      _FloatParam("alllight_spcblue"), _FloatParam("alllight_spcalpha")));
   // �W�������̊�����ݒ�
   SetLightAmbColor(GetColorF(_FloatParam("alllight_ambred"), _FloatParam("alllight_ambgreen"),
      _FloatParam("alllight_ambblue"), _FloatParam("alllight_ambalpha")));
   // �����ʒu�̏�����
   _lightPosition = _VecParam("fixedlight_pos");
   // �����̈ʒu�A�e���͈͋y�ы�����������ݒ肵�A�|�C���g�����𐶐��A���C�g�n���h���ɏ���ۑ�����
   _lightHandle = CreatePointLightHandle(AppMath::ToDX(_lightPosition),
      _FloatParam("fixedlight_area"), _FloatParam("fixedlight_atten_first"),
      _FloatParam("fixedlight_atten_second"), _FloatParam("fixedlight_atten_third"));
   // �����̊g�U���ˌ���ݒ�
   SetLightDifColorHandle(_lightHandle, GetColorF(_FloatParam("fixedlight_difred"), 
      _FloatParam("fixedlight_difgreen"), _FloatParam("fixedlight_difblue"), _FloatParam("fixedlight_difalpha")));
   // �����̋��ʔ��ˌ���ݒ�
   SetLightSpcColorHandle(_lightHandle, GetColorF(_FloatParam("fixedlight_spcred"),
      _FloatParam("fixedlight_spcgreen"), _FloatParam("fixedlight_spcblue"), _FloatParam("fixedlight_spcalpha")));
   // �����̊�����ݒ�
   SetLightAmbColorHandle(_lightHandle, GetColorF(_FloatParam("fixedlight_ambred"),
      _FloatParam("fixedlight_ambgreen"), _FloatParam("fixedlight_ambblue"), _FloatParam("fixedlight_ambalpha")));
   // �V���h�E�}�b�v�̉𑜓x��ݒ肵�V���h�E�}�b�v�𐶐��A�V���h�E�}�b�v�n���h���ɕۑ�����
   const auto ShadowResolution = _IntParam("shadow_resolution");
   _shadowHandle = MakeShadowMap(ShadowResolution, ShadowResolution);
   auto shadowMaxArea = _VecParam("shadow_maxarea");
   auto shadowMinArea = _VecParam("shadow_minarea");
   // �V���h�E�}�b�v�̉e���͈͂�ݒ�
   SetShadowMapDrawArea(_shadowHandle, AppMath::ToDX(shadowMinArea), AppMath::ToDX(shadowMaxArea));
}

void LightAndShadow::Update() {
   namespace AppMath = AppFrame::Math;
   // �v���C���[�̓��ʒu��ObjectServer����擾
   auto playerHeadPos= _gameMain.objServer().GetVecData("PlayerHeadPos");
   // �v���C���[�����W����Œ�������W�ւ̃x�N�g�������ߐ��K�����A�V���h�E�}�b�v�̌����Ƃ���
   auto shadowDirection = (playerHeadPos - _lightPosition).Normalize();
   // �V���h�E�}�b�v�̌������X�V
   SetShadowMapLightDirection(_shadowHandle, AppMath::ToDX(shadowDirection));
}

void LightAndShadow::Render() {
   // �V���h�E�}�b�v�̕`����s��
   ShadowMap_DrawSetup(_shadowHandle);
   // �V���h�E�}�b�v�`��p��ObjectServer�̕`�����
   _gameMain.objServer().Render();
   // �V���h�E�}�b�v�̕`����I������
   ShadowMap_DrawEnd();
   // �g�p����V���h�E�}�b�v�n���h���̐ݒ�
   SetUseShadowMap(0, _shadowHandle);

#ifdef _DEBUG
   //�������̂̃f�o�b�O�`��
   namespace AppMath = AppFrame::Math;
   using Utility = AppFrame::Math::Utility;
   auto spherePos = _lightPosition;
   spherePos.SetY(_lightPosition.GetY() + LightSphereDiffY);
   DrawSphere3D(AppMath::ToDX(spherePos), LightSphereRadius, LightSphereDiv,
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue),
      Utility::GetColorCode(LightSphereRed, LightSphereGreen, LightSphereBlue), TRUE);
#endif
}

void LightAndShadow::SetDifColor(float red, float green, float blue) {
   SetLightDifColorHandle(_lightHandle, GetColorF(red, green, blue, _param->GetFloatParam("fixedlight_difalpha")));
}

void LightAndShadow::SetAmbColor(float red, float green, float blue) {
   SetLightAmbColorHandle(_lightHandle, GetColorF(red, green, blue, _param->GetFloatParam("fixedlight_ambalpha")));
}

