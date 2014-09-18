//krynn 2001/12/6
//PKPetShop

#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "pet_skill.h"
#include "readmap.h"
#include "log.h"
#include "enemy.h"
#include "npc_pkpetshop.h"
#include "battle.h"

#ifdef _PKPETSHOP
#define MAXSHOPPET 33

static void NPC_PKPetShop_selectWindow( int meindex, int talker, int num,int select);
void NPC_PKPetShop_BuyMain(int meindex,int talker,int before );
void NPC_PKPetShop_GetPetList(char *argstr,char * argtoken2);
BOOL NPC_PKPetShop_SetNewPet(int meindex,int talker,char *data);
BOOL NPC_PKPetShop_SellNewPet(int meindex,int talker,char *data);

void NPC_PKPetShop_Menu(int meindex,int talker);
int NPC_PKPetShop_GetLimtPetList(int talker,char *argstr,char *token2,int sell);
void NPC_PKPetShop_SellMain(int meindex,int talker,int select);
int NPC_GetSellPetList(int itemindex,int flg,char *argstr,char *argtoken,int select,int sell);
BOOL NPC_AddPetBuy(int meindex, int talker,int petID,int kosuu,double rate);
void NPC_PetStrStr(int petID,double rate,char *name,char *token2, int index);
int NPC_SellPetstrsStr(int itemindex,int flg,double rate,char *argtoken,int select,int sell);
void NPC_LimitPetShop(int meindex,int talker,int select);
void NPC_PKPetShop_ExpressmanCheck(int meindex,int talker);
 

/*--¥ï¡¼¥¯    ÄêµÁ--*/
enum{
	NPC_PKPETSHOP_WORK_NO 		= CHAR_NPCWORKINT1,
	NPC_PKPETSHOP_WORK_EV 		= CHAR_NPCWORKINT2,
	NPC_PKPETSHOP_WORK_EXPRESS	= CHAR_NPCWORKINT3,
};

		  
typedef struct {
	char	arg[32];
	int		type;
}PKPETSHOP_NPC_Shop;


static PKPETSHOP_NPC_Shop		TypeTable[] = {
	{ "FIST",		0 },
	{ "AXE",		1 },
	{ "CLUB",		2 },
	{ "SPEAR",		3},
	{ "BOW",		4},
	{ "SHIELD",		5},
	{ "HELM",		6 },
	{ "ARMOUR",		7 },
	{ "BRACELET",	8},
	{ "ANCLET",		9 },
	{ "NECKLACE",	10},
	{ "RING",		11},
	{ "BELT",		12},
	{ "EARRING",	13},
	{ "NOSERING",	14},
	{ "AMULET",		15},
	{ "OTHER",		16},
	{ "BOOMERANG",	17},
	{ "BOUNDTHROW",	18},
	{ "BREAKTHROW",	19},
#ifdef _ITEM_TYPETABLE
	{ "DISH",	20},
	{ "METAL",	21},
	{ "JEWEL",	22},
	{ "WARES",	23},
	{ "WBELT",	24},
	{ "WSHIELD", 25},
	{ "WSHOES",	26},
	{ "WGLOVE",	27},
	{ "ANGELTOKEN",	28},
	{ "HEROTOKEN",	29},
#endif	
	{ "ACCESSORY",	30},
	{ "OFFENCE",	40},
	{ "DEFENCE",	50},

};


/*********************************
* ªì©l
*********************************/
BOOL NPC_PKPetShopInit( int meindex )
{

	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;

	/*--¥¿¥¤¥×ÀßÄê--*/
	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEPKPetShop );
	
	if((argstr = NPC_Util_GetArgStr(meindex, argstr1, sizeof(argstr1))) == NULL ) 
	{
		print("NPC_PKPetShopInit_GetArgStrErr");
		return FALSE;
	}
	if(strstr(argstr,"LIMITSHOP") != NULL) 
	{		/*-  ¤¤¼è¤ê·´ÍÑ¥Õ¥é¥°--*/
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_NO, 1);
	}
	else
	{
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_NO, 0);
	}
	
	if(strstr( argstr, "EVENT") != NULL) 
	{		/*-  ¤¤¼è¤ê·´ÍÑ¥Õ¥é¥°--*/
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_EV, 1);
	}
	else
	{
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_EV, 0);
	}

	if(strstr( argstr, "EXPRESS") != NULL) 
	{		/*-±¿Á÷²°¥Õ¥é¥°--*/
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_EXPRESS, 1);
	}
	else
	{
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_EXPRESS, 0);
	}
	return TRUE;
}


