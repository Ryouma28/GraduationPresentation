//=============================================================================
//
// ���Ƃ̋��������߂鏈�� [distanceNext.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _DISTANCENEXT_H_
#define _DISTANCENEXT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "network.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DISTANCE_MAXNUM 3		//�����̍ő包��
#define MAX_ROUND 3		// �����

//=============================================================================
// �O���錾
//=============================================================================
class CUi;
class CNumber;

//=============================================================================
// �N���X��`
//=============================================================================
class CDistanceNext
{
public:
	CDistanceNext();			// �R���X�g���N�^
	~CDistanceNext();			// �f�X�g���N�^
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �J������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	static CDistanceNext *Create(void);	// �N���G�C�g����
	static HRESULT Load(void);	// ���[�h����

	void SetPosition(D3DXVECTOR3 &pos);					// �ʒu�̐ݒ�
	void SetDistance(D3DXVECTOR3 &distance);			// �����̐ݒ�
	void SetIntervalNum(D3DXVECTOR3 &interval);			// �����ǂ����̋���
	void SetNowRound(void);								// ���݂̎����

	D3DXVECTOR3 GetPosition(void) { return m_pos; }		// �ʒu�̎擾
	D3DXVECTOR3 GetDistance(void) { return m_distance; }// �����̎擾
	int GetNowRound(void) { return m_nNowRound; }		// ���݂̎���񐔂��擾

	void ShowInspector(void) {};

public:
	void SetTransform(void);
	void SetNumber(int nValue);	// ���l�̐ݒ�

	float CalculateCourseRange(int nRound, unsigned int &nPointNum, D3DXVECTOR3 &pos);	// �R�[�X�̎c�苗�������߂鏈��

	CNumber	*m_apNumber[DISTANCE_MAXNUM];

	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_distance;			// �I�t�Z�b�g
	D3DXVECTOR3 m_IntervalNum;		// �����ǂ����̋���
	int m_nNowRound;				// ���݂̎����

	int m_nRank[MAX_PLAYER];		// �v���C���[�S���̃����N
};
#endif