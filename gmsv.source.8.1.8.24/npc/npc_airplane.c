#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_airplane.h"
#include "handletime.h"

/* 
 * ¥[¬ü¯èªÅ (Made from Bus)
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
	NPC_WORK_ONEWAYFLG = CHAR_NPCWORKINT11,
	NPC_WORK_RUNWAVE = CHAR_NPCWORKINT13,
};

/* µñ  ¥á¥Ã¥»¡¼¥¸¤Îenum */
enum {
	NPC_AIR_MSG_GETTINGON,
	NPC_AIR_MSG_NOTPARTY,
	NPC_AIR_MSG_OVERPARTY,
	NPC_AIR_MSG_DENIEDITEM,
	NPC_AIR_MSG_ALLOWITEM,
	NPC_AIR_MSG_LEVEL,
	NPC_AIR_MSG_GOLD,
	NPC_AIR_MSG_EVENT,
	NPC_AIR_MSG_START,
	NPC_AIR_MSG_END,
#ifdef _NPC_AIRDELITEM
	NPC_AIR_MSG_DELITEM,
#endif
#ifdef _NPC_AIRLEVEL
	NPC_AIR_MSG_MAXLEVEL,
#endif
};
typedef struct {
	char	option[32];
	char	defaultmsg[128];
}NPC_AIR_MSG;
NPC_AIR_MSG		airmsg[] = {
	{ "msg_gettingon",	"PAON¡I¡]§AµLªk©ó¤¤³~¥[¤J§Ú­Ì­ò¡I¡^"},
	{ "msg_notparty",	"PAPAON¡I¡IµLªk¥H¹Î¶¤¥[¤J­ò¡I"},
	{ "msg_overparty",	"PAON¡I¡I¤H¼Æ¤wº¡¡C"},
	{ "msg_denieditem",		"PAPAON¡I¡I§Ú¥i¤£­n³o­Ó¹D¨ã¡I"},
	{ "msg_allowitem",		"«z³á~(·Q­n¨º­Ó¹D¨ã°Ú!)"},
	{ "msg_level",		"PAPAON¡I¡I§Aªºµ¥¯ÅÁÙ¤£°÷­ò¡I"},
	{ "msg_stone",		"PAPAON¡I¡Iª÷¿ú¤£¨¬­ò¡I"},
	{ "msg_event",		"PAON¡I¡I§AµLªk¥[¤J­ò¡I"},
	{ "msg_start",		"«z³á~(¥Xµo¶i¦æ)"},
	{ "msg_end",		"«z³á~(¨ìÃ¹)"}
#ifdef _NPC_AIRDELITEM
	,{ "msg_delitem",  "§A¨S¦³·f­¼ªº¹D¨ã"}
#endif
#ifdef _NPC_AIRLEVEL
    ,{ "msg_maxlevel",  "§Aªºµ¥¯Å¹L°ª®@"}
#endif
};

static int NPC_AirSetPoint( int meindex, char *argstr);
static void NPC_AirSetDestPoint( int meindex, char *argstr);
static BOOL NPC_AirCheckDeniedItem( int meindex, int charaindex, char *argstr);
static BOOL NPC_AirCheckLevel( int meindex, int charaindex, char *argstr);
static int NPC_AirCheckStone( int meindex, int charaindex, char *argstr);
static void NPC_AirSendMsg( int meindex, int talkerindex, int tablenum);
static int NPC_AirGetRoutePointNum( int meindex, char *argstr );
static void NPC_Air_walk( int meindex);
#ifdef _NPC_AIRLEVEL
static BOOL NPC_AirCheckMaxLevel( int meindex, int charaindex, char *argstr);
#endif

#define		NPC_AIR_LOOPTIME		100
#define		NPC_AIR_WAITTIME_DEFAULT	180
#define		NPC_AIR_WAITINGMODE_WAITTIME	5000

