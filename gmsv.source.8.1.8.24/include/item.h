
#ifndef __ITEM_H__
#define __ITEM_H__

#include "char.h"

#define NULLITEM    "0"

typedef enum
{
    ITEM_FIST =0,
    ITEM_AXE,
    ITEM_CLUB,
    ITEM_SPEAR,
    ITEM_BOW,
    ITEM_SHIELD,
    ITEM_HELM,
    ITEM_ARMOUR,

	ITEM_BRACELET =8,
	ITEM_MUSIC,
	ITEM_NECKLACE,
	ITEM_RING,
	ITEM_BELT,
	ITEM_EARRING,
	ITEM_NOSERING,
	ITEM_AMULET,
    /* ****** */
    ITEM_OTHER =16,
    ITEM_BOOMERANG,		// °j±Û¼Ð
    ITEM_BOUNDTHROW,	// §ëÂY©òÀY
    ITEM_BREAKTHROW,	// §ëÂY¥Û
    ITEM_DISH =20,
#ifdef _ITEM_INSLAY
	ITEM_METAL,
	ITEM_JEWEL,
#endif
#ifdef _ITEM_CHECKWARES
	ITEM_WARES,			//³fª«
#endif

#ifdef _ITEM_EQUITSPACE
	ITEM_WBELT,			//¸y±a
	ITEM_WSHIELD,		//¬Þ
	ITEM_WSHOES,		//¾c¤l
#endif
#ifdef _EQUIT_NEWGLOVE 
	ITEM_WGLOVE,		//¤â®M
#endif

#ifdef _ALCHEMIST
	ITEM_ALCHEMIST =30,
#endif

#ifdef _ANGEL_SUMMON
	//ITEM_ANGELTOKEN,
	//ITEM_HEROTOKEN,
#endif

    ITEM_CATEGORYNUM,
    
}ITEM_CATEGORY;

typedef enum
{
	ITEM_FIELD_ALL,
	ITEM_FIELD_BATTLE,
	ITEM_FIELD_MAP,
}ITEM_FIELDTYPE;

typedef enum
{
	ITEM_TARGET_MYSELF,
	ITEM_TARGET_OTHER,
	ITEM_TARGET_ALLMYSIDE,
	ITEM_TARGET_ALLOTHERSIDE,
	ITEM_TARGET_ALL,
}ITEM_TARGETTYPE;

