//=============================================================================
//
// �������� [speed.h]
// Author : Seiya Takahashi
//
//=============================================================================
#ifndef _SPEED_H_
#define _SPEED_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_DIGIT 3		// �ő包��

//=============================================================================
// �O���錾
//=============================================================================
class CNumber;
class CUi;

//=============================================================================
// �N���X��`
//=============================================================================
class CSpeed : public CScene
{
public:
	CSpeed(PRIORITY obj);							// �R���X�g���N�^
	~CSpeed();										// �f�X�g���N�^
	HRESULT Init(void);								// ����������
	void Uninit(void);								// �J������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	static CSpeed *Create(void);					// �N���G�C�g����
	static void SetDigit(float fDigit) { m_fDigit = fDigit; }// �����ݒ�
	static float GetDigit(void) { return m_fDigit; }// �����擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

	static float GetSpeed(void) { return m_fDigit; }

private:
	void SetTime(int nTime);	// ���Ԑݒ�

	CNumber *m_apNumber[MAX_DIGIT];// �����|�C���^
	DWORD m_dTimeFrame;			// �t���[����

	static float m_fDigit;			// ����
};
#endif