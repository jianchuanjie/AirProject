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


//申明所需全局变量
float		g_fWorldLeft = 0.f;	// 世界上下左右边界
float		g_fWorldRight = 10.f;
float		g_fWorldTop = 0.f;
float		g_fWorldBottom = 10.f;
//
float		g_fVelocityLeft = 0.f;	// 控制飞机的上下左右速度
float		g_fVelocityRight = 0.f;
float		g_fVelocityUp = 0.f;
float		g_fVelocityDown = 0.f;
float		g_fVelocityLeft2 = 0.f;	// 控制飞机2的上下左右速度
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


//申明所需函数
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




// 初始化各种模板数据 
void InitTemplateData()
{
	// 我们控制的Control Sprite
	strcpy(g_ControlSprite.szName, "ControlSprite");
	g_ControlSprite.iHp = CONTROL_MAX_HP;
	g_ControlSprite.iScore = 0;
	g_ControlSprite.iDamage = 500;	// 敌机碰到我，也会被击伤...
	g_ControlSprite.eSpriteType = SPRITE_CONTROL;
	g_ControlSprite.iBulletType = 0;		// Unused
	g_ControlSprite.fFireAfterCreate = 0.f;	// Unused
	g_ControlSprite.fBulletTime = 0.3f;	// 0.3秒发射一次子弹
	g_ControlSprite.fBulletTime2 = 0.f;	// Unused
	g_ControlSprite.fFloatTime = 0.f;	// 飞行中上下浮动控制
	g_ControlSprite.bFloatUp = true;
	strcpy(g_ControlSprite2.szName, "ControlSprite2");
	g_ControlSprite2.iHp = CONTROL_MAX_HP;
	g_ControlSprite2.iScore = 0;
	g_ControlSprite2.iDamage = 500;	// 敌机碰到我，也会被击伤...
	g_ControlSprite2.eSpriteType = SPRITE_CONTROL;
	g_ControlSprite2.iBulletType = 0;		// Unused
	g_ControlSprite2.fFireAfterCreate = 0.f;	// Unused
	g_ControlSprite2.fBulletTime = 0.3f;	// 0.3秒发射一次子弹
	g_ControlSprite2.fBulletTime2 = 0.f;	// Unused
	g_ControlSprite2.fFloatTime = 0.f;	// 飞行中上下浮动控制
	g_ControlSprite2.bFloatUp = true;
	g_isPlayer1Alive = true;
	g_isPlayer2Alive = g_bIsTwoPlayer ? true : false;
	


	//	敌方战机
	g_VerTemplate.szName[0] = '\0';
	g_VerTemplate.iHp = 100;	// 生命值100
	g_VerTemplate.iScore = 50;		// 积分50
	g_VerTemplate.iDamage = 500;	// 碰到自身，伤害500
	g_VerTemplate.eSpriteType = SPRITE_VER;
	g_VerTemplate.iBulletType = 0;		// Unused
	g_VerTemplate.fFireAfterCreate = 1.f;	// 出生1s后开始发射子弹
	g_VerTemplate.fBulletTime = 1.f;	// 1秒发射一次子弹
	g_VerTemplate.fBulletTime2 = 0.f;
	g_VerTemplate.fFloatTime = 0.f;	// 飞行中上下浮动控制
	g_VerTemplate.bFloatUp = true;
	//	敌方BOSS战机
	g_BossTemplate.szName[0] = '\0';
	g_BossTemplate.iHp = 1000;	// 生命值100
	g_BossTemplate.iScore = 50;		// 积分50
	g_BossTemplate.iDamage = 1000;	// 碰到自身，伤害500
	g_BossTemplate.eSpriteType = SPRITE_VER;
	g_BossTemplate.iBulletType = 0;		// Unused
	g_BossTemplate.fFireAfterCreate = 1.f;	// 出生1s后开始发射子弹
	g_BossTemplate.fBulletTime = 1.f;	// 1秒发射一次子弹
	g_BossTemplate.fBulletTime2 = 0.f;
	g_BossTemplate.fFloatTime = 0.f;	// 飞行中上下浮动控制
	g_BossTemplate.bFloatUp = true;
	//	敌方Elite战机
	g_EliteTemplate.szName[0] = '\0';
	g_EliteTemplate.iHp = 300;	// 生命值100
	g_EliteTemplate.iScore = 50;		// 积分50
	g_EliteTemplate.iDamage = 500;	// 碰到自身，伤害500
	g_EliteTemplate.eSpriteType = SPRITE_VER;
	g_EliteTemplate.iBulletType = 0;		// Unused
	g_EliteTemplate.fFireAfterCreate = 1.f;	// 出生1s后开始发射子弹
	g_EliteTemplate.fBulletTime = 1.f;	// 1秒发射一次子弹
	g_EliteTemplate.fBulletTime2 = 0.f;
	g_EliteTemplate.fFloatTime = 0.f;	// 飞行中上下浮动控制
	g_EliteTemplate.bFloatUp = true;
	//	子弹
	g_Bullet1Template.szName[0] = '\0';
	g_Bullet1Template.iHp = 1;		// 生命值1
	g_Bullet1Template.iScore = 10;		// 积分10
	g_Bullet1Template.iDamage = 0;		// 伤害值由发射的Sprite决定
	g_Bullet1Template.eSpriteType = SPRITE_BULLET1;
	g_Bullet1Template.iBulletType = 0;		// 类型由发射的Sprite决定
	g_Bullet1Template.fFireAfterCreate = 0.f;	// Unused
	g_Bullet1Template.fBulletTime = 0.0f;	// Unused
	g_Bullet1Template.fBulletTime2 = 0.f;	// Unused
	g_Bullet1Template.fFloatTime = 0.f;	// 飞行中上下浮动控制
	g_Bullet1Template.bFloatUp = true;
}






