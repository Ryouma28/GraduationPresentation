//=============================================================================
//
// �s�[�X�Z���N�g���� [pieceSelect.h]
// Author : Ryouma Inoue
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"
#include "box.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_CORE 17		// �R�A�̍ő吔

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;
class CPiece;

//=============================================================================
// �N���X��`
//=============================================================================
class CPieceSelect : public CScene
{
public:
	//�R�A�^�C�v
	typedef enum
	{
		PIECETYPE_NONE = -1,		// �Ȃ�
		PIECETYPE_CORE_00,			// �R�A
		PIECETYPE_CORE_01,			// �R�A
		PIECETYPE_CORE_02,			// �R�A
		PIECETYPE_CORE_03,			// �R�A
		PIECETYPE_CORE_04,			// �R�A
		PIECETYPE_CORE_05,			// �R�A
		PIECETYPE_CORE_06,			// �R�A
		PIECETYPE_CORE_07,			// �R�A
		PIECETYPE_CORE_08,			// �R�A
		PIECETYPE_CORE_09,			// �R�A
		PIECETYPE_CORE_10,			// �R�A
		PIECETYPE_CORE_11,			// �R�A
		PIECETYPE_CORE_12,			// �R�A
		PIECETYPE_CORE_13,			// �R�A
		PIECETYPE_CORE_14,			// �R�A
		PIECETYPE_CORE_15,			// �R�A
		PIECETYPE_CORE_16,			// �R�A
		PIECETYPE_CORE_17,			// �R�A
		PIECETYPE_MAX				// �ő吔
	} PIECETYPE;					// �^�C�v

	CPieceSelect(PRIORITY obj);		// �R���X�g���N�^
	~CPieceSelect();				// �f�X�g���N�^
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �J������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

	static CPieceSelect *Create(void);	// �N���G�C�g����
	static HRESULT Load(void);			// ���[�h����
	static bool GetPuzzle(int nDepth, int nWidth) { return m_bPuzzle[nDepth][nWidth]; }	// �p�Y�����擾

	int GetPieceNum(void) { return m_nPieceNum; }									// �s�[�X��
	float GetSpeed(int nPieceNum) { return m_fSpeed[nPieceNum]; }					// �X�s�[�h�擾
	float GetRate(int nPieceNum) { return m_fRate[nPieceNum]; }						// �X�s�[�h�㏸���擾
	float GetTurning(int nPieceNum) { return m_fTurning[nPieceNum]; }				// ���񑬓x
	float GetDecay(int nPieceNum) { return m_fDecay[nPieceNum]; }					// ������
	int GetPower(int nPieceNum) { return m_nPower[nPieceNum]; }						// �p���[
	bool GetRoute(int nPieceNum) { return m_bRoute[nPieceNum]; }					// ���[�g
	bool GetRank(int nPieceNum) { return m_bRank[nPieceNum]; }						// �����L���O

	void ShowInspector(void) {};
	void SetPiece(PIECETYPE type,D3DXVECTOR3 pos,D3DXVECTOR3 size, std::string Add);
	void LoadPiece(void);

private:
	CScene2D *m_pPieceSelect[MAX_CORE];				// Scene2D�̃|�C���^
	CScene2D *m_pPieceExplanation;					// Scene2D�̃|�C���^
	CPiece *m_pPiece[Piece_Num];					// �s�[�X

	PIECETYPE m_type;								// �s�[�X�̃^�C�v

	bool m_bSelect[MAX_CORE];						// �I�΂�Ă��邩�ǂ���
	bool m_bPiece;									// �u���Ă邩�ǂ���

	int m_nPieceNum;								// �s�[�X��
	int m_nSelect;									// �I���J�E���g
	int m_nSelectCnt;								// �Z���N�g�J�E���g
	float m_fSpeed[Piece_Num];						// �X�s�[�h
	float m_fRate[Piece_Num];						// �X�s�[�h�㏸��
	float m_fTurning[Piece_Num];					// ���񑬓x
	float m_fDecay[Piece_Num];						// ������
	int m_nPower[Piece_Num];						// �p���[
	bool m_bRoute[Piece_Num];						// ���[�g��\�����邩
	bool m_bRank[Piece_Num];						// �����L���O��\�����邩
	bool m_bPlacement;																// �z�u���Ă��邩�ǂ���

	static bool m_bPuzzle[Box_Depth][Box_Width];	// �g�p���Ă��邩�ǂ���
	bool m_bPuzzleStorage[Box_Depth][Box_Width];	// �ۊ�
	bool m_bRelease;

};
#endif