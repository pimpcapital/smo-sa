#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_bus.h"
#include "handletime.h"

/* 
 * ¥Ð¥¹¤ß¤¿¤¤¤Ê»ö¤ò¤¹¤ëNPC
 */
 
enum {
	NPC_WORK_ROUTETOX = CHAR_NPCWORKINT1,		/* ¤É¤³¤Ø¡ª  ºÂ   */
	NPC_WORK_ROUTETOY = CHAR_NPCWORKINT2,		/* ¤É¤³¤Ø¡ª  ºÂ   */
	NPC_WORK_ROUTEPOINT = CHAR_NPCWORKINT3,		/* º£²¿    ¤« */
	NPC_WORK_ROUNDTRIP = CHAR_NPCWORKINT4,		/* ¹Ô¤­¤«µ¢¤ê¤«  £°¡u¹Ô¤­ £±¡uµ¢¤ê  */
	NPC_WORK_MODE = CHAR_NPCWORKINT5,
	NPC_WORK_CURRENTROUTE = CHAR_NPCWORKINT6, 
	NPC_WORK_ROUTEMAX = CHAR_NPCWORKINT7,
	NPC_WORK_WAITTIME = CHAR_NPCWORKINT8,
	NPC_WORK_CURRENTTIME = CHAR_NPCWORKINT9,
	NPC_WORK_SEFLG = CHAR_NPCWORKINT10,
};

/* µñ  ¥á¥Ã¥»¡¼¥¸¤Îenum */
enum {
	NPC_BUS_MSG_GETTINGON,
	NPC_BUS_MSG_NOTPARTY,
	NPC_BUS_MSG_OVERPARTY,
	NPC_BUS_MSG_DENIEDITEM,
	NPC_BUS_MSG_ALLOWITEM,
	NPC_BUS_MSG_LEVEL,
	NPC_BUS_MSG_GOLD,
	NPC_BUS_MSG_EVENT,
	NPC_BUS_MSG_START,
	NPC_BUS_MSG_END,
};
typedef struct {
	char	option[32];
	char	defaultmsg[128];
}NPC_BUS_MSG;
NPC_BUS_MSG		busmsg[] = {
	{ "msg_gettingon",	"PAON¡I¡]§AµLªk©ó¤¤³~¥[¤J§Ú­Ì­ò¡I¡^"},
	{ "msg_notparty",	"PAPAON¡I¡IµLªk¥H¹Î¶¤¥[¤J­ò¡I"},
	{ "msg_overparty",	"PAON¡I¡I¤H¼Æ¤wº¡¡C"},
	{ "msg_denieditem",		"PAPAON¡I¡I§Ú¥i¤£­n³o­Ó¹D¨ã¡I"},
	{ "msg_allowitem",		"«z³á~(·Q­n¨º­Ó¹D¨ã°Ú!)"},
	{ "msglevel",		"PAPAON¡I¡I§Aªºµ¥¯ÅÁÙ¤£°÷­ò¡I"},
	{ "msg_stone",		"PAPAON¡I¡Iª÷¿ú¤£¨¬­ò¡I"},
	{ "msg_event",		"PAON¡I¡I§AµLªk¥[¤J­ò¡I"},
	{ "msg_start",		"«z³á~(¥Xµo¶i¦æ)"},
	{ "msg_end",		"«z³á~(¨ìÃ¹)"}
	
};

static int NPC_BusSetPoint( int meindex, char *argstr);
static void NPC_BusSetDestPoint( int meindex, char *argstr);
static BOOL NPC_BusCheckDeniedItem( int meindex, int charaindex, char *argstr);
static BOOL NPC_BusCheckLevel( int meindex, int charaindex, char *argstr);
static int NPC_BusCheckStone( int meindex, int charaindex, char *argstr);
static void NPC_BusSendMsg( int meindex, int talkerindex, int tablenum);
static int NPC_BusGetRoutePointNum( int meindex, char *argstr );
static void NPC_Bus_walk( int meindex);

#define		NPC_BUS_LOOPTIME		200