////////////////////////////////////////////////////////////////////////////////
//
//
int			g_iGameState		=	0;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();
//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。


//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态. 
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void GameMainLoop( float	fDeltaTime )
{
	switch( g_iGameState )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
	{
				// 判断输赢
		if (IsGameLost())
		{
			// 游戏结束。调用游戏结算函数并把游戏状态修改为结束状态
			g_iGameState = 0;
			GameEnd();
		}
		else // 游戏未结束，继续游戏
		{
			GameRun(fDeltaTime);
		}
	}
	break;
		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void GameInit()
{
	//获取世界边界
	g_fWorldLeft = dGetScreenLeft();
	g_fWorldRight = dGetScreenRight();
	g_fWorldTop = dGetScreenTop();
	g_fWorldBottom = dGetScreenBottom();

	g_fVelocityLeft = 0.f;
	g_fVelocityRight = 0.f;
	g_fVelocityUp = 0.f;
	g_fVelocityDown = 0.f;

	//敌方战机创造时间
	g_fVerCreateTime = 2.f;
	g_fBossCtreateTime = 15.f;
	g_fEliteCreateTime = 5.f;

	g_bControlCanFire = false;
	// 初始化Control Sprite的数值
	g_ControlSprite.iHp = CONTROL_MAX_HP;
	g_ControlSprite.iScore = 0;
	g_ControlSprite.fBulletTime = 0.3f;

	// 初始化Control Sprite2的数值
	if (g_bIsTwoPlayer == true)
	{
		g_ControlSprite2.iHp = CONTROL_MAX_HP;
		g_ControlSprite2.iScore = 0;
		g_ControlSprite2.fBulletTime = 0.3f;
	}

	InitTemplateData();

	// 隐藏 "按空格开始游戏" 这个提示图片 
	dSetSpriteVisible("GameBegin", false);

	//显示玩家
	dSetSpriteVisible("ControlSprite", true);
	if (g_bIsTwoPlayer==true)
		dSetSpriteVisible("ControlSprite2", true);

	//启动玩家
	dSetSpriteEnable("ControlSprite", true);
	if (g_bIsTwoPlayer == true)
		dSetSpriteEnable("ControlSprite2", true);

	//血条显示控制
	dSetSpriteVisible("iHp", true);
	if (g_bIsTwoPlayer == true)
	{
		dSetSpriteVisible("iHp2", true);
	}


	static bool	bInited = false;
	if (!bInited)
	{
		// 从文件里读取历史最高积分
		// 本案例只记录单一积分值，有兴趣的话，可以做多个积分的排名然后进行存储与读取
		// 后面游戏结束那里，文件的存储将作为教学任务
		FILE *pfp = fopen("Score.dat", "rb");
		if (NULL != pfp)
		{
			fread(&g_iMaxScore, sizeof(g_iMaxScore), 1, pfp);
			fread(&g_iMaxScore2, sizeof(g_iMaxScore2), 1, pfp);
			fclose(pfp);
			//更新最大积分
			dSetTextValue("MaxScoreText", g_iMaxScore);
			dSetTextValue("MaxScoreText2", g_iMaxScore2);
		}
	}



}
//==============================================================================
//
// 每局游戏进行中
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
	// 遍历当前已经创建的Sprite，执行各自的循环功能(各自的子弹发射等)
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
// 本局游戏结束
void GameEnd()
{

	//将控制的Sprite速度归零，坐标归于屏幕左边
	dSetSpriteLinearVelocity(g_ControlSprite.szName, 0.f, 0.f);
	dSetSpritePosition(g_ControlSprite.szName, g_fWorldLeft + 5.f, 0.f);
	dSetSpriteLinearVelocity(g_ControlSprite2.szName, 0.f, 0.f);
	dSetSpritePosition(g_ControlSprite2.szName, g_fWorldLeft + 5.f, 0.f);
	// 恢复正常颜色
	dSetSpriteColorGreen(g_ControlSprite.szName, 255);
	dSetSpriteColorBlue(g_ControlSprite.szName, 255);
	dSetSpriteColorGreen(g_ControlSprite2.szName, 255);
	dSetSpriteColorBlue(g_ControlSprite2.szName, 255);
	// 删除本局的所有Sprite
	GList_DeleteAllSprite();

	// 显示 "按空格开始游戏" 这个提示图片
	dSetTextValue("CurScoreText", 0);
	dSetTextValue("CurScoreText2", 0);
	dSetSpriteVisible("GameBegin", true);
	dSetTextValue("iHp", 0);
	dSetTextValue("iHp2", 0);


	g_bControlCanFire = false;
	g_bControlCanFire2 = false;
	g_bIsTwoPlayer = false;

	//隐藏血条
	dSetSpriteVisible("iHp", false);
	dSetSpriteVisible("iHp2", false);


	if (g_iMaxScore < g_ControlSprite.iScore)
	{
		g_iMaxScore = g_ControlSprite.iScore;

		// 写文件
		// [Your Code]
		FILE *pfp = fopen("Score.dat", "wb");
		if (NULL != pfp)
		{
			fwrite(&g_iMaxScore, sizeof(g_iMaxScore), 1, pfp);
			fclose(pfp);
		}
		// [End Your Code]

		//更新最大积分
		dSetTextValue("MaxScoreText", g_iMaxScore);

	}


	if (g_iMaxScore2 < g_ControlSprite2.iScore)
	{
		g_iMaxScore2 = g_ControlSprite2.iScore;

		// 写文件
		// [Your Code]
		FILE *pfp = fopen("Score.dat", "wb");
		if (NULL != pfp)
		{
			fwrite(&g_iMaxScore2, sizeof(g_iMaxScore2), 1, pfp);
			fclose(pfp);
		}
		// [End Your Code]

		//更新最大积分
		dSetTextValue("MaxScoreText2", g_iMaxScore2);

	}


}
//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseMove(const float fMouseX, const float fMouseY)
{

}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY)
{

}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseUp(const int iMouseType, const float fMouseX, const float fMouseY)
{

}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void OnKeyDown(const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress)
{
	// 按下空格，游戏开始

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
			// W A S D键控制移动。按键按下，给某方向的速度变量赋值
			if (KEY_A == iKey)
				g_fVelocityLeft = 30.f;			// 左
			else if (KEY_D == iKey)
				g_fVelocityRight = 30.f;		// 右
			else if (KEY_W == iKey)
				g_fVelocityUp = 15.f;			// 上
			else if (KEY_S == iKey)
				g_fVelocityDown = 15.f;			// 下
			// 更新移动
			UpdateMovement();

			// 游戏进行中，按下空格发射子弹
			if (KEY_SPACE == iKey)
				g_bControlCanFire = true;
		}
		if (g_bIsTwoPlayer)
		{
			// W A S D键控制移动。按键按下，给某方向的速度变量赋值
			if (KEY_LEFT == iKey)
				g_fVelocityLeft2 = 30.f;			// 左
			else if (KEY_RIGHT == iKey)
				g_fVelocityRight2 = 30.f;		// 右
			else if (KEY_UP == iKey)
				g_fVelocityUp2 = 15.f;			// 上
			else if (KEY_DOWN == iKey)
				g_fVelocityDown2 = 15.f;			// 下
			// 更新移动
			UpdateMovement2();

			// 游戏进行中，按下0发射子弹
			if (KEY_NUMPAD0 == iKey)
				g_bControlCanFire2 = true;
		}



	}


}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp( const int iKey )
{
	if (2 == g_iGameState)
	{
		// W A S D键松开，清零某方向的速度变量
		if (KEY_A == iKey)
		{
			// 左
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

		// 游戏进行中，松开空格，停止发射子弹
		if (KEY_SPACE == iKey)
			g_bControlCanFire = false;
		// 更新移动
		UpdateMovement();

		if (g_bIsTwoPlayer == true)
		{
			if (KEY_LEFT == iKey)
			{
				// 左
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
			// 更新移动
			UpdateMovement2();


			// 游戏进行中，松开空格，停止发射子弹
			if (KEY_NUMPAD0 == iKey)
				g_bControlCanFire2 = false;
		}




	}
}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	if (2 != g_iGameState)
		return;
	SGameSprite	*pSrcSprite = NULL;
	SGameSprite *pTarSprite = NULL;
	/*先判断碰撞体是否为玩家，如果是一号玩家，再判断碰到的是否为二号玩家，如果不是，则获取被碰撞体的数据进行碰撞。二号玩家同理。其次逻辑顺序同下。
	源码：	SGameSprite pSrcSprite = IsControlSprite(szSrcName) ? &g_ControlSprite : GList_GetSpriteByName(szSrcName);
			SGameSprite pTarSprite = IsControlSprite(szTarName) ? &g_ControlSprite : GList_GetSpriteByName(szTarName);
	这里只是改变了一下szSrcName和szTarName的取值逻辑。
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

	// 只要有一个为空，我们就认为此碰撞无效
	if (NULL == pSrcSprite || NULL == pTarSprite)
		return;
	// 碰撞是双方的，所以双方都处理自己的碰撞,所以Src和Tar碰撞时，需要处		// 理Src->Tar，也要处理Tar->Src
	// Src 碰撞--> Tar. 4种敌机共用一个碰撞处理函数，其它都是单独专用的
	if (SPRITE_CONTROL == pSrcSprite->eSpriteType)
		ControlColOther(pSrcSprite, pTarSprite);
	else if (SPRITE_BULLET1 == pSrcSprite->eSpriteType)
		Bullet1ColOther(pSrcSprite, pTarSprite);
	else
		EnemyColOther(pSrcSprite, pTarSprite);
	// Tar 碰撞--> Src
	if (SPRITE_CONTROL == pTarSprite->eSpriteType)
		ControlColOther(pTarSprite, pSrcSprite);
	else if (SPRITE_BULLET1 == pTarSprite->eSpriteType)
	{
			Bullet1ColOther(pTarSprite, pSrcSprite);
	}
	else
		EnemyColOther(pTarSprite, pSrcSprite);
	// 死亡判断，死亡之后删除该Sprite。
	// 我们控制的ControlSprite的死亡判断在GameMainLoop里进行，不在这里
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
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{

	if (strstr(szName, "Bullet") != NULL)
		GList_DeleteSprite(szName, true);
}



void UpdateMovement()
{
	// 相减原理：当键按下的时候，给变量赋值，弹起的时候，给变量清0。
	// 这样当左右键同时按下的时候，两者相减为0，所以静止不动
	// 松开其中的左键或者右键，该方向的变量被清0，另一方向未松开，值未被			清零，所以相减之后速度非0，就朝还没释放的按键方向移动
	float	fVelX = g_fVelocityRight - g_fVelocityLeft;
	float	fVelY = g_fVelocityDown - g_fVelocityUp;

	dSetSpriteLinearVelocityX(g_ControlSprite.szName, fVelX);
	dSetSpriteLinearVelocityY(g_ControlSprite.szName, fVelY);
}


void UpdateMovement2()
{
	// 相减原理：当键按下的时候，给变量赋值，弹起的时候，给变量清0。
	// 这样当左右键同时按下的时候，两者相减为0，所以静止不动
	// 松开其中的左键或者右键，该方向的变量被清0，另一方向未松开，值未被			清零，所以相减之后速度非0，就朝还没释放的按键方向移动
	float	fVelX2 = g_fVelocityRight2 - g_fVelocityLeft2;
	float	fVelY2 = g_fVelocityDown2 - g_fVelocityUp2;

	dSetSpriteLinearVelocityX(g_ControlSprite2.szName, fVelX2);
	dSetSpriteLinearVelocityY(g_ControlSprite2.szName, fVelY2);
}






// 创建普通类型子弹，iBulletType：0为自己发射的子弹，1为敌方发射的子弹，2为玩家2发射的子弹；		Pos：发射坐标
SGameSprite *CreateBullet1(const int iBulletType, const float fPosX, const float fPosY)
{
	SGameSprite	*pSprite = (SGameSprite*)malloc(sizeof(SGameSprite));
	// 拷贝子弹数据模板内容，这样不需要一个一个单独赋值
	memcpy(pSprite, &g_Bullet1Template, sizeof(SGameSprite));
	sprintf(pSprite->szName, "Monster_Bullet1_%d", g_iCreatedSpriteCount++);
	pSprite->iDamage = BULLET_DAMAGE_1;
	pSprite->iBulletType = iBulletType;
	// 添加到列表管理
	GList_AddSprite(pSprite);
	// 克隆精灵的显示图片
	dCloneSprite("Bullet1_Template", pSprite->szName);
	// 给予坐标及速度
	dSetSpritePosition(pSprite->szName, fPosX, fPosY);
	// 敌方的子弹，方向是反的(由右向左)，所以需要翻转
	if (1 == iBulletType)
	{
		// 固定速度
		dSetSpriteLinearVelocityX(pSprite->szName, -30.f);
	}

	else
	{
		dSetSpriteFlipX(pSprite->szName, true);
		// 固定速度
		dSetSpriteLinearVelocityX(pSprite->szName, 60.f);
	}
	// 设置世界边界限制，及碰撞模式为NULL,即自行处理
	dSetSpriteWorldLimit(pSprite->szName, WORLD_LIMIT_NULL, g_fWorldLeft, g_fWorldTop, g_fWorldRight, g_fWorldBottom);
	return pSprite;
}


void CreateVerTick(float fDeltaTime)
{
	// 是否到时间创建
	g_fVerCreateTime -= fDeltaTime;
	if (g_fVerCreateTime <= 0.f)
	{
		// 随机一个时间，作为下次出生的时间
		g_fVerCreateTime = (float)dRandomRange(5, 10);
		// 飞机群的第一个飞机的起始Y坐标
		int	iPosBase = dRandomRange((int)g_fWorldBottom, (int)g_fWorldTop);
		// 随机数量
		int	iCount = dRandomRange(VER_MIN_CREATE, VER_MAX_CREATE);
		for (int iLoop = 0; iLoop < iCount; iLoop++)
		{
			// 创建Sprite
			SGameSprite *pSprite = (SGameSprite*)malloc(sizeof(SGameSprite));
			memcpy(pSprite, &g_VerTemplate, sizeof(SGameSprite));
			sprintf(pSprite->szName, "Monster_VerticalSprite_%d", g_iCreatedSpriteCount++);
			pSprite->fFloatTime = (float)dRandomRange(0, 10) / 10.f;							// 上升和下降都取随机值
			pSprite->bFloatUp = dRandomRange(0, 1) == 1 ? true : false;
			dCloneSprite("VerticalSprite_Template", pSprite->szName);
			// 添加到链表中
			GList_AddSprite(pSprite);
			// 坐标生成：
			int	iRandom = dRandomRange((int)g_fWorldRight + 20, (int)g_fWorldRight + 40);
			float	fPosY = g_fWorldTop + iPosBase + 10.f * iLoop;
			dSetSpritePosition(pSprite->szName, (float)iRandom, fPosY);
			// 固定速度
			dSetSpriteLinearVelocityX(pSprite->szName, -10.f);
			// 设置世界边界限制，及碰撞模式为NULL,即自行处理
			dSetSpriteWorldLimit(pSprite->szName, WORLD_LIMIT_NULL, g_fWorldLeft - 10.f, g_fWorldTop, g_fWorldRight + 100.f, g_fWorldBottom);
		}

	}
}

void CreateBossTick(float fDeltaTime)
{
	// 是否到时间创建
	g_fBossCtreateTime -= fDeltaTime;
	if (g_fBossCtreateTime <= 0.f)
	{
		// 随机一个时间，作为下次出生的时间
		g_fBossCtreateTime = (float)dRandomRange(10, 15);
		// 飞机群的第一个飞机的起始Y坐标
		int	iPosBase = dRandomRange((int)g_fWorldBottom , (int)g_fWorldTop);
		// 随机数量
		int	iCount = dRandomRange(VER_MIN_CREATE, 2);
		for (int iLoop = 0; iLoop < iCount; iLoop++)
		{
			// 创建Sprite
			SGameSprite *pSprite = (SGameSprite*)malloc(sizeof(SGameSprite));
			memcpy(pSprite, &g_BossTemplate, sizeof(SGameSprite));
			sprintf(pSprite->szName, "Monster_BigBoss_%d", g_iCreatedSpriteCount++);
			pSprite->fFloatTime = (float)dRandomRange(0, 10) / 10.f;							// 上升和下降都取随机值
			pSprite->bFloatUp = dRandomRange(0, 1) == 1 ? true : false;
			dCloneSprite("BigBoss_Template", pSprite->szName);
			// 添加到链表中
			GList_AddSprite(pSprite);
			// 坐标生成：
			int	iRandom = dRandomRange((int)g_fWorldRight + 20, (int)g_fWorldRight + 40);
			float	fPosY = g_fWorldTop + iPosBase + 10.f * iLoop;
			dSetSpritePosition(pSprite->szName, (float)iRandom, fPosY);
			// 固定速度
			dSetSpriteLinearVelocityX(pSprite->szName, -10.f);
			// 设置世界边界限制，及碰撞模式为NULL,即自行处理
			dSetSpriteWorldLimit(pSprite->szName, WORLD_LIMIT_NULL, g_fWorldLeft - 10.f, g_fWorldTop, g_fWorldRight + 100.f, g_fWorldBottom);
		}

	}
}

void CreateEliteTick(float fDeltaTime)
{
	// 是否到时间创建
	g_fEliteCreateTime -= fDeltaTime;
	if (g_fEliteCreateTime <= 0.f)
	{
		// 随机一个时间，作为下次出生的时间
		g_fEliteCreateTime = (float)dRandomRange(7, 10);
		// 飞机群的第一个飞机的起始Y坐标
		int	iPosBase = dRandomRange((int)g_fWorldBottom, (int)g_fWorldTop);
		// 随机数量
		int	iCount = dRandomRange(2, 3);
		for (int iLoop = 0; iLoop < iCount; iLoop++)
		{
			// 创建Sprite
			SGameSprite *pSprite = (SGameSprite*)malloc(sizeof(SGameSprite));
			memcpy(pSprite, &g_EliteTemplate, sizeof(SGameSprite));
			sprintf(pSprite->szName, "Monster_HorizontalSprite_%d", g_iCreatedSpriteCount++);
			pSprite->fFloatTime = (float)dRandomRange(0, 10) / 10.f;							// 上升和下降都取随机值
			pSprite->bFloatUp = dRandomRange(0, 1) == 1 ? true : false;
			dCloneSprite("HorizontalSprite_Template", pSprite->szName);
			// 添加到链表中
			GList_AddSprite(pSprite);
			// 坐标生成：
			int	iRandom = dRandomRange((int)g_fWorldRight + 20, (int)g_fWorldRight + 40);
			float	fPosY = g_fWorldTop + iPosBase + 10.f * iLoop;
			dSetSpritePosition(pSprite->szName, (float)iRandom, fPosY);
			// 固定速度
			dSetSpriteLinearVelocityX(pSprite->szName, -10.f);
			// 设置世界边界限制，及碰撞模式为NULL,即自行处理
			dSetSpriteWorldLimit(pSprite->szName, WORLD_LIMIT_NULL, g_fWorldLeft - 10.f, g_fWorldTop, g_fWorldRight + 100.f, g_fWorldBottom);
		}

	}
}




//=====================================================================
// 已经创建的Ver Sprite，每个游戏循环都执行此函数。进行子弹的发射等
void VerLoopTick(SGameSprite *pSprite, float fDeltaTime)
{
	pSprite->fFireAfterCreate -= fDeltaTime;
	if (pSprite->fFireAfterCreate <= 0.f)
	{
		// 子弹1的创建
		pSprite->fBulletTime -= fDeltaTime;
		if (pSprite->fBulletTime <= 0.f)
		{
			// 子弹时间固定，不需要随机
			pSprite->fBulletTime = 1;//g_VerTemplate.fBulletTime;
			CreateBullet1(1, dGetSpritePositionX(pSprite->szName), dGetSpritePositionY(pSprite->szName));
		}
	}


	if (pSprite->bFloatUp)
	{
		// 上漂
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
		//　下漂
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

// ControlSprite与其它Sprite碰撞之后的数值处理
void ControlColOther(SGameSprite *pControl, SGameSprite *pOther)
{
	// 永远要注意指针使用之前的NULL判断。
	if (NULL == pOther || NULL == pControl)
		return;
	// 碰撞到自己的子弹不处理
	if (IsControlSpriteBullet(pOther))
		return;
	else if (IsControlSpriteBullet2(pOther))
		return;
	int	iHp = 0;
	// 碰撞到加血精灵，增加Hp.
	if (SPRITE_HEALTH == pOther->eSpriteType)
	{
		// 加血精灵的Damage作为增加的Hp。且不能超过最大HP
		iHp = pControl->iHp + pOther->iDamage;
		if (iHp > CONTROL_MAX_HP)
			iHp = CONTROL_MAX_HP;
		pControl->iHp = iHp;
		// 加上加血精灵的积分，如果不想让加血精灵有积分，将其积分设置为0即可
		pControl->iScore += pOther->iScore;
	}
	// 碰撞到敌军、敌军子弹.
	else
	{
		// 自己被对方打伤，先扣自己HP(如果自己死亡了，在下个循环里游戏即刻结束)
		pControl->iHp -= pOther->iDamage;

		// 如果我方的伤害大于对方的HP,则在对方的碰撞函数里，对方将死亡，在此增加积分。
		if (pControl->iDamage >= pOther->iHp)
		{
			// 增加我方积分(只要该敌人死亡，即使是与敌人同归于尽，也是有积分的)
			pControl->iScore += pOther->iScore;
		}
		dPlayEffect("playerExplode", 3.0f, dGetSpritePositionX(pControl->szName), dGetSpritePositionY(pControl->szName), 0.f);


	}
	// 根据各hp等级，显示不同颜色
	if (pControl->iHp <= 200)
	{
		// 绿蓝为0，纯红
		dSetSpriteColorGreen(pControl->szName, 0);
		dSetSpriteColorBlue(pControl->szName, 0);
	}
	else if (pControl->iHp <= 500)
	{
		// 半红
		dSetSpriteColorGreen(pControl->szName, 128);
		dSetSpriteColorBlue(pControl->szName, 128);
	}
	else
	{
		// 正常
		dSetSpriteColorGreen(pControl->szName, 255);
		dSetSpriteColorBlue(pControl->szName, 255);
	}
	//更新积分显示
	if (!strcmp(pControl->szName,"ControlSprite"))
		dSetTextValue("CurScoreText", pControl->iScore);
	if (!strcmp(pControl->szName, "ControlSprite2"))
		dSetTextValue("CurScoreText2", pControl->iScore);



}


void Bullet1ColOther(SGameSprite *pBullet, SGameSprite *pOther)
{
	// 永远要注意指针使用之前的NULL判断。
	if (NULL == pOther || NULL == pBullet)
		return;
	// 子弹为我方子弹
	if (IsControlSpriteBullet(pBullet))
	{
		// 我方子弹碰撞到加血精灵Health、特殊子弹Bullet2、ControlSprite的时			// 候，不进行处理
		if (SPRITE_HEALTH == pOther->eSpriteType || SPRITE_BULLET2 == pOther->eSpriteType || SPRITE_CONTROL == pOther->eSpriteType)
			return;
		// 碰到对方，自己掉血
		pBullet->iHp -= pOther->iDamage;
		// 子弹爆炸的特效，选用smallExplosion
		dPlayEffect("smallExplosion", 1.0, dGetSpritePositionX(pBullet->szName), dGetSpritePositionY(pBullet->szName), 0.f);
		// 我方子弹击中敌人，增加积分
		g_ControlSprite.iScore += pOther->iScore;
		GList_DeleteSprite(pBullet, true);
		//更新积分显示
		dSetTextValue("CurScoreText", g_ControlSprite.iScore);
	}
	// 子弹为玩家2的子弹
	else if (IsControlSpriteBullet2(pBullet))
	{
		// 我方子弹碰撞到加血精灵Health、特殊子弹Bullet2、ControlSprite的时			// 候，不进行处理
		if (SPRITE_HEALTH == pOther->eSpriteType || SPRITE_BULLET2 == pOther->eSpriteType || SPRITE_CONTROL == pOther->eSpriteType)
			return;
		// 碰到对方，自己掉血
		pBullet->iHp -= pOther->iDamage;
		// 子弹爆炸的特效，选用smallExplosion
		dPlayEffect("smallExplosion", 1.0, dGetSpritePositionX(pBullet->szName), dGetSpritePositionY(pBullet->szName), 0.f);
		// 我方子弹击中敌人，增加积分
		g_ControlSprite2.iScore += pOther->iScore;
		GList_DeleteSprite(pBullet, true);
		//更新积分显示
		dSetTextValue("CurScoreText2", g_ControlSprite2.iScore);
	}
	// 敌方子弹
	else
	{
		// 只有在碰撞到ControlSprite及其子弹的情况才进行处理。
		if (SPRITE_CONTROL == pOther->eSpriteType || (IsControlSpriteBullet(pOther) || IsControlSpriteBullet2(pOther)))
		{
			// 碰到对方，自己掉血
			pBullet->iHp -= pOther->iDamage;
			// 特效
			dPlayEffect("smallExplosion", 1.0, dGetSpritePositionX(pBullet->szName), dGetSpritePositionY(pBullet->szName), 0.f);
			GList_DeleteSprite(pBullet, true);
		}

	}
}




void EnemyColOther(SGameSprite *pEnemy, SGameSprite *pOther)
{
	// 永远要注意指针使用之前的NULL判断。
	if (NULL == pOther || NULL == pEnemy)
		return;
	// 只有碰撞到ControlSprite、我方子弹这两种情况才进行处理。
	// 其它的，比如碰撞到敌方自己的飞机、敌方子弹、等都不做处理
	if ((IsControlSpriteBullet(pOther) || IsControlSpriteBullet2(pOther))|| SPRITE_CONTROL == pOther->eSpriteType)
	{
		// 自己被扣血
		pEnemy->iHp -= pOther->iDamage;
		// 碰撞死亡后播特效
		if (IsDead(pEnemy))
		{
			if (SPRITE_BIG_BOSS == pEnemy->eSpriteType)
				dPlayEffect("bigExplode", 3.f, dGetSpritePositionX(pEnemy->szName), dGetSpritePositionY(pEnemy->szName), 0.f);
			else
				dPlayEffect("enemyExplode", 3.f, dGetSpritePositionX(pEnemy->szName), dGetSpritePositionY(pEnemy->szName), 0.f);
		}
	}
}


// 判断是否是ControlSprite发射的子弹
bool IsControlSpriteBullet(SGameSprite *pSprite)
{
	if (NULL != pSprite)
	{
		// 先判断是否是子弹类型的Sprite
		if (SPRITE_BULLET1 == pSprite->eSpriteType || SPRITE_BULLET2 == pSprite->eSpriteType)
			return (0 == pSprite->iBulletType);
	}
	return false;
}

bool IsControlSpriteBullet2(SGameSprite *pSprite)
{
	if (NULL != pSprite)
	{
		// 先判断是否是子弹类型的Sprite
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