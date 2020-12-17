//=============================================================================
//
// �v���C���[���� [player.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "character.h"
#include "box.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_KEY 30
#define NORMAL_SPEED 6.0f
#define SP_SPEED 2.0f

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCapsule;
class CMeshCube;
class CMeshOrbit;
class CColliderSphere;
class CColliderBox;
class CScene2D;
class CModel;
class CNumber;
class CDistanceNext;
class CUi;
class CShadow;

//=============================================================================
// �N���X��`
//=============================================================================
class CPlayer : public CCharacter
{
public:
	//���[�V�����^�C�v
	typedef enum
	{
		ANIMATIONTYPE_NONE = -1,				// �Ȃ�
		ANIMATIONTYPE_OVERTAKE_RIGHT,			// �ǂ�����(�E����)
		ANIMATIONTYPE_OVERTAKE_LEFT,			// �ǂ�����(������)
		ANIMATIONTYPE_RUN,						// ����
		ANIMATIONTYPE_ATTACK_1,					// ��{�U��1
		ANIMATIONTYPE_DAMAGE,					// �_���[�W
		ANIMATIONTYPE_DETH,						// ���S
		ANIMATIONTYPE_MAX						// �A�j���[�V�����̍ő吔
	} ANIMATIONTYPE;							// �A�j���[�V�����^�C�v

	typedef enum
	{
		DRIFT_RIGHT = 0,		// �h���t�g�E
		DRIFT_LEFT,				// �h���t�g��
		DRIFT_MAX				// �h���t�g�ő�
	} DRIFTTYPE;				// �h���t�g���

	CPlayer(PRIORITY obj);						// �v���C���[�̃R���X�g���N�^(�I�u�W�F�N�g�^�C�v)
	~CPlayer();									// �v���C���[�̃f�X�g���N�^
	HRESULT Init(void);							// ����������
	void Uninit(void);							// �J������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static CPlayer *Create(void);				// �v���C���[����
	static HRESULT Load(void);					// �f�ރf�[�^�̎擾
	void SetDeathblow(float nValue);			// �K�E�Z�|�C���g���̐ݒ�
	void SetEvent(bool bValue);					// �C�x���g�t���O�̐ݒ�

	void OnTriggerEnter(CCollider *col);
	void OnCollisionEnter(CCollider *col);
	void ShowInspector(void) {};

	void BehaviorForMaxFrame(void);				// �ő�t���[�����ɓ��B�����Ƃ��̏���
	void BehaviorForMaxKey(void);				// �ő�L�[���ɓ��B�����Ƃ��̏���

	float GetDeathblow(void) { return m_fDeathblow; }				// �K�E�Z�|�C���g���̎擾
	D3DXVECTOR3 GetMove(void) { return m_move; }					// �ړ��ʂ̎擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }				// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetRotDest(void) { return m_dest; }					// ��]�ŏI���B�n�_
	D3DXVECTOR3 GetCameraRot(void) { return m_cameraRot; }			// �J�����̉�]���
	CDistanceNext *GetDistanceNext(void) { return m_pDistanceNext; }// ���̖ڕW�Ƃ̋������擾
	void SetCameraRot(D3DXVECTOR3 cameraRot) { m_cameraRot = cameraRot; }
	void SetSpeed(int nCntSpeed);
	void SetTurning(int nCntTurning);
	int GetNumRound(void) { return m_nRound; }
	bool GetEvent(void ) { return m_bEvent; }

private:
#ifdef _DEBUG
	void Debug(void);																// �f�o�b�O�����֐�
#endif

	void MoveNearEnemy(void);														// �߂��ɂ���G�Ɉړ����鏈��
	void Collision(void);															// �����蔻�菈��
	void Input(void);																// �L�[���͏��֐�
	void InputKeyboard(float fTireRotSpeed, D3DXVECTOR3 aVec);						// �L�[�{�[�h���͏���
	void InputGemepad(float nValueH, float nValueV, float fTireRotSpeed, D3DXVECTOR3 aVec);// �Q�[���p�b�h���͏���
	bool CollisionWall(void);														// �ǂ̓����蔻��
	bool CollisionWallWithRay(void);												// ���C�ɂ��ǂ̓����蔻��
	void SlopeMove(void);															// ��̏���

	/*================= �v���C���[�֘A =================*/
	D3DXVECTOR3						m_dest;											// ���f���̍ŏI���B�_
	D3DXVECTOR3						m_difference;									// ���f���̍ő��]
	D3DXVECTOR3						m_move;											// �ړ���
	D3DXVECTOR3						m_size;											// �傫��
	D3DXVECTOR3						m_rot;											// ��]��
	D3DXVECTOR3						m_cameraRot;									// �J�����̉�]���
	D3DXCOLOR						m_color;										// �F
	D3DXVECTOR3						m_vectorOld;									// �O��̕����x�N�g��
	float							m_fSpeed;										// �X�s�[�h
	float							m_fPuzzleSpeed[Piece_Num];						// �p�Y���̃X�s�[�h
	float							m_fPuzzleMaxSPeed;								// �p�Y���̃X�s�[�h
	float							m_fPuzzleRate[Piece_Num];						// �X�s�[�h�㏸��
	float							m_fPuzzleMaxRate;								// �X�s�[�h�㏸��
	float							m_fPuzzleTurning[Piece_Num];					// ���񑬓x
	float							m_fPuzzleMaxTurning;							// ���񑬓x
	float							m_fPuzzleDecay[Piece_Num];						// �������擾
	float							m_fPuzzleMaxDecay;								// �������擾
	float							m_fPuzzlePower[Piece_Num];						// �p���[
	float							m_fPuzzleMaxPower;								// �p���[
	float							m_fDeathblow;									// �K�E�Z�|�C���g
	float							m_fAcceleration;								// �����x
	int								m_nLife;										// �̗�
	int								m_nActionCount;									// ���̃A�N�V�����܂ł̃J�E���^
	int								m_nParticleCount;								// �p�[�e�B�N�������܂ł̃J�E���^
	int								m_nPointNum;									// �|�C���g�ԍ�
	bool							m_bJump;										// �W�����v
	bool							m_bEvent;										// �C�x���g�����t���O
	bool							m_bColliderWithWall;							// �ǂ̓����蔻��
	bool							m_bHit;											// �q�b�g����
	bool							m_bDrift[DRIFT_MAX];							// �v���C���[�̃h���t�g�t���O
	bool							m_bMove;										// ���ݓ����Ă��邩�̃t���O
	bool							m_bAccel;										// �A�N�Z���������Ă��邩�ǂ���
	bool							m_bRankingSign[Piece_Num];
	bool							m_bRanking;
	CNumber							*m_pRank;										// �����L���O�pUI
	CDistanceNext					*m_pDistanceNext;								// ���̃v���C���[�Ƃ̋�����UI
	CShadow							*m_pShadow;										// �e�̏��|�C���^

	// ���[�X�Q�[���֘A
	int								m_nRound;										// ���݂̎����

	/*=============== 3D�����_�����O�֘A ===============*/
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;										// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH						m_pMesh;										// ���b�V�����ւ̃|�C���^
	DWORD							m_nNumMat;										// �}�e���A�����̐�
	LPD3DXBUFFER					m_pBuffMat;										// �}�e���A�����ւ̃|�C���^
	D3DXMATRIX						m_mtxWorld;										// ���[���h�}�g���b�N�X

	CMeshOrbit						*m_pMeshOrbit;									// �O�Ղ̃|�C���^

	/*===============�����蔻��===============*/
	CColliderSphere					*m_pColPlayerSphere;							// �v���C���[�̓����蔻��̃|�C���^
};
#endif