typedef enum
{
    ITEM_ID,
    ITEM_BASEIMAGENUMBER,
    ITEM_COST,
    ITEM_TYPE,
	ITEM_ABLEUSEFIELD,
	ITEM_TARGET,
    ITEM_LEVEL,                     /*  LEVEL  */
#ifdef _ITEM_MAXUSERNUM
	ITEM_DAMAGEBREAK,				//ª««~¨Ï¥Î¦¸¼Æ
#endif

#ifdef _ITEMSET4_TXT
	ITEM_USEPILENUMS,				//ª««~°ïÅ|¦¸¼Æ
	ITEM_CANBEPILE,					//¬O§_¥i°ïÅ|

	ITEM_NEEDSTR,
	ITEM_NEEDDEX,
	ITEM_NEEDTRANS,
	ITEM_NEEDPROFESSION,
#endif

#ifdef _TAKE_ITEMDAMAGE
	ITEM_DAMAGECRUSHE,
	ITEM_MAXDAMAGECRUSHE,
#endif

#ifdef _ADD_DEAMGEDEFC
	ITEM_OTHERDAMAGE,
	ITEM_OTHERDEFC,
#endif

#ifdef _SUIT_ITEM
	ITEM_SUITCODE,
#endif

    ITEM_ATTACKNUM_MIN,             /*    Äã  ·â²ó¿ô  */
    ITEM_ATTACKNUM_MAX,             /*    ¹â  ·â²ó¿ô  */
    ITEM_MODIFYATTACK,              /*    ·â    ²½¬¤  */
    ITEM_MODIFYDEFENCE,             /*    ¸æ    ²½¬¤  */
    ITEM_MODIFYQUICK,               /*  QUICK  ²½¬¤  */

    ITEM_MODIFYHP,                  /*  HP  ²½¬¤    */
    ITEM_MODIFYMP,                  /*  MP  ²½¬¤    */
    ITEM_MODIFYLUCK,                /*  LUCK  ²½¬¤    */
    ITEM_MODIFYCHARM,               /*  CHARM  ²½¬¤    */
    ITEM_MODIFYAVOID,               /*  ²ó    ½¤Àµ    */
	ITEM_MODIFYATTRIB,				/*  Â°À­½¤Àµ */
	ITEM_MODIFYATTRIBVALUE,			/*  Â°À­½¤ÀµÃÍ */
	ITEM_MAGICID,					/*  ¼ö½Ñ  ¹æ */
	ITEM_MAGICPROB,					/*  ¼ö½Ñ       */
	ITEM_MAGICUSEMP,				/*  ¾Ã  MP */

#ifdef _ITEMSET5_TXT
	ITEM_MODIFYARRANGE,
	ITEM_MODIFYSEQUENCE,

	ITEM_ATTACHPILE,
	ITEM_HITRIGHT,	//ÃB¥~©R¤¤
#endif
#ifdef _ITEMSET6_TXT
	ITEM_NEGLECTGUARD,
//	ITEM_BEMERGE,
#endif
    /*  ¥¹  ¡¼¥¿¥¹½¤ÀµÃÍ¡e*/
    ITEM_POISON,					/*   ¬¤¤¯¤¿¤Ó¤Ë¥À¥á¡¼¥¸          */
    ITEM_PARALYSIS,          		/* ¤·¤Ó¤ì¡b1  ¤Î¹Ô  ¤¬¤Ç¤­¤Ê¤¤¡e */
    ITEM_SLEEP,              		/* Ì²¤ê¡e¹Ô  ¤Ç¤­¤Ê¤¤            */
    ITEM_STONE,              		/* ÀÐ¡e¹Ô  ¤Ç¤­¤Ê¤¤              */
    ITEM_DRUNK,              		/* ¿ì¤¦¡e      ¤¬²¼¤¬¤ë     */
    ITEM_CONFUSION,          		/* º®Íð¡e  ·â    ¤ò¸í¤ë     */

	ITEM_CRITICAL,					/* ¥¯¥ê  ¥£¥«¥ë  ½¤Àµ */

	ITEM_USEACTION,					/* »È¤Ã¤¿»þ¤Î¥¢¥¯¼³¥ç¥ó */
    ITEM_DROPATLOGOUT,              /* ¤Ò¥°¥¢¥¦¥È¤¹¤ë»þ¤Ë  ¤¹¤«¤É¤¦¤«  */
    ITEM_VANISHATDROP,              /*   ¤·¤¿»þ¤Ë¾Ã¤¨¤ë¤«¤É¤¦¤« */
    ITEM_ISOVERED,                  /*  ¾å¤Ë¾è¤Ã¤«¤é¤é¤ì¤ë¤«¤É¤¦¤«¡e*/
	ITEM_CANPETMAIL,				/* ¥Ú¥Ã¥È¥á¡¼¥ë¤ÇÁ÷¤ì¤ë¤« */
	ITEM_CANMERGEFROM,				/* ¹ç©¨¸µ¤Ë¤Ê¤ì¤ë¤« */
	ITEM_CANMERGETO,				/* ¹ç©¨Àè¤Ë¤Ê¤ì¤ë¤« */

    ITEM_INGVALUE0,                 /* ©¨Ê¬(5¸ÄÊ¬) */
    ITEM_INGVALUE1,
    ITEM_INGVALUE2,
    ITEM_INGVALUE3,
    ITEM_INGVALUE4,
    
	ITEM_PUTTIME,					/*  ¥¢¥¤  ¥à¤¬  ¤«¤ì¤¿»þ´Ö */
    ITEM_LEAKLEVEL,                 /*    Ì©¤¬¤É¤ì¤À¤±¤Ð¤ì¤¿¤«  */
	ITEM_MERGEFLG,					/*  ¹ç©¨¤µ¤ì¤¿¥¢¥¤  ¥à¤«¤É¤¦¤« */
	ITEM_CRUSHLEVEL,				/*  ²õ¤ìÅÙ¹ç¤¤ 0  2 £°¤Ï²õ¤ì¤Æ¤Ê¤¤ 2¤ÏÁ´²õ */

    ITEM_VAR1,              	/*    ÍÑºî¶È       */
    ITEM_VAR2,              	/*    ÍÑºî¶È       */
    ITEM_VAR3,              	/*    ÍÑºî¶È       */
    ITEM_VAR4,              	/*    ÍÑºî¶È       */

	ITEM_DATAINTNUM,

}ITEM_DATAINT;

