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

//申明操作链表的函数
extern int GList_GetListSize();
// 根据名字获取Sprite
extern SGameSprite	*GList_GetSpriteByName(const char *szName);
// 根据索引获取Sprite，如果要遍历链表并删除其中的某个元素，请从后面往前面遍历(即索引初始化为链表大小然后递减)，否则必然出错
extern SGameSprite	*GList_GetSpriteByIndex(const int iIndex);
// 添加一个Sprite到链表里
extern SpriteStruct	*GList_AddSprite(SGameSprite *pSprite);
// 根据名字删除Sprite. bDeleteImage : 是否删除该Sprite在地图上的图片显示
extern void	GList_DeleteSprite(const char *szName, bool bDeleteImage = true);
// 根据指针删除Sprite. bDeleteImage : 是否删除该Sprite在地图上的图片显示
extern void	GList_DeleteSprite(SGameSprite *pSprite, bool bDeleteImage = true);
// 删除所有Sprite. bDeleteImage : 是否删除该Sprite在地图上的图片显示
extern void	GList_DeleteAllSprite(bool bDeleteImage = true);




#endif		//__LISTX_H_
