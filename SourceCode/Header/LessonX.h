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

#define MAX_NAME_LEN  128 // ���ֵ���󳤶�

#define	CONTROL_MAX_HP 1000	// �ҷ�ս�����������ֵ��1000
#define BULLET_DAMAGE_1 100		// �ӵ�1���˺�ֵ��100
#define VER_MIN_CREATE	1
#define VER_MAX_CREATE	6

enum ESpriteType
{
	SPRITE_CONTROL,				// ���ǿ��Ƶ�ս��
	SPRITE_VER,					// �з�ս��
	SPRITE_BULLET1,				// ��ǰ��������ӵ����ҿ��Ա�����
	SPRITE_BULLET2,				//�����ӵ�
	SPRITE_HEALTH,				//��Ѫ����
	SPRITE_BIG_BOSS				//BOSS
};

struct SGameSprite
{
	char szName[MAX_NAME_LEN];			// ���������
	int iHp;								// Health Point����ֵ
	int iScore;							// ���ٱ�������Ի�õķ���ֵ
	int iDamage;							// ���������з����Ը��з���ɵ��˺�
	ESpriteType eSpriteType;				// ���������
	int tBulletType;				//�ж������1���ӵ��������2���ӵ������1Ϊ0�����2Ϊ1��
	int iBulletType;				// ���ӵ����ͣ�ͨ���������ж��ǲ����ܵ��˺�
	float fFireAfterCreate;					// ���鱻�����೤ʱ����Է����ӵ�
	float fBulletTime;						// �ӵ�1�������ʱ����
	float fBulletTime2;
	float fFloatTime;						// ���¸�����ʱ��
	bool bFloatUp;						// �Ƿ����¸���


};




#endif // _LESSON_X_H_