typedef enum
{
    ITEM_NAME,                      /*    Á°     Åö¤Î  Á°     */
    ITEM_SECRETNAME,                /*    Á°    ¹¹¤µ¤ì¤ë²Ä  À­  ¤ê    */
    ITEM_EFFECTSTRING,              /*  ¸ú²Ì  »ú    */
    ITEM_ARGUMENT,                  /*  ¥¢¥¤  ¥à¤Î°ú¿ô  */
#ifdef _ITEM_INSLAY
	ITEM_TYPECODE,
	ITEM_INLAYCODE,
#endif
	ITEM_CDKEY,						/*  ¥¢¥¤  ¥à¤Î  Á°¤ò  ½é¤Ë  ¹¹¤·¤¿¿Í¤Î           */
#ifdef _ITEM_FORUSERNAMES
	ITEM_FORUSERNAME,
	ITEM_FORUSERCDKEY,
#endif
// CoolFish: 2001/10/11
#ifdef _UNIQUE_P_I
    ITEM_UNIQUECODE,		  /* ª««~½s½X */
#endif

    ITEM_INGNAME0,                  /*  ©¨Ê¬¤Î  Á°(5¸ÄÊ¬) */
    ITEM_INGNAME1,
    ITEM_INGNAME2,
    ITEM_INGNAME3,
    ITEM_INGNAME4,


    ITEM_INITFUNC,                  /* °ú¿ô
                                     * ITEM_Item*
                                     * ÊÖ¤êÃÍ BOOL
                                     * ÊÖ¤êÃÍ¤Î°Õ  ¤Ï CHAR_INITFUNC
                                     * ¤È  ¤¸  */
    ITEM_FIRSTFUNCTION = ITEM_INITFUNC,
    ITEM_PREOVERFUNC,               /* CHAR_PREOVERFUNC ¤ò»²¾È */
    ITEM_POSTOVERFUNC,              /* CHAR_POSTOVERFUNC ¤ò»²¾È*/
    ITEM_WATCHFUNC,                 /* CHAR_WATCHFUNC ¤ò»²¾È */
    ITEM_USEFUNC,                   /* °ú¿ô¤Ï¡b
                                     * int charaindex ¥­¥ã¥é¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     * int charitemindex ¼«Ê¬¤Î
                                     *              ¥¢¥¤  ¥à    ¤Î²¿    
                                     *              ¤ò»È¤Ã¤¿¤«
                                     */
    ITEM_ATTACHFUNC,                /* °ú¿ô¤Ï¡b
                                     * int charaindex ¥­¥ã¥é¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     * int itemindex  ¥¢¥¤  ¥à¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     *      ¥­¥ã¥é¥¯¥¿¤Î  ¤Ã¤Æ¤¤¤ë¥¢¥¤  ¥à
                                     *      ¤Î¥¢¥¤  ¥à  ¤Ç¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     *      ¤Ç¤Ï¤Ê¤¤»ö¤ËÃí°Õ¡e
                                     */
    ITEM_DETACHFUNC,                /* °ú¿ô¤Ï¡b
                                     * int charaindex ¥­¥ã¥é¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     * int itemindex  ¥¢¥¤  ¥à¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     *      ¥­¥ã¥é¥¯¥¿¤Î  ¤Ã¤Æ¤¤¤ë¥¢¥¤  ¥à
                                     *      ¤Î¥¢¥¤  ¥à  ¤Ç¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     *      ¤Ç¤Ï¤Ê¤¤»ö¤ËÃí°Õ¡e
                                     */
    ITEM_DROPFUNC, 		            /*   ¤È¤·¤¿¤È¤­
                                     * °ú¿ô¤Ï
                                     *  int charaindex   ¤È¤·¤¿¥­¥ã¥é
                                     *  int itemindex ¥¢¥¤  ¥à¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     */
    ITEM_PICKUPFUNC,              /* ¥¢¥¤  ¥à¤ò½¦¤Ã¤¿»þ
                                     * °ú¿ô¤Ï
                                     *  int charaindex  ½¦¤Ã¤¿¥­¥ã¥éindex
                                     *  int itemindex ¥¢¥¤  ¥à¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     */
#ifdef _Item_ReLifeAct
	ITEM_DIERELIFEFUNC,					/*ANDY_ADD
										´_¬¡¹D¨ã	
									 */
#endif

#ifdef _CONTRACT
	ITEM_CONTRACTTIME,
	ITEM_CONTRACTARG,
#endif

    ITEM_LASTFUNCTION,

    ITEM_DATACHARNUM = ITEM_LASTFUNCTION,

#ifdef _ANGEL_SUMMON
	ITEM_ANGELMISSION = ITEM_INGNAME0,
	ITEM_ANGELINFO = ITEM_INGNAME1,
	ITEM_HEROINFO = ITEM_INGNAME2,
#endif

}ITEM_DATACHAR;