/* ÂÔ¤Á»þ´Ö¥Ç¥Õ¥©¥ë¥È */
#define		NPC_BUS_WAITTIME_DEFAULT	180

#define		NPC_BUS_WAITINGMODE_WAITTIME	5000

/*********************************
* ½é´ü½è  
*********************************/
BOOL NPC_BusInit( int meindex )
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int i;
	char	buf[256];
	int	routenum;
	int	waittime;
	int seflg;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));

	/* ¤Ê¤±¤ì¤Ð¤¤¤±¤Ê¤¤°ú¿ô¤Î¥Á¥§¥Ã¥¯ */
	routenum = NPC_Util_GetNumFromStrWithDelim( argstr, "routenum");//¦æ¾p¸ô½u¼Æ¶q

	if( routenum == -1 ) {
		print( "npcbus:nothing routenum \n");
		return FALSE;
	}
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTEMAX, routenum);
	
	for( i = 1; i <= routenum; i ++ ) {
		char routetostring[64];
		snprintf( routetostring, sizeof( routetostring), "routeto%d", i);
		if( NPC_Util_GetStrFromStrWithDelim( argstr, routetostring,buf, sizeof(buf))
			== NULL ) 
		{
			print( "npcbus:nothing route to \n");
			return FALSE;
		}

	}
	waittime = NPC_Util_GetNumFromStrWithDelim( argstr, "waittime");

	if( waittime == -1 ) waittime = NPC_BUS_WAITTIME_DEFAULT;
	CHAR_setWorkInt( meindex, NPC_WORK_WAITTIME, waittime);

	seflg = NPC_Util_GetNumFromStrWithDelim( argstr, "seflg");

	if( seflg == -1 ) seflg = TRUE;
	CHAR_setWorkInt( meindex, NPC_WORK_SEFLG, seflg);
    
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEBUS );
	
	CHAR_setWorkInt( meindex, NPC_WORK_MODE, 0);
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 2);
	CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 0);
	CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, 0);
			
	CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
						NPC_BUS_WAITINGMODE_WAITTIME);
    
    /* ¸½ºß¤Î»þ´Ö¤ò¥»¥Ã¥È */
    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);

    for( i = 0; i < CHAR_PARTYMAX; i ++) {
    	CHAR_setWorkInt( meindex, CHAR_WORKPARTYINDEX1 + i, -1);
    }
	
	/* ¥ë¡¼¥È·èÄê¤¹¤ë */
{
	int rev;
	int r = CHAR_getWorkInt( meindex, NPC_WORK_ROUTEMAX);
	CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, RAND( 1, r));
	//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));

	/*   ¤í¥¹¥¿¡¼¥È */
	rev = NPC_Util_GetNumFromStrWithDelim( argstr, "reverse");

	if( rev == 1 ) {
		int num = NPC_BusGetRoutePointNum( meindex, argstr);

		if( num <= 0 ) {
			print( "npcbus:Very strange¡I\n");
			return FALSE;
		}
		CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, num-1);
		CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 1);
	}
	/* ¥ë¡¼¥È¤ò¥»¥Ã¥È¤¹¤ë */
	NPC_BusSetPoint( meindex, argstr);
	/* ¹Ô¤­Àè¤ò  ¼¨¤¹¤ë */
	NPC_BusSetDestPoint( meindex, argstr);
}

    return TRUE;
}


