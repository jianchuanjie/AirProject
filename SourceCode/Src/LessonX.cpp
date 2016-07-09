/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "List.h"

SGameSprite g_ControlSprite;
SGameSprite g_ControlSprite2;
SGameSprite g_VerTemplate;
SGameSprite g_Bullet1Template;
SGameSprite *CreateBullet1(const int iBulletType, const float fPosX, const float fPosY);
SGameSprite g_BossTemplate;
SGameSprite g_EliteTemplate;


//��������ȫ�ֱ���
float		g_fWorldLeft = 0.f;	// �����������ұ߽�
float		g_fWorldRight = 10.f;
float		g_fWorldTop = 0.f;
float		g_fWorldBottom = 10.f;
//
float		g_fVelocityLeft = 0.f;	// ���Ʒɻ������������ٶ�
float		g_fVelocityRight = 0.f;
float		g_fVelocityUp = 0.f;
float		g_fVelocityDown = 0.f;
float		g_fVelocityLeft2 = 0.f;	// ���Ʒɻ�2�����������ٶ�
float		g_fVelocityRight2 = 0.f;
float		g_fVelocityUp2 = 0.f;
float		g_fVelocityDown2 = 0.f;
float		g_fVerCreateTime = 0.f;
float		g_fBossCtreateTime = 0.f;
float		g_fEliteCreateTime = 0.f;


bool		g_bControlCanFire;
bool		g_bControlCanFire2;
int			g_iCreatedSpriteCount = 0;
int			g_iMaxScore = 0;
int			g_iMaxScore2 = 0;
bool		g_bIsTwoPlayer = false;
bool		g_isPlayer1Alive;
bool		g_isPlayer2Alive;


//�������躯��
void CreateVerTick(float fDeltaTime);
void CreateBossTick(float fDeltaTime);
void CreateEliteTick(float fDeltaTime);

void VerLoopTick(SGameSprite *pSprite, float fDeltaTime);
bool IsDead(SGameSprite *pSprite);
void ControlColOther(SGameSprite *pControl, SGameSprite *pOther);
void Bullet1ColOther(SGameSprite *pBullet, SGameSprite *pOther);
void EnemyColOther(SGameSprite *pEnemy, SGameSprite *pOther);
bool IsControler(const char *szName);
bool IsControlSprite(const char *szName);
bool IsControlSprite2(const char *szName);
bool IsControlSpriteBullet(SGameSprite *pSprite);
bool IsGameLost();
void UpdateMovement2();
bool IsControlSpriteBullet2(SGameSprite *pSprite);




// ��ʼ������ģ������ 
void InitTemplateData()
{
	// ���ǿ��Ƶ�Control Sprite
	strcpy(g_ControlSprite.szName, "ControlSprite");
	g_ControlSprite.iHp = CONTROL_MAX_HP;
	g_ControlSprite.iScore = 0;
	g_ControlSprite.iDamage = 500;	// �л������ң�Ҳ�ᱻ����...
	g_ControlSprite.eSpriteType = SPRITE_CONTROL;
	g_ControlSprite.iBulletType = 0;		// Unused
	g_ControlSprite.fFireAfterCreate = 0.f;	// Unused
	g_ControlSprite.fBulletTime = 0.3f;	// 0.3�뷢��һ���ӵ�
	g_ControlSprite.fBulletTime2 = 0.f;	// Unused
	g_ControlSprite.fFloatTime = 0.f;	// ���������¸�������
	g_ControlSprite.bFloatUp = true;
	strcpy(g_ControlSprite2.szName, "ControlSprite2");
	g_ControlSprite2.iHp = CONTROL_MAX_HP;
	g_ControlSprite2.iScore = 0;
	g_ControlSprite2.iDamage = 500;	// �л������ң�Ҳ�ᱻ����...
	g_ControlSprite2.eSpriteType = SPRITE_CONTROL;
	g_ControlSprite2.iBulletType = 0;		// Unused
	g_ControlSprite2.fFireAfterCreate = 0.f;	// Unused
	g_ControlSprite2.fBulletTime = 0.3f;	// 0.3�뷢��һ���ӵ�
	g_ControlSprite2.fBulletTime2 = 0.f;	// Unused
	g_ControlSprite2.fFloatTime = 0.f;	// ���������¸�������
	g_ControlSprite2.bFloatUp = true;
	g_isPlayer1Alive = true;
	g_isPlayer2Alive = g_bIsTwoPlayer ? true : false;
	


	//	�з�ս��
	g_VerTemplate.szName[0] = '\0';
	g_VerTemplate.iHp = 100;	// ����ֵ100
	g_VerTemplate.iScore = 50;		// ����50
	g_VerTemplate.iDamage = 500;	// ���������˺�500
	g_VerTemplate.eSpriteType = SPRITE_VER;
	g_VerTemplate.iBulletType = 0;		// Unused
	g_VerTemplate.fFireAfterCreate = 1.f;	// ����1s��ʼ�����ӵ�
	g_VerTemplate.fBulletTime = 1.f;	// 1�뷢��һ���ӵ�
	g_VerTemplate.fBulletTime2 = 0.f;
	g_VerTemplate.fFloatTime = 0.f;	// ���������¸�������
	g_VerTemplate.bFloatUp = true;
	//	�з�BOSSս��
	g_BossTemplate.szName[0] = '\0';
	g_BossTemplate.iHp = 1000;	// ����ֵ100
	g_BossTemplate.iScore = 50;		// ����50
	g_BossTemplate.iDamage = 1000;	// ���������˺�500
	g_BossTemplate.eSpriteType = SPRITE_VER;
	g_BossTemplate.iBulletType = 0;		// Unused
	g_BossTemplate.fFireAfterCreate = 1.f;	// ����1s��ʼ�����ӵ�
	g_BossTemplate.fBulletTime = 1.f;	// 1�뷢��һ���ӵ�
	g_BossTemplate.fBulletTime2 = 0.f;
	g_BossTemplate.fFloatTime = 0.f;	// ���������¸�������
	g_BossTemplate.bFloatUp = true;
	//	�з�Eliteս��
	g_EliteTemplate.szName[0] = '\0';
	g_EliteTemplate.iHp = 300;	// ����ֵ100
	g_EliteTemplate.iScore = 50;		// ����50
	g_EliteTemplate.iDamage = 500;	// ���������˺�500
	g_EliteTemplate.eSpriteType = SPRITE_VER;
	g_EliteTemplate.iBulletType = 0;		// Unused
	g_EliteTemplate.fFireAfterCreate = 1.f;	// ����1s��ʼ�����ӵ�
	g_EliteTemplate.fBulletTime = 1.f;	// 1�뷢��һ���ӵ�
	g_EliteTemplate.fBulletTime2 = 0.f;
	g_EliteTemplate.fFloatTime = 0.f;	// ���������¸�������
	g_EliteTemplate.bFloatUp = true;
	//	�ӵ�
	g_Bullet1Template.szName[0] = '\0';
	g_Bullet1Template.iHp = 1;		// ����ֵ1
	g_Bullet1Template.iScore = 10;		// ����10
	g_Bullet1Template.iDamage = 0;		// �˺�ֵ�ɷ����Sprite����
	g_Bullet1Template.eSpriteType = SPRITE_BULLET1;
	g_Bullet1Template.iBulletType = 0;		// �����ɷ����Sprite����
	g_Bullet1Template.fFireAfterCreate = 0.f;	// Unused
	g_Bullet1Template.fBulletTime = 0.0f;	// Unused
	g_Bullet1Template.fBulletTime2 = 0.f;	// Unused
	g_Bullet1Template.fFloatTime = 0.f;	// ���������¸�������
	g_Bullet1Template.bFloatUp = true;
}