typedef enum
{
    ITEM_WORKOBJINDEX,
    ITEM_WORKCHARAINDEX,
#ifdef _MARKET_TRADE
	ITEM_WORKTRADEINDEX,
	ITEM_WORKTRADETYPE,
	ITEM_WORKTRADESELLINDEX,
#endif
#ifdef _ITEM_ORNAMENTS
	ITEM_CANPICKUP,
#endif
#ifdef _ITEM_TIME_LIMIT
	ITEM_WORKTIMELIMIT,
#endif
    ITEM_WORKDATAINTNUM,
}ITEM_WORKDATAINT;



typedef struct tagItem
{
    int         data[ITEM_DATAINTNUM];
    STRING64    string[ITEM_DATACHARNUM];
    int         workint[ITEM_WORKDATAINTNUM];

    void*       functable[ITEM_LASTFUNCTION-ITEM_FIRSTFUNCTION];
}ITEM_Item;

typedef struct tagITEM_table
{
    int         use; 
    ITEM_Item   itm;
    int         randomdata[ITEM_DATAINTNUM];
}ITEM_table;


typedef struct tagITEM_exists
{
    BOOL        use;
    ITEM_Item   itm;
}ITEM_exists;

#ifdef _CONTRACT
#define MAX_CONTRACTTABLE	10
typedef struct tagITEM_contract
{
    int         used;
    char		detail[2048];
    int			argnum;
}ITEM_contractTable;
#endif


#define		ITEM_CHECKINDEX(index)		\
	_ITEM_CHECKINDEX( __FILE__, __LINE__, index)
INLINE BOOL _ITEM_CHECKINDEX( char *file, int line, int index);


BOOL ITEM_initExistItemsArray( int num );
BOOL ITEM_endExistItemsArray( void );
#define		ITEM_initExistItemsOne( itm) \
	_ITEM_initExistItemsOne( __FILE__, __LINE__, itm)
int _ITEM_initExistItemsOne( char *file, int line, ITEM_Item* itm );

#define		ITEM_endExistItemsOne( index ) \
			_ITEM_endExistItemsOne( index, __FILE__, __LINE__)

void _ITEM_endExistItemsOne( int index , char *file, int line);

#define ITEM_getInt( Index, element) _ITEM_getInt( __FILE__, __LINE__, Index, element )
INLINE int _ITEM_getInt( char *file, int line, int index ,ITEM_DATAINT element);


#define ITEM_setInt( Index, element, data) _ITEM_setInt( __FILE__, __LINE__, Index, element, data)
INLINE int _ITEM_setInt( char *file, int line, int index ,ITEM_DATAINT element, int data);


