#include "version.h"
#include <ctype.h>

#include "object.h"
#include "char_base.h"
#include "char.h"
#include "item.h"
#include "util.h"
#include "handletime.h"
#include "anim_tbl.h"
#include "npc_door.h"
#include "lssproto_serv.h"
#include "npcutil.h"
#include "npccreate.h"
#include "log.h"


/*
 *   ½àÅª¤ÊÅ¹¤Î¥ë¡¼¥Á¥ó¡eby nakamura
 *
 * NPCARGUMENT¤ËÍå  ¤·¤¿¥¢¥¤  ¥à¤ò  ¸Â¤Ë½ê  ¤·¤Æ¤¤¤ë¶³Å¹¡e
 *     SIMPLESHOP_MAXINFINITITEM¼ï    ¸Â¤ËÀ¸©¨¤¹¤ë¤³¤È¤¬¤Ç¤­¤ë¡e
 *   ¸ÂÀ¸©¨¥¢¥¤  ¥à¤È  ¤¸¼ï  ¤Î¥¢¥¤  ¥à¤ò  ¤Ã¤¿¤é  ¤Ã¤¿¤ä¤Ä¤¬
 * À¤³¦¤«¤é¾Ã  ¤¹¤ë¡e
 * ¤Þ¤¿¡b¥×¥ì¥¤¥ä¡¼¤«¤é¤â¤Î¤ò  ¤¤¤È¤ë¤³¤È¤¬¤Ç¤­¤ë¤¬¡b¤¢¤ëÃÍÃÊ
 * (NPC_SIMPLESHOPMAXBUYPRICE)¤è¤ê¹â¤¤¶â¤ò»Ù  ¤ï¤Ê¤¯¤Æ¤Ï¤¤¤±
 * ¤Ê¤¤  ¼è¤Ï¤Ç¤­¤Ê¤¤¡e  ¤¤¤È¤Ã¤¿¥¢¥¤  ¥à¤Ï¤½¤Î¾ì¤Ç¤Ê¤¯¤Ê¤ë¡e
 * ¤Þ¤¿¡b¥¢¥¤  ¥à¤ÎÀßÄê¤Çcost¤¬ÀßÄê¤µ¤ì¤Æ¤¤¤Ê¤¤¤â¤Î¤â  ¤¤¤È¤ì¤Ê¤¤¡e
 * 2¿Í°Ê¾å¤Î¥×¥ì¥¤¥ä¡¼¤¬Å¹¤Ë¤­¤¿¤È¤­¤Ï¡b  ¤ÎµÒ¤òÍ¥Àè¤¹¤ë¡e¤½¤¦¤¹¤ë
 * ¤³¤È¤Ë¤è¤êÅ¹¤¬±Êµ×¤ËÃ¯¤«¤òÁê¼ê¤·¤Æ¤¤¤ë¤³¤È¤ò  ¤°¡e
 *
 * ¥¿¥¤¥×  ¡uSimpleShop
 * °·¤¦¥¤¥Ù¥ó¥È: init, talked, specialtalked
 *
 *
 */
static void NPC_SimpleShopOpenShopWindow( int meindex, int cliindex,
                                          char *npcarg );
static void NPC_SimpleShopGetEscapedItemString( int shopindex,
                                                int cliindex, char *str );
static BOOL NPC_SimpleShopProcessBuyMsg( char *msg, int shop, int cli );
static BOOL NPC_SimpleShopProcessSellMsg( char *msg, int shop, int cli );

static BOOL NPC_LimitBuyInShopProcessSellMsg(char *msg,int shop,int cli);


static BOOL NPC_SimpleShopFillItem( int meindex, char *npcarg );
static int NPC_SimpleShopGetItemNum( int meindex );
static void NPC_SimpleShopNormalWindow( int meindex,
                                        int playerindex, char *str );
static int NPC_ShopWindowIndexToItemTableIndex( int charindex,
                                                int shopwindowindex );
static BOOL NPC_SimpleShopOpenShopOrNot( char *msg, char *openshopstr );
//static void NPC_SimpleShopSetLeakLevel( int meindex );

#define NPC_SIMPLESHOPMAXBUYPRICE 9999
#define SIMPLESHOPTALKBUFSIZE 256

#define SIMPLESHOP_FATALERRSTR "¦nµh¡I¹ï¤£°_,³Ìªñª¬ªp¤£¤Ó¨Î¡C¡C¡C"

#define SIMPLESHOP_MAXINFINITITEM 40

