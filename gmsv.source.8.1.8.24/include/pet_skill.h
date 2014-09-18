#ifndef __PET_SKILL_H__
#define __PET_SKILL_H__

#include "util.h"

typedef enum
{
	PETSKILL_FIELD_ALL,			/* ¤¹¤Ù¤Æ¤Î¾ì½ê¤Ç»È¤¨¤ë */
	PETSKILL_FIELD_BATTLE,				/* Àï    ¤Î¤ß */
	PETSKILL_FIELD_MAP,				/* ÄÌ¾ï¥Þ¥Ã¥×¾å¤Î¤ß */

}PETSKILL_FIELDTYPE;

typedef enum
{
	PETSKILL_TARGET_MYSELF,		/* ¼«Ê¬¤Î¤ß */
	PETSKILL_TARGET_OTHER,			/* Â¾¤Î¿Í  ¼«Ê¬´Þ¤à) */
	PETSKILL_TARGET_ALLMYSIDE,		/*     Á´   */
	PETSKILL_TARGET_ALLOTHERSIDE,	/* Áê¼êÂ¦Á´   */
	PETSKILL_TARGET_ALL,			/* Á´¤Æ */
	PETSKILL_TARGET_NONE,			/* Ã¯¤âÁª  ½Ð  ¤Ê¤¤¡e  ¸æ¤ä¤¿¤á¤Î»þ */
	PETSKILL_TARGET_OTHERWITHOUTMYSELF,/* Â¾¤Î¿Í  ¼«Ê¬´Þ¤Þ¤Ê¤¤) */
	PETSKILL_TARGET_WITHOUTMYSELFANDPET,  /* ¼«Ê¬¤È¥Ú¥Ã¥È°Ê³° */
}PETSKILL_TARGETTYPE;

typedef enum
{
	PETSKILL_ID,
	PETSKILL_FIELD,
	PETSKILL_TARGET,
#ifdef _PETSKILL2_TXT
	PETSKILL_USETYPE,
#endif
	PETSKILL_COST,
	PETSKILL_ILLEGAL,
	PETSKILL_DATAINTNUM,
}PETSKILL_DATAINT;

typedef enum
{
	PETSKILL_NAME,					/*      */
	PETSKILL_COMMENT,				/* ¥³¥á¥ó¥È*/
	PETSKILL_FUNCNAME,				/* ´Ø¿ô   */
	PETSKILL_OPTION,				/* ¥ª¥×¼³¥ç¥ó */
#ifdef _CFREE_petskill
	PETSKILL_FREE,					/*±ø¥ó*/
	PETSKILL_KINDCODE,				/*ºØÃþ½X*/
#endif

	PETSKILL_DATACHARNUM,
}PETSKILL_DATACHAR;

typedef struct tagPetskill
{
	int			data[PETSKILL_DATAINTNUM];
	STRING64	string[PETSKILL_DATACHARNUM];

}Petskill;

typedef int (*PETSKILL_CALLFUNC)( int, int, int, char * );

INLINE BOOL PETSKILL_CHECKINDEX( int index );
INLINE int PETSKILL_getInt( int index, PETSKILL_DATAINT element);
INLINE int PETSKILL_setInt( int index, PETSKILL_DATAINT element, int data);
INLINE char* PETSKILL_getChar( int index, PETSKILL_DATACHAR element);
INLINE BOOL PETSKILL_setChar( int index ,PETSKILL_DATACHAR element, char* new );
int PETSKILL_getPetskillNum( void);

#define		PETSKILL_GetArray( charaindex, havepetskill)	_PETSKILL_GetArray( __FILE__, __LINE__, charaindex, havepetskill)
int _PETSKILL_GetArray( char *file, int line, int charaindex, int havepetskill );

BOOL PETSKILL_initPetskill( char *filename);
BOOL PETSKILL_reinitPetskill( void );
int PETSKILL_getPetskillArray( int petskillid);
PETSKILL_CALLFUNC PETSKILL_getPetskillFuncPointer(char* name);

int PETSKILL_Use(
	int charaindex,
	int toindex,
	int array,
	char *data
	//BOOL isCLI	// Robin 2001/02/26 if owner is player
);

int PETSKILL_ContinuationAttack(
	int charaindex,
	int toindex,
	int array,
	char *data
);
int PETSKILL_ChargeAttack(
	int charaindex,
	int toindex,
	int array,
	char *data
);


#define PETSKILL_ID_GBREAK		0	// ¥¬¡¼¥É¥Ö¥ì¥¤¥¯
#define PETSKILL_ID_RENZOKU		1	//   Â³  ·â
#define PETSKILL_ID_GUARDIAN	2	// Ãé¸¤¥Ï¥Á¸ø
#define PETSKILL_ID_CHARGE		3	// ¥Á¥ã¡¼¥¸  ·â
#define PETSKILL_ID_ICHIGEKI	100	// °ì·â  »¦
#define PETSKILL_ID_POWERBALANCE 110	//     ¤Î¿Ø

