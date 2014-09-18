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
 * 田旦心凶中卅儀毛允月NPC
 */
 
enum {
	NPC_WORK_ROUTETOX = CHAR_NPCWORKINT1,		/* 升仇尺〞  甄   */
	NPC_WORK_ROUTETOY = CHAR_NPCWORKINT2,		/* 升仇尺〞  甄   */
	NPC_WORK_ROUTEPOINT = CHAR_NPCWORKINT3,		/* 漆窒    井 */
	NPC_WORK_ROUNDTRIP = CHAR_NPCWORKINT4,		/* 墊五井窖曰井  ㄟ「墊五 ㄠ「窖曰  */
	NPC_WORK_MODE = CHAR_NPCWORKINT5,
	NPC_WORK_CURRENTROUTE = CHAR_NPCWORKINT6, 
	NPC_WORK_ROUTEMAX = CHAR_NPCWORKINT7,
	NPC_WORK_WAITTIME = CHAR_NPCWORKINT8,
	NPC_WORK_CURRENTTIME = CHAR_NPCWORKINT9,
	NPC_WORK_SEFLG = CHAR_NPCWORKINT10,
};

/* 蛐  丟永本□斥及enum */
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
	{ "msg_gettingon",	"PAON！（你無法於中途加入我們唷！）"},
	{ "msg_notparty",	"PAPAON！！無法以團隊加入唷！"},
	{ "msg_overparty",	"PAON！！人數已滿。"},
	{ "msg_denieditem",		"PAPAON！！我可不要這個道具！"},
	{ "msg_allowitem",		"哇喔~(想要那個道具啊!)"},
	{ "msglevel",		"PAPAON！！你的等級還不夠唷！"},
	{ "msg_stone",		"PAPAON！！金錢不足唷！"},
	{ "msg_event",		"PAON！！你無法加入唷！"},
	{ "msg_start",		"哇喔~(出發進行)"},
	{ "msg_end",		"哇喔~(到羅)"}
	
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

/* 謹切凜棉犯白巧伙玄 */
#define		NPC_BUS_WAITTIME_DEFAULT	180

#define		NPC_BUS_WAITINGMODE_WAITTIME	5000

/*********************************
* 賡渝質  
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

	/* 卅仃木壬中仃卅中婁醒及民尼永弁 */
	routenum = NPC_Util_GetNumFromStrWithDelim( argstr, "routenum");//行駛路線數量

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
    
    /* 蜇箕及凜棉毛本永玄 */
    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);

    for( i = 0; i < CHAR_PARTYMAX; i ++) {
    	CHAR_setWorkInt( meindex, CHAR_WORKPARTYINDEX1 + i, -1);
    }
	
	/* 伙□玄瑁爛允月 */
{
	int rev;
	int r = CHAR_getWorkInt( meindex, NPC_WORK_ROUTEMAX);
	CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, RAND( 1, r));
	//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));

	/*   欠旦正□玄 */
	rev = NPC_Util_GetNumFromStrWithDelim( argstr, "reverse");

	if( rev == 1 ) {
		int num = NPC_BusGetRoutePointNum( meindex, argstr);

		if( num <= 0 ) {
			print( "npcbus:Very strange！\n");
			return FALSE;
		}
		CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, num-1);
		CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 1);
	}
	/* 伙□玄毛本永玄允月 */
	NPC_BusSetPoint( meindex, argstr);
	/* 墊五燮毛  憎允月 */
	NPC_BusSetDestPoint( meindex, argstr);
}

    return TRUE;
}


/*********************************
*   仄井仃日木凶凜及質  
*********************************/
void NPC_BusTalked( int meindex , int talkerindex , char *szMes ,
                     int color )
{
    int i;
    int	partyflg = FALSE;
	
    /* 皿伊奶乩□卞覆仄化分仃  殺允月 */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }
	/* 憤坌及由□  奴  曇菸  井升丹井譬屯月 */
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
			if( strstr( szMes, "出發" )  ||
				strstr( szMes, "出發" )  ||
				strstr( szMes, "Go" )  ||
				strstr( szMes, "go" ))
			{
				CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
				
				/* 伙□皿楮醒及裟請仄毛洶仁簧蘸卞允月 */
	 			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
				/* SE   日允  穴件乒旦及陲太   */
				if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
					CHAR_sendSEoArroundCharacter( 
									CHAR_getInt( meindex, CHAR_FLOOR),
									CHAR_getInt( meindex, CHAR_X),
									CHAR_getInt( meindex, CHAR_Y),
									60,
									TRUE);
				}
				/* 請  允月凜及丟永本□斥*/
				for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
					int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
					if( CHAR_CHECKINDEX( partyindex)) {
						NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_START);
					}
				}
			}
		}
