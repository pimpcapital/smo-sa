#include "version.h"
#include <string.h>
#include "object.h"
#include "char_base.h"
#include "char.h"
#include "util.h"
#include "npcutil.h"
#include "npc_oldman.h"
#include "title.h"
#include "char_data.h"
#include "readmap.h"
#include "lssproto_serv.h"
#include "log.h"
#include "handletime.h"





enum {
	E_INT_GRAPHIC = CHAR_NPCWORKINT1,		/*   ¿ÈÁ°¤Î  Áü */
	E_INT_GRAPHIC2 = CHAR_NPCWORKINT2,		/*   ¿È  ¤Î  Áü */
	E_INT_BORN = CHAR_NPCWORKINT3,			/* ÃÂÀ¸»þ´Ö */
	E_INT_DEAD = CHAR_NPCWORKINT4,			/* ¾Ã¤¨¤ë»þ´Ö*/ 
	E_INT_MODE = CHAR_NPCWORKINT5,			/* ¸½ºß¤Î¥â¡¼¥É */
	E_INT_NOWGRAPHIC = CHAR_NPCWORKINT6,	/* ¸½ºß¤Î¥°¥é¥Õ¥£¥Ã¥¯¥Ê¥ó¥Ð¡¼ */

};


typedef struct {
	char	arg[32];
	int		born;
	int 	dead;
}NPC_TimeMan;


#define YOAKE 700
#define NICHIBOTU 300
#define SHOUGO 125
#define YONAKA 500

static NPC_TimeMan	TimeTble[] = {

	{"ALLNIGHT",	NICHIBOTU+1,	YOAKE}, 	/* ÂÀ  ¤¬½Ð¤Æ¤¤¤Ê¤¤»þ */
	{"ALLNOON",		YOAKE+1,		NICHIBOTU},/* ÂÀ  ¤¬½Ð¤Æ¤¤¤ë»þ */
	{"AM",			YONAKA+1,		SHOUGO}, 	/* AM »þ¤Î¤ß */
	{"PM",			SHOUGO+1,		YONAKA}, 	/* PM »þ¤Î¤ß */
	{"FORE",		YOAKE+1,		SHOUGO},	/* ÂÀ  ¤¬¤Ç¤Æ¤«¤éÀµ¸á¤Þ¤Ç */
	{"AFTER",		SHOUGO+1,		NICHIBOTU},/* Àµ¸á¤«¤éÂÀ  ¤¬ÄÀ¤à¤Þ¤Ç */
	{"EVNING",		NICHIBOTU+1,	YONAKA}, 	/* ÂÀ  ¤¬ÄÀ¤ó¤Ç¤«¤é¿¿    ¤Þ¤Ç */
	{"MORNING",		YONAKA+1,		YOAKE}, 	/* ¿¿    ¤«¤éÂÀ  ¤¬¾º¤ë¤Þ¤Ç */
	{"FREE",		0,				1024},
};




BOOL NPC_TimeManInit( int meindex )
{

	int i=0;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[32];
	
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))==NULL){
		print("GetArgStrErr");
		return FALSE;
	}

	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPETOWNPEOPLE );
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "change_no", buf, sizeof( buf)) != NULL ){
		if(strstr(buf,"CLS")!=NULL){
			CHAR_setWorkInt( meindex,E_INT_GRAPHIC2, 9999 );
		}else{
			CHAR_setWorkInt( meindex,E_INT_GRAPHIC2, atoi(buf) );
		}
	}else{
		CHAR_setWorkInt( meindex,E_INT_GRAPHIC2, 9999 );
	}

	CHAR_setWorkInt( meindex, E_INT_GRAPHIC, CHAR_getInt( meindex, CHAR_BASEIMAGENUMBER) );

	if( NPC_Util_GetStrFromStrWithDelim( argstr, "time", buf, sizeof( buf)) != NULL ){
		for(i=0 ; i<9 ; i++){
			if(strstr(buf,TimeTble[i].arg) != NULL){
				CHAR_setWorkInt( meindex, E_INT_BORN, TimeTble[i].born);
				CHAR_setWorkInt( meindex, E_INT_DEAD, TimeTble[i].dead);
				return TRUE;
			}
		}
	}
	
	return FALSE;

}