/*********************************
* ½é´ü½è  
*********************************/
BOOL NPC_AirInit( int meindex )
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int	i;
	char	buf[256],buf1[256];
	int	routenum;
	int	waittime;
	int	seflg;
	int	onewayflg;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	/* ¤Ê¤±¤ì¤Ð¤¤¤±¤Ê¤¤°ú¿ô¤Î¥Á¥§¥Ã¥¯ */
	routenum = NPC_Util_GetNumFromStrWithDelim( argstr, "routenum");
	if( routenum == -1 ) {
		print( "npcair:nothing routenum \n");
		return FALSE;
	}
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTEMAX, routenum);
	
	for( i = 1; i <= routenum; i ++ ) {
		char routetostring[64];
		snprintf( routetostring, sizeof( routetostring), "routeto%d", i);
		if( NPC_Util_GetStrFromStrWithDelim( argstr, routetostring,buf, sizeof(buf))
			== NULL ) 
		{
			print( "npcair:nothing route to \n");
			return FALSE;
		}
	}
	//ANDY_ADD	NPC_WORK_RUNWAVE
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "WAVE",buf1, sizeof(buf1)) == NULL )	{
		CHAR_setWorkInt( meindex, NPC_WORK_RUNWAVE, 77);
	}else	{
		CHAR_setWorkInt( meindex, NPC_WORK_RUNWAVE, atoi( buf1) );
	}

	waittime = NPC_Util_GetNumFromStrWithDelim( argstr, "waittime");
	if( waittime == -1 ) waittime = NPC_AIR_WAITTIME_DEFAULT;
	CHAR_setWorkInt( meindex, NPC_WORK_WAITTIME, waittime);

	seflg = NPC_Util_GetNumFromStrWithDelim( argstr, "seflg");
	if( seflg == -1 ) seflg = TRUE;
	CHAR_setWorkInt( meindex, NPC_WORK_SEFLG, seflg);
	
	onewayflg = NPC_Util_GetNumFromStrWithDelim( argstr, "oneway");
	if( onewayflg == -1 ) onewayflg = FALSE;	// default
	CHAR_setWorkInt( meindex, NPC_WORK_ONEWAYFLG, onewayflg);
	
	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEBUS );
	
	CHAR_setWorkInt( meindex, NPC_WORK_MODE, 0);
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 2);
	CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 0);
	CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, 0);
			
	CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
		NPC_AIR_WAITINGMODE_WAITTIME);
    
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
		int num = NPC_AirGetRoutePointNum( meindex, argstr);
		if( num <= 0 ) {
			print( "npcairplane:Very strange¡I\n");
			return FALSE;
		}
		CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, num-1);
		CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 1);
	}
	/* ¥ë¡¼¥È¤ò¥»¥Ã¥È¤¹¤ë */
	NPC_AirSetPoint( meindex, argstr);
	/* ¹Ô¤­Àè¤ò  ¼¨¤¹¤ë */
	NPC_AirSetDestPoint( meindex, argstr);
}

    return TRUE;
}


/*********************************
*   ¤·¤«¤±¤é¤ì¤¿»þ¤Î½è  
*********************************/
void NPC_AirTalked( int meindex , int talkerindex , char *szMes ,
                     int color )
{
    int i;
    int	partyflg = FALSE;
	int npc_wave = CHAR_getWorkInt( meindex, NPC_WORK_RUNWAVE);
	
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
		//NPC_AirCheckJoinParty( meindex, talkerindex, TRUE);
	}
	else {
		if( CHAR_getWorkInt( meindex, NPC_WORK_MODE) == 0 ) {
			int i;
	//		#define NPC_AIR_DEBUGROUTINTG	"routingtable:"
			if( strstr( szMes, "¥Xµo" )  ||
				strstr( szMes, "¥Xµo" )  ||
				strstr( szMes, "Go" )  ||
				strstr( szMes, "go" ))
			{
				CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
				
				/* ¥ë¡¼¥×´Ø¿ô¤Î¸Æ½Ð¤·¤ò¬¤¤¯Â®ÅÙ¤Ë¤¹¤ë */
	 			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_AIR_LOOPTIME);
				
				/* SE   ¤é¤¹  ¥Þ¥ó¥â¥¹¤Î¶«¤Ó   */
				if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
					//andy_reEdit	NPC_WORK_RUNWAVE
					CHAR_sendSEoArroundCharacter( 
									CHAR_getInt( meindex, CHAR_FLOOR),
									CHAR_getInt( meindex, CHAR_X),
									CHAR_getInt( meindex, CHAR_Y),
									npc_wave,
									TRUE);
				}
				/* ½Ð  ¤¹¤ë»þ¤Î¥á¥Ã¥»¡¼¥¸*/
				for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
					int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
					if( CHAR_CHECKINDEX( partyindex)) {
						NPC_AirSendMsg( meindex, partyindex, NPC_AIR_MSG_START);
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
						NPC_AIR_WAITINGMODE_WAITTIME);
		    /* ¸½ºß¤Î»þ´Ö¤ò¥»¥Ã¥È */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
		}
		else if( strstr( szMes, NPC_AIR_DEBUGROUTINTG )) {
			/* ¥Ç¥Ð¥Ã¥°ÍÑ */
			char *p = strstr( szMes,NPC_AIR_DEBUGROUTINTG);
			char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
			char *argstr;

			argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
			if( p) {
				int a = atoi( p+strlen(NPC_AIR_DEBUGROUTINTG));
				if( a <0 ) a = 1;
				CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, a);
			}
			//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
			/* ¥ë¡¼¥È¤ò¥»¥Ã¥È¤¹¤ë */
			NPC_AirSetPoint( meindex, argstr);
		}