/*********************************
*   ¤·¤«¤±¤é¤ì¤¿»þ¤Î½è  
*********************************/
void NPC_PKPetShopTalked( int meindex , int talker , char *szMes ,int color )
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buff[1024];
	char	buf2[256];
	char 	token[1024];
	int 	i = 1;
	BOOL	sellonlyflg = FALSE;
	char	sellmsg[1024];

    /* ¥×¥ì¥¤¥ä¡¼¤ËÂÐ¤·¤Æ¤À¤±  ±þ¤¹¤ë */
    if( CHAR_getInt( talker , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) 
	{
    	return;
    }

	/*--  ¤ÎÁ°¤Ë¤¤¤ë¤«¤É¤¦¤«¡ª--*/
	if(NPC_Util_isFaceToFace( meindex, talker, 2) == FALSE) 
	{		/* £±¥°¥ê¥Ã¥É°Ê  ¤Î¤ß */
		if( NPC_Util_CharDistance( talker, meindex ) > 1) return;
	}

    if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1))) == NULL)
	{
    	print("NPC_PKPetShopInit_GetArgStrErr");
    	return;
    }

	if( NPC_Util_GetStrFromStrWithDelim( argstr, "sellonly_msg", 
										 token, sizeof( token))
		!= NULL)
	{
		sellonlyflg = TRUE;
		strcpysafe(sellmsg, sizeof( sellmsg), token);
	}

	/*--  ÀÜ  ¤¦¥¦¥¤¥ó¥É¥¦¤Ë¤¤¤±¤ë¤«¤É¤¦¤«--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "buy_msg", 
		buff, sizeof( buff)) != NULL )
	{
	    while(getStringFromIndexWithDelim(buff,",",i,buf2,sizeof(buf2)) != FALSE)
		{
			i++;
			if( strstr( szMes, buf2) != NULL) {
				if( CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EV) == 0) {
					/*-  ¤¤¼è¤ê·´  ¤«¤É¤¦¤«¤Î¥Á¥§¥Ã¥¯--*/
					if( CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) == 1) {
						if( sellonlyflg ) {
							NPC_PKPetShop_selectWindow( meindex, talker, 3, -1);
							return;
						}
					}
					else{
						NPC_PKPetShop_selectWindow( meindex, talker, 1, -1);
						return;
					}
				}else{
					if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) == 1) {
						if( sellonlyflg) {
							NPC_PKPetShop_selectWindow( meindex, talker, 3, -1);
							return;
						}
					}else{
						NPC_PKPetShop_selectWindow( meindex, talker, 1, -1);
						return;
					}
		 			return;
		 	 	}
			}
		}
	}
	i=1;

	/*--  ÀÜ  ¤ë¥¦¥¤¥ó¥É¥¦¤Ë¤¤¤±¤ë¤«¤É¤¦¤«--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "sell_msg", 
		buff, sizeof( buff)) != NULL )
	{
	    while( getStringFromIndexWithDelim(buff,",", i,buf2,sizeof(buf2))
	     != FALSE )
		{
			i++;
			if(strstr(szMes,buf2) != NULL) {
				NPC_PKPetShop_selectWindow( meindex, talker, 2, -1);
				return;
			}
		}
	}
	i = 1;


	/*--¤½¤ÎÂ¾¤Î¥Ò¥ó¥È¤ò¤¯¤ì¤ë¸ÀÍÕ¤Ç  ¤·¤«¤±¤ë--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "other_msg", 
		buff, sizeof( buff)) != NULL )
	{
	    while(getStringFromIndexWithDelim( buff, ",", i, buf2, sizeof( buf2))
	     !=FALSE)
		{
			i++;
			if(strstr(szMes,buf2) != NULL) {
				/*--¥Ò¥ó¥È¥á¥Ã¥»¡¼¥¸--*/
				if(NPC_Util_GetStrFromStrWithDelim( argstr, "hint_msg", 
				token, sizeof( token)) != NULL)
				{
					CHAR_talkToCli( talker, meindex, token, CHAR_COLORWHITE);
					return;
				}
			}
		}	
	}

	/*-  ¤¤¼è¤ê·´  ¤«¤É¤¦¤«¤Î¥Á¥§¥Ã¥¯--*/
	if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) == 1)
	{
		if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EV) == 1) 
		{
			if( sellonlyflg) 
			{
				CHAR_talkToCli( talker, meindex, sellmsg, CHAR_COLORWHITE);
				return;
			}
		}
		else
		{	/*--  ¤¤¼è¤ê·´  ¤Î¥á¥Ã¥»¡¼¥¸--*/
			if( sellonlyflg) 
			{
				NPC_PKPetShop_selectWindow( meindex, talker, 3, -1);
				return;
			}
		}
	}
	else
	{
		if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EV) == 1) {
			if( sellonlyflg) {
				CHAR_talkToCli( talker, meindex, sellmsg, CHAR_COLORWHITE);
				return;
			}
		}else{
			
			if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EXPRESS) == 1) {
				NPC_PKPetShop_ExpressmanCheck( meindex, talker);
			}else{ 
				/*--¤³¤³¤Þ¤Ç  ¤¿¤é¶¦ÄÌ¥¦¥¤¥ó¥É¥¦(¥á¥Ë¥å¡¼)  ¼¨--*/
				/*--  ¤¤¼è¤ê·´  ¤Ç¤Ê¤¯¤Æ  ÄÌ¤ÎÅ¹¤Ê¤é¥á¥Ë¥å¡¼  ¼¨--*/
				NPC_PKPetShop_selectWindow( meindex, talker, 0, -1);
			}
		}
	}
				
}

static void NPC_PKPetShop_selectWindow( int meindex, int talker, int num,int select)
{

	print("\n NPC_PKPetShop_selectWindow()");
	print("\n num = %d ", num);
	switch( num) {
	  case 0:
		/*--¥á¥Ë¥å¡¼    --*/
		/*--¥Ñ¥é¥á¡¼¥¿Á÷¤ê--*/
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
		
		if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EXPRESS) == 1 ) 
		{
			if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) ==0 ) 
			{
				NPC_PKPetShop_ExpressmanCheck( meindex, talker);
			}
		}
		else if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) == 1) 
		{
		}
		else
		{
		  	NPC_PKPetShop_Menu( meindex, talker);
		}
	  	break;

	  case 1:
	  	/*--  ¤¦    --*/
	  	NPC_PKPetShop_BuyMain( meindex, talker, select);
	  	break;

	  case 2:
	  	/*--  ¤ë    --*/
	  	NPC_PKPetShop_SellMain( meindex, talker, select);
	  	break;

	  case 3:
	  	/*--  ¤¤¼è¤ê·´  ¤Ç¤¹¤è    --*/
	  	NPC_LimitPetShop( meindex, talker, select);
	  	break;

	}
}



/*-----------------------------------------
 * ¥¯¥é¥¤¥¢¥ó¥È¤«¤éÊÖ¤Ã¤Æ¤­¤¿»þ¤Ë¸Æ¤Ó½Ð¤µ¤ì¤ë¡e
 *
-------------------------------------------*/
void NPC_PKPetShopWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	/*--       ¤Î¤Þ¤ï¤ê¤Ë¤¤¤Ê¤¤¤È¤­¤Ï½ª   --*/
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 3) {
		/*--¥Ñ¥é¥á¡¼¥¿Á÷¤ê--*/
		CHAR_send_P_StatusString( talkerindex, CHAR_P_STRING_GOLD);
		return;
	}

	makeStringFromEscaped( data);
	switch( seqno){

	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG:
		/*--³Æ¥á¥Ë¥å¡¼¤Ë  ¤Ð¤¹--*/
		/*--  ¤¦--*/
		if(atoi( data) == 1 )	NPC_PKPetShop_selectWindow(meindex, talkerindex, 1, -1);

		/*--  ¤ë--*/
		if(atoi( data) == 2)	NPC_PKPetShop_selectWindow(meindex, talkerindex, 2, -1);

		/*--½Ð¤ë--*/
		if(atoi( data) == 3)	return;/*--²¿¤â¤·¤Ê¤¤--*/
	
		break;


	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG:
		/*--¥¢¥¤  ¥à¤ÎÄÉ²Ã--*/
		if(NPC_PKPetShop_SetNewPet(meindex , talkerindex, data) == TRUE) {

			NPC_PKPetShop_selectWindow( meindex, talkerindex, 1, 0);

		}else{
			NPC_PKPetShop_selectWindow( meindex, talkerindex ,0, -1);
		}

		break;


	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG:
		/*--¥¢¥¤  ¥à¤Îºï½ü--*/
		if(NPC_PKPetShop_SellNewPet(meindex , talkerindex, data) == TRUE) {
			NPC_PKPetShop_selectWindow( meindex, talkerindex, 2, 0);

		}else{
			NPC_PKPetShop_selectWindow( meindex,  talkerindex, 0, -1);
		}

		break;
			
	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT:
		 if(select == WINDOW_BUTTONTYPE_YES) {
			NPC_PKPetShop_selectWindow( meindex, talkerindex ,2, -1);

		}else  if(select == WINDOW_BUTTONTYPE_NO) {
			return;
		}else if(select == WINDOW_BUTTONTYPE_OK) {
				NPC_PKPetShop_selectWindow( meindex, talkerindex, 2, -1);
		}
		break;
	
	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_EXPRESS:
		if(atoi(data) == 2) {
			NPC_PKPetShop_selectWindow( meindex, talkerindex, 1, -1);
		}else if(atoi( data) == 4) {
			NPC_PKPetShop_selectWindow( meindex, talkerindex, 2, -1);
		}
	}
}