/*********************************
*   ¤·¤«¤±¤é¤ì¤¿»þ¤Î½è  
*********************************/
void NPC_BusTalked( int meindex , int talkerindex , char *szMes ,
                     int color )
{
    int i;
    int	partyflg = FALSE;
	
    /* ¥×¥ì¥¤¥ä¡¼¤ËÂÐ¤·¤Æ¤À¤±  ±þ¤¹¤ë */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }
	/* ¼«Ê¬¤Î¥Ñ¡¼  ¥£  ¾èµÒ  ¤«¤É¤¦¤«Ä´¤Ù¤ë */
	for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
		int index = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
		if( CHAR_CHECKINDEX(index)){
			if( index == talkerindex) {
				partyflg = TRUE;
			}
		}
	}
	if( !partyflg ) {
		//NPC_BusCheckJoinParty( meindex, talkerindex, TRUE);
	}
	else {
		if( CHAR_getWorkInt( meindex, NPC_WORK_MODE) == 0 ) {
			int i;
	//		#define NPC_BUS_DEBUGROUTINTG	"routingtable:"
			if( strstr( szMes, "¥Xµo" )  ||
				strstr( szMes, "¥Xµo" )  ||
				strstr( szMes, "Go" )  ||
				strstr( szMes, "go" ))
			{
				CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
				
				/* ¥ë¡¼¥×´Ø¿ô¤Î¸Æ½Ð¤·¤ò¬¤¤¯Â®ÅÙ¤Ë¤¹¤ë */
	 			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
				/* SE   ¤é¤¹  ¥Þ¥ó¥â¥¹¤Î¶«¤Ó   */
				if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
					CHAR_sendSEoArroundCharacter( 
									CHAR_getInt( meindex, CHAR_FLOOR),
									CHAR_getInt( meindex, CHAR_X),
									CHAR_getInt( meindex, CHAR_Y),
									60,
									TRUE);
				}
				/* ½Ð  ¤¹¤ë»þ¤Î¥á¥Ã¥»¡¼¥¸*/
				for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
					int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
					if( CHAR_CHECKINDEX( partyindex)) {
						NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_START);
					}
				}
			}
		}
#if 0
		else if( strstr( szMes, "°±¤î" )  ||
			strstr( szMes, "°±¤î" )  ||
			strstr( szMes, "stop" )  ||
			strstr( szMes, "Stop" ))
		{
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,2);

			/* ¥ë¡¼¥×´Ø¿ô¤Î¥¤¥ó¥¿¡¼¥Ð¥ë¤òÂ¿¤¯¤¹¤ë  */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
						NPC_BUS_WAITINGMODE_WAITTIME);
		    /* ¸½ºß¤Î»þ´Ö¤ò¥»¥Ã¥È */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
		}
		else if( strstr( szMes, NPC_BUS_DEBUGROUTINTG )) {
			/* ¥Ç¥Ð¥Ã¥°ÍÑ */
			char *p = strstr( szMes,NPC_BUS_DEBUGROUTINTG);
			char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
			char *argstr;

			argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
			if( p) {
				int a = atoi( p+strlen(NPC_BUS_DEBUGROUTINTG));
				if( a <0 ) a = 1;
				CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, a);
			}
			//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
			/* ¥ë¡¼¥È¤ò¥»¥Ã¥È¤¹¤ë */
			NPC_BusSetPoint( meindex, argstr);
		}
#endif
	}
}
/**************************************
 * ¥ë¡¼¥×´Ø¿ô
 **************************************/