enum{
    OPENSHOPTOKEN=1,
        MSGTOKEN,
        MAINTOKEN,
        BUYTOKEN,
        OKBUYTOKEN,
        SELLTOKEN,
        OKSELLTOKEN,
        POORTOKEN,
        ITEMFULLTOKEN,
        ANOTHERPLAYERTOKEN,
        RAREITEMTOKEN,
        THANKYOUTOKEN,
        ITEMLISTTOKEN,

        BUYINITEMLISTTOKEN,
        };
void NPC_SimpleShopTalked( int meindex, int talker, char *msg, int color )
{
#define SHOPRANGE 3
    char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE], token[NPC_UTIL_GETARGSTR_LINEMAX];
    char *npcarg;
    if( CHAR_getInt( talker,CHAR_WHICHTYPE) != CHAR_TYPEPLAYER ){
        return;
    }
    if( NPC_Util_charIsInFrontOfChar( talker, meindex, SHOPRANGE )
        && ! CHAR_getFlg( talker,CHAR_ISDIE )  ){
        npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof(npcarg1));
        getStringFromIndexWithDelim( npcarg,"|", OPENSHOPTOKEN,token, sizeof(token));
        if( NPC_SimpleShopOpenShopOrNot( msg, token )
           && NPC_Util_isFaceToFace( meindex, talker, SHOPRANGE ) ){
            CHAR_setWorkInt( meindex, CHAR_WORKSHOPCLIENTINDEX, talker );
            NPC_SimpleShopOpenShopWindow( meindex, talker, npcarg );
        }else{
            int tokennum;
            int i;
            char tmp[NPC_UTIL_GETARGSTR_LINEMAX];
            getStringFromIndexWithDelim( npcarg,"|", MSGTOKEN,token, sizeof(token));
            tokennum = 1;
            for( i=0;token[i]!='\0';i++ ){
                if( token[i] == ',' ) tokennum++;
            }
            getStringFromIndexWithDelim( token,",", rand()%tokennum+1,tmp, sizeof(tmp));
            CHAR_talkToCli( talker, meindex, tmp, CHAR_COLORWHITE );
        }
    }
}

void NPC_SimpleShopSpecialTalked( int meindex, int talker, char *msg, int color )
{
    if( CHAR_getInt(talker,CHAR_WHICHTYPE) == CHAR_TYPEPLAYER
        && NPC_Util_isFaceToFace( meindex, talker, SHOPRANGE ) ){
        char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE], token[NPC_UTIL_GETARGSTR_LINEMAX];
        char *npcarg;
        npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof(npcarg1));
        switch( tolower(msg[0]) ){
        case 's':
        {
            int     ret;
            if( CHAR_getWorkInt(meindex,CHAR_WORKSHOPCLIENTINDEX)!= talker ){
                getStringFromIndexWithDelim( npcarg,"|",ANOTHERPLAYERTOKEN,
                                             token, sizeof(token));
                NPC_SimpleShopNormalWindow( meindex, talker, token );
                return;
            }
            ret = getStringFromIndexWithDelim( msg, "|", 3, token, sizeof(token));
            if( ret == FALSE ){
                token[0] = 'e';
                token[1] = '\0';
            }
            switch( tolower(token[0])){
            case 'b':
                if( (ret=NPC_SimpleShopProcessBuyMsg(msg,meindex,talker))
                    <0){
                    switch( ret ){
                    case -2:
                        getStringFromIndexWithDelim( npcarg,"|",POORTOKEN,
                                                     token,sizeof(token));
                        NPC_SimpleShopNormalWindow( meindex,talker,token);
                        break;
                    case -3:
                        getStringFromIndexWithDelim( npcarg,"|",
                                                     ITEMFULLTOKEN,
                                                     token,sizeof(token));
                        NPC_SimpleShopNormalWindow(meindex,talker,token);
                        break;
                    case -1:
                    default:
                        NPC_SimpleShopNormalWindow(meindex, talker,
                                                   SIMPLESHOP_FATALERRSTR
                            );
                        break;
                    }
                }else{
                    NPC_SimpleShopFillItem( meindex, npcarg );
                    getStringFromIndexWithDelim(npcarg,"|",THANKYOUTOKEN,
                                                token, sizeof(token));
                    NPC_SimpleShopNormalWindow(meindex,talker,token);
                }
                break;
            case 's':
                if( CHAR_getWorkInt(meindex,CHAR_WORKSHOPCLIENTINDEX) != talker ){
                    getStringFromIndexWithDelim( npcarg,"|",
                                                 ANOTHERPLAYERTOKEN,
                                                 token, sizeof(token));
                    NPC_SimpleShopNormalWindow( meindex, talker, token );
                    return;
                }
                if((ret=NPC_SimpleShopProcessSellMsg(msg,meindex,talker)) <0 ){
                    switch(ret){
                    case -2:
                        getStringFromIndexWithDelim( npcarg,"|",
                                                     RAREITEMTOKEN,
                                                     token, sizeof(token) );
                        NPC_SimpleShopNormalWindow( meindex,talker,token);
                        break;
                    case -1:
                    default:
                        NPC_SimpleShopNormalWindow(meindex, talker,
                                                   SIMPLESHOP_FATALERRSTR );
                        break;
                    }
                }else{
                    getStringFromIndexWithDelim(npcarg,"|",THANKYOUTOKEN,
                                                token, sizeof(token));
                    NPC_SimpleShopNormalWindow(meindex,talker,token);
                }
                break;
            case 'e':
            default:
                CHAR_setWorkInt( meindex, CHAR_WORKSHOPCLIENTINDEX, -1 );
                break;
            }
            break;
        }
        case 'i':
            if( CHAR_getWorkInt(meindex,CHAR_WORKSHOPCLIENTINDEX)
                == talker ){
                NPC_SimpleShopOpenShopWindow( meindex, talker, npcarg );
            }
            break;
        default:
            break;
        }
    }
}

