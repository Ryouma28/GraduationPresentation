//=============================================================================
//
// ���[�U�[�C���^�[�t�F�[�X���� [ui.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "write.h"
#include "counter.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CUi::CUi(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene(obj)
{
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_currentPos = D3DXVECTOR3_ZERO;
	m_currentCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCurrentFrame = 0;
	m_nCurrentPoint = 0;
	m_nOpTypeNow = 0;

	m_vAsset = {};
	m_vOpType = {};
	m_vPoint = {};
	m_CntMap = {};
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUi::~CUi()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CUi::Init(void)
{
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CUi::Uninit(void)
{
	Delete();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUi::Update(void)
{
	if (m_vOpType.size() > m_nOpTypeNow)
	{
		switch (m_vOpType[m_nOpTypeNow])
		{
		case OPERATIONTYPE_MOVE:
			PointMove();
			break;
		case OPERATIONTYPE_DELETE:
			Delete();
			break;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUi::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CUi *CUi::Create(void)
{
	CUi *pUi;		// �w�i�̃|�C���^

	pUi = new CUi(CScene::PRIORITY_UI);		// �w�i�̐���

	if (pUi == NULL) { return NULL; }
	pUi->Init();							// �w�i�̏�����

	return pUi;
}

//==============================================================================
// �A�Z�b�g�̐�������
//==============================================================================
HRESULT CUi::Load(void)
{
	//�e�N�X�`���̓ǂݍ���
	return S_OK;
}

//==============================================================================
// �X�N���v�g�f�[�^���[�h����
//==============================================================================
bool CUi::LoadScript(const std::string &add)
{
	FILE *pFile;
	char cReadText[128];		//����
	char cHeadText[128];		//��r
	char cDie[128];
	int nCntPointer = 0;		//�|�C���^�[�̐��l

	char sAdd[64];				//���f���̃A�h���X
	std::string Add;

	int nCntMotion = 0;			//�Q�Ƃ���|�C���^�̒l��������
	int nCntKey = 0;

	int nMaxModel = 0;

	//�e�L�X�g�f�[�^���[�h
	pFile = fopen(add.c_str(), "r");

	if (pFile != NULL)
	{
		//�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//�X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//�G���h�X�N���v�g������܂�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//���s
				if (strcmp(cReadText, "\n") == 0)
				{
					continue;
				}
				else if (strcmp(cHeadText, "UISET") == 0)
				{//�L�����N�^�[�̏������̂Ƃ�
					// �쐬
					CScene2D *scene = CScene2D::Create(CScene2D::PRIORITY_UI);

					scene->SetCenter(CScene2D::TEXTUREVTX_CENTER);		// ���S �̐ݒ�
					scene->SetSize(D3DXVECTOR3(100.0f, 70.0f, 0.0f));	// �傫�� �̐ݒ�
					scene->SetRotation(D3DXVECTOR3(-0.79f, -0.79f, 0.0f));
					scene->SetPosition(D3DXVECTOR3(320.0f, 150.0f, 0.0f));	// �ʒu �̐ݒ�
					scene->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F �̐ݒ�
					scene->SetTransform();		// ���_��� �̐ݒ�

					if (scene == NULL)
					{// �����Ɏ��s�����Ƃ�
						return false;
					}

					// �z��̍Ō���ɓ����
					m_vAsset.push_back(scene);

					while (strcmp(cHeadText, "END_UISET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
						{// �A�h���X���̂Ƃ�
							sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾
							Add = sAdd;
							scene->BindTexture(Add);
						}
						else if (strcmp(cHeadText, "POS") == 0)
						{//�ʒu
							D3DXVECTOR3 pos;
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&pos.x,
								&pos.y,
								&pos.z);

							// �ʒu����ݒ肷��
							scene->SetPosition(pos);
							scene->SetTransform();
						}
						else if (strcmp(cHeadText, "ROT") == 0)
						{//��]
							D3DXVECTOR3 rot;
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&rot.x,
								&rot.y,
								&rot.z);

							// ��]����ݒ肷��
							scene->SetRotation(rot);
							scene->SetTransform();
						}
						else if (strcmp(cHeadText, "SIZE") == 0)
						{//��]
							D3DXVECTOR3 size;
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&size.x,
								&size.y,
								&size.z);

							// ��]����ݒ肷��
							scene->SetSize(size);
							scene->SetTransform();
						}
						else if (strcmp(cHeadText, "COL") == 0)
						{//��]
							D3DXCOLOR col;
							sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie,
								&col.r,
								&col.g,
								&col.b,
								&col.a);

							// ��]����ݒ肷��
							scene->SetColor(col);
							scene->SetTransform();
						}
					}
				}
				else if (strcmp(cHeadText, "COUNTERSET") == 0)
				{//�J�E���^�[���̂Ƃ�
				 // �쐬
					std::string Add = {};
					std::string Tag = {};
					D3DXVECTOR3 pos = D3DXVECTOR3_ZERO;
					D3DXVECTOR3 size = D3DXVECTOR3_ZERO;
					D3DXVECTOR3 interval = D3DXVECTOR3_ZERO;
					int nLength = 1;

					while (strcmp(cHeadText, "END_COUNTERSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
						{// �A�h���X���̂Ƃ�
							sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾
							Add = sAdd;
						}
						else if (strcmp(cHeadText, "TAG") == 0)
						{//�ʒu
							sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾
							Tag = sAdd;
						}
						else if (strcmp(cHeadText, "LENGTH") == 0)
						{//�ʒu
							sscanf(cReadText, "%s %s %d", &cDie, &cDie,
								&nLength);
						}
						else if (strcmp(cHeadText, "POS") == 0)
						{//�ʒu
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&pos.x,
								&pos.y,
								&pos.z);
						}
						else if (strcmp(cHeadText, "SIZE") == 0)
						{//��]
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&size.x,
								&size.y,
								&size.z);
						}
						else if (strcmp(cHeadText, "INTERVAL") == 0)
						{//��]
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&interval.x,
								&interval.y,
								&interval.z);
						}
					}

					CCounter *pCounter = CCounter::Create(nLength);

					if (pCounter != NULL)
					{
						pCounter->BindTexture(Add);
						pCounter->SetSize(size);	// �傫�� �̐ݒ�
						pCounter->SetPosition(pos);	// �ʒu �̐ݒ�
						pCounter->SetIntervalNum(interval);
						pCounter->SetNumber(35);
						pCounter->SetTransform();		// ���_��� �̐ݒ�

						// �}�b�v�ɓ����
						m_CntMap.insert(std::map<std::string, CCounter*>::value_type(Tag, pCounter));
					}
				}
				else if (strcmp(cHeadText, "TASKSET") == 0)
				{// �^�X�N����
					MOVEPOINT point;
					int nType = -1;

					while (strcmp(cHeadText, "END_TASKSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						if (strcmp(cHeadText, "TYPE") == 0)
						{// �A�h���X���̂Ƃ�
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nType);						//�A�h���X�̎擾
							m_vOpType.push_back((OPERATIONTYPE)nType);
						}
						else if (strcmp(cHeadText, "POS") == 0)
						{//�ʒu
							D3DXVECTOR3 pos = D3DXVECTOR3_ZERO;
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&pos.x,
								&pos.y,
								&pos.z);

							// �ʒu����ݒ肷��
							point.pos = pos;
						}
						else if (strcmp(cHeadText, "COL") == 0)
						{//��]
							D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
							sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie,
								&col.r,
								&col.g,
								&col.b,
								&col.a);

							// �F����ݒ肷��
							point.col = col;
						}
						else if (strcmp(cHeadText, "FRAME") == 0)
						{//��]
							int nFrame = 0;
							sscanf(cReadText, "%s %s %d", &cDie, &cDie,
								&nFrame);

							// �t���[������ݒ肷��
							point.nFrame = nFrame;
						}
					}

					if (nType == OPERATIONTYPE_MOVE)
					{
						// �����i�[����
						m_vPoint.push_back(point);
					}
				}
			}
		}

		//�t�@�C����
		fclose(pFile);
	}
	else
	{

	}

	return false;
}