void NPC_BusLoop( int meindex)
{
	int	i;
	switch( CHAR_getWorkInt( meindex, NPC_WORK_MODE )) {
	  case 0:
	    /* ÂÔ¤Á¥â¡¼¥É¤Î»þ¡f»þ´Ö¤ò¥Á¥§¥Ã¥¯¤¹¤ë */
		/* »þ´Ö¤¬·Ð¤Ã¤¿¤Î¤Ç¡f½Ð  ¤¹¤ë */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) 
			+ CHAR_getWorkInt( meindex, NPC_WORK_WAITTIME) 
			< NowTime.tv_sec)
		{
			/* SE   ¤é¤¹  ¥Þ¥ó¥â¥¹¤Î¶«¤Ó   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
				CHAR_sendSEoArroundCharacter( 
								CHAR_getInt( meindex, CHAR_FLOOR),
								CHAR_getInt( meindex, CHAR_X),
								CHAR_getInt( meindex, CHAR_Y),
								60,
								TRUE);
			}
			/* ½Ð  ¤¹¤ë»þ¤Î¥á¥Ã¥»¡¼¥¸*/
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
				if( CHAR_CHECKINDEX( partyindex)) {
					NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_START);
				}
			}
			
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
			/* ¥ë¡¼¥×´Ø¿ô¤Î¸Æ½Ð¤·¤ò¬¤¤¯Â®ÅÙ¤Ë¤¹¤ë */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
		}
		return;
	  case 1:
	  	/* ¬¤¤¯ */
	  	NPC_Bus_walk( meindex);
	  case 2:
		/* »ß¤Þ¤Ã¤Æ¤¤¤ë¥â¡¼¥É */
		/* »þ´Ö¤¬·Ð¤Ã¤¿¤Î¤Ç¡f½Ð  ¤¹¤ë */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) 
			+ (CHAR_getWorkInt( meindex, NPC_WORK_WAITTIME) /3)
			< NowTime.tv_sec)
		{
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
			/* ¥ë¡¼¥×´Ø¿ô¤Î¸Æ½Ð¤·¤ò¬¤¤¯Â®ÅÙ¤Ë¤¹¤ë */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
		
		}
		return;
	  case 3:
		/* ÅþÃå¤·¤Æ¤â¡f¥¯¥é¥¤¥¢¥ó¥È¤Î  ¼ÌÂÔ¤Á¤Î°Ù¤Ë¡f
		 * ¾¯¤·¤³¤³¤Ç¥¦¥§¥¤¥È¤ò¤¤¤ì¤Æ¤ä¤ë
		 */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) + 3
			< NowTime.tv_sec)
		{
			char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
			char *argstr;

			argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
			/* ¥ë¡¼¥×´Ø¿ô¤Î¸Æ½Ð¤·¤òÃÙ¤¯¤¹¤ë */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
						NPC_BUS_WAITINGMODE_WAITTIME);
			
			/* ¥ë¡¼¥È·èÄê¤¹¤ë */
			{
				int r = CHAR_getWorkInt( meindex, NPC_WORK_ROUTEMAX);
				CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, RAND( 1, r));
				//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
			}
			/* ¹Ô¤­µ¢¤ê¥Õ¥é¥°  ¹¹ */
			CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 
							CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP)^1);

			/* ±­¥Ý¥¤¥ó¥È¤ÎÄ´Àá */
			/* µ¢¤ê¤Ï  ¼ì½è   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP) == 1)  {
				/* ¤½¤Î¥ë¡¼¥È¤Î  ¹â¥Ý¥¤¥ó¥È¿ô¤ò  ¤ë */
				int num = NPC_BusGetRoutePointNum( meindex, argstr);
				CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, num-1);
			}
			else {
				CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 
							CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT) +1);
			}
			/* ±­¤Î¥Ý¥¤¥ó¥È¤ÎºÂ  ¤ò¥»¥Ã¥È¤¹¤ë */
			NPC_BusSetPoint( meindex, argstr);
			/* ¹Ô¤­Àè¤ò  ¼¨¤¹¤ë */
			NPC_BusSetDestPoint( meindex, argstr);
			/* ¥Ñ¡¼  ¥£  ¤±¤ë½è  ¤ò¤¹¤ë */
			CHAR_DischargeParty( meindex, 0);
		    /* ¸½ºß¤Î»þ´Ö¤ò¥»¥Ã¥È */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
			/* ¥â¡¼¥É¥¯¥ê¥¢ */
			CHAR_setWorkInt( meindex, NPC_WORK_MODE, 0);
		}
		return;
	  default:
	    break;
	}
}
/**************************************
 * ¬¤¤¯¡e
 **************************************/