////////////////////////////////////////////////////////////////////////////////
//
//
int			g_iGameState		=	0;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();
//==============================================================================
//
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�


//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬. 
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void GameMainLoop( float	fDeltaTime )
{
	switch( g_iGameState )
	{
		// ��ʼ����Ϸ�������һ���������
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
		break;

		// ��Ϸ�����У����������Ϸ�߼�
	case 2:
	{
				// �ж���Ӯ
		if (IsGameLost())
		{
			// ��Ϸ������������Ϸ���㺯��������Ϸ״̬�޸�Ϊ����״̬
			g_iGameState = 0;
			GameEnd();
		}
		else // ��Ϸδ������������Ϸ
		{
			GameRun(fDeltaTime);
		}
	}
	break;
		// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
void GameInit()
{
	//��ȡ����߽�
	g_fWorldLeft = dGetScreenLeft();
	g_fWorldRight = dGetScreenRight();
	g_fWorldTop = dGetScreenTop();
	g_fWorldBottom = dGetScreenBottom();

	g_fVelocityLeft = 0.f;
	g_fVelocityRight = 0.f;
	g_fVelocityUp = 0.f;
	g_fVelocityDown = 0.f;

	//�з�ս������ʱ��
	g_fVerCreateTime = 2.f;
	g_fBossCtreateTime = 15.f;
	g_fEliteCreateTime = 5.f;

	g_bControlCanFire = false;
	// ��ʼ��Control Sprite����ֵ
	g_ControlSprite.iHp = CONTROL_MAX_HP;
	g_ControlSprite.iScore = 0;
	g_ControlSprite.fBulletTime = 0.3f;

	// ��ʼ��Control Sprite2����ֵ
	if (g_bIsTwoPlayer == true)
	{
		g_ControlSprite2.iHp = CONTROL_MAX_HP;
		g_ControlSprite2.iScore = 0;
		g_ControlSprite2.fBulletTime = 0.3f;
	}

	InitTemplateData();

	// ���� "���ո�ʼ��Ϸ" �����ʾͼƬ 
	dSetSpriteVisible("GameBegin", false);

	//��ʾ���
	dSetSpriteVisible("ControlSprite", true);
	if (g_bIsTwoPlayer==true)
		dSetSpriteVisible("ControlSprite2", true);

	//�������
	dSetSpriteEnable("ControlSprite", true);
	if (g_bIsTwoPlayer == true)
		dSetSpriteEnable("ControlSprite2", true);

	//Ѫ����ʾ����
	dSetSpriteVisible("iHp", true);
	if (g_bIsTwoPlayer == true)
	{
		dSetSpriteVisible("iHp2", true);
	}


	static bool	bInited = false;
	if (!bInited)
	{
		// ���ļ����ȡ��ʷ��߻���
		// ������ֻ��¼��һ����ֵ������Ȥ�Ļ���������������ֵ�����Ȼ����д洢���ȡ
		// ������Ϸ��������ļ��Ĵ洢����Ϊ��ѧ����
		FILE *pfp = fopen("Score.dat", "rb");
		if (NULL != pfp)
		{
			fread(&g_iMaxScore, sizeof(g_iMaxScore), 1, pfp);
			fread(&g_iMaxScore2, sizeof(g_iMaxScore2), 1, pfp);
			fclose(pfp);
			//����������
			dSetTextValue("MaxScoreText", g_iMaxScore);
			dSetTextValue("MaxScoreText2", g_iMaxScore2);
		}
	}



}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{

	g_ControlSprite.fBulletTime -= fDeltaTime;
	if (g_ControlSprite.fBulletTime < 0 && g_bControlCanFire)
	{
		g_ControlSprite.fBulletTime = 0.3;
		CreateBullet1(0, dGetSpritePositionX(g_ControlSprite.szName), dGetSpritePositionY(g_ControlSprite.szName));
	}

	if (g_bIsTwoPlayer == true)
	{
		g_ControlSprite2.fBulletTime -= fDeltaTime;
		if (g_ControlSprite2.fBulletTime < 0 && g_bControlCanFire2)
		{
			g_ControlSprite2.fBulletTime = 0.3;
			CreateBullet1(2, dGetSpritePositionX(g_ControlSprite2.szName), dGetSpritePositionY(g_ControlSprite2.szName));
		}
	}
	CreateVerTick(fDeltaTime);
	CreateBossTick(fDeltaTime);
	CreateEliteTick(fDeltaTime);
	if (g_isPlayer1Alive)
		dSetTextValue("iHp", g_ControlSprite.iHp);
	if (g_isPlayer2Alive)
		dSetTextValue("iHp2", g_ControlSprite2.iHp);
	// ������ǰ�Ѿ�������Sprite��ִ�и��Ե�ѭ������(���Ե��ӵ������)
	int	iListSize = GList_GetListSize();
	int	iLoop = 0;
	for (iLoop = 0; iLoop < iListSize; iLoop++)
	{
		SGameSprite	*pSprite = GList_GetSpriteByIndex(iLoop);
		if (NULL != pSprite)
		{
			switch (pSprite->eSpriteType)
			{
			case SPRITE_VER:
				VerLoopTick(pSprite, fDeltaTime);
				break;
			};
		}
	}



}
//==============================================================================
//
// ������Ϸ����
void GameEnd()
{

	//�����Ƶ�Sprite�ٶȹ��㣬���������Ļ���
	dSetSpriteLinearVelocity(g_ControlSprite.szName, 0.f, 0.f);
	dSetSpritePosition(g_ControlSprite.szName, g_fWorldLeft + 5.f, 0.f);
	dSetSpriteLinearVelocity(g_ControlSprite2.szName, 0.f, 0.f);
	dSetSpritePosition(g_ControlSprite2.szName, g_fWorldLeft + 5.f, 0.f);
	// �ָ�������ɫ
	dSetSpriteColorGreen(g_ControlSprite.szName, 255);
	dSetSpriteColorBlue(g_ControlSprite.szName, 255);
	dSetSpriteColorGreen(g_ControlSprite2.szName, 255);
	dSetSpriteColorBlue(g_ControlSprite2.szName, 255);
	// ɾ�����ֵ�����Sprite
	GList_DeleteAllSprite();

	// ��ʾ "���ո�ʼ��Ϸ" �����ʾͼƬ
	dSetTextValue("CurScoreText", 0);
	dSetTextValue("CurScoreText2", 0);
	dSetSpriteVisible("GameBegin", true);
	dSetTextValue("iHp", 0);
	dSetTextValue("iHp2", 0);


	g_bControlCanFire = false;
	g_bControlCanFire2 = false;
	g_bIsTwoPlayer = false;

	//����Ѫ��
	dSetSpriteVisible("iHp", false);
	dSetSpriteVisible("iHp2", false);


	if (g_iMaxScore < g_ControlSprite.iScore)
	{
		g_iMaxScore = g_ControlSprite.iScore;

		// д�ļ�
		// [Your Code]
		FILE *pfp = fopen("Score.dat", "wb");
		if (NULL != pfp)
		{
			fwrite(&g_iMaxScore, sizeof(g_iMaxScore), 1, pfp);
			fclose(pfp);
		}
		// [End Your Code]

		//����������
		dSetTextValue("MaxScoreText", g_iMaxScore);

	}


	if (g_iMaxScore2 < g_ControlSprite2.iScore)
	{
		g_iMaxScore2 = g_ControlSprite2.iScore;

		// д�ļ�
		// [Your Code]
		FILE *pfp = fopen("Score.dat", "wb");
		if (NULL != pfp)
		{
			fwrite(&g_iMaxScore2, sizeof(g_iMaxScore2), 1, pfp);
			fclose(pfp);
		}
		// [End Your Code]

		//����������
		dSetTextValue("MaxScoreText2", g_iMaxScore2);

	}


}
//==========================================================================
//
// ����ƶ�
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseMove(const float fMouseX, const float fMouseY)
{

}
//==========================================================================
//
// �����
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY)
{

}
//==========================================================================
//
// ��굯��
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseUp(const int iMouseType, const float fMouseX, const float fMouseY)
{

}
//==========================================================================
//
// ���̰���
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
void OnKeyDown(const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress)
{
	// ���¿ո���Ϸ��ʼ

	if (KEY_SPACE == iKey && 0 == g_iGameState)
	{
		g_iGameState = 1;
	}
	else if (KEY_2 == iKey && 0 == g_iGameState)
	{
		g_iGameState = 1;
		g_bIsTwoPlayer = true;

	}
	else if (2 == g_iGameState)
	{
		if (g_isPlayer1Alive)
		{
			// W A S D�������ƶ����������£���ĳ������ٶȱ�����ֵ
			if (KEY_A == iKey)
				g_fVelocityLeft = 30.f;			// ��
			else if (KEY_D == iKey)
				g_fVelocityRight = 30.f;		// ��
			else if (KEY_W == iKey)
				g_fVelocityUp = 15.f;			// ��
			else if (KEY_S == iKey)
				g_fVelocityDown = 15.f;			// ��
			// �����ƶ�
			UpdateMovement();

			// ��Ϸ�����У����¿ո����ӵ�
			if (KEY_SPACE == iKey)
				g_bControlCanFire = true;
		}
		if (g_bIsTwoPlayer)
		{
			// W A S D�������ƶ����������£���ĳ������ٶȱ�����ֵ
			if (KEY_LEFT == iKey)
				g_fVelocityLeft2 = 30.f;			// ��
			else if (KEY_RIGHT == iKey)
				g_fVelocityRight2 = 30.f;		// ��
			else if (KEY_UP == iKey)
				g_fVelocityUp2 = 15.f;			// ��
			else if (KEY_DOWN == iKey)
				g_fVelocityDown2 = 15.f;			// ��
			// �����ƶ�
			UpdateMovement2();

			// ��Ϸ�����У�����0�����ӵ�
			if (KEY_NUMPAD0 == iKey)
				g_bControlCanFire2 = true;
		}



	}


}
//==========================================================================
//
// ���̵���
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
void OnKeyUp( const int iKey )
{
	if (2 == g_iGameState)
	{
		// W A S D���ɿ�������ĳ������ٶȱ���
		if (KEY_A == iKey)
		{
			// ��
			g_fVelocityLeft = 0.f;
		}
		else if (KEY_D == iKey)
		{
			g_fVelocityRight = 0.f;
		}
		else if (KEY_W == iKey)
		{
			g_fVelocityUp = 0.f;
		}
		else if (KEY_S == iKey)
		{
			g_fVelocityDown = 0.f;
		}

		// ��Ϸ�����У��ɿ��ո�ֹͣ�����ӵ�
		if (KEY_SPACE == iKey)
			g_bControlCanFire = false;
		// �����ƶ�
		UpdateMovement();

		if (g_bIsTwoPlayer == true)
		{
			if (KEY_LEFT == iKey)
			{
				// ��
				g_fVelocityLeft2 = 0.f;
			}
			else if (KEY_RIGHT == iKey)
			{
				g_fVelocityRight2 = 0.f;
			}
			else if (KEY_UP == iKey)
			{
				g_fVelocityUp2 = 0.f;
			}
			else if (KEY_DOWN == iKey)
			{
				g_fVelocityDown2 = 0.f;
			}
			// �����ƶ�
			UpdateMovement2();


			// ��Ϸ�����У��ɿ��ո�ֹͣ�����ӵ�
			if (KEY_NUMPAD0 == iKey)
				g_bControlCanFire2 = false;
		}




	}
}
//===========================================================================
//
// �����뾫����ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	if (2 != g_iGameState)
		return;
	SGameSprite	*pSrcSprite = NULL;
	SGameSprite *pTarSprite = NULL;
	/*���ж���ײ���Ƿ�Ϊ��ң������һ����ң����ж��������Ƿ�Ϊ������ң�������ǣ����ȡ����ײ������ݽ�����ײ���������ͬ������߼�˳��ͬ�¡�
	Դ�룺	SGameSprite pSrcSprite = IsControlSprite(szSrcName) ? &g_ControlSprite : GList_GetSpriteByName(szSrcName);
			SGameSprite pTarSprite = IsControlSprite(szTarName) ? &g_ControlSprite : GList_GetSpriteByName(szTarName);
	����ֻ�Ǹı���һ��szSrcName��szTarName��ȡֵ�߼���
	*/
	if (IsControler(szSrcName))
	{
		if (IsControlSprite(szSrcName))
		{
			pSrcSprite = &g_ControlSprite;
			if (!IsControlSprite2(szTarName))
				pTarSprite = GList_GetSpriteByName(szTarName);
		}
		if (IsControlSprite2(szSrcName))
		{
			pSrcSprite = &g_ControlSprite2;
			if (!IsControlSprite2(szTarName))
				pTarSprite = GList_GetSpriteByName(szTarName);
		}
	}
	else
	{
		pSrcSprite = GList_GetSpriteByName(szSrcName);
		pTarSprite = GList_GetSpriteByName(szTarName);
	}

	// ֻҪ��һ��Ϊ�գ����Ǿ���Ϊ����ײ��Ч
	if (NULL == pSrcSprite || NULL == pTarSprite)
		return;
	// ��ײ��˫���ģ�����˫���������Լ�����ײ,����Src��Tar��ײʱ����Ҫ��		// ��Src->Tar��ҲҪ����Tar->Src
	// Src ��ײ--> Tar. 4�ֵл�����һ����ײ���������������ǵ���ר�õ�
	if (SPRITE_CONTROL == pSrcSprite->eSpriteType)
		ControlColOther(pSrcSprite, pTarSprite);
	else if (SPRITE_BULLET1 == pSrcSprite->eSpriteType)
		Bullet1ColOther(pSrcSprite, pTarSprite);
	else
		EnemyColOther(pSrcSprite, pTarSprite);
	// Tar ��ײ--> Src
	if (SPRITE_CONTROL == pTarSprite->eSpriteType)
		ControlColOther(pTarSprite, pSrcSprite);
	else if (SPRITE_BULLET1 == pTarSprite->eSpriteType)
	{
			Bullet1ColOther(pTarSprite, pSrcSprite);
	}
	else
		EnemyColOther(pTarSprite, pSrcSprite);
	// �����жϣ�����֮��ɾ����Sprite��
	// ���ǿ��Ƶ�ControlSprite�������ж���GameMainLoop����У���������
	if (SPRITE_CONTROL != pSrcSprite->eSpriteType)
	{
		if (IsDead(pSrcSprite))
			GList_DeleteSprite(pSrcSprite, true);
	}
	//
	if (SPRITE_CONTROL != pTarSprite->eSpriteType)
	{
		if (IsDead(pTarSprite))
			GList_DeleteSprite(pTarSprite, true);
	}




}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{

	if (strstr(szName, "Bullet") != NULL)
		GList_DeleteSprite(szName, true);
}



