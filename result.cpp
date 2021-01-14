//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "manager.h"
#include "scene.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "bg.h"
#include "number.h"
#include "ranking.h"
#include "sound.h"
#include "camera.h"
#include "sky.h"
#include "object.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_MAGNIFICATION 5						// �{��

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
int CResult::m_nKill = 0;						// �|������
int CResult::m_nSeconds = 0;					// �b��
int CResult::m_nMinutes = 0;					// ����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	CCamera *pCamera = CManager::GetCamera();		// �J�����̎擾

	int nTotal = 0;

	// ��̍쐬
	CSky::Create();

	// ���f�����̓ǂݍ���
	CObject::LoadModelTest("data/text/model.txt");

	// �\����𐶐�
	CObject *pObj = CObject::Create();

	if (pObj != NULL)
	{
		pObj->BindModel("data/model/Podium.x");
		pObj->SetPosition(D3DXVECTOR3(14013.69f, -3849.46f, -16564.68f));
		pObj->SetRotation(D3DXVECTOR3(0.0f, 1.10f, 0.0f));
		pObj->SetSize(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	}

	if (pCamera != NULL)
	{
		pCamera->SetStoker(false);
		pCamera->SetPosCamera(D3DXVECTOR3(13951.36f, -3800.70f, -16626.40f), D3DXVECTOR3(-0.12f, -1.88f, 0.0f));
	}

	CRanking::SetResultIndex(m_nKill * ((MAX_MAGNIFICATION - m_nMinutes) * 100));	// �����L���O�ɍ���̓��_�𑗂�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CResult::Uninit(void)
{
	m_nKill = 0;		// Kill����������
	m_nSeconds = 0;		// �b����������
	m_nMinutes = 0;		// ������������

	// �|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
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

				CFade::SetFade(CManager::MODE_TITLE, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// �R���g���[���[����������Ă���Ƃ�
		 //�Q�[���̑J��
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// �Q�[���p�b�h��A�{�_���������ꂽ�Ƃ�
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// �Q�[���p�b�h��START�{�^���������ꂽ�Ƃ�
			{
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�

				CFade::SetFade(CManager::MODE_TITLE, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CResult::LoadAsset(void)
{
	CBg::Load();
	CNumber::Load();
}

//=============================================================================
// �G�̓������̐ݒ�
//=============================================================================
void CResult::SetIdxKill(int nValue)
{
	m_nKill = nValue;
}

//=============================================================================
// �b���̐ݒ�
//=============================================================================
void CResult::SetSeconds(int nValue)
{
	m_nSeconds = nValue;
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void CResult::SetMinutes(int nValue)
{
	m_nMinutes = nValue;
}