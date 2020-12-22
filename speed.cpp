//=============================================================================
//
// �������� [speed.cpp]
// Author : Seiya Takahashi
//
//=============================================================================
#include "speed.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "player.h"
#include "takaseiLibrary.h"
#include "ui.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define INTERVAL 33.0f		// �����Ɛ����̊�

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
float CSpeed::m_fDigit = 0.0f;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSpeed::CSpeed(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene(obj)
{
	SetObjType(PRIORITY_UI);	//�I�u�W�F�N�g�^�C�v

	//�l�̏�����
	m_dTimeFrame = 0;			// �t���[�����̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSpeed::~CSpeed()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSpeed::Init(void)
{
	// �ő包���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		// ��������
		m_apNumber[nCnt] = CNumber::Create();

		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->BindTexture("data/tex/number_speed.png");
			m_apNumber[nCnt]->SetPosition(D3DXVECTOR3(55 + INTERVAL * nCnt, 687.0f, 0));	// �ʒu�ݒ�
			m_apNumber[nCnt]->SetSize(D3DXVECTOR3(INTERVAL, 60, 0));						// �傫���ݒ�
			m_apNumber[nCnt]->SetTransform();
		}
	}

	CUi *pSpeedUi = CUi::Create();

	if (pSpeedUi != NULL)
	{
		pSpeedUi->LoadScript("data/text/ui/SpeedMeter.txt");
		pSpeedUi->SetPosition(D3DXVECTOR3(201.0f, 692.0f, 0.0f));
	}

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CSpeed::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CSpeed::Update(void)
{
	// �v���C���[���擾
	CPlayer *pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 pos, posOld;		// ���݂̈ʒu�A�O��̈ʒu
	float fDistance;				// ����

	// �v���C���[������Ƃ�
	if (pPlayer != NULL)
	{
		// �v���C���[�̈ʒu�擾
		pos = pPlayer->GetPosition();

		// ���݂̈ʒu��O��̈ʒu�ɑ��
		posOld = pPlayer->GetPosOld();;
	}

	m_dTimeFrame++;				// �J�E���^�[��1���Z����

	// ���݂ƑO��Ƃ̋����v�Z
	fDistance = CTakaseiLibrary::OutputDistance(posOld, pos);

	// ����������	fDigit = fDistance / m_dTimeFrame;
	m_fDigit = fDistance;

	// �����ݒ�
	SetDigit(m_fDigit);

	if (m_fDigit >= 50)
	{
		CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

		pSound->PlaySoundA(SOUND_LABEL_SE_Speed50Km);		// �_���[�W���̍Đ�

	}
	else if (m_fDigit >= 100)
	{
		CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

		pSound->PlaySoundA(SOUND_LABEL_SE_Speed100Km);		// �_���[�W���̍Đ�

	}
	// �J�E���^��0�ȏ�̂Ƃ�
	if (m_fDigit >= 0)
	{
		SetTime((int)m_fDigit);		// �^�C���̍X�V
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSpeed::Draw(void)
{

}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CSpeed *CSpeed::Create(void)
{
	CSpeed *pSpeed;

	pSpeed = new CSpeed(CScene::PRIORITY_UI);
	pSpeed->Init();
	return pSpeed;
}

//=============================================================================
// �^�C���̕ύX
//=============================================================================
void CSpeed::SetTime(int nTime)
{
	int nNumber;
	int nLength = CManager::LengthCalculation(nTime);

	// �^�C�����t���[���Ŋ���
	nTime = (int)(nTime / 1.0f);

	// �ő包���܂ŃJ�E���g
	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		// �\�����鐔���v�Z
		nNumber = nTime % (int)powf(10.0f, (float)MAX_DIGIT - nCount) / (int)powf(10.0f, MAX_DIGIT - 1.0f - nCount);

		// �����ݒ�
		m_apNumber[nCount]->SetNumber(nNumber);

		if (MAX_DIGIT - nCount <= nLength)
		{// �ő包����菬�������������Ƃ�
			if (!m_apNumber[nCount]->GetActive())
			{
				m_apNumber[nCount]->SetActive(true);
			}
		}
		else
		{// �ő包�����傫�����������Ƃ�
			if (m_apNumber[nCount]->GetActive())
			{
				m_apNumber[nCount]->SetActive(false);
			}
		}
	}
}