void UpdateMovement()
{
	// ���ԭ���������µ�ʱ�򣬸�������ֵ�������ʱ�򣬸�������0��
	// ���������Ҽ�ͬʱ���µ�ʱ���������Ϊ0�����Ծ�ֹ����
	// �ɿ����е���������Ҽ����÷���ı�������0����һ����δ�ɿ���ֵδ��			���㣬�������֮���ٶȷ�0���ͳ���û�ͷŵİ��������ƶ�
	float	fVelX = g_fVelocityRight - g_fVelocityLeft;
	float	fVelY = g_fVelocityDown - g_fVelocityUp;

	dSetSpriteLinearVelocityX(g_ControlSprite.szName, fVelX);
	dSetSpriteLinearVelocityY(g_ControlSprite.szName, fVelY);
}


void UpdateMovement2()
{
	// ���ԭ���������µ�ʱ�򣬸�������ֵ�������ʱ�򣬸�������0��
	// ���������Ҽ�ͬʱ���µ�ʱ���������Ϊ0�����Ծ�ֹ����
	// �ɿ����е���������Ҽ����÷���ı�������0����һ����δ�ɿ���ֵδ��			���㣬�������֮���ٶȷ�0���ͳ���û�ͷŵİ��������ƶ�
	float	fVelX2 = g_fVelocityRight2 - g_fVelocityLeft2;
	float	fVelY2 = g_fVelocityDown2 - g_fVelocityUp2;

	dSetSpriteLinearVelocityX(g_ControlSprite2.szName, fVelX2);
	dSetSpriteLinearVelocityY(g_ControlSprite2.szName, fVelY2);
}






