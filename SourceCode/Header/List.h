#ifndef _LISTX_H_
#define _LISTX_H_
#include <windows.h>
#include "CommonAPI.h"
#include "LessonX.h"

struct	SpriteStruct
{
	SGameSprite		*pSprite;
	SpriteStruct	*pNext;
	SpriteStruct	*pPrev;
};

//������������ĺ���
extern int GList_GetListSize();
// �������ֻ�ȡSprite
extern SGameSprite	*GList_GetSpriteByName(const char *szName);
// ����������ȡSprite�����Ҫ��������ɾ�����е�ĳ��Ԫ�أ���Ӻ�����ǰ�����(��������ʼ��Ϊ�����СȻ��ݼ�)�������Ȼ����
extern SGameSprite	*GList_GetSpriteByIndex(const int iIndex);
// ���һ��Sprite��������
extern SpriteStruct	*GList_AddSprite(SGameSprite *pSprite);
// ��������ɾ��Sprite. bDeleteImage : �Ƿ�ɾ����Sprite�ڵ�ͼ�ϵ�ͼƬ��ʾ
extern void	GList_DeleteSprite(const char *szName, bool bDeleteImage = true);
// ����ָ��ɾ��Sprite. bDeleteImage : �Ƿ�ɾ����Sprite�ڵ�ͼ�ϵ�ͼƬ��ʾ
extern void	GList_DeleteSprite(SGameSprite *pSprite, bool bDeleteImage = true);
// ɾ������Sprite. bDeleteImage : �Ƿ�ɾ����Sprite�ڵ�ͼ�ϵ�ͼƬ��ʾ
extern void	GList_DeleteAllSprite(bool bDeleteImage = true);




#endif		//__LISTX_H_