#endif
	}
}
/**************************************
 * ¥ë¡¼¥×´Ø¿ô
 **************************************/
void NPC_AirLoop( int meindex)
{
	int	i;
	int npc_wave = CHAR_getWorkInt( meindex, NPC_WORK_RUNWAVE);
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
				//ANDY_reEdit
				CHAR_sendSEoArroundCharacter( 
								CHAR_getInt( meindex, CHAR_FLOOR),
								CHAR_getInt( meindex, CHAR_X),
								CHAR_getInt( meindex, CHAR_Y),
								npc_wave,
								TRUE);
			}
			/* ½Ð  ¤¹¤ë»þ¤Î¥á¥Ã¥»¡¼¥¸*/
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
				if( CHAR_CHECKINDEX( partyindex)) {
					NPC_AirSendMsg( meindex, partyindex, NPC_AIR_MSG_START);
				}
			}
			
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
			/* ¥ë¡¼¥×´Ø¿ô¤Î¸Æ½Ð¤·¤ò¬¤¤¯Â®ÅÙ¤Ë¤¹¤ë */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_AIR_LOOPTIME);
		}
		return;
	  case 1:
	  	/* ¬¤¤¯ */
	  	NPC_Air_walk( meindex);
	  case 2:
		/* »ß¤Þ¤Ã¤Æ¤¤¤ë¥â¡¼¥É */
		/* »þ´Ö¤¬·Ð¤Ã¤¿¤Î¤Ç¡f½Ð  ¤¹¤ë */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) 
			+ (CHAR_getWorkInt( meindex, NPC_WORK_WAITTIME) /3)
			< NowTime.tv_sec)
		{
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
			/* ¥ë¡¼¥×´Ø¿ô¤Î¸Æ½Ð¤·¤ò¬¤¤¯Â®ÅÙ¤Ë¤¹¤ë */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_AIR_LOOPTIME);
		
		}
		return;
	  case 3:
		/* ÅþÃå¤·¤Æ¤â¡f¥¯¥é¥¤¥¢¥ó¥È¤Î  ¼ÌÂÔ¤Á¤Î°Ù¤Ë¡f
		 * ¾¯¤·¤³¤³¤Ç¥¦¥§¥¤¥È¤ò¤¤¤ì¤Æ¤ä¤ë
		 */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) + 3	< NowTime.tv_sec){
			char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
			char *argstr;
			argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_AIR_WAITINGMODE_WAITTIME);
			{
				int r = CHAR_getWorkInt( meindex, NPC_WORK_ROUTEMAX);
				CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, RAND( 1, r));
			}
			CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 
				CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP)^1);
			if( CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP) == 1)  {
				int num = NPC_AirGetRoutePointNum( meindex, argstr);
				CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, num-1);
			}else {
				CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 
					CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT) +1);
			}
			NPC_AirSetPoint( meindex, argstr);
			NPC_AirSetDestPoint( meindex, argstr);
			CHAR_DischargeParty( meindex, 0);
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
			if ((CHAR_getWorkInt(meindex, NPC_WORK_ONEWAYFLG) == 1) &&
			    (CHAR_getWorkInt(meindex, NPC_WORK_ROUNDTRIP) == 1) ){
			  CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_AIR_LOOPTIME);
			  CHAR_setWorkInt( meindex, NPC_WORK_MODE, 1);
			} else
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
static void NPC_Air_walk( int meindex)
{
	POINT	start, end;
	int dir;
	int ret;
	int i;
	int npc_wave = CHAR_getWorkInt( meindex, NPC_WORK_RUNWAVE );

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
		if( NPC_AirSetPoint( meindex, argstr) == FALSE ) {
			/*     ¤ËÅþÃå*/
			/* ÂÔ¤Á¥â¡¼¥É¤Ë¤¹¤ë */
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,3);
			
			/* SE   ¤é¤¹  ¥Þ¥ó¥â¥¹¤Î¶«¤Ó   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
				//ANDY_reEdit
				CHAR_sendSEoArroundCharacter( 
					CHAR_getInt( meindex, CHAR_FLOOR),
					CHAR_getInt( meindex, CHAR_X),
					CHAR_getInt( meindex, CHAR_Y),
					npc_wave,
					TRUE);
			}
			/* Ãå¤¤¤¿»þ¤Î¥á¥Ã¥»¡¼¥¸*/
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
				if( CHAR_CHECKINDEX( partyindex)) {
					NPC_AirSendMsg( meindex, partyindex, NPC_AIR_MSG_END);
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

#if 0
	/* ¤Ò¤Ã¤«¤«¤Ã¤¿»þ¤Î°Ù¤Î½è   */
	for( i = 0; i < 100; i ++ ) {	
		if( dir < 0 ) {
			dir = RAND( 0,7);
		}	
		dir = NPC_Util_SuberiWalk( meindex, dir);
		if( dir >= 0 && dir <= 7) break;
	}
#endif
	
	if( dir >= 0 && dir <= 7 ) {
		/* ¬¤¤¯ */
		ret = CHAR_walk( meindex, dir, 0);

		if( ret == CHAR_WALKSUCCESSED ) {
			/* ¼«Ê¬¤¬¿Æ¤Ê¤éÃç´Ö¤ò¬¤¤«¤»¤ë */
			int	i;
			int	mefl=CHAR_getInt( meindex, CHAR_FLOOR);
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int toindex = CHAR_getWorkInt( meindex, i + CHAR_WORKPARTYINDEX1);
				int fl = CHAR_getInt( toindex, CHAR_FLOOR);
				int xx = CHAR_getInt( toindex, CHAR_X);
				int yy = CHAR_getInt( toindex, CHAR_Y);
				if( CHAR_CHECKINDEX(toindex) &&
				    (mefl==fl) && (abs(xx-end.x)+abs(yy-end.y)<10) ) {
					int	parent_dir;
					/* »Ò¤Î°Ì  ¤È¡f¿Æ¤Î¬¤¤­Á°¤Î°Ì  ¤«¤é  ¸þ¤òµá¤á¤ë */
					/* ¬¤¤¯ */
					start.x = xx;
					start.y = yy;
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
static int NPC_AirSetPoint( int meindex, char *argstr)
{
	char	buf[4096];
	char	buf2[256];
	char	buf3[256];
	int floor,warpx,warpy;
	int ret;
	char routetostring[64];
	
	snprintf( routetostring, sizeof( routetostring), "routeto%d", 
				CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, routetostring,buf, sizeof(buf))
		== NULL ) 
	{
		print( "npcair:nothing route \n");
		return FALSE;
	}

	ret = getStringFromIndexWithDelim( buf, ";", 
		CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT),
		buf2, sizeof(buf2));
	if( ret == FALSE ) return FALSE;

	// Arminius: add floor
	
	ret = getStringFromIndexWithDelim( buf2, ",", 1,
		buf3, sizeof(buf3));
	if( ret == FALSE) return FALSE;
	floor = atoi(buf3);

	ret = getStringFromIndexWithDelim( buf2, ",", 2,
		buf3, sizeof(buf3));
	if( ret == FALSE) return FALSE;
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTETOX, atoi( buf3));
	warpx = atoi(buf3);
	
	ret = getStringFromIndexWithDelim( buf2, ",", 3,
		buf3, sizeof(buf3));
	if( ret == FALSE) return FALSE;
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTETOY, atoi( buf3));
	warpy = atoi(buf3);

	if (floor!=CHAR_getInt(meindex, CHAR_FLOOR)) {
		int	i;
		CHAR_warpToSpecificPoint(meindex, floor, warpx, warpy);

		for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
			int toindex = CHAR_getWorkInt( meindex, i + CHAR_WORKPARTYINDEX1);
			if( CHAR_CHECKINDEX(toindex) ) {
				CHAR_warpToSpecificPoint(toindex, floor, warpx, warpy);
			}
		}
		CHAR_setWorkInt( meindex, NPC_WORK_ROUTETOX, warpx);
		CHAR_setWorkInt( meindex, NPC_WORK_ROUTETOY, warpy);
	}

	return TRUE;
}
/**************************************
 * route  ¹æ¤«¤é¡f  Á°¤¬¤¢¤Ã¤¿¤é¤½¤ì¤ò
 * ¾Î¹æ¤Î¤È¤³¤Ë¥»¥Ã¥È¤¹¤ë¡e
 **************************************/
