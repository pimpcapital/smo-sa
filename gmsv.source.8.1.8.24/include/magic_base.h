#ifndef __MAGIC_BASE_H__
#define __MAGIC_BASE_H__

#include "util.h"

typedef enum
{
	MAGIC_FIELD_ALL,			/* 允屯化及樺赭匹銀尹月 */
	MAGIC_FIELD_BATTLE,				/* 爵    及心 */
	MAGIC_FIELD_MAP,				/* 騷橘穴永皿曉及心 */

}MAGIC_FIELDTYPE;

typedef enum
{
	MAGIC_TARGET_MYSELF,		/* 憤坌及心 */
	MAGIC_TARGET_OTHER,			/* 職及諦  憤坌殖戈) */
	MAGIC_TARGET_ALLMYSIDE,		/*     蟈   */
	MAGIC_TARGET_ALLOTHERSIDE,	/* 鍬澎礎蟈   */
	MAGIC_TARGET_ALL,			/* 蟈化 */
	MAGIC_TARGET_NONE,			/* 簿手薊  請  卅中〔  豢支凶戶及凜 */
	MAGIC_TARGET_OTHERWITHOUTMYSELF,/* 職及諦  憤坌殖引卅中) */
	MAGIC_TARGET_WITHOUTMYSELFANDPET,  /* 憤坌午矢永玄動陸 */
	MAGIC_TARGET_WHOLEOTHERSIDE,/*     及扔奶玉蟈   */

#ifdef __ATTACK_MAGIC

        MAGIC_TARGET_SINGLE,            // 針對敵方的某一人
        MAGIC_TARGET_ONE_ROW,           // 針對敵方的某一列
        MAGIC_TARGET_ALL_ROWS,          // 針對敵方的所有人

#endif
}MAGIC_TARGETTYPE;

typedef enum
{
	MAGIC_ID,					/* 棟    寞 */
	MAGIC_FIELD,				/* 銀尹月樺赭 */
	MAGIC_TARGET,				/* 覆擂 */
	MAGIC_TARGET_DEADFLG,		/* 韶氏分樊手覆擂卞殖戶月井 */
#ifdef __ATTACK_MAGIC
  MAGIC_IDX ,
#endif
	MAGIC_DATAINTNUM,
}MAGIC_DATAINT;

typedef enum
{
	MAGIC_NAME,					/* 熱諸   */
	MAGIC_COMMENT,				/* 戊丟件玄*/
	MAGIC_FUNCNAME,				/* 楮醒   */
	MAGIC_OPTION,				/* 左皿撲亦件 */
	MAGIC_DATACHARNUM,
}MAGIC_DATACHAR;

typedef struct tagMagic
{
	int			data[MAGIC_DATAINTNUM];
	STRING64	string[MAGIC_DATACHARNUM];

}Magic;

#ifdef __ATTACK_MAGIC

typedef struct tagAttMagic
{
  unsigned int  uiSpriteNum;// 此咒術在Spr_x.bin的編號
  unsigned int  uiAttackType;// 攻擊的方式：單人，整排( 輪流 ) ,  整排( 輪流 ) , 整排( 同時 ) , 全體( 輪流 ) , 全體( 同時 )
  unsigned int  uiSliceTime;// 輪流攻擊時的時間差
  unsigned int  uiShowType;             // 顯示的位置方式：中央、指定
  int           siSx;                   // 顯示的位置 - X軸
  int           siSy;                   // 顯示的位置 - Y軸
  unsigned int  uiShowBehindChar;       // 顯示在人物的前方或下方
  unsigned int  uiShakeScreen;          // 是否震動畫面
  unsigned int  uiShakeFrom;            // 震動畫面的起始時間( 毫秒 )
  unsigned int  uiShakeTo;              // 震動畫面的結束時間( 毫秒 _
  unsigned int  uiPrevMagicNum;         // 前置咒術的索引號( 0XFFFFFFFFFF 表示無前置咒術 )
  int           siPrevMagicSx;          // 前置咒術的顯示位置 - X軸
  int           siPrevMagicSy;          // 前置咒術的顯示位置 - Y軸
  int           siPrevMagicOnChar;      // 前置咒術顯示在人物的前方或下方
  unsigned int  uiPostMagicNum;         // 後置咒術的索引號( 0XFFFFFFFF 表示無後置咒術 )
  int           siPostMagicSx;          // 後置咒術的顯示位置 - X軸
  int           siPostMagicSy;          // 後置咒術的顯示位置 - Y軸
  int           siPostMagicOnChar;      // 後置咒術顯示在人物的前方或下方
  int           siField[3][5];          // 攻擊索引
}AttMagic;

#endif

#ifdef _MAGIC_TOCALL

typedef struct tagToCallMagic
{
  unsigned int  uiSpriteNum;// 此咒術在Spr_x.bin的編號
  unsigned int  uiAttackType;// 攻擊的方式：單人，整排( 輪流 ) ,  整排( 輪流 ) , 整排( 同時 ) , 全體( 輪流 ) , 全體( 同時 )
  unsigned int  uiSliceTime;// 輪流攻擊時的時間差
  unsigned int  uiShowType;             // 顯示的位置方式：中央、指定
  int           siSx;                   // 顯示的位置 - X軸
  int           siSy;                   // 顯示的位置 - Y軸
  unsigned int  uiShowBehindChar;       // 顯示在人物的前方或下方
  unsigned int  uiShakeScreen;          // 是否震動畫面
  unsigned int  uiShakeFrom;            // 震動畫面的起始時間( 毫秒 )
  unsigned int  uiShakeTo;              // 震動畫面的結束時間( 毫秒 _
  unsigned int  uiPrevMagicNum;         // 前置咒術的索引號( 0XFFFFFFFFFF 表示無前置咒術 )
  int           siPrevMagicSx;          // 前置咒術的顯示位置 - X軸
  int           siPrevMagicSy;          // 前置咒術的顯示位置 - Y軸
  int           siPrevMagicOnChar;      // 前置咒術顯示在人物的前方或下方
  unsigned int  uiPostMagicNum;         // 後置咒術的索引號( 0XFFFFFFFF 表示無後置咒術 )
  int           siPostMagicSx;          // 後置咒術的顯示位置 - X軸
  int           siPostMagicSy;          // 後置咒術的顯示位置 - Y軸
  int           siPostMagicOnChar;      // 後置咒術顯示在人物的前方或下方
  int			isPostDisappear;		// 咒術一般攻擊完時是否馬上消失
  int			ToCallMagicNo;			// 召喚術的編號
}ToCallMagic;

#endif

typedef int (*MAGIC_CALLFUNC)( int, int, int, int );

INLINE BOOL MAGIC_CHECKINDEX( int index );
INLINE int MAGIC_getInt( int index, MAGIC_DATAINT element);
INLINE int MAGIC_setInt( int index, MAGIC_DATAINT element, int data);
INLINE char* MAGIC_getChar( int index, MAGIC_DATACHAR element);
INLINE BOOL MAGIC_setChar( int index ,MAGIC_DATACHAR element, char* new );
int MAGIC_getMagicNum( void);
BOOL MAGIC_initMagic( char *filename);
BOOL MAGIC_reinitMagic( void );

#ifdef __ATTACK_MAGIC

BOOL ATTMAGIC_initMagic( char *filename );
BOOL ATTMAGIC_reinitMagic( void );

#endif

int MAGIC_getMagicArray( int magicid);
MAGIC_CALLFUNC MAGIC_getMagicFuncPointer(char* name);
// Nuke +1 08/23 : For checking the validity of magic target
int MAGIC_isTargetValid( int magicid, int toindex);

#endif