static void NPC_Bus_walk( int meindex)
{	
	POINT	start, end;
	int dir;
	int ret;
	int i;

	/* ¬¤¤¯´Ø·¸ */
	/* ÅþÃå¤·¤¿»þ¤Î½è   */
	start.x = CHAR_getInt( meindex, CHAR_X);
	start.y = CHAR_getInt( meindex, CHAR_Y);
	end.x = CHAR_getWorkInt( meindex, NPC_WORK_ROUTETOX);
	end.y = CHAR_getWorkInt( meindex, NPC_WORK_ROUTETOY);
	
	/* ÅþÃå¤·¤¿¤Î¤Ç±­¤Î¥Ý¥¤¥ó¥È¤Ë */
	if( start.x == end.x && start.y == end.y ) {
		int add = 1;
		char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
		char *argstr;

		argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));

		if( CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP ) == 1 ) {
			add *= -1;
		}
		CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 
						CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT) +add);
		if( NPC_BusSetPoint( meindex, argstr) == FALSE ) {
			/*     ¤ËÅþÃå*/
			/* ÂÔ¤Á¥â¡¼¥É¤Ë¤¹¤ë */
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,3);
			
			/* SE   ¤é¤¹  ¥Þ¥ó¥â¥¹¤Î¶«¤Ó   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
				CHAR_sendSEoArroundCharacter( 
								CHAR_getInt( meindex, CHAR_FLOOR),
								CHAR_getInt( meindex, CHAR_X),
								CHAR_getInt( meindex, CHAR_Y),
								60,
								TRUE);
			}
			/* Ãå¤¤¤¿»þ¤Î¥á¥Ã¥»¡¼¥¸*/
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
				if( CHAR_CHECKINDEX( partyindex)) {
					NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_END);
				}
			}
		    /* ¸½ºß¤Î»þ´Ö¤ò¥»¥Ã¥È */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
			return;
		}
		else {
			return;
		}
	}
	/*-------------------------------------------------------*/
	/* ¬¤¤«¤»¤ë½è   */
	
	/*   ¸þ¤òµá¤á¤ë */
	dir = NPC_Util_getDirFromTwoPoint( &start,&end );

	/* º£¤¤¤ë¾ì½ê¤ÎÂÔ    ¥Ñ¡¼  ¥£¬¤¤­¤Ç»È¤¦   */
	end.x = CHAR_getInt( meindex, CHAR_X);
	end.y = CHAR_getInt( meindex, CHAR_Y);
	
	/* ¤Ò¤Ã¤«¤«¤Ã¤¿»þ¤Î°Ù¤Î½è   */
	for( i = 0; i < 100; i ++ ) {	
		if( dir < 0 ) {
			dir = RAND( 0,7);
		}	
		dir = NPC_Util_SuberiWalk( meindex, dir);
		if( dir >= 0 && dir <= 7) break;
	}
	
	if( dir >= 0 && dir <= 7 ) {
		/* ¬¤¤¯ */
		ret = CHAR_walk( meindex, dir, 0);

		if( ret == CHAR_WALKSUCCESSED ) {
			/* ¼«Ê¬¤¬¿Æ¤Ê¤éÃç´Ö¤ò¬¤¤«¤»¤ë */
			int	i;
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int toindex = CHAR_getWorkInt( meindex, i + CHAR_WORKPARTYINDEX1);
				if( CHAR_CHECKINDEX(toindex) ) {
					int		parent_dir;
					/* »Ò¤Î°Ì  ¤È¡f¿Æ¤Î¬¤¤­Á°¤Î°Ì  ¤«¤é  ¸þ¤òµá¤á¤ë */
					/* ¬¤¤¯ */
					start.x = CHAR_getInt( toindex, CHAR_X);
					start.y = CHAR_getInt( toindex, CHAR_Y);
					parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end );
					/* ¥°¥é¥Ç¥£¥¦¥¹¥ª¥×¼³¥ç¥ó¬¤¤­¤ò  ¸½¤¹¤ë°Ù¤Ë¡f
					 * ±­¤Î»Ò¤ÏÁ°¤Î»Ò¤Î  ¤òÄÉ¤¦¤è¤¦¤Ë¤¹¤ë
					 */
					end = start;
					if( parent_dir != -1 ) {
						CHAR_walk( toindex, parent_dir, 0);
					}
				}
			}
	    }
	}
}
/**************************************
 * ±­¤Î¾ì½ê¤ò¥»¥Ã¥È¤¹¤ë
 **************************************/
