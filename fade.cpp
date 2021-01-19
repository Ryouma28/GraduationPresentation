//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "fade.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEX_FADE "data/tex/gamefade.png"
#define ACTION_COUNT (20)

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CFade::m_pVtxBuffFade = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
CFade::FADE				CFade::m_fade = CFade::FADE_NONE;							// �t�F�[�h���
CFade::FADETYPE			CFade::m_fadeType = CFade::FADETYPE_NORMAL;					// �t�F�[�h�^�C�v
CManager::MODE			CFade::m_modeNext = CManager::MODE_NONE;					// ���̉�ʁi���[�h�j
D3DXCOLOR				CFade::m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// �t�F�[�h�F
D3DXVECTOR3				CFade::m_pos = D3DXVECTOR3_ZERO;							// �ʒu
D3DXVECTOR3				CFade::m_move = D3DXVECTOR3_ZERO;							// �ړ���
bool					CFade::m_bReflection = false;								// �o�E���h�������ǂ���
int						CFade::m_nCntAction = 0;									// �A�N�V�����J�E���^�[

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// ����������
//=============================================================================
void CFade::Init(CManager::MODE modeNext)
{
	CRenderer *Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	m_fade = FADE_IN;										// �t�F�[�h�C���ɐݒ�
	m_fadeType = FADETYPE_NORMAL;								// �t�F�[�h�^�C�v���Ȃ��ɐݒ�
	m_modeNext = modeNext;									// ���̃��[�h������
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// ������ʁi�s�����j
	m_bReflection = false;
	m_nCntAction = 0;

	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);			// �ʒu
	m_move = D3DXVECTOR3_ZERO;			// �ړ���

	MakeVertex(pDevice);									// ���_���̐ݒ�
}

//=============================================================================
// �I������
//=============================================================================
void CFade::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuffFade != NULL)
	{
		m_pVtxBuffFade->Release();
		m_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{// �������Ă����Ƃ�
		switch (m_fadeType)
		{
		case FADETYPE_NORMAL:	// �m�[�}���t�F�[�h�������Ƃ�
			NormalFade();
			break;
		case FADETYPE_SLIDE:	// �X���C�h�t�F�[�h�������Ƃ�
			SlideFade();
			break;
		}

		// ���_���̍X�V
		SetTransform();
	}
}

//=============================================================================
// Draw�֐�
//=============================================================================
void CFade::Draw(void)
{
	CRenderer *Renderer = CManager::GetRenderer();						//�����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();					//�f�o�C�X�̎擾

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	switch (m_fadeType)
	{
	case FADETYPE_NORMAL:	// �m�[�}���t�F�[�h�������Ƃ�
	// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);
		break;
	case FADETYPE_SLIDE:	// �X���C�h�t�F�[�h�������Ƃ�
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetResource(TEX_FADE));
		break;
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void CFade::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffFade, NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuffFade->Unlock();
}

//=============================================================================
// ���_���̍X�V
//=============================================================================
void CFade::SetTransform(void)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - SCREEN_WIDTH / 2, m_pos.y - SCREEN_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + SCREEN_WIDTH / 2, m_pos.y - SCREEN_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - SCREEN_WIDTH / 2, m_pos.y + SCREEN_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + SCREEN_WIDTH / 2, m_pos.y + SCREEN_HEIGHT / 2, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuffFade->Unlock();
}

//=============================================================================
// �ʏ�t�F�[�h
//=============================================================================
void CFade::NormalFade(void)
{
	switch (m_fade)
	{
	case FADE_IN:	// �t�F�[�h�C���������Ƃ�
		m_colorFade.a -= 0.05f;			// ��ʂ𓧖��ɂ��Ă���
		if (m_colorFade.a <= 0.0f)
		{// ���l��0�ȉ��̂Ƃ�
			m_colorFade.a = 0.0f;		// ���l��0�ɂ���
			m_fade = FADE_NONE;			// �t�F�[�h��Ԃ�ύX
		}
		break;
	case FADE_OUT:	// �t�F�[�h�A�E�g�������Ƃ�
		m_colorFade.a += 0.05f;			// ��ʂ��Â����Ă���
		if (m_colorFade.a > 1.5f)
		{// ���l��1.5�ȏゾ�����Ƃ�
		 //�t�F�[�h�C�������ɐ؂�ւ�
			m_colorFade.a = 1.0f;		// ���l��1.0�ɂ���
			m_fade = FADE_IN;			// �t�F�[�h��Ԃ�ύX

			//���[�h�ؑ�
			CManager::SetMode(m_modeNext);
		}
		break;
	}
}

//=============================================================================
// �X���C�h�t�F�[�h
//=============================================================================
void CFade::SlideFade(void)
{
	switch (m_fade)
	{
	case FADE_IN:	// �t�F�[�h�C���������Ƃ�
		if (m_nCntAction > ACTION_COUNT)
		{
			m_colorFade.a -= 0.05f;			// ��ʂ𓧖��ɂ��Ă���
			if (m_colorFade.a <= 0.0f)
			{// ���l��0�ȉ��̂Ƃ�
				m_colorFade.a = 0.0f;		// ���l��0�ɂ���
				m_fade = FADE_NONE;			// �t�F�[�h��Ԃ�ύX
			}
		}
		else
		{
			m_nCntAction++;					// �J�E���^�Ƀv���X����
		}
		break;
	case FADE_OUT:	// �t�F�[�h�A�E�g�������Ƃ�
		m_move.y += 0.5f;			// ��ʂ��Â����Ă���
		if (m_pos.y >= SCREEN_HEIGHT / 2)
		{// �ʒu��640�ȏゾ�����Ƃ�
			if (m_bReflection)
			{
				//�t�F�[�h�C�������ɐ؂�ւ�
				m_pos.y = SCREEN_HEIGHT / 2;		// �ʒu��360�ɂ���
				m_move = D3DXVECTOR3_ZERO;			// �ړ��ʂ�0�ɂ���
				m_fade = FADE_IN;					// �t�F�[�h��Ԃ�ύX

				//���[�h�ؑ�
				CManager::SetMode(m_modeNext);
			}
			else
			{
				m_pos.y = SCREEN_HEIGHT / 2;		// �ʒu��360�ɂ���

				m_bReflection = true;
				m_move *= -0.5f;
			}
		}

		m_pos += m_move;
		break;
	}
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CFade *CFade::Create(void)
{
	CFade *pFade;
	pFade = new CFade();

	if (pFade != NULL)
	{// �t�F�[�h�����݂��Ă����Ƃ�
		pFade->Init(CManager::MODE_TITLE);				// ����������
	}
	return pFade;
}

//=============================================================================
// �t�F�[�h�̏�Ԑݒ�
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext, FADETYPE type)
{
	m_fade = FADE_OUT;										// �t�F�[�h��Ԃ̐ݒ�
	m_fadeType = type;										// �t�F�[�h�^�C�v
	m_modeNext = modeNext;									// ���̃��[�h������

	switch (m_fadeType)
	{
	case FADETYPE_NORMAL:	// �m�[�}���t�F�[�h�������Ƃ�
		m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//�������(����)
		m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		break;
	case FADETYPE_SLIDE:	// �X���C�h�t�F�[�h�������Ƃ�
		m_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�������(����)
		m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f - SCREEN_HEIGHT / 2, 0.0f);
		m_nCntAction = 0;
		m_bReflection = false;
		break;
	}
}