/*-----------------------------------------
 *
 *  ¤¦½è  ¥á¥¤¥ó(¥¯¥é¥¤¥¢¥ó¥ÈÁ÷¿®¾ð  ºî©¨  
 *
 *krynn 2001/12/9 ¥[ªºµùÄÀ
 *before == -1
 *before <> -1
 *krynn end
 *-----------------------------------------*/
void NPC_PKPetShop_BuyMain(int meindex,int talker,int before )
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char token[NPC_UTIL_GETARGSTR_BUFSIZE];
	int fd = getfdFromCharaIndex( talker);

    if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
       	print("itemshop_GetArgStr_Err");
       	return;
    }

	/*--Á°²ó¤Î¥Ç¡¼¥¿»È¤¦¤«---*/
	if(before != -1) 
	{	/*--Á°²ó¤Î¥Ç¡¼¥¿¤¬»Ä¤Ã¤Æ¤¤¤ë¤Î¤ÇÂ¾¤Î¾ð  ¤ÏÁ÷¤é¤Ê¤¯    --*/
		sprintf(token,"0|0");

		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);
	}else{

		char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
		char buff2[256];
    	char buff[256];

		/*--³Æ¥á¥Ã¥»¡¼¥¸¤ò¼è  ¤·¤Æ¥¯¥é¥¤¥¢¥ó¥È¤ËÁ÷¤ë¥Ç¡¼¥¿¤òºî¤ë-*/
		/*--¤³¤Î¤ä¤ê  ¤Ï¤­¤¿¤Ê¤¤¤«¤â--*/
		NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "buy_main", buff2, sizeof( buff2));
		sprintf(token,"0|1|%d|%s|%s|%s|", CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getChar( meindex, CHAR_NAME), buff, buff2);

		NPC_Util_GetStrFromStrWithDelim( argstr, "what_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "level_msg", buff2, sizeof( buff));
		snprintf( token2, sizeof( token2), "%s|%s", buff, buff2);

		/*--  »ú  ¹ç  --*/
		strncat( token, token2, sizeof( token));
			
		NPC_Util_GetStrFromStrWithDelim( argstr, "realy_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "itemfull_msg", buff2, sizeof( buff2));
		sprintf( token2, "|%s|%s", buff, buff2);

		/*--  »ú    ·ë--*/
		strncat(token , token2,sizeof(token));
		strcpy(token2, "|");
			
		/*--¥¢¥¤  ¥à¾ð  ¤Î      --*/
		NPC_PKPetShop_GetPetList( argstr, token2 );
		// krynn 2001/12/12 bebug ¥Î
		print("%s",token2);
		// end krynn

		/*--¥á¥Ã¥»¡¼¥¸¤È¥¢¥¤  ¥à¾ð  ¤Î¹ç  --*/
		strncat( token, token2, sizeof( token));
	}

		/*--  »ú  ¤ò¥¨¥¹¥±¡¼¥×¤µ¤»¤ë--*/
//		makeEscapeString( token, escapedname, sizeof(escapedname));

	/*--¤³¤³¤ÇÁ÷¿®--*/
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

}

/*------------------------------------------------------
 *¥¢¥¤  ¥à´Ø·¸¤Î  »ú  ¤òºî¤ë(  ¤¦  
 *------------------------------------------------------*/
void NPC_PKPetShop_GetPetList(char *argstr,char *argtoken)
{

	int i = 1;
	int tmp;
	int EnemyCnt;
	char *name ;
	char buff2[256];
	char buff[NPC_UTIL_GETARGSTR_LINEMAX];
	char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
	double rate = 1.0;
	int loopcnt = 0;

	/*-¥ì¡¼¥È¤ò  ¤ë¡e  ¤±¤ì¤Ð£±¤Ç¸ÇÄê--*/
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "buy_rate", buff2, sizeof( buff2))
	 != NULL){
		rate = atof( buff2);
	}
		
	/*-  °·¤¦¾¦  ¤ò¼è    --*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "PetList", buff, sizeof( buff))
	 != NULL )
	{
	    while( getStringFromIndexWithDelim(buff,",",i,buff2,sizeof(buff2))
	     !=FALSE )
	    {
			i++;
	    	/*--ÀßÄê¥Õ¥¡¥¤¥ë¤Î¥¢¥¤  ¥à¤¬ "-"¤Ç¶èÀÚ¤é¤ì¤Æ¤¤¤ë¤«¤Î¥Á¥§¥Ã¥¯--*/
			if(strstr( buff2, "-") == NULL) {
				/*--¤Þ¤º  Á°      --*/
				// krynn 2001/12/10
				EnemyCnt = ENEMY_getEnemyNum();
				for( tmp=0 ; tmp < EnemyCnt ; tmp++ )
				{
					if( ENEMY_getInt( tmp , ENEMY_ID ) == atoi( buff2 ) )
					{
						print("\nNPC_PKPetShop_GetPetList: tmp = %d", tmp);
						break;
					}
				}
				if( tmp == EnemyCnt )
				{
					return;
				}
				name = ENEMY_getChar( tmp , ENEMY_NAME );
				// krynn end
				/*--  Á°¤¬        ¤Ê¤é¡b¥¢¥¤  ¥à¤¬Â¸ºß¤·¤Ê¤¤¤Î¤Ç¤½¤Î  ¹æ¤Ï  »ë--*/
				if(name == NULL) continue;

				loopcnt++;
				if(loopcnt == MAXSHOPPET) break;

				/*--  ºÝ¤Î¥×¤Ò¥È¥³¥ë¤Ä¤¯¤ê--*/
				NPC_PetStrStr( atoi( buff2), rate, name, token2, tmp);

				/*--¥á¥Ã¥»¡¼¥¸  »ú  ¤È  ·ë--*/
	    		strncat( argtoken, token2, sizeof(token2));
			}
			else
			{
				return;
				/*krynn 2001/12/13 ³o¬qÀ³¸Ó¥Î¤£¨ì¤F¡A¥ý mark¡A§ï¬°ª½±µ return
				--¥¢¥¤  ¥à¤¬  15-25  ¤Î·Á¤Ç¶èÀÚ¤é¤ì¤Æ¤¤¤ë¾ì¹ç--
				int start;
				int end;

				//-"-"¤Ç¶èÀÚ¤é¤ì¤¿»Ï¤á¤Î¿ôÃÍ¤È  ¤Î¿ôÃÍ¤ò¼è  --
				getStringFromIndexWithDelim( buff2, "-", 1, token2, sizeof(token2));
				start = atoi( token2);
				getStringFromIndexWithDelim( buff2, "-", 2 ,token2, sizeof(token2));
				end = atoi( token2);

				//--  ¹æ¤¬µÕ¤Ë¤Ê¤Ã¤Æ¤¤¤¿¤é¡b  ¤ìÂØ¤¨¤ë
				if(start > end)
				{
					tmp = start;
					start = end;
					end = tmp;
				}

				end++;

				//--"-"¤Ç¶èÀÚ¤é¤ì¤¿Ê¬¤Î¥¢¥¤  ¥à¾ð  ¤ò  ¤ë--
				for(; start < end ; start++ ) 
				{
					//--¤Þ¤º  Á°      --

				 	name = ITEM_getNameFromNumber( start );
					//--  Á°¤¬        ¤Ê¤é¡b¥¢¥¤  ¥à¤¬Â¸ºß¤·¤Ê¤¤¤Î¤Ç¤½¤Î  ¹æ¤Ï  »ë--
					if(name == NULL) continue;

					loopcnt++;
					if(loopcnt == MAXSHOPPET) break;

					//--  ºÝ¤Î¥×¤Ò¥È¥³¥ë¤Ä¤¯¤ê--
					NPC_PetStrStr( start, rate, name, token2, tmp);

					//--¥á¥Ã¥»¡¼¥¸  »ú  ¤È  ·ë--
		    		strncat( argtoken, token2, sizeof(token2));
				}*/
			}
		}
	}
}