static void NPC_AirSetDestPoint( int meindex, char *argstr)
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
static BOOL NPC_AirCheckDeniedItem( int meindex, int charaindex, char *argstr)
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
BOOL NPC_AirCheckAllowItem( int meindex, int charaindex, BOOL pickupmode)
{
	char	buf[1024];
	BOOL	found = TRUE;
	BOOL	pickup = FALSE;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "pickupitem", buf, sizeof( buf))
		!= NULL ) 
	{
		pickup = TRUE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "allowitem", buf, sizeof( buf))
		!= NULL ) 
	{
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
						/* ¾ò·ï¤¬Â·¤Ã¤Æ¤¤¤ë¤«¤é¡f¤½¤Î¥¢¥¤  ¥à¤ò¼è¤ë */
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
static BOOL NPC_AirCheckLevel( int meindex, int charaindex, char *argstr)
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

#ifdef _NPC_AIRLEVEL
static BOOL NPC_AirCheckMaxLevel( int meindex, int charaindex, char *argstr)
{
	int		level;
	
	/* ¤Ê¤±¤ì¤Ð¤¤¤±¤Ê¤¤°ú¿ô¤Î¥Á¥§¥Ã¥¯ */
	level = NPC_Util_GetNumFromStrWithDelim( argstr, "maxlevel");
	if( level == -1 ) {
		return TRUE;
	}
	if( CHAR_getInt( charaindex, CHAR_LV) < level ) return TRUE;
	
	return FALSE;
}
#endif

/**************************************
 * ¸æ¶â¤ò¥Á¥§¥Ã¥¯¤¹¤ë
 * -1 ÂÌ   0°Ê¾å¡v    ¡f¤«¤Ä  Í×Stone
 **************************************/
static int NPC_AirCheckStone( int meindex, int charaindex, char *argstr)
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
static void NPC_AirSendMsg( int meindex, int talkerindex, int tablenum)
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buf[256];
	char	msg[256];
	if( tablenum < 0 || tablenum >= arraysizeof( airmsg)) return;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, airmsg[tablenum].option, buf, sizeof( buf))
		!= NULL ) 
	{
		strcpy( msg, buf);
	}
	else {
		snprintf( msg, sizeof(msg),airmsg[tablenum].defaultmsg);
	}
	CHAR_talkToCli( talkerindex, meindex, msg, CHAR_COLORYELLOW);
}
/**************************************
 * ¥ë¡¼¥È  ¡¼¥Ö¥ë¤Î¥Ý¥¤¥ó¥È¤Î¿ô¤ò¼è  ¤¹¤ë
 **************************************/