// ������ͨ�����ӵ���iBulletType��0Ϊ�Լ�������ӵ���1Ϊ�з�������ӵ���2Ϊ���2������ӵ���		Pos����������
SGameSprite *CreateBullet1(const int iBulletType, const float fPosX, const float fPosY)
{
	SGameSprite	*pSprite = (SGameSprite*)malloc(sizeof(SGameSprite));
	// �����ӵ�����ģ�����ݣ���������Ҫһ��һ��������ֵ
	memcpy(pSprite, &g_Bullet1Template, sizeof(SGameSprite));
	sprintf(pSprite->szName, "Monster_Bullet1_%d", g_iCreatedSpriteCount++);
	pSprite->iDamage = BULLET_DAMAGE_1;
	pSprite->iBulletType = iBulletType;
	// ��ӵ��б����
	GList_AddSprite(pSprite);
	// ��¡�������ʾͼƬ
	dCloneSprite("Bullet1_Template", pSprite->szName);
	// �������꼰�ٶ�
	dSetSpritePosition(pSprite->szName, fPosX, fPosY);
	// �з����ӵ��������Ƿ���(��������)��������Ҫ��ת
	if (1 == iBulletType)
	{
		// �̶��ٶ�
		dSetSpriteLinearVelocityX(pSprite->szName, -30.f);
	}

	else
	{
		dSetSpriteFlipX(pSprite->szName, true);
		// �̶��ٶ�
		dSetSpriteLinearVelocityX(pSprite->szName, 60.f);
	}
	// ��������߽����ƣ�����ײģʽΪNULL,�����д���
	dSetSpriteWorldLimit(pSprite->szName, WORLD_LIMIT_NULL, g_fWorldLeft, g_fWorldTop, g_fWorldRight, g_fWorldBottom);
	return pSprite;
}