//**********************************************************************
//
//  ÀÐ  -- Â¾¿Í¤ò¼é¤ê¤Ê¤¬¤é  ·â
//
int PETSKILL_Guardian(
	int charaindex,
	int toindex,
	int array,
	char *data
);
//
//**********************************************************************

int PETSKILL_PowerBalance(
	int charaindex,
	int toindex,
	int array,
	char *data
);

//**********************************************************************
//
//  ÀÐ  -- ¶¯  ¤À¤¬²ó  ¤µ¤ì¤ë³Î  ¤¬¹â¤¤
//
int PETSKILL_Mighty(
	int charaindex,
	int toindex,
	int array,
	char *data
);
//
//**********************************************************************


//**********************************************************************
//
//  ÀÐ  -- ¥¹  ¡¼¥¿¥¹°Û¾ï  ·â
//
int PETSKILL_StatusChange(
	int charaindex,
	int toindex,
	int array,
	char *data

);
//
//**********************************************************************


//*******************************************************
//
// ÀÐ  -- ÄÌ¾ï  ·â
//
int PETSKILL_NormalAttack(
	int charaindex,
	int toindex,
	int array,
	char *data

);
//
//*******************************************************

//*******************************************************
//
// ÀÐ  -- ÄÌ¾ï  ¸æ
//
int PETSKILL_NormalGuard(
	int charaindex,
	int toindex,
	int array,
	char *data

);
//
//*******************************************************

//*******************************************************
// ÀÐ  -- ÂÔµ¡(²¿¤â¤·¤Ê¤¤)
//
int PETSKILL_None(
	int charaindex,
	int toindex,
	int array,
	char *data

);
//
//*******************************************************

//*******************************************************
// ÀÐ  -- ¼Mµå°ì½µ(±£¤ì¤Æ£±¥¿¡¼¥ó  ¤ËÁê¼ê¤Î    ¤«¤é  ·â)
//
int PETSKILL_EarthRound(
	int charaindex,
	int toNo,
	int array,
	char *data

);
//
//*******************************************************

//*******************************************************
// ÀÐ  -- ¥¬¡¼¥É¥Ö¥ì¥¤¥¯
//
int PETSKILL_GuardBreak(
	int charaindex,
	int toNo,
	int array,
	char *data

);
///////////////////////////////////////////
#ifdef _SKILL_GUARDBREAK2//¯}°£¨¾¿m2 vincent add 2002/05/20
int PETSKILL_GuardBreak2(
	int charaindex,
	int toNo,
	int array,
	char *data

);
#endif
//
//*******************************************************
//*******************************************************
// ÀÐ  --     ¤ì
//
int PETSKILL_Abduct(
	int charaindex,
	int toNo,
	int array,
	char *data

);
//
//*******************************************************
//*******************************************************
// ÀÐ  --   ¤à
int PETSKILL_Steal(
	int charaindex,
	int toNo,
	int array,
	char *data

);
//*******************************************************
#ifdef _BATTLESTEAL_FIX
int PETSKILL_StealMoney(
	int charaindex,
	int toNo,
	int array,
	char *data

);
#endif

#ifdef _ITEM_INSLAY
int PETSKILL_Inslay(
	int index,
	int toNo,
	int array,
	char *data

);
#endif

#ifdef _PETSKILL_FIXITEM
int PETSKILL_Fixitem(
	int index,
	int toNo,
	int array,
	char *data

);
#endif

// ÀÐ  -- ¥¢¥¤  ¥à¹ç©¨
//
int PETSKILL_Merge(
	int charaindex,
	int toNo,
	int array,
	char *data

);
//
//*******************************************************

//*******************************************************
// ÀÐ  -- ¥Î¡¼¥¬¡¼¥É
//
int PETSKILL_NoGuard(
	int charaindex,
	int toNo,
	int array,
	char *data

);
//
//*******************************************************