void NPC_LimitBuyInShopSpecialTalked( int meindex, int talker, char *msg, int color )
{
    if( CHAR_getInt(talker,CHAR_WHICHTYPE) == CHAR_TYPEPLAYER
        && NPC_Util_isFaceToFace( meindex, talker, SHOPRANGE ) ){
        char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE], token[NPC_UTIL_GETARGSTR_LINEMAX];
        char *npcarg;
        npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof(npcarg1));
        switch( tolower(msg[0]) ){
        case 's':
        {
            int     ret;
            if( CHAR_getWorkInt(meindex,CHAR_WORKSHOPCLIENTINDEX) != talker ){
                getStringFromIndexWithDelim( npcarg,"|",
                                             ANOTHERPLAYERTOKEN,
                                             token, sizeof(token));
                NPC_SimpleShopNormalWindow( meindex, talker, token );
                return;
            }
            ret = getStringFromIndexWithDelim( msg, "|", 3, token, sizeof( token));
            if( ret == FALSE ){
                token[0] = 'e';
                token[1] = '\0';
            }
            switch( tolower(token[0])){
            case 'b':
                if( ( ret = NPC_SimpleShopProcessBuyMsg( msg, meindex, talker)) < 0){
                    switch( ret ){
                    case -2:
                        getStringFromIndexWithDelim( npcarg,"|",POORTOKEN,
                                                     token,sizeof(token));
                        NPC_SimpleShopNormalWindow( meindex,talker,token);
                        break;
                    case -3:
                        getStringFromIndexWithDelim( npcarg,"|",
                                                     ITEMFULLTOKEN,
                                                     token,sizeof(token));
                        NPC_SimpleShopNormalWindow(meindex,talker,token);
                        break;
                    case -1:
                    default:
                        NPC_SimpleShopNormalWindow(meindex, talker,
                                                   SIMPLESHOP_FATALERRSTR
                            );
                        break;
                    }
                }else{
                    NPC_SimpleShopFillItem( meindex, npcarg );
                    getStringFromIndexWithDelim(npcarg,"|",THANKYOUTOKEN,
                                                token, sizeof(token));
                    NPC_SimpleShopNormalWindow(meindex,talker,token);

                }
                break;
            case 's':
                if( CHAR_getWorkInt(meindex,CHAR_WORKSHOPCLIENTINDEX) != talker ){
                    getStringFromIndexWithDelim( npcarg,"|",
                                                 ANOTHERPLAYERTOKEN,
                                                 token, sizeof(token));
                    NPC_SimpleShopNormalWindow( meindex, talker, token );
                    return;
                }
                if((ret=NPC_LimitBuyInShopProcessSellMsg(
                    msg,meindex,talker))<0){
                    switch(ret){
                    case -2:
                        getStringFromIndexWithDelim( npcarg,"|",
                                                     RAREITEMTOKEN,
                                                     token,sizeof(token));
                        NPC_SimpleShopNormalWindow( meindex,talker,token);
                        break;
                    case -1:
                    default:
                        NPC_SimpleShopNormalWindow(meindex, talker, SIMPLESHOP_FATALERRSTR
                            );
                        break;
                    }
                }else{
                    getStringFromIndexWithDelim(npcarg,"|",THANKYOUTOKEN, token, sizeof(token));
                    NPC_SimpleShopNormalWindow(meindex,talker,token);

                }
                break;
            case 'e':
            default:
                CHAR_setWorkInt( meindex, CHAR_WORKSHOPCLIENTINDEX, -1 );
                break;
            }
            break;
        }
        case 'i':
            if( CHAR_getWorkInt(meindex,CHAR_WORKSHOPCLIENTINDEX)
                == talker ){
                NPC_SimpleShopOpenShopWindow( meindex, talker, npcarg );
            }
            break;
        default:
            break;
        }
    }
}