static int NPC_BusSetPoint( int meindex, char *argstr)
{
	char	buf[4096];
	char	buf2[256];
	char	buf3[256];
	int ret;
	char routetostring[64];
	
	snprintf( routetostring, sizeof( routetostring), "routeto%d", 
				CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, routetostring,buf, sizeof(buf))
		== NULL ) 
	{
		print( "npcbus:nothing route \n");
		return FALSE;
	}
	ret = getStringFromIndexWithDelim( buf, ";", 
								CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT),
								buf2, sizeof(buf2));
		
	if( ret == FALSE ) return FALSE;

	ret = getStringFromIndexWithDelim( buf2, ",", 1,
								buf3, sizeof(buf3));
	if( ret == FALSE) return FALSE;
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTETOX, atoi( buf3));
	
	ret = getStringFromIndexWithDelim( buf2, ",", 2,
								buf3, sizeof(buf3));
	if( ret == FALSE) return FALSE;
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTETOY, atoi( buf3));
	return TRUE;
}
/**************************************
 * route  ¹æ¤«¤é¡f  Á°¤¬¤¢¤Ã¤¿¤é¤½¤ì¤ò
 * ¾Î¹æ¤Î¤È¤³¤Ë¥»¥Ã¥È¤¹¤ë¡e
 **************************************/
static void NPC_BusSetDestPoint( int meindex, char *argstr)
{
	char 	buf[256];
	char	routename[256];

	snprintf( routename, sizeof( routename), "routename%d", 
				CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));

	if( NPC_Util_GetStrFromStrWithDelim( argstr, routename, buf, sizeof( buf))
		!= NULL ) 
	{
		CHAR_setChar( meindex, CHAR_OWNTITLE, buf);
		CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX));
	}
}
/**************************************
 * »ØÄê¤µ¤ì¤¿¥¢¥¤  ¥à¤ò  ¤Ã¤Æ¤¤¤ë¤«¥Á¥§¥Ã¥¯¤¹¤ë
 *   ¤Ã¤Æ¤¤¤¿¤é¤À¤á
 **************************************/
static BOOL NPC_BusCheckDeniedItem( int meindex, int charaindex, char *argstr)
{
	char	buf[1024];
	BOOL	found = TRUE;

	if( NPC_Util_GetStrFromStrWithDelim( argstr, "denieditem", buf, sizeof( buf))
		!= NULL ) 
	{
		int	i;
		int ret;
		for( i = 1; ; i ++) {
			int itemid;
			char buf2[64];
			int j;
			ret = getStringFromIndexWithDelim( buf, ",", i, buf2, sizeof(buf2));
			if( ret == FALSE ) break;
			itemid = atoi( buf2);
			for( j = 0; j < CHAR_MAXITEMHAVE; j ++) {
				int itemindex = CHAR_getItemIndex( charaindex, j);
				if( ITEM_CHECKINDEX( itemindex)) {
					if( ITEM_getInt( itemindex, ITEM_ID) == itemid) {
						found = FALSE;
						break;
					}
				}
			}
		}
	}
	return found;
}
/**************************************
 * »ØÄê¤µ¤ì¤¿¥¢¥¤  ¥à¤ò  ¤Ã¤Æ¤¤¤ë¤«¥Á¥§¥Ã¥¯¤¹¤ë
 *   ¤Ã¤Æ¤¤¤Ê¤¤¤È¤À¤á
 **************************************/
BOOL NPC_BusCheckAllowItem( int meindex, int charaindex, BOOL pickupmode)
{
	char	buf[1024];
	BOOL	found = TRUE;
	BOOL	pickup = FALSE;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "pickupitem", buf, sizeof( buf))!= NULL ){
		pickup = TRUE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "allowitem", buf, sizeof( buf))!= NULL ){
		int	i;
		int ret;
		for( i = 1; ; i ++) {
			int itemid;
			char buf2[64];
			int j;
			BOOL	getflg;
			ret = getStringFromIndexWithDelim( buf, ",", i, buf2, sizeof(buf2));
			if( ret == FALSE ) break;
			itemid = atoi( buf2);
			getflg = FALSE;
			for( j = 0; j < CHAR_MAXITEMHAVE; j ++) {
				int itemindex = CHAR_getItemIndex( charaindex, j);
				if( ITEM_CHECKINDEX( itemindex)) {
					if( ITEM_getInt( itemindex, ITEM_ID) == itemid) {
						if( pickupmode && pickup && !getflg) {
							CHAR_DelItem( charaindex, j);
							getflg = TRUE;
						}
						break;
					}
				}
			}
			if( j == CHAR_MAXITEMHAVE) {
				found = FALSE;
				break;
			}
		}
	}
	return found;
}