// Terry add 2001/11/05
#ifdef __ATTACK_MAGIC
int PETSKILL_AttackMagic(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _VARY_WOLF
int PETSKILL_Vary( int cindex, int tindex, int id, char* data);
#endif

#ifdef _SKILL_WILDVIOLENT_ATT
//vincent add 2002/05/16
int PETSKILL_WildViolentAttack(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _SKILL_SPEEDY_ATT
//vincent add 2002/05/16
int PETSKILL_SpeedyAttack(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _SKILL_SACRIFICE
//vincent add 2002/05/30
int PETSKILL_Sacrifice(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _SKILL_REFRESH
//vincent add 2002/08/08
int PETSKILL_Refresh(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _SKILL_WEAKEN  //vincentÃd§Þ:µê®z
int PETSKILL_Weaken(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _SKILL_DEEPPOISON  //vincentÃd§Þ:¼@¬r 
int PETSKILL_Deeppoison(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _SKILL_BARRIER  //vincentÃd§Þ:Å]»Ù
int PETSKILL_Barrier(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _SKILL_NOCAST  //vincentÃd§Þ:¨IÀq
int PETSKILL_Nocast(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _SKILL_ROAR //vincentÃd§Þ:¤j§q
int PETSKILL_Roar(
	int charaindex,
	int toindex,
	int array,
	char *data
);
#endif

#ifdef _PSKILL_FALLGROUND
int PETSKILL_FallGround( int charaindex, int toNo, int array, char *data );
#endif
#ifdef _PETSKILL_EXPLODE
int PETSKILL_Explode( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PRO_BATTLEENEMYSKILL
int ENEMYSKILL_ReLife( int enemyindex, int toNo, int array, char *data );
int ENEMYSKILL_ReHP( int enemyindex, int toNo, int array, char *data );

int ENEMYSKILL_EnemyHelp( int enemyindex, int toNo, int array, char *data );
#endif

#ifdef _SKILL_DAMAGETOHP
int PETSKILL_DamageToHp( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_TIMID
int PETSKILL_BattleTimid( int charaindex, int toNo, int array, char *data );
#endif
#ifdef _PETSKILL_2TIMID
int PETSKILL_2BattleTimid( int charaindex, int toNo, int array, char *data );
#endif
#ifdef _PETSKILL_ANTINTER
int PETSKILL_AntInter(int charaindex, int toindex, int array, char* data);		// Ãdª«§Þ¯à¾Ô°«¼Ò²Õ
#endif

#ifdef _PETSKILL_PROPERTY
int PETSKILL_BattleProperty( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_TEAR
int PETSKILL_BattleTearDamage( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _BATTLE_LIGHTTAKE
int PETSKILL_Lighttakeed( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _BATTLE_ATTCRAZED
int PETSKILL_AttackCrazed( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _SHOOTCHESTNUT	// Syu ADD Ãd§Þ¡G¥á®ß¤l
int PETSKILL_AttackShoot( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _Skill_MPDAMAGE
int PETSKILL_MpDamage( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_SETDUCK
int PETSKILL_SetDuck( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _MAGICPET_SKILL
int PETSKILL_SetMagicPet( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _SKILL_TOOTH
int PETSKILL_ToothCrushe( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PSKILL_MODIFY
int PETSKILL_Modifyattack( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PSKILL_MDFYATTACK
int PETSKILL_Mdfyattack( int charaindex, int toNo, int array, char *data );
#endif


#ifdef _MAGIC_SUPERWALL
int	PETSKILL_MagicStatusChange( int charaindex, int toindex, int array, char *data );
#endif

#ifdef _PET_SKILL_SARS				// WON ADD ¬r·Ù½¯©µ
int PETSKILL_Sars( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _SONIC_ATTACK				// WON ADD ­µªi§ðÀ»
int PETSKILL_Sonic( int charaindex, int toNo, int array, char *data );
#endif
#ifdef _PETSKILL_REGRET
int PETSKILL_Regret( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_GYRATE
int PETSKILL_Gyrate( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_ACUPUNCTURE //°w¨ë¥~¥Ö
int PETSKILL_Acupuncture( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_RETRACE
int PETSKILL_Retrace( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_HECTOR
int PETSKILL_Hector( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_FIREKILL
int PETSKILL_Firekill( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_DAMAGETOHP   //·t¤ë¨g¯T(¶Ý¦å§ÞªºÅÜÅé) 
int PETSKILL_DamageToHp2( int charaindex, int toNo, int array, char *data );
#endif

#ifdef _PETSKILL_BECOMEFOX
int PETSKILL_BecomeFox( int charaindex, int toNo, int array, char* data);
#endif

#ifdef _PETSKILL_BECOMEPIG
int PETSKILL_BecomePig( int charaindex, int toNo, int array, char* data);
#endif

#ifdef _PETSKILL_SHOWMERCY
int PETSKILL_ShowMercy(int charaindex, int toNo, int array, char* data);
#endif

#ifdef _PETSKILL_COMBINED
int PETSKILL_Combined(int charaindex, int toNo, int array, char* data);
#endif

#ifdef _PETSKILL_LER
int PETSKILL_BatFly(int charaindex, int toNo, int array, char* data);					// ¹pº¸§Þ - ¸s½»¥|Â«
int PETSKILL_DivideAttack(int charaindex, int toNo, int array, char* data);		// ¹pº¸§Þ - ¤À¨­¦aµõ
#endif

#ifdef _PETSKILL_BATTLE_MODEL
int PETSKILL_BattleModel(int charaindex, int toindex, int array, char* data);		// Ãdª«§Þ¯à¾Ô°«¼Ò²Õ
#endif

#endif