static int NPC_AirGetRoutePointNum( int meindex, char *argstr )
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
		print( "npcair:nothing route \n");
		return -1;
	}
	for( i = 1; ; i ++ ) {
		ret = getStringFromIndexWithDelim( buf, ";", i, buf2, sizeof(buf2));
		if( ret == FALSE) break;
	}
	return( i -1);
}
BOOL NPC_AirCheckJoinParty( int meindex, int charaindex, BOOL msgflg)
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
		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_GETTINGON);
		return FALSE;
	}
	/* ¤Ñ¡¼¤Æ¤£¤À¤Ã¤¿¤é¤À¤á */
	if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE ) != CHAR_PARTY_NONE) {
		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_NOTPARTY);
		return FALSE;
	}
	/* ¥Ñ¡¼  ¥£¤Î¿Í¿ô¤ò¥Á¥§¥Ã¥¯¤¹¤ë */
	if( CHAR_getEmptyPartyArray( meindex) == -1 ) {
		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_OVERPARTY);
		return FALSE;
	}
	/* ¥¢¥¤  ¥à¤Î¥Á¥§¥Ã¥¯¤ò¤¹¤ë(¶Ø»ß¥¢¥¤  ¥à) */
	if( !NPC_AirCheckDeniedItem( meindex, charaindex, argstr)) {
		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_DENIEDITEM);
		return FALSE;
	}