void NPC_TimeManTalked( int meindex , int talkerindex , char *msg ,int color )
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char token[512];
	char buf[512];
	int tokennum=0;
	int i;


	/*--  ¤ÎÁ°¤Ë¤¤¤ë¤«¤É¤¦¤«¡ª--*/
	if(NPC_Util_isFaceToFace( meindex ,talkerindex ,2)==FALSE){
		/* £±¥°¥ê¥Ã¥É°Ê  ¤Î¤ß */
		if( NPC_Util_CharDistance( talkerindex ,meindex ) > 1) return;
	}

	if(CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER)==9999){
		/*--¾Ã¤¨¤Æ¤¤¤ë¥â¡¼¥É--*/
		return;
	}

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))==NULL){
		print("GetArgStrErr");	
		return;
	}
	

	if( CHAR_getInt(talkerindex,CHAR_WHICHTYPE) != CHAR_TYPEPLAYER )return;
	
	if( NPC_Util_charIsInFrontOfChar( talkerindex, meindex, 3 ) ==FALSE) return;




	if(CHAR_getWorkInt(meindex,E_INT_MODE)==0){
		/*--¥á¥Ã¥»¡¼¥¸¤òÊÖ¤¹ */
		NPC_Util_GetStrFromStrWithDelim(argstr,"main_msg",buf,sizeof( buf) );
 	
 	}else{
 		/*¥°¥é¥Õ¥£¥Ã¥¯    ¤ò  ¤¨¤¿¤À¤±¤Ê¤Î¤Ç¡b°ã¤¦¥á¥Ã¥»¡¼¥¸¤òÊÖ¤¹ */
		NPC_Util_GetStrFromStrWithDelim(argstr,"change_msg",buf,sizeof( buf) );
 	}

   	tokennum = 1;
   	/* ¥³¥ó¥Þ¤Ç¶èÀÚ¤é¤ì¤¿¥È¡¼¥¯¥ó¤¬²¿¤³¤¢¤ë¤«¿ô¤¨¤ë */
	 for( i=0;buf[i]!='\0';i++ ){
          if( buf[i] == ',' ) tokennum++;
     }
 
    /* ¥é¥ó¥À¥à¤Ç¤É¤ì¤òÃý¤ë¤«·è¤á¤Æ¡b¤½¤Î¥È¡¼¥¯¥ó¤ò¼è¤ê¤À¤¹ */
    getStringFromIndexWithDelim( buf,",", rand()%tokennum+1,token, sizeof(token));
	/*--¤·¤ã¤Ù¤ë--*/
	CHAR_talkToCli( talkerindex, meindex, token, CHAR_COLORWHITE );


}

/** ¤Þ¤ï¤ê¤Ë²¿¤«¤¬µ¯¤³¤Ã¤¿¤È¤­¤Ë¸Æ¤Ð¤ì¤ë*/
void NPC_TimeManWatch( int meobjindex, int objindex, CHAR_ACTION act,
                    int x,int y,int dir, int* opt,int optlen )
{
	int meindex=0;
	int index;
	LSTIME	nowlstime;
	int born;
	int dead;
	
	if( OBJECT_getType( objindex) != OBJTYPE_CHARA) return;
	index = OBJECT_getIndex( objindex);

	/* ¥×¥ì¥¤¥ä¡¼¤Ë¤Î¤ß  ±þ¤¹¤ë */
	if( CHAR_getInt( index, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER) return;

    meindex = OBJECT_getIndex( meobjindex);
	born = CHAR_getWorkInt( meindex , E_INT_BORN );
	dead = CHAR_getWorkInt( meindex , E_INT_DEAD );

//	print("now_hour=%d",nowlstime.hour);
	
	/* »þ´Ö¤ò¸«¤Æ¾õÂÖ¤ò·è¤á¤ë */
	if(born < dead){
		RealTimeToLSTime( NowTime.tv_sec, &nowlstime);
		if( (born < nowlstime.hour) && (dead > nowlstime.hour) ){
			if(CHAR_getWorkInt(meindex,E_INT_NOWGRAPHIC)
				==CHAR_getWorkInt( meindex, E_INT_GRAPHIC)) return;

			CHAR_setInt(meindex,CHAR_BASEIMAGENUMBER,
							CHAR_getWorkInt( meindex, E_INT_GRAPHIC));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
			CHAR_setWorkInt(meindex,E_INT_MODE,0);		
			CHAR_setWorkInt(meindex,E_INT_NOWGRAPHIC,
								CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER));
		}else{
			/*-¾Ã¤¨¤Æ¤¤¤ë--*/
			if(CHAR_getWorkInt(meindex,E_INT_NOWGRAPHIC)
				==CHAR_getWorkInt( meindex, E_INT_GRAPHIC2)) return;

			CHAR_setInt(meindex,CHAR_BASEIMAGENUMBER,
								CHAR_getWorkInt( meindex, E_INT_GRAPHIC2));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
			CHAR_setWorkInt(meindex,E_INT_MODE,1);
			CHAR_setWorkInt(meindex,E_INT_NOWGRAPHIC,
								CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER));		

		}
	}else{
		RealTimeToLSTime( NowTime.tv_sec, &nowlstime);
		if( (born < nowlstime.hour && 1024 > nowlstime.hour) 
			|| ( 0 < nowlstime.hour && dead > nowlstime.hour) 
		){
			if(CHAR_getWorkInt(meindex,E_INT_NOWGRAPHIC)
				==CHAR_getWorkInt( meindex, E_INT_GRAPHIC)) return;
	
			CHAR_setInt(meindex,CHAR_BASEIMAGENUMBER,
							CHAR_getWorkInt( meindex, E_INT_GRAPHIC));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
			CHAR_setWorkInt(meindex,E_INT_MODE,0);
			CHAR_setWorkInt(meindex,E_INT_NOWGRAPHIC,
							CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER));		
		}else{
			if(CHAR_getWorkInt(meindex,E_INT_NOWGRAPHIC)
				==CHAR_getWorkInt( meindex, E_INT_GRAPHIC2)) return;
			
			CHAR_setInt(meindex,CHAR_BASEIMAGENUMBER,
							CHAR_getWorkInt( meindex, E_INT_GRAPHIC2));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
			CHAR_setWorkInt(meindex,E_INT_MODE,1);
			CHAR_setWorkInt(meindex,E_INT_NOWGRAPHIC,
							CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER));		
		}
	}

}