/*
 * Å¹¤¬¥Î¡¼¥Þ¥ë¥¦¥¤¥ó¥É¥¦¤ò½Ð¤¹¡e¥¦¥¤¥ó¥É¥¦¤Î¤Ü¤¿¤ó¤ò²¡¤¹¤È¡b
 *   ½é¤ËÅ¹¤Ë  ¤·¤«¤±¤¿¤È¤­¤È  ¤¸¾õÂÖ¤Ë¤â¤É¤ë¡e
 * int meindex : Å¹¤Îindex
 * int playerindex : ¥¦¥¤¥ó¥É¥¦¤ò½Ð¤¹¥×¥ì¥¤¥ä¡¼¤Îindex
 * char *str :
 */
static void NPC_SimpleShopNormalWindow( int meindex, int playerindex,
                                        char *str )
{
    int fd;
    int objindex;
    char tmp[512], *name;

    objindex = CHAR_getWorkInt(meindex,CHAR_WORKOBJINDEX);
    name = CHAR_getChar( meindex, CHAR_NAME );
    snprintf(tmp,sizeof(tmp),"W|%d|N|%s|123|%s", objindex, name, str );
    fd = getfdFromCharaIndex(playerindex);
    if( fd == -1 ) return;
    lssproto_TK_send(fd,-1,tmp,CHAR_COLORWHITE );
}

/*
 * Å¹¥¦¥¤¥ó¥É¥¦¤Î¥¤¥ó¥Ç¥¯¥¹¤«¤é¡b¥¢¥¤  ¥à  ¡¼¥Ö¥ë¤Î¥¤¥ó¥Ç¥¯¥¹¤Ë  ´¹
 * °ú¿ô
 * int charindex:¥­¥ã¥é¤Î¥¤¥ó¥Ç¥¯¥¹
 * int shopwindowindex:Å¹¥¦¥¤¥ó¥É¥¦¤Î¥¤¥ó¥Ç¥¯¥¹¡e0¤«¤é¤Ï¤¸¤Þ¤ë¡e
 * ÊÖ¤êÃÍ
 * ¥¢¥¤  ¥à  ¡¼¥Ö¥ë¤Î¥¤¥ó¥Ç¥¯¥¹¡e¥¨¥é¡¼¤Ï-1¡e
 */
static int NPC_ShopWindowIndexToItemTableIndex( int charindex,
                                                int shopwindowindex )
{
    int i, counter;

    counter = 0;
    for( i=CHAR_STARTITEMARRAY;i<CHAR_MAXITEMHAVE;i++ ){
        if( ITEM_CHECKINDEX( CHAR_getItemIndex(charindex,i) ) ){
            if( shopwindowindex == counter ) return i;
            counter++;
        }
    }
    return -1;
}

static BOOL NPC_SimpleShopProcessBuyMsg(char *msg, int shopindex,
                                        int playerindex )
{
    int shopwindowindex, itemtableindex,itemind;
    int price, tmpgold;
    char buf[64];

    if(!CHAR_CHECKINDEX(shopindex)||!CHAR_CHECKINDEX(playerindex))
        return -1;

    if( !getStringFromIndexWithDelim( msg,"|",4,buf,sizeof(buf) )){
        return -1;
    }
    shopwindowindex = atoi(buf);
    itemtableindex=NPC_ShopWindowIndexToItemTableIndex( shopindex, shopwindowindex );
    itemind = CHAR_getItemIndex(shopindex,itemtableindex );
    if( ! ITEM_CHECKINDEX(itemind) ) return -1;
    price = ITEM_getInt( itemind, ITEM_COST )
        * NPC_Util_buyRate(playerindex);

    tmpgold = CHAR_getInt(playerindex,CHAR_GOLD);
    if( tmpgold < price ){
        return -2;
    }

    if( ! NPC_Util_moveItemToChar(playerindex,itemind,TRUE ) ){
        return -3;
    }
	CHAR_AddGold( playerindex, price );

    CHAR_sendStatusString( playerindex , "P");
    return 1;
}