//==============================================================================
// �e�N�X�`������
//==============================================================================
void CUi::CreateTexture(std::string Add)
{
	CScene2D *pScene2D = CScene2D::Create(CScene2D::PRIORITY_UI);

	if (pScene2D != NULL)
	{
		pScene2D->BindTexture(Add);			// �e�N�X�`���̐ݒ�
		if (pScene2D->GetActive())
		{
			pScene2D->SetActive(true);
		}
		m_vAsset.push_back(pScene2D);								// �Ō���ɓ����
	}
}

//==============================================================================
// �e�N�X�`���폜
//==============================================================================
void CUi::DeleteTexture(int nIndex)
{
	if (m_vAsset[nIndex] == NULL) { return; }			// ���g�����݂��Ă��Ȃ��Ƃ�

	m_vAsset[nIndex]->Release();			// �폜�\��
	m_vAsset[nIndex] = NULL;				// NULL����
	m_vAsset.erase(m_vAsset.begin() + nIndex);	// �w��v�f���폜
}

//==============================================================================
// �f�o�b�O��������
//==============================================================================
void CUi::SceneDebug(void)
{
	for (unsigned int nCount = 0; nCount < m_vAsset.size(); nCount++)
	{
		char aTag[16];
		memset(&aTag, 0, sizeof(aTag));
		sprintf(aTag, "TEXTURE [%d]", nCount);
		D3DXVECTOR3 pos, rot, size;
		D3DXCOLOR col;
		bool bActive;

		if (ImGui::CollapsingHeader(aTag))
		{
			pos = m_vAsset[nCount]->GetPosition();
			rot = m_vAsset[nCount]->GetRotation();
			size = m_vAsset[nCount]->GetSize();
			col = m_vAsset[nCount]->GetColor();
			bActive = m_vAsset[nCount]->GetActive();

			ImGui::DragFloat3("pos", (float*)&pos);
			ImGui::DragFloat3("rot", (float*)&rot);
			ImGui::DragFloat3("size", (float*)&size);
			ImGui::DragFloat4("col", (float*)&col, 0.01f);

			m_vAsset[nCount]->SetPosition(pos);
			m_vAsset[nCount]->SetRotation(rot);
			m_vAsset[nCount]->SetSize(size);
			m_vAsset[nCount]->SetColor(col);

			if (bActive != m_vAsset[nCount]->GetActive())
			{
				m_vAsset[nCount]->SetActive(bActive);
			}

			m_vAsset[nCount]->SetTransform();

			if (ImGui::Button("delete"))
			{
				DeleteTexture(nCount);
			}
		}
	}
}