#if 0
		else if( strstr( szMes, "停止" )  ||
			strstr( szMes, "停止" )  ||
			strstr( szMes, "stop" )  ||
			strstr( szMes, "Stop" ))
		{
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,2);

			/* 伙□皿楮醒及奶件正□田伙毛聶仁允月  */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
						NPC_BUS_WAITINGMODE_WAITTIME);
		    /* 蜇箕及凜棉毛本永玄 */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
		}
		else if( strstr( szMes, NPC_BUS_DEBUGROUTINTG )) {
			/* 犯田永弘迕 */
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
			/* 伙□玄毛本永玄允月 */
			NPC_BusSetPoint( meindex, argstr);
		}
#endif
	}
}
/**************************************
 * 伙□皿楮醒
 **************************************/
void NPC_BusLoop( int meindex)
{
	int	i;
	switch( CHAR_getWorkInt( meindex, NPC_WORK_MODE )) {
	  case 0:
	    /* 謹切乒□玉及凜〕凜棉毛民尼永弁允月 */
		/* 凜棉互煩勻凶及匹〕請  允月 */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) 
			+ CHAR_getWorkInt( meindex, NPC_WORK_WAITTIME) 
			< NowTime.tv_sec)
		{
			/* SE   日允  穴件乒旦及陲太   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
				CHAR_sendSEoArroundCharacter( 
								CHAR_getInt( meindex, CHAR_FLOOR),
								CHAR_getInt( meindex, CHAR_X),
								CHAR_getInt( meindex, CHAR_Y),
								60,
								TRUE);
			}
			/* 請  允月凜及丟永本□斥*/
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
				if( CHAR_CHECKINDEX( partyindex)) {
					NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_START);
				}
			}
			
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
			/* 伙□皿楮醒及裟請仄毛洶仁簧蘸卞允月 */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
		}
		return;
	  case 1:
	  	/* 洶仁 */
	  	NPC_Bus_walk( meindex);
	  case 2:
		/* 鞅引勻化中月乒□玉 */
		/* 凜棉互煩勻凶及匹〕請  允月 */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) 
			+ (CHAR_getWorkInt( meindex, NPC_WORK_WAITTIME) /3)
			< NowTime.tv_sec)
		{
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
			/* 伙□皿楮醒及裟請仄毛洶仁簧蘸卞允月 */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
		
		}
		return;
	  case 3:
		/* 讒邋仄化手〕弁仿奶失件玄及  樟謹切及啃卞〕
		 * 劑仄仇仇匹它尼奶玄毛中木化支月
		 */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) + 3
			< NowTime.tv_sec)
		{
			char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
			char *argstr;

			argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
			/* 伙□皿楮醒及裟請仄毛贊仁允月 */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
						NPC_BUS_WAITINGMODE_WAITTIME);
			
			/* 伙□玄瑁爛允月 */
			{
				int r = CHAR_getWorkInt( meindex, NPC_WORK_ROUTEMAX);
				CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, RAND( 1, r));
				//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
			}
			/* 墊五窖曰白仿弘  凳 */
			CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 
							CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP)^1);

			/* 戚禾奶件玄及譬濡 */
			/* 窖曰反  潰質   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP) == 1)  {
				/* 公及伙□玄及  嫖禾奶件玄醒毛  月 */
				int num = NPC_BusGetRoutePointNum( meindex, argstr);
				CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, num-1);
			}
			else {
				CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 
							CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT) +1);
			}
			/* 戚及禾奶件玄及甄  毛本永玄允月 */
			NPC_BusSetPoint( meindex, argstr);
			/* 墊五燮毛  憎允月 */
			NPC_BusSetDestPoint( meindex, argstr);
			/* 由□  奴  仃月質  毛允月 */
			CHAR_DischargeParty( meindex, 0);
		    /* 蜇箕及凜棉毛本永玄 */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
			/* 乒□玉弁伉失 */
			CHAR_setWorkInt( meindex, NPC_WORK_MODE, 0);
		}
		return;
	  default:
	    break;
	}
}
/**************************************
 * 洶仁〔
 **************************************/