static BOOL NPC_SimpleShopProcessSellMsg(char *msg, int shopindex,
                                         int playerindex )
{
    int sellwindowindex, itemtableindex,itemind, id;
    int price, tmpgold;
    char buf[64];
    if(!CHAR_CHECKINDEX(shopindex)||!CHAR_CHECKINDEX(playerindex))
        return -1;
    if( !getStringFromIndexWithDelim( msg,"|",4,buf,sizeof(buf) )){
        return -1;
    }
    sellwindowindex = atoi(buf);
    itemtableindex = sellwindowindex + CHAR_STARTITEMARRAY;
    itemind = CHAR_getItemIndex(playerindex,itemtableindex );
    if( ! ITEM_CHECKINDEX(itemind) ) return -1;

    price = ITEM_getInt( itemind, ITEM_COST );

    if( price <= 0 || price > NPC_SIMPLESHOPMAXBUYPRICE ){
        return -2;
    }
    price *= NPC_Util_sellRate(playerindex);

	CHAR_AddGold( playerindex, price );

	CHAR_sendStatusString( playerindex , "P");

    id = ITEM_getInt(itemind,ITEM_ID);
	{
		LogItem(
			CHAR_getChar( playerindex, CHAR_NAME ), /* ¥­¥ã¥é   */
			CHAR_getChar( playerindex, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD ¦bitemªºlog¤¤¼W¥[item¦WºÙ
			itemind,
#else
       		ITEM_getInt( itemind, ITEM_ID ),  /* ¥¢¥¤  ¥à  ¹æ */
#endif
			"Sell",
			CHAR_getInt( playerindex,CHAR_FLOOR),
			CHAR_getInt( playerindex,CHAR_X ),
 	      	CHAR_getInt( playerindex,CHAR_Y ),
	        ITEM_getChar( itemind, ITEM_UNIQUECODE),
			ITEM_getChar( itemind, ITEM_NAME),
			ITEM_getInt( itemind, ITEM_ID)
		);
	}

    if( ! NPC_Util_moveItemToChar( shopindex,itemind,TRUE ) ){
        return -1;
    }
    NPC_Util_RemoveItemByID(shopindex,id,FALSE);
    return 1;
}

static BOOL NPC_LimitBuyInShopProcessSellMsg(char *msg,
                                 int shopindex, int playerindex )
{
    int sellwindowindex, itemtableindex,itemind, id,i,buyokflag, setid;
    int price, tmpgold;
    char buf[BUFSIZ];
    char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE], token[NPC_UTIL_GETARGSTR_LINEMAX];
    char *npcarg;

    if(!CHAR_CHECKINDEX(shopindex)||!CHAR_CHECKINDEX(playerindex))
        return -1;
    if( !getStringFromIndexWithDelim( msg,"|",4,buf,sizeof(buf) )){
        return -1;
    }
    sellwindowindex = atoi(buf);
    itemtableindex = sellwindowindex + CHAR_STARTITEMARRAY;
    itemind = CHAR_getItemIndex(playerindex,itemtableindex );
    if( ! ITEM_CHECKINDEX(itemind) ) return -1;
    id = ITEM_getInt(itemind,ITEM_ID);
    npcarg = NPC_Util_GetArgStr( shopindex, npcarg1, sizeof(npcarg1));
    getStringFromIndexWithDelim( npcarg, "|", BUYINITEMLISTTOKEN, token, sizeof( token) );
    buyokflag = FALSE;
    for( i=1;getStringFromIndexWithDelim(token,",",i,buf,sizeof(buf));  i++){

	  char *tok1 = NULL;
	  char *tok2 = NULL;
	  int no1=0, no2=0;
	  tok1 = strtok( buf, "-" );
	  tok2 = strtok( NULL, "-" );

	  setid = atoi(buf);

	  if( tok1 == NULL || tok2 == NULL ){
		if( setid == 0 ) return -2;
		if( setid == id ) buyokflag = TRUE;
	  } else {
		no1 = atoi( tok1 );
		no2 = atoi( tok2 );
		if( no1 <= id && id <= no2 ){
		  buyokflag = TRUE;
		}
	  }
    }
    if( buyokflag == FALSE ) return -2;
    price = ITEM_getInt( itemind, ITEM_COST );
    price *= NPC_Util_sellRate(playerindex);

	CHAR_AddGold( playerindex, price );

    CHAR_sendStatusString( playerindex , "P");
    if( ! NPC_Util_moveItemToChar( shopindex,itemind,TRUE ) ){
        return -1;
    }

	{
		LogItem(
			CHAR_getChar( playerindex, CHAR_NAME ), /* ¥­¥ã¥é   */
			CHAR_getChar( playerindex, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD ¦bitemªºlog¤¤¼W¥[item¦WºÙ
			itemind,
#else
       		ITEM_getInt( itemind, ITEM_ID ),  /* ¥¢¥¤  ¥à  ¹æ */
#endif
			"Sell",
			CHAR_getInt( playerindex,CHAR_FLOOR),
			CHAR_getInt( playerindex,CHAR_X ),
 	      	CHAR_getInt( playerindex,CHAR_Y ),
	        ITEM_getChar( itemind, ITEM_UNIQUECODE),
			ITEM_getChar( itemind, ITEM_NAME),
			ITEM_getInt( itemind, ITEM_ID)
		);
	}

    /*   ¤¤¼è¤Ã¤¿¥¢¥¤  ¥à¤ò¾Ãµî */
    NPC_Util_RemoveItemByID(shopindex,id,FALSE);

    return 1;

}

/*
 * Å¹¤Ë¤¢¤ë¥¢¥¤  ¥à¤Î¿ô¤ò  ¤ë
 */
static int NPC_SimpleShopGetItemNum( int meindex )
{
    int counter, i;

    counter = 0;
    for( i=CHAR_STARTITEMARRAY;i<CHAR_MAXITEMHAVE;i++ ){
        /* ¥¢¥¤  ¥à  ¤òÁ´  ¥µ¡¼¥Á¤·¤Æ¤â¤Ã¤Æ¤ë¤ä¤Ä¤ò¥«¥¦¥ó¥È¤¹¤ë */
        if( ITEM_CHECKINDEX( CHAR_getItemIndex(meindex,i) ) ){
            counter++;
        }
    }
    return counter;

}

/*
 * ¤¢¤ë¥­¥ã¥é¥¤¥ó¥Ç¥¯¥¹¤Î¥­¥ã¥é¤Ë¡b¶³¤ß¤»window¤ò³«¤¯¤¿¤á¤Î
 * ¾ð  ¤òÍ¿¤¨¤ë¡e
 * int meindex:¼«Ê¬¤Î(Å¹¤Î)¥¤¥ó¥Ç¥¯¥¹
 * int cliindex:µÒ¤Î¥¤¥ó¥Ç¥¯¥¹
 */
static void NPC_SimpleShopOpenShopWindow( int meindex, int cliindex,
                                          char *npcarg )
{
    int fd;
    int objindex;

    char tmp[1024*12], itemstr[1024 * 8];

    char maincaption[SIMPLESHOPTALKBUFSIZE];
    char buycaption[SIMPLESHOPTALKBUFSIZE];
    char okbuycaption[SIMPLESHOPTALKBUFSIZE];
    char sellcaption[SIMPLESHOPTALKBUFSIZE];
    char oksellcaption[SIMPLESHOPTALKBUFSIZE];
    char poorcaption[SIMPLESHOPTALKBUFSIZE];
    char itemfullcaption[SIMPLESHOPTALKBUFSIZE];

    getStringFromIndexWithDelim( npcarg,"|",MAINTOKEN,
                                 maincaption,sizeof(maincaption) );
    getStringFromIndexWithDelim( npcarg,"|",BUYTOKEN,
                                 buycaption,sizeof(buycaption) );
    getStringFromIndexWithDelim( npcarg,"|",OKBUYTOKEN,
                                 okbuycaption,sizeof(okbuycaption) );
    getStringFromIndexWithDelim( npcarg,"|",SELLTOKEN,
                                 sellcaption,sizeof(sellcaption) );
    getStringFromIndexWithDelim( npcarg,"|",OKSELLTOKEN,
                                 oksellcaption,sizeof(oksellcaption) );
    getStringFromIndexWithDelim( npcarg,"|",POORTOKEN,
                                 poorcaption,sizeof(poorcaption));
    getStringFromIndexWithDelim( npcarg,"|",ITEMFULLTOKEN,
                                 itemfullcaption,sizeof(itemfullcaption));

    tmp[0] = 0;
    NPC_SimpleShopGetEscapedItemString( meindex, cliindex, itemstr );
    objindex = CHAR_getWorkInt( meindex,CHAR_WORKOBJINDEX );
    snprintf( tmp,sizeof(tmp),"S|%d|%s|%s|%s|"
              "%s|%s|%s|%s|"
              "-1|%d%s", objindex,
              maincaption,
              buycaption,
              okbuycaption,
              sellcaption,
              oksellcaption,
              poorcaption,
              itemfullcaption,
              NPC_SimpleShopGetItemNum( meindex ),
              itemstr );
    fd = getfdFromCharaIndex( cliindex );

    if( fd == -1 )return;

    lssproto_TK_send(fd, -1 ,tmp, CHAR_COLORWHITE );


}


/*
 * TK¤ÇÁ÷¤ë¤¿¤á¤Î  »ú  ¤ò¤Ä¤¯¤ë¡e
 */
static void NPC_SimpleShopGetEscapedItemString( int shopindex,
                                                int cliindex, char *str )
{
    int i, cost, cl, imageno, itemindex;
    char *itemname;
    char tmp[1024];
    char escapedname[256];
//    char *oli; /* oli means onelineinfo */

    str[0] = '\0';

    /* ¤Þ¤ºÅ¹¤Î  ¤Á  ¤Î¥ê¥¹¥È */
    for( i=CHAR_STARTITEMARRAY;i<CHAR_MAXITEMHAVE;i++ ){
        itemindex=CHAR_getItemIndex( shopindex , i );
        if( ITEM_CHECKINDEX(itemindex) ){
            itemname = ITEM_getChar( itemindex, ITEM_SECRETNAME );
            makeEscapeString( itemname, escapedname, sizeof(escapedname));
            cost = ITEM_getInt( itemindex, ITEM_COST );
            /* ¶â³Û¤ÎÄ´À°¡eMERCHANTLEVEL¤Ë¤è¤Ã¤Æ  ¤ï¤ë¡e */
            cost *= NPC_Util_buyRate(cliindex);
            cl = ITEM_getInt( itemindex, ITEM_LEVEL );
            imageno = ITEM_getInt( itemindex, ITEM_BASEIMAGENUMBER );
            //oli =  ITEM_getMemo( itemindex );
#if 0
            snprintf( tmp, sizeof(tmp), "|%s|%d|%d|%d|%s",
                      escapedname, cost, cl, imageno, oli );
#endif
            snprintf( tmp, sizeof(tmp), "|%s|%d|%d|%d|",
                      escapedname, cost, cl, imageno );
            strcat( str, tmp );
        }
    }

    /* ±­¤Ë¡bµÒ¤Î  ¤Á  ¤Î¥ê¥¹¥È */
    for( i=CHAR_STARTITEMARRAY;i<CHAR_MAXITEMHAVE;i++ ){
        itemindex=CHAR_getItemIndex( cliindex , i );
        if( ITEM_CHECKINDEX(itemindex) ){
            cost = ITEM_getInt( itemindex, ITEM_COST );
            /* ¶â³Û¤ÎÄ´À°¡eMERCHANTLEVEL¤Ë¤è¤Ã¤Æ  ¤ï¤ë¡e */
            cost *=NPC_Util_sellRate(cliindex);
            snprintf( tmp, sizeof(tmp), "|%d", cost );
            strcat( str, tmp );
        }else{
            /* ¥¢¥¤  ¥à¤ò  ¤Ã¤Æ¤Ê¤¤¾ì½ê¤Ë¤Ï0¤ò¤¤¤ì¤ë     */
            strcat( str, "|0" );
        }
    }

}

/*
 * Å¹¤¬  ¸Â¤Ë  ¤Ã¤Æ¤¤¤ë¥¢¥¤  ¥à¤ò  ¤¿¤»¤ë¡e½é´ü²½»þ¤È¼è¤ê°ú¤­¤Î  
 * ¤Ë¸Æ¤Ö¡e
 */
static BOOL NPC_SimpleShopFillItem( int meindex, char *npcarg )
{
    char token[256], buf[16];
    int i,itemid, num;

    getStringFromIndexWithDelim( npcarg,"|",ITEMLISTTOKEN,token,
                                 sizeof(token) );

    for( i=1; i<=SIMPLESHOP_MAXINFINITITEM;i++ ){
        if( getStringFromIndexWithDelim( token,",",i,buf,sizeof(buf) )){
            itemid = atoi( buf );
            if( itemid == 0 ){
                NPC_Util_ReleaseHaveItemAll(meindex);
                return FALSE;
            }
            num=NPC_Util_countHaveItem(meindex,itemid);
            if( num < 0 ){
                return FALSE;/* ÉÔÀµ¤Êid¤Ê¤É¤Îfatal err */
            }else if( num == 0 ){ /* ¤â¤Ã¤Æ¤Ê¤«¤Ã¤¿¤é  ½¼ */
                NPC_Util_createItemToChar( meindex,itemid, FALSE);

            }else if( num>=2 ){ /* ¤â¤Á¤¹¤®¤Î¾ì¹çºï½ü */
                NPC_Util_RemoveItemByID(meindex,itemid,FALSE);
            }
        }else{
            break;
        }
    }

    /* °ì²ó¤·¤­¤Ù¤Ä¤µ¤ì¤¿¾õÂÖ¤Ë¤¹¤ë */
    //NPC_SimpleShopSetLeakLevel( meindex );

    return TRUE;
}


/*
 * ½é´ü²½¤¹¤ë¡e
 */
BOOL NPC_SimpleShopInit( int meindex )
{
    unsigned int mlevel;
    char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE];
    char *npcarg;

    /* °ì²ó¼º  ¤·¤¿¤¢¤È¤Ï2ÅÙ¤Èºî¤é¤Ê¤¤¤è¤¦¤Ë¤¹¤ë */
    int createindex = CHAR_getInt( meindex, CHAR_NPCCREATEINDEX );

/*    print("shopinit start\n" );*/
    if( NPC_CHECKCREATEINDEX(createindex) ){
        NPC_create[createindex].intdata[NPC_CREATETIME] = -1;
    }

    CHAR_setInt( meindex , CHAR_HP , 0 );
    CHAR_setInt( meindex , CHAR_MP , 0 );
    CHAR_setInt( meindex , CHAR_MAXMP , 0 );
    CHAR_setInt( meindex , CHAR_STR , 0 );
    CHAR_setInt( meindex , CHAR_TOUGH, 0 );
    CHAR_setInt( meindex , CHAR_LV , 0 );
    mlevel = ((100<<16)|(100<<0)); /* ¾å°Ì2¥Ð¥¤¥È¤¬  ¤¦¤È¤­¤Î    ¡b
                                    ²¼°Ì2¥Ð¥¤¥È¤¬  ¤ë¤È¤­¤Î    ¡e */
    CHAR_setInt( meindex, CHAR_MERCHANTLEVEL, mlevel );

    CHAR_setWorkInt( meindex, CHAR_WORKSHOPCLIENTINDEX, -1 );

    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPESHOP );
    CHAR_setFlg( meindex , CHAR_ISOVERED , 0 );
    CHAR_setFlg( meindex , CHAR_ISATTACKED , 0 );

    npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof(npcarg1));

    /* ¥¢¥¤  ¥à  ¤¿¤»¤ë¡e¼º  ¤·¤¿¤éreturn FALSE */
    if( ! NPC_SimpleShopFillItem( meindex,npcarg ) ){
        print( "SHOP INIT ERROR: npcarg=%s\n", npcarg );
        return FALSE;
    }

/*    print("shopinit end\n" );*/


    return TRUE;
}