/*-----------------------------------------------------------------
	¥×¤Ò¥È¥³¥ëºî©¨
-------------------------------------------------------------------*/
void NPC_PetStrStr(int petID,double rate,char *name,char *token2,int index)
{
	int i;
	int gold;
	int level;
	int graNo;
	int TempNo;
	int EnemyTempNum;
	char info[1024];
	//char tryItem[256];	// krynn 2001/12/12 ¥u¬O­n¬Ý getItemInfoFromNumber Åª¥X¨Óªº¦r¦êªø¤°»ò¼Ëªº
	char escape[256] = {"PK Server Ãd"};	

	//krynn 2001/12/10 try
	//gold  = ITEM_getcostFromITEMtabl( itemID);
	//level = ITEM_getlevelFromITEMtabl( itemID);
	//graNo = ITEM_getgraNoFromITEMtabl( itemID);
	TempNo = ENEMY_getInt( index , ENEMY_TEMPNO );
	EnemyTempNum = ENEMYTEMP_getEnemyNum();
	for( i=0 ; i < EnemyTempNum ; i++ )
	{
		if( ENEMYTEMP_getInt( i , E_T_TEMPNO ) == TempNo )
		{
			break;
		}
	}
	if( i == EnemyTempNum )
	{
		return;
	}
	gold  = RAND(0,20);
	level = 0;
	graNo = ENEMYTEMP_getInt( i , E_T_IMGNUMBER );
	//print("\nPKPetShop::NPC_PetStrStr(): TempNo = %d ; graNo = %d",TempNo,graNo);
	/*--¥ì¡¼¥È¤ò¤«¤±¤ë--*/
	gold=(int)(gold * rate);

	//strcpy( tryItem,ITEM_getItemInfoFromNumber( 10 ) );
	//print("PKPetShop::NPC_PetStrStr(): try = %s\n",tryItem);
	//krynn end

	makeEscapeString( escape, info, sizeof( info));

	makeEscapeString( name, escape, sizeof( escape));

	sprintf( token2, "%s|0|%d|%d|%d|%s|", escape, level, gold, graNo, info);
}


/*-------------------------------------------
 *(  ¤¦)
 *¥¯¥é¥¤¥¢¥ó¥È¤«¤éÊÖ¤Ã¤¿  ¤¿·ë²Ì¤ò  ±Ç¤µ¤»¤ë
 *
 *------------------------------------------*/
BOOL NPC_PKPetShop_SetNewPet(int meindex,int talker,char *data)
{

	char buf[1024];
	char buff2[128];
	int i = 1, j = 1;
	int select;
	int kosuu = 0;
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	double rate = 1.0;
	int gold = 0;		
	int EmptyPetCnt=0;

	/*--ÊÖ¤Ã¤Æ¤­¥û¥Ç¡¼¥¿¤ò¥»¥ì¥¯¥È¤È¸Ä¿ô¤ËÊ¬²ò--*/
	print("\nNPC_PKPetShop_SetNewPet: data = %s",data);
	getStringFromIndexWithDelim( data, "|", 1, buf, sizeof( buf));
	select=atoi(buf);		// krynn 2001/12/10  select ¬Oª±®a¶Ç¦^­n¶Rªº²Ä´X¼Ë
	print("\nNPC_PKPetShop_SetNewPet: select = %d",select);
	if(select == 0) return FALSE;
	getStringFromIndexWithDelim( data, "|", 2, buf, sizeof( buf));
	kosuu=atoi(buf);
	print("\nNPC_PKPetShop_SetNewPet: kosuu(ª±®a­n¶Rªº¼Æ¶q) = %d",kosuu);

	if( kosuu <= 0 ) return FALSE;
    
	/* ¤´¼ç¿Í  ¤Î¥Ú¥Ã¥È¤Î¶õ¤­¤òÃµ¤¹ */
    if( !CHAR_CHECKINDEX(talker) )
	{
		return FALSE;
	}
	for( i=0 ; i < CHAR_MAXPETHAVE ; i++ ) 
	{
	    if( CHAR_getCharPet( talker,i) == -1 )
		{
			EmptyPetCnt++;
		}
    }
	// ¦³ EmptyPetCnt ­ÓÃdª«ªÅ¦ì
    /* ¶õ¤­¤¬  ¤¤ */
 	print("\nNPC_PKPetShop_SetNewPet:EmptyPetCnt(ª±®a¦³ªºªÅ¦ì) = %d",EmptyPetCnt);
	if( EmptyPetCnt <= 0 ) return FALSE;
	if( EmptyPetCnt > CHAR_MAXPETHAVE )
	{
		EmptyPetCnt = CHAR_MAXPETHAVE;
	}
	if( kosuu > EmptyPetCnt )
	{
		kosuu = EmptyPetCnt;
	}

	// krynn 2001/12/10
	// À³¸Ó¥Î¤£¨ì¡A©Ò¥H mark °_¨Ó
	/*--¸Ä¿ô¤Î¥Á¥§¥Ã¥¯  Åö¤ËÁ´    ¤ë¤«¡ª
	for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {
		itemindex = CHAR_getItemIndex( talker , i );

		if( !ITEM_CHECKINDEX( itemindex) ) {
			kosuucnt++;
		 }
	}

	--ÊÖ¤Ã¤Æ  ¤¿¸Ä¿ô¤Î  ¤¬  ºÝ¤Î¸Ä¿ô  ¥µ¡¼¥ÐÂ¦  ¤è¤êÂ¿¤¤¤È¶³¤«¤·¤¤¤Î¤Ç
	--¥µ¡¼¥Ð¡¼Â¦¤Î  ¤ò  ¤ì¤ë
	if( kosuucnt < kosuu){
		kosuu = kosuucnt;
	}
	--¥¼¤Ò¤Î¾ì¹ç¤Ï¥¨¥é¡¼
	if(kosuucnt == 0 ){
		return FALSE;
	}
	krynn end */

	i = 1;

	/*--¶³Å¹¤Î¥Õ¥¡¥¤¥ë  ¤¤Ëô¤Ï¥Õ¥¡¥¤¥ë¤¬³«¤±¤Ê¤«¤Ã¤¿¤È¤­¤Ï½ª  --*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
       	print("shop_GetArgStr_Err");
       	return FALSE;
	}

	/*---¥ì¡¼¥È¤ò¼è    ¤Ê¤±¤ì¤Ð1.0)-*/
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "buy_rate", buf, sizeof( buf))
	 != NULL) {
		rate= atof( buf);
	}
	
	/*--¥¢¥¤  ¥à¤ÎÄÉ²Ã¤ò¹Ô¤¦¤È¤³¤í-*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "PetList", 
		buf, sizeof( buf)) != NULL )
	{
		while(getStringFromIndexWithDelim(buf , "," , j, buff2, sizeof(buff2))
		 != FALSE )
		{
			j++;
			/*--  "-"¤¬´Þ¤Þ¤ì¤Æ¤¤¤ë¤«¤É¤¦¤«--*/
			if(strstr( buff2, "-") == NULL)
			{	// krynn 2001/12/10 try
				//if( ITEM_getcostFromITEMtabl(atoi(buff2)) !=-1) {
					// 
					if ( i == select)
					{
						/*---¥¢¥¤  ¥à¤Îºî©¨---*/
						/*--¸Ä¿ôÊ¬ºî©¨--*/
						if(NPC_AddPetBuy(meindex, talker,atoi(buff2),kosuu,rate) != TRUE)
						{
							return FALSE;
						}
						return TRUE;
					}
					i++;		
				// krynn end}
			}else{
				/*--¥¢¥¤  ¥à¤¬  15-25  ¤Î·Á¤ÇÁ÷¤é¤ì¤¿¾ì¹ç--*/
				int start;
				int end;

				/* "-"¤Ç¶èÀÚ¤é¤ì¤¿»Ï¤á¤Î¿ôÃÍ¤È  ¤Î¿ôÃÍ¤ò¼è  --*/
				getStringFromIndexWithDelim( buff2, "-", 1, argstr, sizeof(argstr));
				start = atoi( argstr);
				getStringFromIndexWithDelim( buff2, "-", 2 ,argstr, sizeof(argstr));
				end = atoi( argstr);
				end++;

				/*--  ¹æ¤¬µÕ¤Ë¤Ê¤Ã¤Æ¤¤¤¿¤é¡b  ¤ìÂØ¤¨¤ë**/
				if(start > end){
					gold = start;
					start = end;
					end = gold;
				}

				/*--"-"¤Ç¶èÀÚ¤é¤ì¤¿Ê¬¤Î¥¢¥¤  ¥à¤ò¾ð  ¤ò  ¤ë--*/
				for(; start < end ; start++ ) {
					if( ITEM_getcostFromITEMtabl( start) != -1) {
						if ( i == select) {
							/*---¥¢¥¤  ¥à¤Îºî©¨---*/
							/*--¸Ä¿ôÊ¬ºî©¨--*/
							if(NPC_AddPetBuy(meindex, talker, start, kosuu, rate) != TRUE)
							{
								return FALSE;
							}
							return TRUE;
						}
						i++;
					}
				}
			}
		}
	}

	return FALSE;

}