void CreateVerTick(float fDeltaTime)
{
	// �Ƿ�ʱ�䴴��
	g_fVerCreateTime -= fDeltaTime;
	if (g_fVerCreateTime <= 0.f)
	{
		// ���һ��ʱ�䣬��Ϊ�´γ�����ʱ��
		g_fVerCreateTime = (float)dRandomRange(5, 10);
		// �ɻ�Ⱥ�ĵ�һ���ɻ�����ʼY����
		int	iPosBase = dRandomRange((int)g_fWorldBottom, (int)g_fWorldTop);
		// �������
		int	iCount = dRandomRange(VER_MIN_CREATE, VER_MAX_CREATE);
		for (int iLoop = 0; iLoop < iCount; iLoop++)
		{
			// ����Sprite
			SGameSprite *pSprite = (SGameSprite*)malloc(sizeof(SGameSprite));
			memcpy(pSprite, &g_VerTemplate, sizeof(SGameSprite));
			sprintf(pSprite->szName, "Monster_VerticalSprite_%d", g_iCreatedSpriteCount++);
			pSprite->fFloatTime = (float)dRandomRange(0, 10) / 10.f;							// �������½���ȡ���ֵ
			pSprite->bFloatUp = dRandomRange(0, 1) == 1 ? true : false;
			dCloneSprite("VerticalSprite_Template", pSprite->szName);
			// ��ӵ�������
			GList_AddSprite(pSprite);
			// �������ɣ�
			int	iRandom = dRandomRange((int)g_fWorldRight + 20, (int)g_fWorldRight + 40);
			float	fPosY = g_fWorldTop + iPosBase + 10.f * iLoop;
			dSetSpritePosition(pSprite->szName, (float)iRandom, fPosY);
			// �̶��ٶ�
			dSetSpriteLinearVelocityX(pSprite->szName, -10.f);
			// ��������߽����ƣ�����ײģʽΪNULL,�����д���
			dSetSpriteWorldLimit(pSprite->szName, WORLD_LIMIT_NULL, g_fWorldLeft - 10.f, g_fWorldTop, g_fWorldRight + 100.f, g_fWorldBottom);
		}

	}
}