/**************************************
 * »ØÄê¤µ¤ì¤¿¥ì¥Ù¥ë°Ê¾å¤«¥Á¥§¥Ã¥¯¤¹¤ë
 **************************************/
static BOOL NPC_BusCheckLevel( int meindex, int charaindex, char *argstr)
{
	int		level;
	
	/* ¤Ê¤±¤ì¤Ð¤¤¤±¤Ê¤¤°ú¿ô¤Î¥Á¥§¥Ã¥¯ */
	level = NPC_Util_GetNumFromStrWithDelim( argstr, "needlevel");
	if( level == -1 ) {
		return TRUE;
	}
	if( CHAR_getInt( charaindex, CHAR_LV) >= level ) return TRUE;
	
	return FALSE;
}
/**************************************
 * ¸æ¶â¤ò¥Á¥§¥Ã¥¯¤¹¤ë
 * -1 ÂÌ   0°Ê¾å¡v    ¡f¤«¤Ä  Í×Stone
 **************************************/
static int NPC_BusCheckStone( int meindex, int charaindex, char *argstr)
{
	int		gold;
	
	/* ¤Ê¤±¤ì¤Ð¤¤¤±¤Ê¤¤°ú¿ô¤Î¥Á¥§¥Ã¥¯ */
	gold = NPC_Util_GetNumFromStrWithDelim( argstr, "needstone");
	if( gold == -1 ) {
		return 0;
	}
	if( CHAR_getInt( charaindex, CHAR_GOLD) >= gold ) return gold;
	
	return -1;
}
/**************************************
 * ¥á¥Ã¥»¡¼¥¸¤òÁ÷¤ë
 * °ú¿ô¤Î¥á¥Ã¥»¡¼¥¸¤¬¤Ê¤±¤ì¤Ð¥Ç¥Õ¥©¥ë¥È¥á¥Ã¥»¡¼¥¸¤òÁ÷¤ë
 **************************************/
static void NPC_BusSendMsg( int meindex, int talkerindex, int tablenum)
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buf[256];
	char	msg[256];
	if( tablenum < 0 || tablenum >= arraysizeof( busmsg)) return;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, busmsg[tablenum].option, buf, sizeof( buf))
		!= NULL ) 
	{
		strcpy( msg, buf);
	}
	else {
		snprintf( msg, sizeof(msg),busmsg[tablenum].defaultmsg);
	}
	CHAR_talkToCli( talkerindex, meindex, msg, CHAR_COLORYELLOW);
}
/**************************************
 * ¥ë¡¼¥È  ¡¼¥Ö¥ë¤Î¥Ý¥¤¥ó¥È¤Î¿ô¤ò¼è  ¤¹¤ë
 **************************************/