INLINE char* ITEM_getChar( int index ,ITEM_DATACHAR element );
INLINE BOOL ITEM_setChar( int index ,ITEM_DATACHAR element , char* new);

INLINE int ITEM_getWorkInt( int index ,ITEM_WORKDATAINT element);
INLINE int ITEM_setWorkInt( int index ,ITEM_WORKDATAINT element, int data);
INLINE int ITEM_getITEM_itemnum( void );
INLINE int ITEM_getITEM_UseItemnum( void );
INLINE BOOL ITEM_getITEM_use( int index );
void ITEM_constructFunctable( int itemindex );
void* ITEM_getFunctionPointer( int itemindex, int functype );
INLINE ITEM_Item *ITEM_getItemPointer( int index );
int ITEM_getItemMaxIdNum( void);


char* ITEM_makeStringFromItemData( ITEM_Item* one, int mode );
char* ITEM_makeStringFromItemIndex( int index, int mode );

BOOL ITEM_makeExistItemsFromStringToArg( char* src , ITEM_Item* item, int mode );
void ITEM_getDefaultItemSetting( ITEM_Item* itm);


INLINE BOOL ITEM_CHECKITEMTABLE( int number );
BOOL    ITEM_readItemConfFile( char* filename );


CHAR_EquipPlace ITEM_getEquipPlace( int charaindex, int itmid );


char*  ITEM_makeItemStatusString( int haveitemindex, int itemindex );
char*   ITEM_makeItemFalseString( void );
char*   ITEM_makeItemFalseStringWithNum( int haveitemindex );


BOOL ITEM_makeItem( ITEM_Item* itm, int number );
int ITEM_makeItemAndRegist( int number );


void ITEM_equipEffect( int index );

void Other_DefcharWorkInt( int index);

char* ITEM_getAppropriateName(int itemindex);
char* ITEM_getEffectString( int itemindex );


int ITEM_getcostFromITEMtabl( int itemid );

#define ITEM_getNameFromNumber( id) _ITEM_getNameFromNumber( __FILE__, __LINE__, id)
INLINE char* _ITEM_getNameFromNumber( char *file, int line, int itemid );


int ITEM_getlevelFromITEMtabl( int itemid );
int ITEM_getgraNoFromITEMtabl( int itemid );
char *ITEM_getItemInfoFromNumber( int itemid );

int ITEM_getdropatlogoutFromITEMtabl( int itemid );
int ITEM_getvanishatdropFromITEMtabl( int itemid );
int ITEM_getcanpetmailFromITEMtabl( int itemid );
int ITEM_getmergeItemFromFromITEMtabl( int itemid );

#ifdef _ITEM_CHECKWARES
BOOL CHAR_CheckInItemForWares( int charaindex, int flg);
#endif

BOOL ITEM_canuseMagic( int itemindex);
// Nuke +1 08/23 : For checking the validity of item target
int ITEM_isTargetValid( int charaindex, int itemindex, int toindex);


#ifdef _IMPOROVE_ITEMTABLE
BOOL ITEMTBL_CHECKINDEX( int ItemID);
int ITEM_getSIndexFromTransList( int ItemID);
int ITEM_getMaxitemtblsFromTransList( void);
int ITEM_getTotalitemtblsFromTransList( void);
#endif

int ITEMTBL_getInt( int ItemID, ITEM_DATAINT datatype);
char *ITEMTBL_getChar( int ItemID, ITEM_DATACHAR datatype);

int ITEM_getItemDamageCrusheED( int itemindex);
void ITEM_RsetEquit( int charaindex);//¦Û°Ê¨ø°£¸Ë³Æ¦ì¸m¿ù»~¤§ª««~
void ITEM_reChangeItemToPile( int itemindex);
void ITEM_reChangeItemName( int itemindex);


#ifdef _SIMPLIFY_ITEMSTRING
void ITEM_getDefaultItemData( int itemID, ITEM_Item* itm);
#endif

#ifdef _CONTRACT
BOOL ITEM_initContractTable( );
#endif

#endif