void CreateBossTick(float fDeltaTime)
{
	// �Ƿ�ʱ�䴴��
	g_fBossCtreateTime -= fDeltaTime;
	if (g_fBossCtreateTime <= 0.f)
	{
		// ���һ��ʱ�䣬��Ϊ�´γ�����ʱ��
		g_fBossCtreateTime = (float)dRandomRange(10, 15);
		// �ɻ�Ⱥ�ĵ�һ���ɻ�����ʼY����
		int	iPosBase = dRandomRange((int)g_fWorldBottom , (int)g_fWorldTop);
		// �������
		int	iCount = dRandomRange(VER_MIN_CREATE, 2);
		for (int iLoop = 0; iLoop < iCount; iLoop++)
		{
			// ����Sprite
			SGameSprite *pSprite = (SGameSprite*)malloc(sizeof(SGameSprite));
			memcpy(pSprite, &g_BossTemplate, sizeof(SGameSprite));
			sprintf(pSprite->szName, "Monster_BigBoss_%d", g_iCreatedSpriteCount++);
			pSprite->fFloatTime = (float)dRandomRange(0, 10) / 10.f;							// �������½���ȡ���ֵ
			pSprite->bFloatUp = dRandomRange(0, 1) == 1 ? true : false;
			dCloneSprite("BigBoss_Template", pSprite->szName);
			// ��ӵ�������
			GList_AddSprite(pSprite);
			// �������ɣ�
			int	iRandom = dRandomRange((int)g_fWorldRight + 20, (int)g_fWorldRight + 40);
			float	fPosY = g_fWorldTop + iPosBase + 10.f * iLoop;
			dSetSpritePosition(pSprite->szName, (float)iRandom, fPosY);
			// �̶��ٶ�
			dSetSpriteLinearVelocityX(pSprite->szName, -10.f);
			// ��������߽����ƣ�����ײģʽΪNULL,�����д���
			dSetSpriteWorldLimit(pSprite->szName, WORLD_LIMIT_NULL, g_fWorldLeft - 10.f, g_fWorldTop, g_fWorldRight + 100.f, g_fWorldBottom);
		}

	}
}

void CreateEliteTick(float fDeltaTime)
{
	// �Ƿ�ʱ�䴴��
	g_fEliteCreateTime -= fDeltaTime;
	if (g_fEliteCreateTime <= 0.f)
	{
		// ���һ��ʱ�䣬��Ϊ�´γ�����ʱ��
		g_fEliteCreateTime = (float)dRandomRange(7, 10);
		// �ɻ�Ⱥ�ĵ�һ���ɻ�����ʼY����
		int	iPosBase = dRandomRange((int)g_fWorldBottom, (int)g_fWorldTop);
		// �������
		int	iCount = dRandomRange(2, 3);
		for (int iLoop = 0; iLoop < iCount; iLoop++)
		{
			// ����Sprite
			SGameSprite *pSprite = (SGameSprite*)malloc(sizeof(SGameSprite));
			memcpy(pSprite, &g_EliteTemplate, sizeof(SGameSprite));
			sprintf(pSprite->szName, "Monster_HorizontalSprite_%d", g_iCreatedSpriteCount++);
			pSprite->fFloatTime = (float)dRandomRange(0, 10) / 10.f;							// �������½���ȡ���ֵ
			pSprite->bFloatUp = dRandomRange(0, 1) == 1 ? true : false;
			dCloneSprite("HorizontalSprite_Template", pSprite->szName);
			// ��ӵ�������
			GList_AddSprite(pSprite);
			// �������ɣ�
			int	iRandom = dRandomRange((int)g_fWorldRight + 20, (int)g_fWorldRight + 40);
			float	fPosY = g_fWorldTop + iPosBase + 10.f * iLoop;
			dSetSpritePosition(pSprite->szName, (float)iRandom, fPosY);
			// �̶��ٶ�
			dSetSpriteLinearVelocityX(pSprite->szName, -10.f);
			// ��������߽����ƣ�����ײģʽΪNULL,�����д���
			dSetSpriteWorldLimit(pSprite->szName, WORLD_LIMIT_NULL, g_fWorldLeft - 10.f, g_fWorldTop, g_fWorldRight + 100.f, g_fWorldBottom);
		}

	}
}