static void NPC_Bus_walk( int meindex)
{	
	POINT	start, end;
	int dir;
	int ret;
	int i;

	/* 洶仁楮溢 */
	/* 讒邋仄凶凜及質   */
	start.x = CHAR_getInt( meindex, CHAR_X);
	start.y = CHAR_getInt( meindex, CHAR_Y);
	end.x = CHAR_getWorkInt( meindex, NPC_WORK_ROUTETOX);
	end.y = CHAR_getWorkInt( meindex, NPC_WORK_ROUTETOY);
	
	/* 讒邋仄凶及匹戚及禾奶件玄卞 */
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
			/*     卞讒邋*/
			/* 謹切乒□玉卞允月 */
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,3);
			
			/* SE   日允  穴件乒旦及陲太   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
				CHAR_sendSEoArroundCharacter( 
								CHAR_getInt( meindex, CHAR_FLOOR),
								CHAR_getInt( meindex, CHAR_X),
								CHAR_getInt( meindex, CHAR_Y),
								60,
								TRUE);
			}
			/* 邋中凶凜及丟永本□斥*/
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
				if( CHAR_CHECKINDEX( partyindex)) {
					NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_END);
				}
			}
		    /* 蜇箕及凜棉毛本永玄 */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
			return;
		}
		else {
			return;
		}
	}
	/*-------------------------------------------------------*/
	/* 洶井六月質   */
	
	/*   輊毛菲戶月 */
	dir = NPC_Util_getDirFromTwoPoint( &start,&end );

	/* 漆中月樺赭及謹    由□  奴洶五匹銀丹   */
	end.x = CHAR_getInt( meindex, CHAR_X);
	end.y = CHAR_getInt( meindex, CHAR_Y);
	
	/* 夫勻井井勻凶凜及啃及質   */
	for( i = 0; i < 100; i ++ ) {	
		if( dir < 0 ) {
			dir = RAND( 0,7);
		}	
		dir = NPC_Util_SuberiWalk( meindex, dir);
		if( dir >= 0 && dir <= 7) break;
	}
	
	if( dir >= 0 && dir <= 7 ) {
		/* 洶仁 */
		ret = CHAR_walk( meindex, dir, 0);

		if( ret == CHAR_WALKSUCCESSED ) {
			/* 憤坌互褪卅日醮棉毛洶井六月 */
			int	i;
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int toindex = CHAR_getWorkInt( meindex, i + CHAR_WORKPARTYINDEX1);
				if( CHAR_CHECKINDEX(toindex) ) {
					int		parent_dir;
					/* 閡及匏  午〕褪及洶五蟆及匏  井日  輊毛菲戶月 */
					/* 洶仁 */
					start.x = CHAR_getInt( toindex, CHAR_X);
					start.y = CHAR_getInt( toindex, CHAR_Y);
					parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end );
					/* 弘仿犯奴它旦左皿撲亦件洶五毛  蜇允月啃卞〕
					 * 戚及閡反蟆及閡及  毛馨丹方丹卞允月
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
 * 戚及樺赭毛本永玄允月
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
 * route  寞井日〕  蟆互丐勻凶日公木毛
 * 憊寞及午仇卞本永玄允月〔
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
 * 隙爛今木凶失奶  丞毛  勻化中月井民尼永弁允月
 *   勻化中凶日分戶
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
 * 隙爛今木凶失奶  丞毛  勻化中月井民尼永弁允月
 *   勻化中卅中午分戶
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
 * 隙爛今木凶伊矛伙動曉井民尼永弁允月
 **************************************/
static BOOL NPC_BusCheckLevel( int meindex, int charaindex, char *argstr)
{
	int		level;
	
	/* 卅仃木壬中仃卅中婁醒及民尼永弁 */
	level = NPC_Util_GetNumFromStrWithDelim( argstr, "needlevel");
	if( level == -1 ) {
		return TRUE;
	}
	if( CHAR_getInt( charaindex, CHAR_LV) >= level ) return TRUE;
	
	return FALSE;
}
/**************************************
 * 豢嗯毛民尼永弁允月
 * -1 蟯   0動曉」    〕井勾  邰Stone
 **************************************/
static int NPC_BusCheckStone( int meindex, int charaindex, char *argstr)
{
	int		gold;
	
	/* 卅仃木壬中仃卅中婁醒及民尼永弁 */
	gold = NPC_Util_GetNumFromStrWithDelim( argstr, "needstone");
	if( gold == -1 ) {
		return 0;
	}
	if( CHAR_getInt( charaindex, CHAR_GOLD) >= gold ) return gold;
	
	return -1;
}
/**************************************
 * 丟永本□斥毛霜月
 * 婁醒及丟永本□斥互卅仃木壬犯白巧伙玄丟永本□斥毛霜月
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
 * 伙□玄  □皮伙及禾奶件玄及醒毛潸  允月
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
	/* ㄠ弘伉永玉動  及心 */
	if( !NPC_Util_charIsInFrontOfChar( charaindex, meindex, 1 )) return FALSE; 
	/*     曇樂反蛐  允月 */
	if( CHAR_getWorkInt( meindex, NPC_WORK_MODE) != 0 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_GETTINGON);
		return FALSE;
	}
	/* 天□化不分勻凶日分戶 */
	if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE ) != CHAR_PARTY_NONE) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_NOTPARTY);
		return FALSE;
	}
	/* 由□  奴及諦醒毛民尼永弁允月 */
	if( CHAR_getEmptyPartyArray( meindex) == -1 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_OVERPARTY);
		return FALSE;
	}
	/* 失奶  丞及民尼永弁毛允月(嗟鞅失奶  丞) */
	if( !NPC_BusCheckDeniedItem( meindex, charaindex, argstr)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_DENIEDITEM);
		return FALSE;
	}
