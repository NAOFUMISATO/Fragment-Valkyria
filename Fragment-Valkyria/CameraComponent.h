#pragma once
/*****************************************************************//**
 * \file   CameraComponent.h
 * \brief  �J�����Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include <tuple>
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief �J�����֌W
    */
   namespace Camera {
      /**
       * \class �J�����Ǘ��N���X
       * \brief �C���Q�[���Ŏg�p����J�������Ǘ�����
       */
      class CameraComponent {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         CameraComponent();
         /**
          * \brief ����������
          */
         void Init();
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Draw();

         /**
          * \brief �J�����̈ʒu��ݒ肷��
          * \param position �J�����̈ʒu
          */
         void SetPosition(Vector4 position) { _position = position; }
         /**
          * \brief �J�����̒����_��ݒ肷��
          * \param target �����_�ɂȂ���W
          * \param forward �J�����̌���
          */
         void SetTarget(Vector4 target, Vector4 forward = { 0,0,0 }) {
            _target = target, _forwardOfTarget = forward;
         }
         /**
          * \brief �����_�����̃x�N�g���̎擾
          * \return �����_�����̒P�ʃx�N�g��
          */
         Vector4 GetForward() const {
            auto vec = _target - _position;
            vec.Normalized();
            return vec;
         }

      private:
         /**
          * \brief �J�����̃r���[�s��̐ݒ�
          * \param cameraPosition �J�����̈ʒu
          * \param cameraTarget �J�����̒����_
          * \param cameraUp �J�����̏����
          * \return �J�����̃r���[�s��
          */
         Matrix44 GetCameraViewMatrix(Vector4& cameraPosition, Vector4& cameraTarget, Vector4& cameraUp);
         /**
          * \brief �J�����̓��e�s��̐ݒ�
          * \param cameraNear �J�����̕`����E(��O)
          * \param cameraFar �J�����̕`����E(��)
          * \param fov ����p
          * \return �J�����̓��e�s��
          */
         Matrix44 GetCameraProjectionMatrix(double cameraNear, double cameraFar, double fov);
         Vector4 _position{ 0, 0, 0 };        //!< �ʒu
         Vector4 _target{ 0, 0, 0 };          //!< �����_
         Vector4 _up{ 0, 1 ,0 };              //!< �����
         Vector4 _forwardOfTarget{ 0, 0, 0 }; //!< �����_�I�u�W�F�N�g�̌���
         double _targetDistance{ 500 };       //!< �����_�I�u�W�F�N�g�Ƃ�Z���W�̋���
         double _vertDistance{ 120 };         //!< �����_�I�u�W�F�N�g�Ƃ�Y���W�̋���
         std::tuple<double, double, double> _nearFarFov{   //!< �J�����̕`����E(��O,��)�y�ю���p��Tuple�^(�����ϊ��Ɏg�p)
            std::make_tuple(10.0,10000.0,AppFrame::Math::Utility::DegreeToRadian(50.0)) };
      };
   }
}