#ifdef _ITEM_CHECKWARES
	if( CHAR_CheckInItemForWares( charaindex, 0) == FALSE )	{
		CHAR_talkToCli( charaindex, -1, "µLªkÄâ±a³fª«¤W¾÷¡C", CHAR_COLORYELLOW);
		return FALSE;
	}
#endif
	/* ¥¢¥¤  ¥à¤Î¥Á¥§¥Ã¥¯¤ò¤¹¤ë(  Í×¥¢¥¤  ¥à) */
	if( !NPC_AirCheckAllowItem( meindex, charaindex, FALSE)) {
		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_ALLOWITEM);
		return FALSE;
	}
#ifdef _NPC_AIRDELITEM
	if( !NPC_AirCheckDelItem( meindex, charaindex, FALSE) ){ //­Y¬O¨S¦©°£¤F¹D¨ã
		if( msgflg ) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_DELITEM);
	    return FALSE;
	}
#endif
	/* ¥ì¥Ù¥ë¤Î¥Á¥§¥Ã¥¯¤ò¤¹¤ë */
	if( !NPC_AirCheckLevel( meindex, charaindex, argstr)) {
		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_LEVEL);
		return FALSE;
	}
#ifdef _NPC_AIRLEVEL
    if( !NPC_AirCheckMaxLevel( meindex, charaindex, argstr)) {
		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_MAXLEVEL);
		return FALSE;
	}
#endif
	/* ¥¤¥Ù¥ó¥È  ¤«¥Á¥§¥Ã¥¯¤¹¤ë */
//	if( CHAR_getInt( charaindex, CHAR_NOWEVENT) != 0 ||
//		CHAR_getInt( charaindex, CHAR_NOWEVENT2) != 0 ||
//		CHAR_getInt( charaindex, CHAR_NOWEVENT3) != 0 )
//	{
//		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_EVENT);
//		return FALSE;
//	}
	/* ¸æ¶â¤Î¥Á¥§¥Ã¥¯¤ò¤¹¤ë  ¶³¶â¤ò¼è¤ë¤Î¤Ç¡f  ½ª¥Á¥§¥Ã¥¯¤Ë¤¹¤ë¤³¤È¡ª   */
	ret = NPC_AirCheckStone( meindex, charaindex, argstr);
	if( ret == -1 ) {
		if( msgflg) NPC_AirSendMsg( meindex, charaindex, NPC_AIR_MSG_GOLD);
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

#ifdef _NPC_AIRDELITEM //¤W­¸¾÷®É,ÀË¬d¬O§_­n¦©°£¹D¨ã
BOOL NPC_AirCheckDelItem( int meindex, int charaindex, BOOL pickupmode)
{
	char	buf[1024];
	BOOL	found = TRUE;
	BOOL	pickup = FALSE;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "delitem", buf, sizeof( buf))
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
						CHAR_DelItem( charaindex, j);
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
#endif