#ifdef _ITEM_CHECKWARES
	if( CHAR_CheckInItemForWares( charaindex, 0) == FALSE )	{
		CHAR_talkToCli( charaindex, -1, "無法攜帶貨物上車。", CHAR_COLORYELLOW);
		return FALSE;
	}
#endif
	/* 失奶  丞及民尼永弁毛允月(  邰失奶  丞) */
	if( !NPC_BusCheckAllowItem( meindex, charaindex, FALSE)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_ALLOWITEM);
		return FALSE;
	}
	/* 伊矛伙及民尼永弁毛允月 */
	if( !NPC_BusCheckLevel( meindex, charaindex, argstr)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_LEVEL);
		return FALSE;
	}
	/* 奶矛件玄  井民尼永弁允月 */
//	if( CHAR_getInt( charaindex, CHAR_NOWEVENT) != 0 ||
//		CHAR_getInt( charaindex, CHAR_NOWEVENT2) != 0 ||
//		CHAR_getInt( charaindex, CHAR_NOWEVENT3) != 0 )
//	{
//		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_EVENT);
//		return FALSE;
//	}
	/* 豢嗯及民尼永弁毛允月  雲嗯毛潸月及匹〕  蔽民尼永弁卞允月仇午〞   */
	ret = NPC_BusCheckStone( meindex, charaindex, argstr);
	if( ret == -1 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_GOLD);
		return FALSE;
	}
	if( ret != 0 ) {
		char msgbuf[128];
		/* 豢嗯毛午月 */
		CHAR_setInt( charaindex, CHAR_GOLD, 
					CHAR_getInt( charaindex, CHAR_GOLD) - ret);
		/* 霜耨 */
		CHAR_send_P_StatusString( charaindex, CHAR_P_STRING_GOLD);
		snprintf( msgbuf, sizeof( msgbuf), "支付了%d Stone！", ret);
		CHAR_talkToCli( charaindex, -1, msgbuf, CHAR_COLORYELLOW);
	}
	/* 由□  奴卞  月 */
	//CHAR_JoinParty_Main( charaindex, meindex);
	
	//fd = getfdFromCharaIndex( charaindex );
	
	//lssproto_PR_send( fd, 1, 1);
	return TRUE;
}
