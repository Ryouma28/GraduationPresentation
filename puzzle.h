//=============================================================================
//
// �p�Y������ [puzzle.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _PUZZLE_H_
#define _PUZZLE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "box.h"

//=============================================================================
// �O���錾
//=============================================================================
class CPieceSelect;
class CUi;

//=============================================================================
// �N���X��`
//=============================================================================
class CPuzzle
{
public:
	CPuzzle();					// �R���X�g���N�^
	~CPuzzle();					// �f�X�g���N�^
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �J������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	static void	LoadAsset(void);												// �A�Z�b�g�̓ǂݍ��ݏ���
	static float GetSpeed(int nPieceNum) { return m_fSpeed[nPieceNum]; }		// �X�s�[�h�̎擾
	static float GetRate(int nPieceNum) { return m_fRate[nPieceNum]; }			// �X�s�[�h�㏸���̎擾
	static float GetTurning(int nPieceNum) { return m_fTurning[nPieceNum]; }	// ���񑬓x�擾
	static float GetDecay(int nPieceNum) { return m_fDecay[nPieceNum]; }		// �������擾
	static int GetPower(int nPieceNum) { return m_nPower[nPieceNum]; }			// �p���[�擾
	static int GetPieceNum(void) { return m_nPieceNum; }						// �s�[�X��
	static bool GetRoute(int nPieceNum) { return m_bRoute[nPieceNum]; }			// ���[�g
	static bool GetRank(int nPieceNum) { return m_bRank[nPieceNum]; }			// �����L���O

private:
	CPieceSelect *m_pPieceSelect;				// �{�b�N�X�̃|�C���^

	static float m_fSpeed[Piece_Num];			// �X�s�[�h
	static float m_fRate[Piece_Num];			// �X�s�[�h�㏸��
	static float m_fTurning[Piece_Num];			// ���񑬓x
	static float m_fDecay[Piece_Num];			// ������
	static int m_nPower[Piece_Num];				// �p���[
	static bool m_bRoute[Piece_Num];			// ���[�g��\�����邩
	static bool m_bRank[Piece_Num];				// �����L���O
	static int m_nPieceNum;						// �s�[�X��

};
#endif