/*---------------------------------------------
 *¥¢¥¤  ¥à¤ÎÄÉ²Ã¤ò¹Ô¤¦
 *--------------------------------------------*/
BOOL NPC_AddPetBuy(int meindex, int talker,int petID,int kosuu,double rate)
{
	int i,j,k,index,EnemyCnt,UpLevel;
	int gold;
	int ret;
	int maxgold;
	int Grade=0;
	char buf[1024];
	char msgbuf[64];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	
	// krynn 2001/12/15 get get's grade of this PKPetShop
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
       	print("shop_GetArgStr_Err");
       	return FALSE;
	}
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "Grade", buf, sizeof( buf))
	 != NULL) 
	{
		Grade = atoi( buf );
	}
	// end krynn

	/*--¥ì¡¼¥È¤ò¤«¤±¤ë--*/
	// krynn 2001/12/11 mark and change
	//gold = ITEM_getcostFromITEMtabl( itemID);
	gold = 4;
	// krynn end
	gold = (int)(gold * rate);
	maxgold = gold * kosuu;

	if(CHAR_getInt( talker, CHAR_GOLD) < maxgold ) return FALSE;

	/*--¶³¶â¤ò¸º¤é¤¹--*/
	CHAR_setInt( talker, CHAR_GOLD,CHAR_getInt( talker, CHAR_GOLD) - maxgold);

	EnemyCnt = ENEMY_getEnemyNum();
	for( index=0 ; index < EnemyCnt ; index++ )
	{
		if( ENEMY_getInt( index , ENEMY_ID ) == petID )
		{
			print("\nNPC_AddPetBuy: index = %d", index);
			break;
		}
	}
	if( index == EnemyCnt )
	{
		return FALSE;
	}

	/*--¸Ä¿ôÊ¬ºî©¨--*/
	for(i = 0 ; i < kosuu ; i++)
	{
		if( (ret = ENEMY_createPetFromEnemyIndex( talker , index )) == -1 )
		{
			return FALSE;
		};
		/******************/
		/* ¥Ú¥Ã¥È¾ð  Á÷¤ë */
		/******************/
		// ¤É¤³¤Ë¤Ï¤¤¤Ã¤¿¤«¤Ê
		for( j = 0 ; j < CHAR_MAXPETHAVE ; j++ )
		{
			if( CHAR_getCharPet( talker , j ) == ret )
			{
				break;
			}
		}
		if( j == CHAR_MAXPETHAVE ){
			return FALSE;
		}
		if( CHAR_CHECKINDEX( ret ) == TRUE ){
			CHAR_setMaxExpFromLevel( ret, Grade);
			UpLevel = CHAR_LevelUpCheck( ret , talker);
			for( k = 0; k < UpLevel; k ++ ){
				CHAR_PetLevelUp( ret );
				CHAR_PetAddVariableAi( ret, AI_FIX_PETLEVELUP );
			}
			CHAR_complianceParameter( ret );
			CHAR_setInt( ret , CHAR_HP , CHAR_getWorkInt( ret , CHAR_WORKMAXHP ) );

			snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
			CHAR_sendStatusString( talker, msgbuf );
			
			snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
			CHAR_sendStatusString( talker, msgbuf );
		}
	}
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
	return TRUE;
}



/*----------------------------------------
 *¥á¥Ë¥å¡¼    
 *----------------------------------------*/
void NPC_PKPetShop_Menu(int meindex,int talker)
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	token[NPC_UTIL_GETARGSTR_LINEMAX];
	char	buff[256];
	int		fd = getfdFromCharaIndex( talker);

	/* Å¹¤Î  Á°  ¥á¥Ã¥»¡¼¥¸| */
	/*--¶³Å¹¤Î¥Õ¥¡¥¤¥ë  ¤¤Ëô¤Ï¥Õ¥¡¥¤¥ë¤¬³«¤±¤Ê¤«¤Ã¤¿¤È¤­¤Ï½ª  --*/
    if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
		print("shop_GetArgStr_Err");
       	return;
    }
    	
    NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
	snprintf(token, sizeof(token),"%s|%s",CHAR_getChar( meindex, CHAR_NAME), buff);

	//	print("%s",escapedname);
	/*--¤³¤³¤ÇÁ÷¿®--*/
	//krynn 2001/12/10	³o¸Ìªº°Ñ¼Æ¦ü¥G¥i¥H©µ¥Î ItemShop ªº code¡A¥ý¥Î¥Î¬Ý
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMENU, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);
	//krynn end
}


