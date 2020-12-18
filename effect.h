//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_EFFECT 3000					// �G�t�F�N�g�̍ő吔

//=============================================================================
// �\���̒�`
//=============================================================================
// �p�[�e�B�N���f�o�b�O���[�h
typedef enum
{
	PARTICLESHAPE_CONE = 0,
	PARTICLESHAPE_SPHERE,
	PARTICLESHAPE_BOX,
	PARTICLESHAPE_MAX
} PARTICLESHAPE;

typedef enum
{
	EFFECTTYPE_ROSE = 0,
	EFFECTTYPE_SMOKE,
	EFFECTTYPE_SANDSMOKE,
	EFFECTTYPE_HALO,
	EFFECTTYPE_SHOCKWAVE,
	EFFECTTYPE_STAR,
	EFFECTTYPE_SLASH,
	EFFECTTYPE_SPHERE,
	EFFECTTYPE_WING,
	EFFECTTYPE_AURA,
	EFFECTTYPE_MAX
} EFFECTTYPE;

typedef enum
{
	EASINGTYPE_NONE = 0,
	EASINGTYPE_IN,
	EASINGTYPE_OUT,
	EASINGTYPE_MAX
} EASINGTYPE;

typedef struct
{
	std::string				texAdd;					// �e�N�X�`���A�h���X
	bool					bBillboard;				// �r���{�[�h�̗L��
	bool					bAlpha;					// �A���t�@�u�����f�B���O�̗L��
	bool					bZBuffer;				// Z�o�b�t�@�̗L��
	bool					bFadeOut;				// �t�F�[�h�A�E�g

	D3DXVECTOR3				size;					// �傫��
	D3DXCOLOR				col;					// �F

	D3DXVECTOR3				moveSize;				// �T�C�Y�̕ω���
	D3DXVECTOR3				moveRot;				// ��]�̕ω���
	D3DXVECTOR3				centerPos;				// ���S�ʒu
	D3DXCOLOR				moveCol;				// �F�̕ω���

	int						nLife;					// �����J�E���g

	float					fGravity;				// �d��
	float					fResistance;			// ��R
	float					fAngle;					// �p�x
	float					fDistance;				// ����
	float					fSpeedRot;				// ��]���x
	float					fMaxSpeed;					// �X�s�[�h

	D3DXVECTOR2				sprite;					// �摜�̕�����
} EFFECT;

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCapsule;
class CMeshSphere;
class CMeshCube;

//=============================================================================
// �N���X��`
//=============================================================================
class CEffect : public CScene
{
public:
	CEffect(PRIORITY obj);															// �R���X�g���N�^
	~CEffect();																		// �f�X�g���N�^
	HRESULT Init(void);																// ����������
	void Uninit(void);																// �I������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��

	static void Create();															// �N���G�C�g����
	static HRESULT Load(void);														// �f�ނ̓ǂݍ���

	void SetPosition(D3DXVECTOR3 pos);												// �ʒu�̐ݒ�
	void SetMove(D3DXVECTOR3 move);													// �ړ��ʂ̐ݒ�
	void SetUse(bool bValue);														// �g�p���ݒ�
	void SpriteAnimation(D3DXVECTOR2 patternNo, int nPatternAnim, int nHeight);		// �X�v���C�g�A�j���[�V��������
	static void SetEffect(const std::string Add, D3DXVECTOR3 pos, D3DXVECTOR3 size,		// �G�t�F�N�g�̔�������
		D3DXVECTOR3 move, D3DXVECTOR3 moveSize, D3DXVECTOR3 moveRot, D3DXCOLOR moveCol, EASINGTYPE easingType,
		D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fGravity, float fResistance,
		bool bBillboard, int nPatternAnim, int nHeight, D3DXVECTOR3 centerPos,
		float fAngle, float fDistance, float fRotationSpeed, D3DXVECTOR2 sprite,
		bool bAlpha, bool bZBuffer, bool bFadeOut);

	static void FallingPetals(bool bGravity, D3DXVECTOR3 pos, int nLife);			// �����U��Ԃт�̃G�t�F�N�g
	static void PetalsGather(D3DXVECTOR3 pos);										// �W�܂�Ԃт�̃G�t�F�N�g
	static void PetalsSoaring(D3DXVECTOR3 pos);										// �Ԃт炪�����オ��G�t�F�N�g
	static void WingStay(D3DXVECTOR3 pos);											// �H�������G�t�F�N�g
	static void SandSmokeEffect(D3DXVECTOR3 pos);									// �����̃G�t�F�N�g
	static void SandSmoke(D3DXVECTOR3 pos);
	static void SandSmokeExplosion(D3DXVECTOR3 pos);								// ���o�����G�t�F�N�g
	static void Shockwave(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);				// �V���b�N�E�E�F�[�u
	static void	Halo(D3DXVECTOR3 pos, D3DXVECTOR3 size);							// �n���[����
	static void Purification(D3DXVECTOR3 pos);										// �򉻕��U
	static void Star(D3DXVECTOR3 pos);												// �X�^�[
	static void Slashing(D3DXVECTOR3 pos, D3DXVECTOR3 rot);							// �a��
	static void Aura(int nCount, bool bJump, D3DXVECTOR3 pos);						// �I�[��
	static void PetalCluster(D3DXVECTOR3 pos, D3DXVECTOR3 rot);						// �Ԃт�̏W����

	bool GetUse(void) { return m_bUse; }											// �g�p��Ԃ̎擾
	int GetAnimPage(void) { return m_AnimPage; }									// �y�[�W���̎擾
	D3DXVECTOR3 GetMove(void) { return m_move; }									// �ړ��ʂ̎擾
	D3DXVECTOR3 GetPosition(void) { return m_pos; }									// �ʒu�̎擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }								// ���[���h�}�g���b�N�X�̎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

	static D3DXVECTOR3 GetRandomPosWithSphere(void);			// �ʒu�̃����_���擾
	static D3DXVECTOR3 GetRandomPosWithCone(float &fRadius);			// �ʒu�̃����_���擾
	static float GetRandomAngle(void);			// �p�x�̃����_���擾

	static void LoadScript(std::string Add);		// �G�t�F�N�g���̓ǂݍ���
	static EFFECT GetEffectData(std::string Tag);		// �G�t�F�N�g�̎擾
	static void CreateEffect(std::string Tag, D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// �G�t�F�N�g�̐���
	static void LoadParticleScript(void);			// �p�[�e�B�N���X�N���v�g�̃��[�h

private:
#ifdef _DEBUG
	void Debug(void);
#endif

	void MakeVertex(void);															// ���_���̍쐬
	void Animation(void);															// �A�j���[�V��������

	static CEffect *m_pEffect[MAX_EFFECT];											// �������ꂽ�G�t�F�N�g�|�C���^

	/* ================ �X�e�[�^�X ================ */
	LPDIRECT3DTEXTURE9				m_pTexture;										// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3						m_pos;											// �|�W�V����

	D3DXVECTOR3						m_centerPos;									// ��]���S
	float							m_fRotationSpeed;								// ��]���x
	float							m_fAngle;
	float							m_fDistance;

	D3DXVECTOR3						m_rot;											// ��]��
	D3DXVECTOR3						m_moveRot;										// ��]�̕ω���
	D3DXVECTOR3						m_move;											// �ړ���
	D3DXVECTOR3						m_posOld;										// �O�̃|�W�V����
	D3DXVECTOR3						m_size;											// �傫��
	D3DXVECTOR3						m_moveSize;										// �傫���̕ω���
	D3DXVECTOR3						m_maxSize;										// �傫���̍ő�l
	D3DXCOLOR						m_col;											// �F
	D3DXCOLOR						m_moveCol;										// �F�̕ω���
	D3DXMATRIX						m_mtxWorld;										// ���[���h�}�g���b�N�X
	EASINGTYPE						m_easingType;									// �C�[�W���O�^�C�v
	float							m_fRadius;										// ���a
	float							m_fResistance;									// ��R
	int								m_nLife;										// ���C�t
	int								m_nMaxLife;										// ���C�t�̍ő�l
	bool							m_bUse;											// �g�p���Ă��邩�ǂ���
	bool							m_bBillboard;									// �r���{�[�h�̎g�p

	/* ================ �p�[�e�B�N�� ================ */
	int								m_AnimPage;										// ���݂̃y�[�W��
	int								m_AnimCount;									// ���̃y�[�W�J�E���g
	int								m_nInterval;									// ���̃y�[�W�܂ł̊Ԋu
	D3DXVECTOR2						m_sprite;										// ������
	bool							m_bAlpha;										// ���Z�����̗L��
	bool							m_bZBuffer;										// Z�o�b�t�@�̗L��
	bool							m_bFadeOut;										// �t�F�[�h�A�E�g�̗L��
	float							m_fGravity;										// �d�͉����x

	/*===============3D�����_�����O�֘A===============*/
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;										// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH						m_pMesh;										// ���b�V�����ւ̃|�C���^
	DWORD							m_nNumMat;										// �}�e���A�����̐�
	LPD3DXBUFFER					m_pBuffMat;										// �}�e���A�����ւ̃|�C���^

	static std::map<std::string, EFFECT> m_effectMap;								// �G�t�F�N�g�}�b�v
};
#endif