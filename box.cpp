//==================================================================================================================
//
// box[box.cpp]
// Author:Ryouma Inoue
//
//==================================================================================================================
#include "manager.h"
#include "renderer.h"
#include "Box.h"
#include "inputKeyboard.h"
#include "player.h"
#include "game.h"
#include "scene2D.h"
#include "takaseiLibrary.h"
#include "piece.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define WhileX 50.0f																// イチマスの長さ横
#define WhileY 50.0f																// イチマスの長さ高さ
#define WhileZ 50.0f																// イチマスの長さ縦

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CBox::m_pTexture = NULL;			// テクスチャ変数

//==================================================================================================================
// グローバル変数
//==================================================================================================================

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CBox::CBox(PRIORITY type = CScene::PRIORITY_FLOOR) :CScene(type)
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CBox::~CBox()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
HRESULT CBox::Init(void)
{
	// 初期化
	m_nCntMove_X = 0;
	m_nCntMove_Y = 0;
	m_bPlacement = false;
	m_bCreate = false;
	m_bMove = false;

	for (int nCntDepth = 0; nCntDepth < Box_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Box_Width; nCntWidth++)
		{
			m_bPuzzle[nCntDepth][nCntWidth] = false;

			m_pBlock[nCntDepth][nCntWidth] = CScene2D::Create(PRIORITY_UI);

			if (m_pBlock[nCntDepth][nCntWidth] != NULL)
			{
				m_pBlock[nCntDepth][nCntWidth]->SetPosition(D3DXVECTOR3(100.0f + nCntWidth * 55.0f,
					100.0f + nCntDepth * 55.0f,
					0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetTransform();
			}
		}
	}
	m_pPiece = CPiece::Create();


	//m_bPuzzle[nCntMove_Y][nCntMove_X] = true;
	//m_bPuzzle[nCntMove_Y][nCntMove_X + 1] = true;
	//m_bPuzzle[nCntMove_Y + 1][nCntMove_X] = true;
	//m_bPuzzle[nCntMove_Y + 1][nCntMove_X + 1] = true;

	return S_OK;
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CBox::Uninit(void)
{

}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CBox::Update(void)
{
	// キーボード取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	m_bPiece = m_pPiece->GetPlaacement();


	if (m_bPiece == true)
	{
		CPiece::Create();
		m_pPiece->SetPlaacement(false);
	}
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CBox::Draw(void)
{

}

//==================================================================================================================
// ポリゴン生成
//==================================================================================================================
CBox *CBox::Create(void)
{
	// シーン動的に確保
	CBox *pBox = new CBox(CScene::PRIORITY_FLOOR);

	if (pBox != NULL)
	{
		// シーン初期化
		pBox->Init();
	}

	// 値を返す
	return pBox;
}

//==================================================================================================================
// テクスチャ情報ロード
//==================================================================================================================
HRESULT CBox::Load(void)
{
	return S_OK;
}

//==================================================================================================================
// テクスチャ情報破棄
//==================================================================================================================
void CBox::Unload(void)
{
	// テクスチャの開放

}