/*-------------------------------------------
 *	  ¤ë½è  (¥×¤Ò¥È¥³¥ë¤òºî©¨  
 *	
 *-------------------------------------------*/
void NPC_PKPetShop_SellMain(int meindex,int talker,int before)
{

	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	token[NPC_UTIL_GETARGSTR_BUFSIZE];
	int fd = getfdFromCharaIndex( talker);

    if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
       	print("shop_GetArgStr_Err");
       	return;
    }

	/*--Á°²ó¤Î¥Ç¡¼¥¿»È¤¦¤«---*/
	if(before != -1) {

		/*--Á°²ó¤Î¥Ç¡¼¥¿¤¬»Ä¤Ã¤Æ¤¤¤ë¤Î¤ÇÂ¾¤Î¾ð  ¤ÏÁ÷¤é¤Ê¤¯    --*/
		sprintf(token,"1|0");
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN
							+CHAR_getWorkInt(meindex,NPC_PKPETSHOP_WORK_NO), 
					WINDOW_BUTTONTYPE_NONE, 
					CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					token);
	
	}else{

		char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
		char buff2[256];
	   	char buff[256];

		/*--³Æ¥á¥Ã¥»¡¼¥¸¤ò¼è  ¤·¤Æ¥¯¥é¥¤¥¢¥ó¥È¤ËÁ÷¤ë¥Ç¡¼¥¿¤òºî¤ë-*/
		/*--¤³¤Î¤ä¤ê  ¤Ï¤­¤¿¤Ê¤¤¤«¤â--*/
		NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "sell_main", buff2, sizeof( buff));
		sprintf( token, "1|1|%d|%s|%s|%s|", CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getChar( meindex, CHAR_NAME), buff, buff2);

		NPC_Util_GetStrFromStrWithDelim( argstr, "stone_msg", buff, sizeof( buff));

		if(CHAR_getWorkInt(meindex,NPC_PKPETSHOP_WORK_EXPRESS) == 1 ) {
			NPC_Util_GetStrFromStrWithDelim( argstr, "exrealy_msg", buff2, sizeof(buff2));
		}else{
			NPC_Util_GetStrFromStrWithDelim( argstr, "realy_msg", buff2, sizeof( buff2));
		}
		sprintf( token2,"%s|%s|", buff, buff2);

		/*--¥¢¥¤  ¥à¾ð  ¤Î      --*/
		NPC_PKPetShop_GetLimtPetList( talker,argstr, token2, -1);

		/*--¥á¥Ã¥»¡¼¥¸¤È¥¢¥¤  ¥à¾ð  ¤Î¹ç  --*/
		strncat( token, token2, sizeof( token));
	
		/*--¤³¤³¤ÇÁ÷¿®--*/
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN+
					CHAR_getWorkInt(meindex,NPC_PKPETSHOP_WORK_NO), 
					WINDOW_BUTTONTYPE_NONE, 
					CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					token);
	}
}





/*--------------------------------

¼«Ê¬¤¬¶³Å¹¤Ë  ¤ë¥¢¥¤  ¥à¤Î  »ú  ¤ò¤Î½à  

 *-------------------------------*/
