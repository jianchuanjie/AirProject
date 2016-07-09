/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_
//
#include <Windows.h>
#include "CommonAPI.h"

extern void	GameMainLoop( float	fDeltaTime );
extern void OnMouseMove( const float fMouseX, const float fMouseY );
extern void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY );
extern void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY );
extern void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress );
extern void OnKeyUp( const int iKey );
extern void OnSpriteColSprite( const char *szSrcName, const char *szTarName );
extern void OnSpriteColWorldLimit( const char *szName, const int iColSide );
extern void InitTemplateData();
extern void UpdateMovement();

#define MAX_NAME_LEN  128 // 名字的最大长度

#define	CONTROL_MAX_HP 1000	// 我方战机的最大生命值是1000
#define BULLET_DAMAGE_1 100		// 子弹1的伤害值是100
#define VER_MIN_CREATE	1
#define VER_MAX_CREATE	6

enum ESpriteType
{
	SPRITE_CONTROL,				// 我们控制的战机
	SPRITE_VER,					// 敌方战机
	SPRITE_BULLET1,				// 朝前方发射的子弹，且可以被销毁
	SPRITE_BULLET2,				//特殊子弹
	SPRITE_HEALTH,				//回血精灵
	SPRITE_BIG_BOSS				//BOSS
};

struct SGameSprite
{
	char szName[MAX_NAME_LEN];			// 精灵的名字
	int iHp;								// Health Point生命值
	int iScore;							// 击毁本精灵可以获得的分数值
	int iDamage;							// 自身碰到敌方可以给敌方造成的伤害
	ESpriteType eSpriteType;				// 精灵的类型
	int tBulletType;				//判断是玩家1的子弹还是玩家2的子弹（玩家1为0，玩家2为1）
	int iBulletType;				// 该子弹类型，通过该类型判断是不是受到伤害
	float fFireAfterCreate;					// 精灵被创建多长时间可以发射子弹
	float fBulletTime;						// 子弹1被发射的时间间隔
	float fBulletTime2;
	float fFloatTime;						// 上下浮动的时间
	bool bFloatUp;						// 是否上下浮动


};




#endif // _LESSON_X_H_