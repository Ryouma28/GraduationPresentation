//=============================================================================
//
// �S�[���pUI���� [finishUi.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _FINISHUI_H_
#define _FINISHUI_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CFinishUi : public CScene2D
{
public:
	CFinishUi(PRIORITY obj);						// �v���C���[�̃R���X�g���N�^(�I�u�W�F�N�g�^�C�v)
	~CFinishUi();									// �v���C���[�̃f�X�g���N�^
	HRESULT Init(void);							// ����������
	void Uninit(void);							// �J������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static CFinishUi *Create(void);				// �v���C���[����
	static HRESULT Load(void);					// �f�ރf�[�^�̎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
#ifdef _DEBUG
	void Debug(void);							// �f�o�b�O�����֐�
#endif

	/*================= �v���C���[�֘A =================*/
	D3DXVECTOR3	m_move;							// �ړ���
	bool		m_bGravity;						// �d�̗͂L��
};
#endif