//=====================================================================
// �Ѿ�������Ver Sprite��ÿ����Ϸѭ����ִ�д˺����������ӵ��ķ����
void VerLoopTick(SGameSprite *pSprite, float fDeltaTime)
{
	pSprite->fFireAfterCreate -= fDeltaTime;
	if (pSprite->fFireAfterCreate <= 0.f)
	{
		// �ӵ�1�Ĵ���
		pSprite->fBulletTime -= fDeltaTime;
		if (pSprite->fBulletTime <= 0.f)
		{
			// �ӵ�ʱ��̶�������Ҫ���
			pSprite->fBulletTime = 1;//g_VerTemplate.fBulletTime;
			CreateBullet1(1, dGetSpritePositionX(pSprite->szName), dGetSpritePositionY(pSprite->szName));
		}
	}


	if (pSprite->bFloatUp)
	{
		// ��Ư
		pSprite->fFloatTime += fDeltaTime;
		if (pSprite->fFloatTime >= 1.f)
		{
			pSprite->bFloatUp = false;
		}
		float	fPosY = dGetSpritePositionY(pSprite->szName);
		fPosY += 6.f * fDeltaTime;
		dSetSpritePositionY(pSprite->szName, fPosY);
	}
	else
	{
		//����Ư
		pSprite->fFloatTime -= fDeltaTime;
		if (pSprite->fFloatTime <= 0.f)
		{
			pSprite->bFloatUp = true;
		}
		float	fPosY = dGetSpritePositionY(pSprite->szName);
		fPosY -= 6.f * fDeltaTime;
		dSetSpritePositionY(pSprite->szName, fPosY);
	}

}

bool IsDead(SGameSprite *pSprite)
{
	if (strstr(pSprite->szName, "ControlSprite"))
	{
		if (NULL != pSprite && pSprite->iHp <= 0)
		{
			dPlayEffect("DeadExplotion", 3.0f, dGetSpritePositionX(pSprite->szName), dGetSpritePositionY(pSprite->szName), 0.f);
			dSetSpriteVisible(pSprite->szName, false);
			return true;
		}
	}
	else if (strstr(pSprite->szName, "Monster_"))
	{
		return NULL != pSprite && pSprite->iHp <= 0;
	}
	return false;
	//return NULL != pSprite && pSprite->iHp <= 0;
}

bool IsControlSprite(const char *szName)
{
	return (strcmp(g_ControlSprite.szName, szName) == 0);
}

bool IsControlSprite2(const char *szName)
{
	return (strcmp(g_ControlSprite2.szName, szName) == 0);
}

bool IsControler(const char *szName)
{
	return (strcmp(g_ControlSprite.szName, szName) == 0 || strcmp(g_ControlSprite2.szName, szName) == 0);
}

// ControlSprite������Sprite��ײ֮�����ֵ����
void ControlColOther(SGameSprite *pControl, SGameSprite *pOther)
{
	// ��ԶҪע��ָ��ʹ��֮ǰ��NULL�жϡ�
	if (NULL == pOther || NULL == pControl)
		return;
	// ��ײ���Լ����ӵ�������
	if (IsControlSpriteBullet(pOther))
		return;
	else if (IsControlSpriteBullet2(pOther))
		return;
	int	iHp = 0;
	// ��ײ����Ѫ���飬����Hp.
	if (SPRITE_HEALTH == pOther->eSpriteType)
	{
		// ��Ѫ�����Damage��Ϊ���ӵ�Hp���Ҳ��ܳ������HP
		iHp = pControl->iHp + pOther->iDamage;
		if (iHp > CONTROL_MAX_HP)
			iHp = CONTROL_MAX_HP;
		pControl->iHp = iHp;
		// ���ϼ�Ѫ����Ļ��֣���������ü�Ѫ�����л��֣������������Ϊ0����
		pControl->iScore += pOther->iScore;
	}
	// ��ײ���о����о��ӵ�.
	else
	{
		// �Լ����Է����ˣ��ȿ��Լ�HP(����Լ������ˣ����¸�ѭ������Ϸ���̽���)
		pControl->iHp -= pOther->iDamage;

		// ����ҷ����˺����ڶԷ���HP,���ڶԷ�����ײ������Է����������ڴ����ӻ��֡�
		if (pControl->iDamage >= pOther->iHp)
		{
			// �����ҷ�����(ֻҪ�õ�����������ʹ�������ͬ���ھ���Ҳ���л��ֵ�)
			pControl->iScore += pOther->iScore;
		}
		dPlayEffect("playerExplode", 3.0f, dGetSpritePositionX(pControl->szName), dGetSpritePositionY(pControl->szName), 0.f);


	}
	// ���ݸ�hp�ȼ�����ʾ��ͬ��ɫ
	if (pControl->iHp <= 200)
	{
		// ����Ϊ0������
		dSetSpriteColorGreen(pControl->szName, 0);
		dSetSpriteColorBlue(pControl->szName, 0);
	}
	else if (pControl->iHp <= 500)
	{
		// ���
		dSetSpriteColorGreen(pControl->szName, 128);
		dSetSpriteColorBlue(pControl->szName, 128);
	}
	else
	{
		// ����
		dSetSpriteColorGreen(pControl->szName, 255);
		dSetSpriteColorBlue(pControl->szName, 255);
	}
	//���»�����ʾ
	if (!strcmp(pControl->szName,"ControlSprite"))
		dSetTextValue("CurScoreText", pControl->iScore);
	if (!strcmp(pControl->szName, "ControlSprite2"))
		dSetTextValue("CurScoreText2", pControl->iScore);



}


