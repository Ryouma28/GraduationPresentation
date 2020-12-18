//=============================================================================
//
// �L�����I������ [characterSelect.cpp]
// Author : Noriyuki Hanzawa
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "characterSelect.h"
#include "bg.h"
#include "inputController.h"
#include "object.h"
#include "camera.h"
#include "sky.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
int CCharacterSelect::m_nCarType = 0;			// �Ԃ̃^�C�v

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCharacterSelect::CCharacterSelect()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCharacterSelect::~CCharacterSelect()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharacterSelect::Init(void)
{
	m_nCarType = 0;
	for (int nCnt = 0; nCnt < MAX_SELECT_UI; nCnt++)
	{
		pBack[nCnt] = CScene2D::Create(CScene::PRIORITY_UI);
	}
	if (pBack[0] != NULL)//Select�̃��S
	{
		pBack[0]->BindTexture("data/tex/Select.png");
		pBack[0]->SetPosition(D3DXVECTOR3(970, 640, 0.0f));
		pBack[0]->SetSize(D3DXVECTOR3(130, 130, 0.0f));
		pBack[0]->SetTransform();
	}
	if (pBack[1] != NULL)//Decide�̃��S
	{
		pBack[1]->BindTexture("data/tex/Decide.png");
		pBack[1]->SetPosition(D3DXVECTOR3(1150, 640, 0.0f));
		pBack[1]->SetSize(D3DXVECTOR3(130, 130, 0.0f));
		pBack[1]->SetTransform();
	}
	if (pBack[2] != NULL)//�ԂP
	{
		pBack[2]->BindTexture("data/tex/car01.jpg");
		pBack[2]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH/2 - 240, 520, 0.0f));
		pBack[2]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[2]->SetTransform();
	}
	if (pBack[3] != NULL)//�ԂQ
	{
		pBack[3]->BindTexture("data/tex/car02.jpg");
		pBack[3]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - 80 , 520, 0.0f));
		pBack[3]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[3]->SetTransform();
	}
	if (pBack[4] != NULL)//�ԂR
	{
		pBack[4]->BindTexture("data/tex/car03.jpg");
		pBack[4]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 + 80 , 520, 0.0f));
		pBack[4]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[4]->SetTransform();
	}
	if (pBack[5] != NULL)//�ԂS
	{
		pBack[5]->BindTexture("data/tex/car04.jpg");
		pBack[5]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 + 240, 520, 0.0f));
		pBack[5]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[5]->SetTransform();
	}

	if (pBack[6] != NULL)//CarSelect�̃��S
	{
		pBack[6]->BindTexture("data/tex/CarSelect.png");
		pBack[6]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 70, 0.0f));
		pBack[6]->SetSize(D3DXVECTOR3(470, 85, 0.0f));
		pBack[6]->SetTransform();
	}
	if (pBack[7] != NULL)//�Z���N�g����Ƃ��̘g�@����ԍŌ�ɕ`�悷�鎖�I
	{
		pBack[7]->BindTexture("data/tex/SelectFrame.png");
		pBack[7]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - 240, 520, 0.0f));
		pBack[7]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[7]->SetTransform();
	}

	//// ��̍쐬
	CSky::Create();
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CCharacterSelect::Uninit(void)
{
	//�|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCharacterSelect::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		if (pInputKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				CFade::SetFade(CManager::MODE_PUZZLE_CUSTOM, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
			//�Ԃ̑I������
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RIGHT)&&m_nCarType< 3)
			{
				pos = pBack[7]->GetPosition();
				pBack[7]->SetPosition(D3DXVECTOR3(pos.x + 160, pos.y, pos.z));
				pBack[7]->SetTransform();
				m_nCarType++;
			}
			else if (pInputKeyboard->GetTriggerKeyboard(DIK_LEFT) && m_nCarType > 0)
			{
				pos = pBack[7]->GetPosition();
				pBack[7]->SetPosition(D3DXVECTOR3(pos.x - 160, pos.y, pos.z));
				pBack[7]->SetTransform();
				m_nCarType--;
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// �R���g���[���[����������Ă���Ƃ�
		 //�Q�[���̑J��
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// �Q�[���p�b�h��A�{�_���������ꂽ�Ƃ�
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// �Q�[���p�b�h��START�{�^���������ꂽ�Ƃ�
			{
				CFade::SetFade(CManager::MODE_PUZZLE_CUSTOM, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCharacterSelect::Draw(void)
{

}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CCharacterSelect::LoadAsset(void)
{
}