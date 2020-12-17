//=============================================================================
//
// ���̋��������߂鏈�� [distanceNext.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "distanceNext.h"
#include "manager.h"
#include "renderer.h"
#include "ui.h"
#include "number.h"
#include "object.h"
#include "player.h"
#include "game.h"
#include "takaseiLibrary.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define DIVISION (0.00622f)

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CDistanceNext::CDistanceNext()
{
	m_pos = D3DXVECTOR3_ZERO;
	m_distance = D3DXVECTOR3_ZERO;
	m_IntervalNum = D3DXVECTOR3_ZERO;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDistanceNext::~CDistanceNext()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CDistanceNext::Init(void)
{
	// ������
	CUi *pUi = CUi::Create();

	if (pUi != NULL)
	{
		pUi->LoadScript("data/text/ui/TargetDistance.txt");
		pUi->SetPosition(D3DXVECTOR3(200.0f, 80.0f, 0.0f));
	}

	for (int nCount = 0; nCount < DISTANCE_MAXNUM; nCount++)
	{
		m_apNumber[nCount] = CNumber::Create();

		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->SetSize(D3DXVECTOR3(50, 90, 0));						// �傫���ݒ�
			m_apNumber[nCount]->BindTexture("data/tex/number_rank.png");
		}
	}

	// �ő�l���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_nRank[nCnt] = 0;
	}

	// �ʒu�֌W�̐ݒ�
	SetTransform();
	SetNumber(0);

	m_nNowRound = 0;
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CDistanceNext::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CDistanceNext::Update(void)
{
	CNetwork *pNetwork = CManager::GetNetwork();				// �l�b�g���[�N�̎擾
	std::vector<CObject*> pointObj = CObject::GetPointObj();	// �I�u�W�F�N�g���擾
	unsigned int pointNum = CObject::GetPointNum();				// ���݂̃|�C���g�ԍ��擾
	D3DXVECTOR3 distance;										// ��_�Ԃ̍�
	CPlayer *pPlayer = CGame::GetPlayer();						// �v���C���[���擾
	D3DXVECTOR3 Target = D3DXVECTOR3_ZERO;						// �G�̈ʒu
	int nRound = 0;												// �G�̎����
	unsigned int nFlag = 0;												// �G�̃t���O�ԍ�
	float fDistance = 0.0f;										// ����

	// ���݂̏��ʎ擾
	int nRank = pNetwork->GetRank(pNetwork->GetId());

	// ���ʂ�2�ʈȉ��̎�
	if (nRank > 1)
	{
		// �ő�l���܂ŃJ�E���g
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			if(nCnt == pNetwork->GetId()) { continue; }

			// ���ʂ���̏��ʎ擾
			if (pNetwork->GetRank(nCnt) == nRank - 1)
			{
				Target = pNetwork->GetPosition(nCnt);	// �G�̈ʒu�擾
				nFlag = pNetwork->GetFlag(nCnt);		// �G�̃t���O�ԍ��擾
				nRound = pNetwork->GetRound(nCnt);		// �G�̎��񐔎擾
				break;
			}
		}

		//// ����x��̎�
		//if (nRound > 0)
		//{
		//	// �ő�`�F�b�N�|�C���g�܂ŃJ�E���g
		//	for (int nCnt = 0; nCnt < pointObj.size(); nCnt++)
		//	{
		//
		//	}
		//}

		//if(nFlag < 0) { return; }

		//if (pNetwork->GetFlag(pNetwork->GetId()) != nFlag)
		//{
		//	if (pointNum < pointObj.size() - 1)
		//	{
		//		// �v���C���[�Ǝ��̃`�F�b�N�|�C���g�̋����v�Z
		//		fDistance = CTakaseiLibrary::OutputDistance(pointObj[pointNum + 1]->GetPosition(), pPlayer->GetPosition());

		//		// �G�܂ł̃`�F�b�N�|�C���g�̐��܂ŉ��Z
		//		for (int nCnt = pointNum; nCnt < nFlag; nCnt++)
		//		{
		//			// �����v�Z
		//			fDistance = fDistance + CTakaseiLibrary::OutputDistance(pointObj[nCnt]->GetPosition(), pointObj[nCnt + 1]->GetPosition());
		//		}

		//		// �G�ƑO�̃`�F�b�N�|�C���g�̋����v�Z
		//		fDistance = fDistance + CTakaseiLibrary::OutputDistance(pointObj[nFlag]->GetPosition(), Target);
		//	}
		//}
		//else
		//{
		//	// �G�ƑO�̃`�F�b�N�|�C���g�̋����v�Z
		//	fDistance = fDistance + CTakaseiLibrary::OutputDistance(pPlayer->GetPosition(), Target);
		//}

		float fPlayerLength = CalculateCourseRange(3, pointNum, pPlayer->GetPosition());

		float fTargetLength = CalculateCourseRange(3, nFlag, Target);

		fDistance = fPlayerLength - fTargetLength;

		fDistance *= DIVISION;

		if (fDistance > 999)
		{
			fDistance = 999;
		}

		// �����ݒ�
		SetNumber((int)fDistance);
	}
	else
	{
		// �[���ɂ���
		SetNumber(0);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDistanceNext::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CDistanceNext *CDistanceNext::Create(void)
{
	CDistanceNext *pDistanceNext;		// �w�i�̃|�C���^

	pDistanceNext = new CDistanceNext();		// �w�i�̐���
	if (pDistanceNext == NULL) { return NULL; }	// NULL��������Ԃ�

	pDistanceNext->Init();				// �w�i�̏�����
	return pDistanceNext;
}

//==============================================================================
// �A�Z�b�g�̐�������
//==============================================================================
HRESULT CDistanceNext::Load(void)
{
	return S_OK;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CDistanceNext::SetPosition(D3DXVECTOR3 &pos)
{
	m_pos = pos;
	SetTransform();
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void CDistanceNext::SetDistance(D3DXVECTOR3 &distance)
{
	m_distance = distance;
	SetTransform();
}

//=============================================================================
// �����ǂ����̋���
//=============================================================================
void CDistanceNext::SetIntervalNum(D3DXVECTOR3 & interval)
{
	m_IntervalNum = interval;
	SetTransform();
}

//=============================================================================
// ���݂̎���񐔂ɉ��Z����
//=============================================================================
void CDistanceNext::SetNowRound(void)
{
	m_nNowRound++;
}

//=============================================================================
// �ʒu�֌W�̍X�V
//=============================================================================
void CDistanceNext::SetTransform(void)
{
	for (int nCount = 0; nCount < DISTANCE_MAXNUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->SetPosition((m_distance + m_pos) + m_IntervalNum * (float)nCount);
			m_apNumber[nCount]->SetTransform();
		}
	}
}

//=============================================================================
// �^�C���̕ύX
//=============================================================================
void CDistanceNext::SetNumber(int nValue)
{
	int nNumber;
	int nLength = CManager::LengthCalculation(nValue);

	// �ő包���܂ŃJ�E���g
	for (int nCount = 0; nCount < DISTANCE_MAXNUM; nCount++)
	{
		// �\�����鐔���v�Z
		nNumber = nValue % (int)powf(10.0f, (float)DISTANCE_MAXNUM - nCount) / (int)powf(10.0f, DISTANCE_MAXNUM - 1.0f - nCount);

		// �����ݒ�
		m_apNumber[nCount]->SetNumber(nNumber);

		if (DISTANCE_MAXNUM - nCount <= nLength)
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

//=============================================================================
// �R�[�X�̎c�苗�������߂鏈��
//=============================================================================
float CDistanceNext::CalculateCourseRange(int nRound, unsigned int &nPointNum, D3DXVECTOR3 &pos)
{
	std::vector<CObject*> pointObj = CObject::GetPointObj();	// �I�u�W�F�N�g���擾
	float fDistance = 0.0f;										// ����

	// �����
	for (int nCount = 0; nCount < MAX_ROUND - (nRound + 1); nCount++)
	{
		for (unsigned int nCntCource = 1; nCntCource < pointObj.size(); nCntCource++)
		{
			// �v���C���[�Ǝ��̃`�F�b�N�|�C���g�̋����v�Z
			fDistance += CTakaseiLibrary::OutputDistance(pointObj[nCntCource]->GetPosition(), pointObj[nCntCource - 1]->GetPosition());
		}
	}

	// �c��`�F�b�N�|�C���g����
	for (unsigned int nCount = nPointNum; nCount < pointObj.size() - 1; nCount++)
	{
		// �v���C���[�Ǝ��̃`�F�b�N�|�C���g�̋����v�Z
		fDistance += CTakaseiLibrary::OutputDistance(pointObj[nCount]->GetPosition(), pointObj[nCount + 1]->GetPosition());
	}

	// �v���C���[�Ǝ��̃`�F�b�N�|�C���g�̋����v�Z
	fDistance += CTakaseiLibrary::OutputDistance(pointObj[nPointNum]->GetPosition(), pos);

	return fDistance;
}