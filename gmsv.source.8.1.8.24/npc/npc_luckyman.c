#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_luckyman.h"


static void NPC_LuckyMan_selectWindow( int meindex, int toindex, int num,char *msg);
void NPC_LuckyManAllHeal( int talker,int mode );
BOOL NPC_LuckyManLevelCheck(int meindex,int talker);
void NPC_LuckyDisp(int meindex,int talker);
BOOL NPC_LuckyCostCheck(int meindex,int talker,int cost);
int NPC_GetMoney(int meindex,int talker,char *buf);



/*********************************
* ½é´ü½è  
*********************************/
BOOL NPC_LuckyManInit( int meindex )
{

	/*--¥­¥ã¥é¤Î¥¿¥¤¥×¤òÀßÄê--**/
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPELUCKYMAN );

	return TRUE;
}




/*********************************
*   ¤·¤«¤±¤é¤ì¤¿»þ¤Î½è  
*********************************/
void NPC_LuckyManTalked( int meindex , int talkerindex , char *szMes ,int color )
{

	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[256];
	char token[512];
	int cost;



	/*--  ¤ÎÁ°¤Ë¤¤¤ë¤«¤É¤¦¤«¡ª--*/
	if(NPC_Util_isFaceToFace( meindex ,talkerindex ,2) == FALSE) {
		/* £±¥°¥ê¥Ã¥É°Ê  ¤Î¤ß */
		if(NPC_Util_isFaceToChara( talkerindex, meindex, 1) == FALSE) return;
	}
	
	/*--ÀßÄê¥Õ¥¡¥¤¥ë¤ò  ¤ß  ¤ß--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1)))== NULL) {
		print("NPC_ExChange.c TypeCheck: GetArgStrErr\n");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return;
	}


	NPC_Util_GetStrFromStrWithDelim( argstr,"Stone", buf, sizeof( buf));
	cost = NPC_GetMoney( meindex, talkerindex, buf);

	NPC_Util_GetStrFromStrWithDelim( argstr,"main_msg", buf, sizeof( buf));
	sprintf( token, buf, cost);
	
	NPC_LuckyMan_selectWindow( meindex, talkerindex, 2, token);

}


static void NPC_LuckyMan_selectWindow( int meindex, int toindex, int num,char *msg)
{

	int fd = getfdFromCharaIndex( toindex);
	char token[1024];
	int buttontype = 0,windowtype = 0,windowno = 0;


	switch( num){
		case 1:
			sprintf(token ,"              ¡¸¤µ¤éªº¹B¶Õ¡¸  "
					"\n\n%s",msg
				);
				buttontype = WINDOW_BUTTONTYPE_OK;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
				windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_START; 
		break;
	
		case 2:
			sprintf(token, "                ¡¸¥e¤R®v¡¸  "
					"\n\n%s",msg
				);
				buttontype = WINDOW_BUTTONTYPE_YESNO;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
				windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_START; 
		break;

		case 3:
			sprintf(token, "                ¡¸¥e¤R®v¡¸  "
					"\n\n%s",msg
				);
				buttontype = WINDOW_BUTTONTYPE_OK;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
				windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_START; 
		break;
	}


	/*--Á÷¿®--*/
	lssproto_WN_send( fd, windowtype, 
				buttontype, 
				windowno,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);


}	


/*-----------------------------------------
¥¯¥é¥¤¥¢¥ó¥È¤«¤éÊÖ¤Ã¤Æ¤­¤¿»þ¤Ë¸Æ¤Ó½Ð¤µ¤ì¤ë¡e
-------------------------------------------*/
void NPC_LuckyManWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[256];
	int money;
	int level;
	
	/*--ÀßÄê¥Õ¥¡¥¤¥ë¤ò  ¤ß  ¤ß--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {
		print("NPC_ExChange.c TypeCheck: GetArgStrErr\n");
		print("NPCName=%s\n", CHAR_getChar(meindex,CHAR_NAME));
		return;
	}

	switch( seqno){
		case CHAR_WINDOWTYPE_WINDOWPETSHOP_START:
			if(select == WINDOW_BUTTONTYPE_YES) {
				NPC_Util_GetStrFromStrWithDelim( argstr, "Stone", buf,sizeof(buf));
			
				if(strstr(buf,"LV") != NULL) {
					char buff2[32];
					level = CHAR_getInt( talkerindex, CHAR_LV);
					getStringFromIndexWithDelim( buf, "*", 2, buff2,sizeof( buff2));
					money = level * atoi( buff2);

					if(NPC_LuckyCostCheck(meindex,talkerindex,money) == FALSE) {
						NPC_Util_GetStrFromStrWithDelim( argstr,"NoMoney", buf, sizeof( buf));
						NPC_LuckyMan_selectWindow( meindex, talkerindex, 3, buf);
						return;
					}
				}else{
					money = atoi( buf);
					if(NPC_LuckyCostCheck(meindex,talkerindex ,money) == FALSE) {
						NPC_Util_GetStrFromStrWithDelim( argstr,"NoMoney", buf,sizeof( buf));
						NPC_LuckyMan_selectWindow( meindex, talkerindex, 3, buf);
						return;
					}
				}

				CHAR_DelGold( talkerindex, money );

				CHAR_send_P_StatusString( talkerindex, CHAR_P_STRING_GOLD);
				NPC_LuckyDisp( meindex, talkerindex);

			}else if(select == WINDOW_BUTTONTYPE_OK) {

			}
		break;
	}
}



void NPC_LuckyDisp(int meindex,int talker)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[16];
	char token[1024];
	char buf2[512];
	int i = 1;

	/*--ÀßÄê¥Õ¥¡¥¤¥ë¤ò  ¤ß  ¤ß--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1)))== NULL) {
		print("NPC_Savepoint.c Init: GetArgStrErr");
		return;
	}

	/*--±¿¤ò¸«¤ë--*/
	sprintf( buf, "luck%d", CHAR_getInt( talker, CHAR_LUCK));
	NPC_Util_GetStrFromStrWithDelim( argstr, buf, buf2, sizeof( buf2));

	while(getStringFromIndexWithDelim( buf2, ",", i, token, sizeof( token))
	!= FALSE)
	{
		i++;
	}
	i--;
	i = rand()%i + 1;

	/*--¥é¥ó¥À¥à¤Ç¥á¥Ã¥»¡¼¥¸¤ò  ¼¨¤µ¤»¤ë--*/
	getStringFromIndexWithDelim( buf2,",", i, token, sizeof( token));
	NPC_LuckyMan_selectWindow( meindex, talker, 1, token);
}


/*---¶³¶â¤Î¥Á¥§¥Ã¥¯-**/
BOOL NPC_LuckyCostCheck(int meindex,int talker,int cost)
{
	/*--¸½ºß¤Ï²¾ÀßÄê---*/
	/*---¶³¶â¤¬Â­¤ê¤ë¤«¤É¤¦¤«¤Î¥Á¥§¥Ã¥¯---*/
	if(CHAR_getInt( talker, CHAR_GOLD) < cost) {
		return FALSE;
	}
	return TRUE;

}


int NPC_GetMoney(int meindex,int talker,char *buf)
{
	int level;
	int money;
	
	if(strstr(buf,"LV") != NULL) {
		char buff2[32];
		level = CHAR_getInt(talker,CHAR_LV);
		getStringFromIndexWithDelim( buf, "*" ,2, buff2,sizeof( buff2));
		money = level * atoi( buff2);
	}else{
		money = atoi( buf);
	}
	return money;

}