void Bullet1ColOther(SGameSprite *pBullet, SGameSprite *pOther)
{
	// ��ԶҪע��ָ��ʹ��֮ǰ��NULL�жϡ�
	if (NULL == pOther || NULL == pBullet)
		return;
	// �ӵ�Ϊ�ҷ��ӵ�
	if (IsControlSpriteBullet(pBullet))
	{
		// �ҷ��ӵ���ײ����Ѫ����Health�������ӵ�Bullet2��ControlSprite��ʱ			// �򣬲����д���
		if (SPRITE_HEALTH == pOther->eSpriteType || SPRITE_BULLET2 == pOther->eSpriteType || SPRITE_CONTROL == pOther->eSpriteType)
			return;
		// �����Է����Լ���Ѫ
		pBullet->iHp -= pOther->iDamage;
		// �ӵ���ը����Ч��ѡ��smallExplosion
		dPlayEffect("smallExplosion", 1.0, dGetSpritePositionX(pBullet->szName), dGetSpritePositionY(pBullet->szName), 0.f);
		// �ҷ��ӵ����е��ˣ����ӻ���
		g_ControlSprite.iScore += pOther->iScore;
		GList_DeleteSprite(pBullet, true);
		//���»�����ʾ
		dSetTextValue("CurScoreText", g_ControlSprite.iScore);
	}
	// �ӵ�Ϊ���2���ӵ�
	else if (IsControlSpriteBullet2(pBullet))
	{
		// �ҷ��ӵ���ײ����Ѫ����Health�������ӵ�Bullet2��ControlSprite��ʱ			// �򣬲����д���
		if (SPRITE_HEALTH == pOther->eSpriteType || SPRITE_BULLET2 == pOther->eSpriteType || SPRITE_CONTROL == pOther->eSpriteType)
			return;
		// �����Է����Լ���Ѫ
		pBullet->iHp -= pOther->iDamage;
		// �ӵ���ը����Ч��ѡ��smallExplosion
		dPlayEffect("smallExplosion", 1.0, dGetSpritePositionX(pBullet->szName), dGetSpritePositionY(pBullet->szName), 0.f);
		// �ҷ��ӵ����е��ˣ����ӻ���
		g_ControlSprite2.iScore += pOther->iScore;
		GList_DeleteSprite(pBullet, true);
		//���»�����ʾ
		dSetTextValue("CurScoreText2", g_ControlSprite2.iScore);
	}
	// �з��ӵ�
	else
	{
		// ֻ������ײ��ControlSprite�����ӵ�������Ž��д���
		if (SPRITE_CONTROL == pOther->eSpriteType || (IsControlSpriteBullet(pOther) || IsControlSpriteBullet2(pOther)))
		{
			// �����Է����Լ���Ѫ
			pBullet->iHp -= pOther->iDamage;
			// ��Ч
			dPlayEffect("smallExplosion", 1.0, dGetSpritePositionX(pBullet->szName), dGetSpritePositionY(pBullet->szName), 0.f);
			GList_DeleteSprite(pBullet, true);
		}

	}
}




void EnemyColOther(SGameSprite *pEnemy, SGameSprite *pOther)
{
	// ��ԶҪע��ָ��ʹ��֮ǰ��NULL�жϡ�
	if (NULL == pOther || NULL == pEnemy)
		return;
	// ֻ����ײ��ControlSprite���ҷ��ӵ�����������Ž��д���
	// �����ģ�������ײ���з��Լ��ķɻ����з��ӵ����ȶ���������
	if ((IsControlSpriteBullet(pOther) || IsControlSpriteBullet2(pOther))|| SPRITE_CONTROL == pOther->eSpriteType)
	{
		// �Լ�����Ѫ
		pEnemy->iHp -= pOther->iDamage;
		// ��ײ��������Ч
		if (IsDead(pEnemy))
		{
			if (SPRITE_BIG_BOSS == pEnemy->eSpriteType)
				dPlayEffect("bigExplode", 3.f, dGetSpritePositionX(pEnemy->szName), dGetSpritePositionY(pEnemy->szName), 0.f);
			else
				dPlayEffect("enemyExplode", 3.f, dGetSpritePositionX(pEnemy->szName), dGetSpritePositionY(pEnemy->szName), 0.f);
		}
	}
}


// �ж��Ƿ���ControlSprite������ӵ�
bool IsControlSpriteBullet(SGameSprite *pSprite)
{
	if (NULL != pSprite)
	{
		// ���ж��Ƿ����ӵ����͵�Sprite
		if (SPRITE_BULLET1 == pSprite->eSpriteType || SPRITE_BULLET2 == pSprite->eSpriteType)
			return (0 == pSprite->iBulletType);
	}
	return false;
}

bool IsControlSpriteBullet2(SGameSprite *pSprite)
{
	if (NULL != pSprite)
	{
		// ���ж��Ƿ����ӵ����͵�Sprite
		if (SPRITE_BULLET1 == pSprite->eSpriteType || SPRITE_BULLET2 == pSprite->eSpriteType)
			return (2 == pSprite->iBulletType);
	}
	return false;
}


bool IsGameLost()
{
	if (g_bIsTwoPlayer == false)
		return IsDead(&g_ControlSprite);
	else
	{
		if (g_isPlayer1Alive)
			if (IsDead(&g_ControlSprite))
			{
				g_isPlayer1Alive = false;
				dSetSpriteEnable("ControlSprite", false);
				dSetTextValue("iHp",0);
			}
		if (g_isPlayer2Alive)
			if (IsDead(&g_ControlSprite2))
			{
				g_isPlayer2Alive = false;
				g_bIsTwoPlayer = false;
				dSetSpriteEnable("ControlSprite2", false);
				dSetTextValue("iHp2",0);
			}
		return !g_isPlayer1Alive && !g_isPlayer2Alive;

	}
	//	return IsDead(&g_ControlSprite) && IsDead(&g_ControlSprite2);
}
//git-hub test1