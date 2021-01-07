//==================================================================================================================
//
// �s�[�X[piece.h]
// Author:Ryouma Inoue
//
//==================================================================================================================
#ifndef _PIECE_H_
#define _PIECE_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define Piece_Depth 8																// �c�̃|���S����
#define Piece_Width 15																// ���̃u���b�N��

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CPlayer;
class CScene2D;

//==================================================================================================================
//
// �N���X�̒�`
//
//==================================================================================================================
class CPiece : public CScene
{
public:

	//================================
	// �X�e�[�^�X�̃^�C�v
	//================================
	enum StatusType
	{
		StatusType_None = 0,						 // ����
		StatusType_Route,							 // ���[�g�ē��\�������񑬓x�㏸
		StatusType_Ranking,							 // ���ʕ\�����ō����x�㏸
		StatusType_All_MediumUp,					 // �ō����x���X�s�[�h�㏸�������񑬓x���p���[�𒆃A�b�v
		StatusType_MaxSpeed_GreatUp,				 // �ō����x��A�b�v
		StatusType_MaxSpeed_MediumUp,				 // �ō����x���A�b�v
		StatusType_MaxSpeed_SmallUp,				 // �ō����x���A�b�v
		StatusType_Rate_GreatUp,					 // �X�s�[�h�㏸����A�b�v
		StatusType_Rate_MediumUp,					 // �X�s�[�h�㏸�����A�b�v
		StatusType_Rate_SmallUp,					 // �X�s�[�h�㏸�����A�b�v
		StatusType_Turning_GreatUp,					 // ���񑬓x��A�b�v
		StatusType_Turning_MediumUp,				 // ���񑬓x���A�b�v
		StatusType_Turning_SmallUp,					 // ���񑬓x���A�b�v
		StatusType_Decay_Down,						 // �X�s�[�h�������ቺ
		StatusType_Max,								 // �ő吔
	};

	//================================
	// �s�[�X�̃^�C�v
	//================================
	enum PieceType
	{
		PieceType_None = 0,
		PieceType_Square,
		PieceType_Rectangle,
		PieceType_L_Type,
		PieceType_Square_1,
		PieceType_Rectangle_1,
		PieceType_Rectangle_2,
		PieceType_Speed,
		PieceType_Speed_1,
		PieceType_Max,
	};

	CPiece(PRIORITY type);																			// �R���X�g���N�^
	~CPiece();																						// �f�X�g���N�^
	HRESULT Init(void);																				// ����������
	void Uninit(void);																				// �I������
	void Update(void);																				// �X�V����
	void Draw(void);																				// �`�揈��

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

	bool GetMove(void) { return m_bMove; }															// �ړ����擾
	bool GetPlaacement(void) { return m_bPlacement; }												// �z�u���擾
	bool GetPuzzle(int nDepth, int nWidth) { return m_bPuzzle[nDepth][nWidth]; }					// �p�Y�����擾
	bool GetMap(void) { return m_bMap; }															// �}�b�v�擾
	bool GetRoute(void) { return m_bRoute; }														// ���[�g�擾
	bool GetRanking(void){ return m_bRanking; }														// �����L���O�擾
	float GetSpeed(void) { return m_fSpeed; }														// �X�s�[�h�擾
	float GetRate(void) { return m_fRate; }															// �X�s�[�h�㏸���擾
	float GetTurning(void) { return m_fTurning; }													// ���񑬓x�擾
	float GetDecay(void) { return m_fDecay; }														// �������擾
	int GetPower(void) { return m_nPower; }															// �p���[�擾

	void SetPieceType(PieceType type) { m_PieceType = type; }										// �^�C�v�ݒ�
	void SetStatusType(StatusType statusType) { m_StatusType = statusType; }						// �X�e�[�^�X�^�C�v
	void SetStatus(void);																			// �X�e�[�^�X�ݒ�
	void SetPiece(void);																			// �s�[�X�ݒ�
	void SetMove(bool bMove) { m_bMove = bMove; }													// �ړ�
	void SetRelease(bool bRelease) { m_bRelease = bRelease; }										// �������ǂ���
	void SetCol(D3DXCOLOR col) { m_col = col; }														// �J���[�ݒ�
	static CPiece *Create(void);																	// ��������
	static HRESULT Load(void);																		// �e�N�X�`����񃍁[�h
	static void Unload(void);																		// �e�N�X�`�����A�����[�h

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;															// �e�N�X�`���ւ̃|�C���^
	bool m_bPuzzle[Piece_Depth][Piece_Width];														// �g�p���Ă��邩�ǂ���
	CScene2D *m_pBlock[Piece_Depth][Piece_Width];													// �V�[��2D�̃|�C���^

	D3DXVECTOR3 m_pos;																				// �ʒu
	D3DXCOLOR m_col;																				// �J���[

	int m_nCntMove_X;																				// �ړ��J�E���gX
	int m_nCntMove_Y;																				// �ړ��J�E���gY
	float m_fSpeed;																					// ����
	float m_fRate;																					// �X�s�[�h�㏸��
	float m_fTurning;																				// ���񑬓x
	float m_fDecay;																					// ������
	int m_nPower;																					// �p���[
	PieceType m_PieceType;																			// �s�[�X�^�C�v
	StatusType m_StatusType;																		// �X�e�[�^�X�^�C�v
	bool m_bPlacement;																				// �z�u���Ă��邩�ǂ���
	bool m_bMove;																					// �������ǂ���
	bool m_bBox[Piece_Depth][Piece_Width];															// �{�b�N�X�̏��
	bool m_bPut;																					// �u��
	bool m_bRelease;																				// ����
	bool m_bMap;																					// �}�b�v��\�����邩
	bool m_bRoute;																					// ���[�g��\�����邩
	bool m_bRanking;																				// �����L���O��\�����邩
};
#endif