//==============================================================================
// �X�N���v�g�̏����o������
//==============================================================================
void CUi::SaveScript(std::string Add)
{
	CWrite *pWrite = new CWrite;
	if (pWrite == NULL) return;

	//�ϐ��錾
	char text[64];				// �e�L�X�g
	char cEqual[8] = { "=" };	//�C�R�[���p
	CScene *pSceneNext = NULL;	//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;

	D3DXVECTOR3 pos = GetPosition();

	std::string Full = "data/text/";
	Full += Add;
	Full += ".txt";

	//�J����
	if (pWrite->Open(Full))
	{
		strcpy(text, "# UI�f�[�^�X�N���v�g\n");
		strcat(text, "# Author : masayasu wakita\n");

		pWrite->TitleWrite(text);				// �^�C�g���̌`���ŏ�������
		pWrite->Write("SCRIPT\n");			// �X�N���v�g�̏I���錾
		pWrite->Write("\n");

		// ���f���̏�� //
		pWrite->IndexWrite("UI�̏��");

		D3DXVECTOR3 assetPos, rot, size;
		D3DXCOLOR col;

		for (unsigned int nCount = 0; nCount < m_vAsset.size(); nCount++)
		{
			assetPos = m_vAsset[nCount]->GetPosition() - pos;
			rot = m_vAsset[nCount]->GetRotation();
			size = m_vAsset[nCount]->GetSize();
			col = m_vAsset[nCount]->GetColor();

			pWrite->Write("UISET\n");					// ���_���̏������݊J�n�錾
			pWrite->Write("	TEXTURE_FILENAME = %s\n", m_vAsset[nCount]->GetAdd().c_str());
			pWrite->Write("	POS = %.2f %.2f %.2f\n", assetPos.x, assetPos.y, assetPos.z);		// ���S�ʒu�̏�������
			pWrite->Write("	ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);		// ���S�ʒu�̏�������
			pWrite->Write("	SIZE = %.2f %.2f %.2f\n", size.x, size.y, size.z);		// ���S�ʒu�̏�������
			pWrite->Write("	COL = %.2f %.2f %.2f %.2f\n", col.r, col.g, col.b, col.a);	// �F���̏�������
			pWrite->Write("END_UISET\n");				// ���_���̏������݊J�n�錾
			pWrite->Write("\n");							// ���s
		}

		pWrite->Write("END_SCRIPT\n");			// �X�N���v�g�̏I���錾
		pWrite->End();

		MessageBox(NULL, "�����蔻��̏������ݏI�����܂����I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//==============================================================================
// UI�̈ʒu
//==============================================================================
void CUi::SetPosition(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 assetPos = D3DXVECTOR3_ZERO;
	D3DXVECTOR3 thisPos = GetPosition();

	for (unsigned int nCount = 0; nCount < m_vAsset.size(); nCount++)
	{
		if (m_vAsset[nCount] == NULL) continue;				// �t�@�C����������Ύ���

		assetPos = m_vAsset[nCount]->GetPosition() - thisPos;				// �O�̈ʒu���擾
		m_vAsset[nCount]->SetPosition(assetPos + pos);			// �V�����ʒu�ɒu��
		m_vAsset[nCount]->SetTransform();
	}

	for (auto itr = m_CntMap.begin(); itr != m_CntMap.end(); itr++)
	{
		if (itr->second == NULL) continue;				// �t�@�C����������Ύ���

		assetPos = itr->second->GetPosition() - thisPos;	// �O�̈ʒu���擾
		itr->second->SetPosition(assetPos + pos);			// �V�����ʒu�ɒu��
		itr->second->SetTransform();

	}

	CScene::SetPosition(pos);
}

//==============================================================================
// UI�̐F
//==============================================================================
void CUi::SetColor(D3DXCOLOR col)
{
	for (unsigned int nCount = 0; nCount < m_vAsset.size(); nCount++)
	{
		if (m_vAsset[nCount] == NULL) continue;				// �t�@�C����������Ύ���

		m_vAsset[nCount]->SetColor(col);			// �V�����ʒu�ɒu��
		m_vAsset[nCount]->SetTransform();
	}
}

//==============================================================================
// ���̏����Ɉڍs
//==============================================================================
void CUi::SetNextOperation(void)
{
	m_nOpTypeNow++;
}

//==============================================================================
// UI�̒l�ړ�
//==============================================================================
CCounter *CUi::GetCounter(std::string Tag)
{
	std::map<std::string, CCounter*>::const_iterator it = m_CntMap.find(Tag);

	if (it == m_CntMap.end())
	{// ������Ȃ������Ƃ�
		return NULL;
	}
	else
	{
		return it->second;
	}
}

//==============================================================================
// UI�̒l�ړ�
//==============================================================================
void CUi::PointMove(void)
{
	D3DXVECTOR3 pos = GetPosition();

	// �ʒu
	if (m_nCurrentFrame == 0)
	{
		m_currentPos = (m_vPoint[m_nCurrentPoint].pos - pos) / (float)m_vPoint[m_nCurrentPoint].nFrame;
		m_currentCol = (m_vPoint[m_nCurrentPoint].col - m_col) / (float)m_vPoint[m_nCurrentPoint].nFrame;
	}

	pos += m_currentPos;
	m_col += m_currentCol;

	SetPosition(pos);
	SetColor(m_col);

	m_nCurrentFrame++;

	if (m_vPoint[m_nCurrentPoint].nFrame <= m_nCurrentFrame)
	{
		// �^�X�N������
		m_nOpTypeNow++;
		m_nCurrentPoint++;
		m_nCurrentFrame = 0;
	}
}

//==============================================================================
// �폜
//==============================================================================
void CUi::Delete(void)
{
	for (unsigned int nCount = 0; nCount < m_vAsset.size(); nCount++)
	{
		if (m_vAsset[nCount] == NULL) continue;				// �t�@�C����������Ύ���

		// �J������
		m_vAsset[nCount]->Release();
		m_vAsset[nCount] = NULL;
	}

	// �z��̗v�f���폜
	m_vAsset.clear();

	// �폜�\��
	Release();
}