int NPC_PKPetShop_GetLimtPetList(int talker, char *argstr, char* token2,int sell)
{

	char token[NPC_UTIL_GETARGSTR_LINEMAX];
	char buff[NPC_UTIL_GETARGSTR_LINEMAX];
	char token3[NPC_UTIL_GETARGSTR_LINEMAX];
	int k = 0 , i = 1 , j = 0;
	int imax;
	int itemtype = 0;
	int itemindex;
	int okflg = 0;
	char buf[256];
	int flg=0;
	int cost;
	
	
	if(sell == -1 ){
		i = CHAR_STARTITEMARRAY;
		imax = CHAR_MAXITEMHAVE;
		flg = -1;
	}else{
		i= sell;
		imax= sell + 1;
		flg = 1;
	}	
	
	/*---  ¤ì¤ë¥¢¥¤  ¥à¤ò¼è  ¤¹¤ë¡e--*/
	/**  ¥¹¥Ú¼³¥ã¥ë¥¢¥¤  ¥à¤ò¤â¤Ã¤Æ¤¤¤¿¤é  ¥ì¡¼¥È¤Ç·×»»--*/
	/*-- ¥¹¥Ú¼³¥ã¥ë½è  ¤Ç¥×¤Ò¥°¥é¥à¤¬    --*/

	for( ; i < imax ; i++ ){
		okflg=0;
		itemindex = CHAR_getItemIndex( talker , i );
		
		if( ITEM_CHECKINDEX( itemindex) ){

			/*--¥¢¥¤  ¥à¤Î¥¿¥¤¥×¤¬°ìÃ×¤·¤¿¤é¡b  ¤ì¤ë--*/
			if( NPC_Util_GetStrFromStrWithDelim( argstr,"LimitItemType",
			buff, sizeof( buff))
			 != NULL )
			{
				k = 1;
				while(getStringFromIndexWithDelim(buff , "," , k, token, sizeof(token))
				 != FALSE )
				{
#ifdef _ITEM_TYPETABLE
					int cmpmaxitem = sizeof(TypeTable)/sizeof(TypeTable[0]);
#endif
					k++;
#ifdef _ITEM_TYPETABLE
					for(j = 0 ; j < cmpmaxitem ; j++){
#else
					for(j = 0 ; j < ITEM_CATEGORYNUM+3 ; j++){
#endif
						if(strcmp( TypeTable[ j].arg  , token) == 0 ) {
							itemtype = TypeTable[ j].type;
							if(ITEM_getInt(itemindex,ITEM_TYPE) == itemtype) {

								/*--  »ú  ºî©¨--*/
								cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
								if(cost != -1) return cost;
								strncat( token2, token3, sizeof( token3));
								okflg = 1;
							}else if(itemtype == 30){
								if( 8 <= ITEM_getInt(itemindex,ITEM_TYPE) 
									&& (ITEM_getInt(itemindex,ITEM_TYPE) <= 15) ){
									/*--  »ú  ºî©¨--*/
									cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
									okflg = 1;
								}
							}else if(itemtype == 40){
								if(( 0 <= ITEM_getInt(itemindex,ITEM_TYPE) 
								  && (ITEM_getInt(itemindex,ITEM_TYPE) <= 4)) 
								 || (17 <= ITEM_getInt(itemindex,ITEM_TYPE) 
								  && (ITEM_getInt(itemindex,ITEM_TYPE) <= 19))
								) {
									/*--  »ú  ºî©¨--*/
									cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
									okflg = 1;
								}
							}else if(itemtype == 50){
								if( 5 <= ITEM_getInt(itemindex,ITEM_TYPE) 
								&& (ITEM_getInt(itemindex,ITEM_TYPE) <= 7) ){
									/*--  »ú  ºî©¨--*/
									cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
									okflg = 1;
								}
							}
							break;
						}
					}
						
					if(okflg == 1) break;
				}
			}
			/*--¥¿¥¤¥×¤Ë¤Ê¤¯¤Æ  ÀÜ  ¹æ¤·¤Æ  ¤ì¤ë¥¢¥¤  ¥à¤«¥Á¥§¥Ã¥¯--*/
			if( (NPC_Util_GetStrFromStrWithDelim( argstr, "LimitItemNo",
			buff,sizeof( buff))
			 != NULL)
				&& okflg == 0 )
			{
				k = 1;
				while(getStringFromIndexWithDelim(buff , "," , k, token, sizeof(token))
				 != FALSE )
				{
					k++;
					/*--²¿¤âÀßÄê¤µ¤ì¤Æ¤Ê¤«¤Ã¤¿¤é¡b  »ë¤¹¤ë-*/
					if(strstr( token, "-")==NULL && strcmp(token,"") != 0) {
						if(ITEM_getInt(itemindex,ITEM_ID) == atoi(token)) {
							/*--  »ú  ºî©¨--*/
							cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
							if(cost != -1) return cost;
							strncat(token2,token3,sizeof(token3));
							okflg=1;
						}
					}else if (strstr( token, "-") != NULL){
						int start;
						int end;
						int work;

						/*--  »ú  ºî©¨--*/
						/* "-"¤Ç¶èÀÚ¤é¤ì¤¿»Ï¤á¤Î¿ôÃÍ¤È  ¤Î¿ôÃÍ¤ò¼è  --*/
						getStringFromIndexWithDelim( token, "-", 1, buf, sizeof(buf));
						start = atoi( buf);
						getStringFromIndexWithDelim( token, "-", 2 ,buf, sizeof(buf));
						end = atoi( buf);

						/*--  ¹æ¤¬µÕ¤Ë¤Ê¤Ã¤Æ¤¤¤¿¤é¡b  ¤ìÂØ¤¨¤ë**/
						if(start > end){
							work = start;
							start = end;
							end = work;
						}

						end++;
						/*--"-"¤Ç¶èÀÚ¤é¤ì¤¿Ê¬¤Î¥¢¥¤  ¥à¤ò¾ð  ¤ò  ¤ë--*/
						if( (start <= ITEM_getInt(itemindex,ITEM_ID))
						 && (ITEM_getInt(itemindex,ITEM_ID) < end) )
						 {
							/*--  »ú  ºî©¨--*/
							cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
							if(cost != -1) return cost;
							strncat(token2,token3,sizeof(token3));
							okflg = 1;
						}
					}
				}
			}

			/*--  ¤ë¤³¤È¤¬¤Ç¤­¤Ê¤¤--*/
			if(okflg == 0) {
				cost = NPC_GetSellPetList(itemindex, 1, argstr, token3, i, sell);
				if(sell != -1) return -1;
				strncat( token2, token3, sizeof( token3));
			}
			
		}
	}
	return -1;
}



/*----------------------------------------------------------

	¥¯¥é¥¤¥¢¥ó¥È¤ËÁ÷¿®¤¹¤ë¥×¤Ò¥È¥³¥ë¤Îºî©¨

 *----------------------------------------------------------*/
int NPC_GetSellPetList(int itemindex,int flg, char *argstr,char *argtoken,int select,int sell)
{

	char buff[256];
	double rate = 0.2;
	char buff2[256];
	char buff3[64];
	int k = 1;
	int cost = -1;

	/*--¥¹¥Ú¼³¥ã¥ë¥ì¡¼¥È--**/
	if(NPC_Util_GetStrFromStrWithDelim( argstr,"special_item",buff, sizeof( buff))
	!= NULL)
	{
		if(NPC_Util_GetStrFromStrWithDelim( argstr,"special_rate",buff2, sizeof( buff2))
		 != NULL )
		{
			rate = atof(buff2);
		}else{
			rate = 1.2;
		}
		
		while(getStringFromIndexWithDelim(buff , "," , k, buff2, sizeof(buff2)) !=FALSE )
		{
			k++;

			if(strstr( buff2, "-") == NULL && strcmp(buff2,"") != 0) {
				if(ITEM_getInt(itemindex,ITEM_ID) == atoi(buff2)){
					cost = NPC_SellPetstrsStr( itemindex,0, rate, argtoken,select,sell);
					return cost;
				}
			}else if (strstr( buff2, "-") != NULL){
				int start;
				int end;
				int work;

				/*--  »ú  ºî©¨--*/
				/* "-"¤Ç¶èÀÚ¤é¤ì¤¿»Ï¤á¤Î¿ôÃÍ¤È  ¤Î¿ôÃÍ¤ò¼è  --*/
				getStringFromIndexWithDelim( buff2, "-", 1, buff3, sizeof(buff3));
				start = atoi( buff3);
				getStringFromIndexWithDelim( buff2, "-", 2 ,buff3, sizeof(buff3));
				end = atoi( buff3);

				/*--  ¹æ¤¬µÕ¤Ë¤Ê¤Ã¤Æ¤¤¤¿¤é¡b  ¤ìÂØ¤¨¤ë**/
				if(start > end){
					work = start;
					start = end;
					end = work;
				}
				end++;
	
				/*--"-"¤Ç¶èÀÚ¤é¤ì¤¿Ê¬¤Î¥¢¥¤  ¥à¤ò¾ð  ¤ò  ¤ë--*/
				if( (start <= ITEM_getInt(itemindex,ITEM_ID))
					&&  (ITEM_getInt(itemindex,ITEM_ID) < end)
				){
			
					cost = NPC_SellPetstrsStr( itemindex,0, rate, argtoken,select,sell);
					return cost;
				}
			}
		}
	}

	/*--¥Î¡¼¥Þ¥ë¥ì¡¼¥È--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr,"sell_rate",buff, sizeof( buff))
	 != NULL )
	{
		rate = atof(buff);
		cost = NPC_SellPetstrsStr( itemindex, flg ,rate, argtoken,select,sell);
		return cost;
	}

	return cost;
}


/*------------------------------------------
 
   »ú  ¤òºî¤ë(¤¦¤êÍÑ)
 
*------------------------------------------*/
int NPC_SellPetstrsStr(int itemindex,int flg,double rate,char *argtoken,int select,int sell)
{
	int cost;
	char escapedname[256];
	char name[256];	
	char *eff;
	

	cost = ITEM_getInt( itemindex, ITEM_COST);
	cost = (int)(cost * rate);

	if(sell != -1) return cost;

		
	//strcpy( escapedname, ITEM_getChar( itemindex, ITEM_NAME));
	strcpy( escapedname, ITEM_getChar( itemindex, ITEM_SECRETNAME));
	makeEscapeString( escapedname, name, sizeof( name));
	eff=ITEM_getChar(itemindex, ITEM_EFFECTSTRING);
	makeEscapeString( eff, escapedname, sizeof(escapedname));



	sprintf(argtoken,"%s|%d|%d|%d|%s|%d|",
			name,
			flg,
			cost,
			ITEM_getInt( itemindex, ITEM_BASEIMAGENUMBER),
			escapedname,
			select
	);

	return -1;

}

/*--------------------------------------------
 *
 *¥¯¥é¥¤¥¢¥ó¥È¤«¤é·ë²Ì¤¬ÊÖ¤Ã¤Æ¤­¤¿¤È¤­    ¤ë    
 *
 *-------------------------------------------*/
BOOL NPC_PKPetShop_SellNewPet(int meindex,int talker,char *data)
{
	int select;
	int cost;
	int oldcost;
	char token[32];
	int k;
	int itemindex;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char token2[256];
	
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {
		print("GetArgStrErr");
		return FALSE;
	}
	
	/*--ÊÖ¤Ã¤Æ  ¥û¥Ç¡¼¥¿¤ÎÊ¬²ò--*/
	getStringFromIndexWithDelim(data , "|" ,1, token, sizeof( token));
	select = atoi(token);
	getStringFromIndexWithDelim(data , "|" ,2, token, sizeof( token));
	oldcost = atoi(token);

	if(select == 0) return FALSE;

	cost = NPC_PKPetShop_GetLimtPetList( talker,argstr, token2,select);


	/*--°ã¤¦¤â¤Î¤ò¤¦¤í¤¦¤È¤·¤¿¤È¤­¤Î¥¨¥é¡¼--*/
	if(oldcost != cost || cost == -1)
	{
		int fd = getfdFromCharaIndex( talker);
		char token[256];
		
		sprintf(token,"\n\n«u§r!¹ï¤£°_"
					"\n\n¹ï¤£°_°Ú ! ¥i¤£¥i¥H¦A¿ï¤@¦¸©O¡H"
		);
		
		k = select;
		itemindex = CHAR_getItemIndex( talker ,k);
		
		
		/*--¤Ò¥°¤Î½Ð  --*/
		if(itemindex != -1) {
			print("\n%s(%d,%d,%d):©M¿ï¾ÜªºªF¦è¤£¦Perror([%s(%d)]­n®ø¥¢¤F¡^",
					CHAR_getChar(talker, CHAR_NAME),
					CHAR_getInt( talker, CHAR_FLOOR),
					CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y ),
					ITEM_getChar(itemindex, CHAR_NAME),
					ITEM_getInt( itemindex, ITEM_ID )
			);
			LogItem(
					CHAR_getChar( talker, CHAR_NAME ), /* ¥­¥ã¥é   */
					CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD ¦bitemªºlog¤¤¼W¥[item¦WºÙ
					itemindex,
#else
	       			ITEM_getInt( itemindex, ITEM_ID ),  /* ¥¢¥¤  ¥à  ¹æ */
#endif
					"SellErr",
					CHAR_getInt( talker, CHAR_FLOOR),
					CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y ),
					ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)
			);
		}else{
			print("\n%s(%d,%d,%d):©M¿ï¾ÜªºªF¦è¤£¦Perror(¨S¦³¥ô¦óitem¦s¦b¡^",
					CHAR_getChar(talker, CHAR_NAME),
					CHAR_getInt( talker, CHAR_FLOOR),
					CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y )
			);
			LogItem(
					CHAR_getChar( talker, CHAR_NAME ), /* ¥­¥ã¥é   */
					CHAR_getChar( talker, CHAR_CDKEY ),
	       			-1,  /* ¥¢¥¤  ¥à  ¹æ */
					"SellErr",
					CHAR_getInt( talker, CHAR_FLOOR),
					CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y ),
					"-1", "NULL", -1 );
		}
		/*--¤³¤³¤ÇÁ÷¿®--*/
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
				WINDOW_BUTTONTYPE_OK, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

		
		return FALSE;
	}

	k = select;
	itemindex=CHAR_getItemIndex( talker ,k);

	if(itemindex != -1) {
		{
			LogItem(
				CHAR_getChar( talker, CHAR_NAME ), /* ¥­¥ã¥é   */
				CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD ¦bitemªºlog¤¤¼W¥[item¦WºÙ
				itemindex,
#else
	       		ITEM_getInt( itemindex, ITEM_ID ),  /* ¥¢¥¤  ¥à  ¹æ */
#endif
				"Sell",
				CHAR_getInt( talker,CHAR_FLOOR),
				CHAR_getInt( talker,CHAR_X ),
				CHAR_getInt( talker,CHAR_Y ),
				ITEM_getChar( itemindex, ITEM_UNIQUECODE),
				ITEM_getChar( itemindex, ITEM_NAME),
				ITEM_getInt( itemindex, ITEM_ID)

			);
		}
		
	}

	CHAR_DelItem( talker, k);
	CHAR_AddGold( talker, cost);
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);

	return TRUE;
}