/*
 * Å¹¥¦¥¤¥ó¥É¥¦¤ò³«¤¯¤«¤É¤¦¤«  Äê¤¹¤ë¡e
 * npcarg¤Î¤¤¤Ã¤³¤á¤Î¥È¡¼¥¯¥ó¤Ë
 * ¤³¤ó¤Ë¤Á¤Ï,¤´¤á¤ó¤¯¤À¤µ¤¤,¤¯¤À¤µ¤¤¤Ê
 * ¤Î¤è¤¦¤ËÍå  ¤µ¤ì¤¿  »ú  ¤ò¤â¤È¤Ë  Äê¤¹¤ë¡e¤³¤Î¤È¤­¤Î¥Ç¥ê¥ß¥¿¤Ï","¡e
 *
 * °ú¿ô
 * msg:¥×¥ì¥¤¥ä¡¼¤¬¤·¤ã¤Ù¤Ã¤¿  »ú  
 * openshopstr:npcarg¤Î¤¤¤Ã¤³¤á¤Î¥È¡¼¥¯¥ó¡e
 * ÊÖ¤êÃÍ
 * ³«¤¯¤Ê¤éTRUE, ³«¤«¤Ê¤¤¤Ê¤éFALSE
 */
static BOOL NPC_SimpleShopOpenShopOrNot( char *msg, char *openshopstr )
{
    int i;
    char buf[256];

    i=1;
    while(getStringFromIndexWithDelim(openshopstr,",",i,buf,sizeof(buf))){
        if( strstr( msg, buf ) ) return TRUE;
        i++;
    }
    return FALSE;
}

/*
 * ¤¹¤Ù¤Æ¤Î¤â¤Á¤â¤Î¤ò°ì²ó¼±  ¤µ¤ì¤¿¾õÂÖ¤Ë¤¹¤ë¡e
 */
#if 0
static void NPC_SimpleShopSetLeakLevel( int meindex )
{
    int i, itemindex;

    for( i=CHAR_STARTITEMARRAY;i<CHAR_MAXITEMHAVE;i++ ){
        itemindex=CHAR_getItemIndex(meindex,i);
        if( ITEM_CHECKINDEX(itemindex)){
            ITEM_setInt(itemindex,ITEM_LEAKLEVEL, 1 );
        }
    }
}
#endif

