//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "title.h"
#include "bg.h"
#include "inputController.h"
#include "meshField.h"
#include "object.h"
#include "camera.h"
#include "sky.h"
#include "sound.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	//CCamera *pCamera = CManager::GetCamera();		// �J�����̎擾

	//if (pCamera != NULL)
	//{// �J���������݂��Ă����Ƃ�
	//	pCamera->SetPosCamera(D3DXVECTOR3(7099.40f, 187.26f, 7523.8f), D3DXVECTOR3(0.1f, -2.33f, 0.0f));		// �ʒu�̐ݒ�
	//}

	//m_pTitleLogo = CTitlelogo::Create();		// �^�C�g�����S�̐���
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		pBack[nCnt] = CScene2D::Create(CScene::PRIORITY_UI);
	}
		if (pBack[0] != NULL)
		{
			pBack[0]->BindTexture("data/tex/title.png");
			pBack[0]->SetPosition(D3DXVECTOR3(300, 300, 0.0f));
			pBack[0]->SetSize(D3DXVECTOR3(600, 330, 0.0f));
			pBack[0]->SetTransform();
		}
		if (pBack[1] != NULL)
		{
			pBack[1]->BindTexture("data/tex/Press.png");
			pBack[1]->SetPosition(D3DXVECTOR3(1050, 600, 0.0f));
			pBack[1]->SetSize(D3DXVECTOR3(100, 45, 0.0f));
			pBack[1]->SetTransform();
		}
		if (pBack[2] != NULL)
		{
			pBack[2]->BindTexture("data/tex/PressLR.png");
			pBack[2]->SetPosition(D3DXVECTOR3(1050, 670, 0.0f));
			pBack[2]->SetSize(D3DXVECTOR3(200, 45, 0.0f));
			pBack[2]->SetTransform();
		}
		if (pBack[3] != NULL)
		{
			pBack[3]->BindTexture("data/tex/TeamName.png");
			pBack[3]->SetPosition(D3DXVECTOR3(190, 670, 0.0f));
			pBack[3]->SetSize(D3DXVECTOR3(250, 45, 0.0f));
			pBack[3]->SetTransform();
		}

	//// ��̍쐬
	CSky::Create();

	//// �e��A�Z�b�g�̐������ݒu
	//CMeshField::LoadRand("data/stage/rand.csv", false);				// �����̓Ǎ�
	//CObject::LoadModel("data/stage/object.csv");					// ���f�����̓Ǎ�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CTitle::Uninit(void)
{
	//�|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		if (pInputKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�

				CFade::SetFade(CManager::MODE_CHARACTER_SELECT, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// �R���g���[���[����������Ă���Ƃ�
			//�Q�[���̑J��
			if (pInputController->GetControllerPress(0, JOYPADKEY_LEFT_SHOULDER) &&			// �Q�[���p�b�h��L�{�_���������ꂽ�Ƃ�
				pInputController->GetControllerPress(0, JOYPADKEY_RIGHT_SHOULDER))			// �Q�[���p�b�h��R�{�^���������ꂽ�Ƃ�
			{
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�

				CFade::SetFade(CManager::MODE_CHARACTER_SELECT, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CTitle::LoadAsset(void)
{

}