static int NPC_BusGetRoutePointNum( int meindex, char *argstr )
{
	int		i;
	char	buf[4096];
	char	buf2[256];
	int ret;
	char routetostring[64];
	
	snprintf( routetostring, sizeof( routetostring), "routeto%d", 
				CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, routetostring,buf, sizeof(buf))
		== NULL ) 
	{
		print( "npcbus:nothing route \n");
		return -1;
	}
	for( i = 1; ; i ++ ) {
		ret = getStringFromIndexWithDelim( buf, ";", i, buf2, sizeof(buf2));
		if( ret == FALSE) break;
	}
	return( i -1);
}
BOOL NPC_BusCheckJoinParty( int meindex, int charaindex, BOOL msgflg)
{
    //int		fd;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int		ret;
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	/* £±¥°¥ê¥Ã¥É°Ê  ¤Î¤ß */
	if( !NPC_Util_charIsInFrontOfChar( charaindex, meindex, 1 )) return FALSE; 
	/*     ¾è¼Ö¤Ïµñ  ¤¹¤ë */
	if( CHAR_getWorkInt( meindex, NPC_WORK_MODE) != 0 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_GETTINGON);
		return FALSE;
	}
	/* ¤Ñ¡¼¤Æ¤£¤À¤Ã¤¿¤é¤À¤á */
	if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE ) != CHAR_PARTY_NONE) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_NOTPARTY);
		return FALSE;
	}
	/* ¥Ñ¡¼  ¥£¤Î¿Í¿ô¤ò¥Á¥§¥Ã¥¯¤¹¤ë */
	if( CHAR_getEmptyPartyArray( meindex) == -1 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_OVERPARTY);
		return FALSE;
	}
	/* ¥¢¥¤  ¥à¤Î¥Á¥§¥Ã¥¯¤ò¤¹¤ë(¶Ø»ß¥¢¥¤  ¥à) */
	if( !NPC_BusCheckDeniedItem( meindex, charaindex, argstr)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_DENIEDITEM);
		return FALSE;
	}
#ifdef _ITEM_CHECKWARES
	if( CHAR_CheckInItemForWares( charaindex, 0) == FALSE )	{
		CHAR_talkToCli( charaindex, -1, "µLªkÄâ±a³fª«¤W¨®¡C", CHAR_COLORYELLOW);
		return FALSE;
	}
#endif
	/* ¥¢¥¤  ¥à¤Î¥Á¥§¥Ã¥¯¤ò¤¹¤ë(  Í×¥¢¥¤  ¥à) */
	if( !NPC_BusCheckAllowItem( meindex, charaindex, FALSE)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_ALLOWITEM);
		return FALSE;
	}
	/* ¥ì¥Ù¥ë¤Î¥Á¥§¥Ã¥¯¤ò¤¹¤ë */
	if( !NPC_BusCheckLevel( meindex, charaindex, argstr)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_LEVEL);
		return FALSE;
	}
	/* ¥¤¥Ù¥ó¥È  ¤«¥Á¥§¥Ã¥¯¤¹¤ë */
//	if( CHAR_getInt( charaindex, CHAR_NOWEVENT) != 0 ||
//		CHAR_getInt( charaindex, CHAR_NOWEVENT2) != 0 ||
//		CHAR_getInt( charaindex, CHAR_NOWEVENT3) != 0 )
//	{
//		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_EVENT);
//		return FALSE;
//	}
	/* ¸æ¶â¤Î¥Á¥§¥Ã¥¯¤ò¤¹¤ë  ¶³¶â¤ò¼è¤ë¤Î¤Ç¡f  ½ª¥Á¥§¥Ã¥¯¤Ë¤¹¤ë¤³¤È¡ª   */
	ret = NPC_BusCheckStone( meindex, charaindex, argstr);
	if( ret == -1 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_GOLD);
		return FALSE;
	}
	if( ret != 0 ) {
		char msgbuf[128];
		/* ¸æ¶â¤ò¤È¤ë */
		CHAR_setInt( charaindex, CHAR_GOLD, 
					CHAR_getInt( charaindex, CHAR_GOLD) - ret);
		/* Á÷¿® */
		CHAR_send_P_StatusString( charaindex, CHAR_P_STRING_GOLD);
		snprintf( msgbuf, sizeof( msgbuf), "¤ä¥I¤F%d Stone¡I", ret);
		CHAR_talkToCli( charaindex, -1, msgbuf, CHAR_COLORYELLOW);
	}
	/* ¥Ñ¡¼  ¥£¤Ë  ¤ë */
	//CHAR_JoinParty_Main( charaindex, meindex);
	
	//fd = getfdFromCharaIndex( charaindex );
	
	//lssproto_PR_send( fd, 1, 1);
	return TRUE;
}