void NPC_LimitPetShop(int meindex,int talker,int select)
{

	int fd = getfdFromCharaIndex( talker);
	char token[NPC_UTIL_GETARGSTR_LINEMAX];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[1024];

	/*--¶³Å¹¤Î¥Õ¥¡¥¤¥ë  ¤¤Ëô¤Ï¥Õ¥¡¥¤¥ë¤¬³«¤±¤Ê¤«¤Ã¤¿¤È¤­¤Ï½ª  --*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {
       	print("shop_GetArgStr_Err");
       	return;
	}

	/*--  ¤¤¼è¤ê·´  Å¹¤Ç¤¹¥á¥Ã¥»¡¼¥¸-*/
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "sellonly_msg", buf, sizeof( buf))
	!=NULL)
	{
		sprintf(token,"\n\n%s", buf);

		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
				WINDOW_BUTTONTYPE_YESNO, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

	}else{
		CHAR_talkToCli( talker, meindex, "³o¬O¶R½æ±Mªù©±¡C",CHAR_COLORWHITE);
	}
	return;
}


/*--±¿Á÷²°¤µ¤ó--*/
void NPC_PKPetShop_ExpressmanCheck(int meindex,int talker)
{
	int fd = getfdFromCharaIndex( talker);
	char token[1024];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[1024];

	/*--¶³Å¹¤Î¥Õ¥¡¥¤¥ë  ¤¤Ëô¤Ï¥Õ¥¡¥¤¥ë¤¬³«¤±¤Ê¤«¤Ã¤¿¤È¤­¤Ï½ª  --*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {
       	print("shop_GetArgStr_Err");
       	return;
	}


	/*--  ¤¤¼è¤ê·´  Å¹¤Ç¤¹¥á¥Ã¥»¡¼¥¸-*/
	NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buf, sizeof( buf));
	sprintf(token,"4\n              %s\n\n%s"
					"\n\n          ¡Õ  ¥´¤u  ¡Ö      "
				  "\n\n          ¡Õ¥æ¥I¦æ§õ¡Ö"
					,CHAR_getChar(meindex,CHAR_NAME),buf);

	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_SELECT, 
			WINDOW_BUTTONTYPE_CANCEL, 
			CHAR_WINDOWTYPE_WINDOWITEMSHOP_EXPRESS,
			CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
			token);

	return;


}


#endif
 // _PKPETSHOP




