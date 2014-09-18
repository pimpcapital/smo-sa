#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "net.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_exchangeman.h"
#include "npc_eventaction.h"
#include "log.h"
#include "battle.h"
#include "handletime.h"
#include "enemy.h"
#include "npc_warp.h"

static void NPC_ExChangeMan_selectWindow( int meindex, int talker,int num);
BOOL NPC_TypeCheck(int meindex,int talker,char *szMes);
int NPC_ExChangeManEventCheck( int meindex, int talker, char *buff1);
BOOL NPC_EventItemCheck(int meindex,int talker,int itemNo,int flg);
BOOL NPC_EventLevelCheck(int meindex,int talker,int level,int flg);
// Arminius 8.14 move to .h (for pet talk)
//BOOL NPC_EventBigSmallCheck(int meindex,int talker,char* buf);
BOOL NPC_EventFreeIfCheck(int meindex,int talker,char* buf,int kosuu,int flg);
BOOL NPC_EventBigSmallLastCheck(int point1,int mypoint,int flg);
BOOL NPC_ENDEventNoCheck(int meindex,int talker,int shiftbit,int flg);
BOOL NPC_NOWEventNoCheck(int meindex,int talker,int shiftbit,int flg);
BOOL NPC_TiemCheck(int meindex,int talker,int time,int flg);
BOOL NPC_EventAddPet(int meindex, int talker, char *buff2,int mode);
BOOL NPC_RandItemGet(int meindex, int talker,int rand_j,char *buf);

#ifdef _JZ_TRANSX
BOOL NPC_EventTrans(int meindex, int talker, char *buff2);
#endif

BOOL NPC_PetLvCheckType2(int petindex,int meindex,int talker,char *buf,int mode);
BOOL NPC_PetLvCheck(int meindex,int talker,char *buf,int mode);
void NPC_RequestMain(int meindex,int talker,char *buf);
void NPC_AcceptMain(int meindex,int  talker ,char*buf);
void NPC_MsgDisp(int meindex,int talker,int num);
BOOL NPC_EventAdd(int meindex,int talker,int mode);
BOOL NPC_AcceptDel(int meindex,int talker,int mode);
BOOL NPC_SavePointCheck(int meindex,int talker,int shiftbit,int flg);

BOOL NPC_EventReduce(int meindex,int talker,char *buf);
BOOL NPC_EventDelItem(int meindex,int talker,char *buf,int breakflg);
BOOL NPC_EventDelItemEVDEL(int meindex,int talker,char *buf,char *nbuf,int breakflg);
BOOL NPC_EventAddItem(int meindex,int talker,char *buf);
int NPC_EventFile(int meindex,int talker,char *buf);
BOOL NPC_EventDelPet(int  meindex,int  talker, int petsel);
void NPC_PetSkillMakeStr(int meindex,int toindex,int select);
void NPC_CleanMain(int meindex,int talker,char *buf);	
void NPC_EventPetSkill( int meindex, int talker, char *data);
BOOL NPC_ItemFullCheck(int meindex,int talker,char *buf,int mode,int evcnt);
void NPC_CharmStatus(int meindex,int talker);
BOOL NPC_ImageCheck(int meindex,int talker,int kosuu,int flg);
int NPC_EventGetCost(int meindex,int talker,char *arg);

BOOL NPC_EventWarpNpc(int meindex,char *buf);


enum {
	CHAR_WORK_EVENTWARP	= CHAR_NPCWORKINT4,

};

/*********************************
* 賡渝質  
*********************************/
BOOL NPC_ExChangeManInit( int meindex )
{

	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[1024*2];
	int i = 1;
	char buf4[256];

	CHAR_setWorkInt(meindex,CHAR_WORK_EVENTWARP,1);
	
#if 0
	char buf2[512];
	char buf3[256];
	int j=1;
	int nameflg=0;
	int itemno;
	char *ret;
	char sendbuf[2][10]={"無法傳送。","可以傳送。"};
	char droplogbuf[2][10]={"沒消失。","消失了。"};
	int flg=0;
#endif			


	/*--NPC及正奶皿毛本永玄允月--*/
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEEVENT );


	/*--澀爛白央奶伙毛  心  心--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1))) == NULL) {
		print("NPC_ExChange.c TypeCheck: GetArgStrErr\n");
		print("NPCName=%s\n", CHAR_getChar( meindex ,CHAR_NAME));
		return FALSE;
	}

	i = 1;
	/*--奶矛件玄NPC互健丹失奶  丞毛民尼永弁允月--*/
	/*--民尼永弁嫩  ｝  午仄化壅尹月井〞｝矢永玄丟□伙匹霜木月井〞--*/
	while(getStringFromIndexWithDelim( argstr ,"EventEnd" ,i ,buf ,sizeof(buf))
	!= FALSE)
	{
		i++;
		/*--仇仇匹踏井木化中月及反｝伐□瓦件弘毛請允-*/
		if(strstr( buf,"GetItem") != NULL) {
			if(strstr( buf,"ItemFullMsg") == NULL) {
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### EventNo=%d %s:ItemFullMsg 脫離###",
				atoi( buf4),
				CHAR_getChar( meindex, CHAR_NAME));
			}
		}

		if(strstr( buf,"GetPet") != NULL) {
			if(strstr( buf,"PetFullMsg") == NULL) {
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### ventNo=%d %s:PetFullMsg 脫離 ###",
				atoi(buf4),
				CHAR_getChar(meindex,CHAR_NAME));
			}
		}

		if(strstr( buf,"GetEgg") != NULL) {
			if(strstr( buf,"PetFullMsg") == NULL) {
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### ventNo=%d %s:PetFullMsg 脫離 ###",
				atoi(buf4),
				CHAR_getChar(meindex,CHAR_NAME));
			}
		}

		if(strstr( buf,"GetStone") != NULL){
			if(strstr( buf,"StoneFullMsg") == NULL){
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### EventNo=%d %s:StonFullMsg 脫離 ###",
				atoi( buf4),
				CHAR_getChar( meindex, CHAR_NAME));
			}
		}

		if(strstr( buf,"DelStone") != NULL) {
			if(strstr( buf,"StoneLessMsg") == NULL) {
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### EventNo=%d %s:StoneLessMsg 脫離 ###",
				atoi(buf4),
				CHAR_getChar(meindex,CHAR_NAME));
			}
		}
	}


	i = 1;

#if 0
	/*--奶矛件玄NPC互健丹失奶  丞毛民尼永弁允月--*/
	/*--民尼永弁嫩  ｝  午仄化壅尹月井〞｝矢永玄丟□伙匹霜木月井〞--*/
	while(getStringFromIndexWithDelim( argstr ,"EventEnd" ,i ,buf ,sizeof(buf))
	!=FALSE)
	{
		i++;
		/*--仇仇匹踏井木化中月及反｝伐□瓦件弘毛請允-*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"GetItem", buf2,sizeof( buf2)) 
		!=NULL)
		{
			j=1;
			flg=0;
			while(getStringFromIndexWithDelim( buf2,",", j, buf3, sizeof(buf3))
			!=FALSE)
			{
				j++;
				if(strstr( buf2, "*")!=NULL){
					getStringFromIndexWithDelim( buf3 ,"*" ,1, buf4 ,sizeof(buf4));
					itemno = atoi(buf4);
				}else{
					/*--失奶  丞瓜件田□必永玄--*/
					itemno = atoi(buf3);
				}
				
				ret=NPC_Util_GetStrFromStrWithDelim( buf,"NoWarning", buf4,sizeof(buf4));
				if(ret != NULL){
					char buff[8];
					int k=1;

					while(getStringFromIndexWithDelim( buf4,"," ,k ,buff, sizeof(buff))
					!=FALSE){
						k++;
						if(itemno == atoi(buff))
						{
							flg=1;
							break;
						}
					}
				}
				
				if(flg == 0){
					if( (itemno >=2400 && itemno < 3000) &&(
					(ITEM_getdropatlogoutFromITEMtabl(itemno) == 1)
					 || (ITEM_getvanishatdropFromITEMtabl(itemno) == 1)
					 || (ITEM_getcanpetmailFromITEMtabl(itemno) == 0))
					 ){
						if(nameflg==0){
							print("\n%s",CHAR_getChar( meindex ,CHAR_NAME));
							nameflg=1;
						}
						print("\nID:%-5d,Logout:%-8s,DropCls:%-8s,SendMail:%-8s,%-16s",
						itemno,
						droplogbuf[ITEM_getdropatlogoutFromITEMtabl(itemno)],
						droplogbuf[ITEM_getvanishatdropFromITEMtabl(itemno)],
						sendbuf[ITEM_getcanpetmailFromITEMtabl(itemno)],
						ITEM_getNameFromNumber(itemno)
						);
					}
				}
				
			}
		}
	
	}
#endif
	return TRUE;

}

/*--      及啖  --*/
BOOL NPC_EventWarpNpc(int meindex,char *buf)
{
	char token[32];
	char buf2[32];
	int floor,x,y;
	int ret;
	int cnt;
	int dcnt = 1;

	/*--蜇箕及伐□皿及賜  --*/
	cnt = CHAR_getWorkInt(meindex,CHAR_WORK_EVENTWARP);
	while(getStringFromIndexWithDelim(buf,",", dcnt,buf2,sizeof(buf2) )
	 !=FALSE ){
		dcnt++;
	}
	
	if(dcnt <= cnt) cnt=1;


	while(getStringFromIndexWithDelim(buf,",", cnt,buf2,sizeof(buf2)) !=FALSE )
	{
		cnt++;
	    ret=getStringFromIndexWithDelim(buf2,".", 1,token,sizeof(token));
        if( ret == FALSE ) {
        	 continue;
       	}
        floor = atoi( token );

        ret=getStringFromIndexWithDelim(buf2,".", 2,token,sizeof(token));
        if( ret == FALSE ){
          continue;
        }
        x = atoi( token );

        ret=getStringFromIndexWithDelim(buf2,".", 3,token,sizeof(token));
        if( ret == FALSE ) {
         continue;
        }
        y = atoi( token );

		CHAR_setWorkInt(meindex,CHAR_WORK_EVENTWARP, cnt);
        CHAR_warpToSpecificPoint(meindex, floor, x, y);
		return TRUE;
	}
	
	return FALSE;

}


/*********************************
*   仄井仃日木凶凜及質  
*********************************/
void NPC_ExChangeManTalked( int meindex , int talkerindex , char *szMes ,int color )
{
	/* 皿伊奶乩□卞覆仄化分仃  殺允月 */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	/*--  及蟆卞中月井升丹井〞--*/
	if(NPC_Util_isFaceToFace( meindex ,talkerindex ,2) == FALSE) {
		/* ㄠ弘伉永玉動  及心 */
		if(NPC_Util_isFaceToChara(talkerindex,meindex,1 ) == FALSE) return;
	}
	
	/*--皿伊奶乩□及伐□弁    毛賡渝祭--*/
	CHAR_setWorkInt( talkerindex ,CHAR_WORKSHOPRELEVANT, 0);
	CHAR_setWorkInt( talkerindex ,CHAR_WORKSHOPRELEVANTSEC, 0);
	CHAR_setWorkInt( talkerindex ,CHAR_WORKSHOPRELEVANTTRD, 0);

	/*--正奶皿  卞坌仃月--*/
	NPC_TypeCheck( meindex, talkerindex, szMes);

}

/*------------------------------------------------------------
 *澀爛白央奶伙毛  心  心｝橢瘀互丐勻凶正奶皿及質  毛允月
 *-------------------------------------------------------------*/
BOOL NPC_TypeCheck(int meindex,int talker,char *szMes)
{

	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[1024*2];
	char buf2[512];
	int i = 1;
	int EvNo;
	int evch;
	
	/*--澀爛白央奶伙毛  心  心--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
		print("NPC_ExChange.c TypeCheck: GetArgStrErr\n");
		print("NPCName=%s\n", CHAR_getChar(meindex,CHAR_NAME));
		return FALSE;
	}

	/*-- 澀爛白央奶伙毛坌喃允月 --*/
	while(getStringFromIndexWithDelim( argstr,"EventEnd",i,buf,sizeof( buf))
	!= FALSE){
		i++;
		CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANT, 0);
		CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);

		/*-- 奶矛件玄瓜件田□毛必永玄 --*/
		if(NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",
								buf2, sizeof( buf2) ) == NULL) continue;

		if(strstr( buf2, "-") != 0) {
			EvNo = -1;
		}else{
			EvNo = atoi( buf2);
		}

		/*-- 白仿弘民尼永弁毛墊丹〔允匹卞奶矛件玄互蔽  仄化中月樺寧反戚尺 --*/
		if(NPC_EventCheckFlg( talker, EvNo) == TRUE) continue;

		/*--平□伐□玉及民尼永弁毛墊丹--*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"KeyWord", buf2,sizeof( buf2) ) 
		 != NULL){
			if(strcmp( szMes, buf2) != 0) continue;
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANT, i);
		}

		/*--矢永玄及  蟆及民尼永弁互  勻化月井毛民尼永弁--*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"Pet_Name", buf2,sizeof( buf2)) != NULL){
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 1);
		}
		evch = NPC_ExChangeManEventCheck( meindex, talker, buf);
		if(evch != -1) {
			if(NPC_Util_GetStrFromStrWithDelim( buf,"TYPE", buf2, sizeof( buf2)) != NULL){
				
				if( strstr( buf2, "REQUEST") != NULL) {
					NPC_RequestMain( meindex, talker, buf);
					return TRUE;
				}else if( strstr( buf2, "ACCEPT") != NULL) {
					/*-匙  毛熬仃午月  --*/
					NPC_AcceptMain( meindex, talker, buf);
					return TRUE;
					
				}else if(strstr( buf2, "MESSAGE") != NULL) {
					char token[14*100];
					char token2[14*50];
					if(NPC_Util_GetStrFromStrWithDelim( buf,"NomalMsg",	token,sizeof( token) )
						!= NULL ) {
						if(NPC_AcceptDel( meindex, talker, 0) == FALSE) return FALSE;
						if(EvNo != -1)
							NPC_NowEventSetFlg( talker, EvNo);
						CHAR_talkToCli( talker, meindex, token, CHAR_COLORWHITE);
						if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp", token2,sizeof( token2) )
							!= NULL){
							NPC_EventWarpNpc(meindex,token2);
						}
						if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", buf2, sizeof( buf2) )
							!=NULL){
							char buf3[16];
							int k = 1;
							if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
							while(getStringFromIndexWithDelim(buf2 , "," ,k, buf3, sizeof(buf3))
								!=FALSE ){	
								k++;
								NPC_EventSetFlg( talker, atoi( buf3));
							}
						}
						if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
							buf2, sizeof( buf2)) != NULL) {
							char buf3[16];
							int k = 1;
							k= 1 ;
							while(getStringFromIndexWithDelim(buf2 , "," , k, 
								buf3, sizeof(buf3))
								!= FALSE ){
								NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
								k++;
							}
						}
#ifdef _ANGEL_SUMMON
						// 完成召喚任務
						if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONOVER",
							buf2, sizeof( buf2)) != NULL)
						{
							int mindex;
							int mission;
							char nameinfo[64];
							
							mission = atoi( buf2);
							mindex = checkIfAngel( talker);
							
							if( mission == missiontable[mindex].mission )
							{
								char msg[1024];
								
								print(" ====完成召喚任務==== ");

								CHAR_setInt( talker, CHAR_HEROCNT, 
									CHAR_getInt( talker, CHAR_HEROCNT)+1 );
								getMissionNameInfo( talker, nameinfo);
								saacproto_ACMissionTable_send( acfd, MISSION_HERO_COMPLETE, 4, nameinfo, "");
								sprintf( msg, " 完成任務 i:%d m:%d %s ", mindex, mission, nameinfo);
								print( msg);
								LogAngel( msg );
							}
							
						}
						
						// 清除召喚任務
						if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONCLEAN",
							buf2, sizeof( buf2)) != NULL)
						{
							int mindex;
							int mission;
							char nameinfo[64];
							
							mission = atoi( buf2);
							mindex = checkIfAngel( talker);
							
							if( mission == missiontable[mindex].mission )
							{
								char msg[1024];

								getMissionNameInfo( talker, nameinfo);
								saacproto_ACMissionTable_send( acfd, mindex, 3, nameinfo, "");

								sprintf( msg, " 放棄任務 i:%d m:%d %s ", mindex, mission, nameinfo);
								print( msg);
								LogAngel( msg );
							}
							
						}
#endif

					}else if(NPC_Util_GetStrFromStrWithDelim(buf, "NomalWindowMsg",
						token, sizeof( token) )
						!=NULL) {
						/*--它奶件玉它匹丟永本□斥毛忒允--*/
						int fd = getfdFromCharaIndex( talker);
#ifdef _NEWEVENT
						if(NPC_Util_GetStrFromStrWithDelim(buf, "NomalWindowMsg1",
							buf2, sizeof(buf2)) != NULL){
							lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
						}else {
#endif
							char token2[14*50];
							if(NPC_AcceptDel( meindex, talker,0) == FALSE) return FALSE;
							if( EvNo != -1) NPC_NowEventSetFlg( talker, EvNo);
							
							if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp",
								token2,sizeof( token2) ) != NULL){
								NPC_EventWarpNpc(meindex,token2);
							}
							if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
								buf2, sizeof( buf2) ) !=NULL){
								char buf3[16];
								int k = 1;
								if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
								while(getStringFromIndexWithDelim(buf2 , "," ,k, 
									buf3, sizeof(buf3)) !=FALSE ){	
									k++;
									NPC_EventSetFlg( talker, atoi( buf3));
								}
							}
							//奶矛件玄白仿弘毛壅允
							if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
								buf2, sizeof( buf2)) != NULL) {
								char buf3[16];
								int k = 1;
								
								k= 1 ;
								while(getStringFromIndexWithDelim(buf2 , "," , k, 
									buf3, sizeof(buf3))	!= FALSE ){
									NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
									k++;
								}
							}
#ifdef _ANGEL_SUMMON
							// 完成召喚任務
							if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONOVER",
								buf2, sizeof( buf2)) != NULL)
							{
								int mindex;
								int mission;
								char nameinfo[64];
								
								mission = atoi( buf2);
								mindex = checkIfAngel( talker);
								
								if( mission == missiontable[mindex].mission )
								{
									char msg[1024];
									
									print(" ====完成召喚任務==== ");

									CHAR_setInt( talker, CHAR_HEROCNT, 
										CHAR_getInt( talker, CHAR_HEROCNT)+1 );
									getMissionNameInfo( talker, nameinfo);
									saacproto_ACMissionTable_send( acfd, MISSION_HERO_COMPLETE, 4, nameinfo, "");
									sprintf( msg, " 完成任務 i:%d m:%d %s ", mindex, mission, nameinfo);
									print( msg);
									LogAngel( msg );
								}
								
							}
							
							// 清除召喚任務
							if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONCLEAN",
								buf2, sizeof( buf2)) != NULL)
							{
								int mindex;
								int mission;
								char nameinfo[64];
								
								mission = atoi( buf2);
								mindex = checkIfAngel( talker);
								
								if( mission == missiontable[mindex].mission )
								{
									print(" ====清除召喚任務==== ");
									
									getMissionNameInfo( talker, nameinfo);
									saacproto_ACMissionTable_send( acfd, mindex, 3, nameinfo, "");
								}
								
							}
#endif
							
							lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_YES,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
#ifdef _NEWEVENT
						}
#endif
					}
					
					return TRUE;
					
				}else if(strstr( buf2, "CLEAN") != NULL) {
					NPC_CleanMain( meindex, talker, buf);
					return TRUE;
				}
			}
		}
	}

	strcpy( buf, "\0");
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "NomalMainMsg", buf, sizeof( buf)) == NULL )	{
		print("\n err:NOT FIND NomalMainMsg");
		return FALSE;
	}
	i = 1;
	while(getStringFromIndexWithDelim( buf, ",", i, buf2, sizeof( buf2))!= FALSE){
		i++;
	}
	i--;
	i = rand()%i + 1;
	getStringFromIndexWithDelim( buf,",", i, buf2, sizeof( buf2));
	CHAR_talkToCli( talker, meindex, buf2,  CHAR_COLORWHITE);
	return TRUE;
}


/*-----------------------------------
 *正奶皿  卞質  毛坌仃月
 ------------------------------------*/
static void NPC_ExChangeMan_selectWindow( int meindex, int talker,int num)
{

	switch( num) {
		/*--伉弁巨旦玄滇--*/
		  case 0:
		  /*-奶矛件玄  卅及匹｝奶矛件玄毛鞅戶月井升丹井毛  丹--*/
		  	NPC_MsgDisp( meindex, talker, 0);
			break;

		  case 1:
		  /*--蕙筋卞奶矛件玄墊丹--*/
	  		NPC_MsgDisp( meindex, talker, 5);
		  	break;

			/*--失弁本皿玄滇--*/
		  case 2:
			/*--熬仃潸曰--*/
			NPC_MsgDisp( meindex, talker, 6);
			break;

		  case 3:
			/*--熬仃潸曰--*/
			NPC_MsgDisp( meindex, talker, 11);
			break;
	}

}

/*-------------------------------------
 *奶矛件玄毛匙  允月質  及丟奶件
 --------------------------------------*/
void NPC_RequestMain(int meindex,int talker,char *buf)
{
	int shiftbit;
	char buf2[128];
		
	/*--奶矛件玄瓜件田□毛必永玄--*/
	NPC_Util_GetStrFromStrWithDelim( buf,"EventNo", buf2,sizeof( buf2) );
	shiftbit = atoi( buf2);

	if(NPC_NowEventCheckFlg( talker, shiftbit) == TRUE) {
		NPC_ExChangeMan_selectWindow( meindex, talker, 0);
	}else{
		NPC_ExChangeMan_selectWindow( meindex, talker, 1);
	}
}

/*---------------------------------------
 * 奶矛件玄蔽  及丟奶件
 ----------------------------------------*/
void NPC_AcceptMain(int meindex,int  talker ,char *buf)
{

	NPC_ExChangeMan_selectWindow( meindex, talker, 2);
}

/*----------------------------------------
 *  白仿弘毛弁伉失允月啃及丟奶件
 ----------------------------------------*/
void NPC_CleanMain(int meindex,int talker,char *buf)
{
	NPC_ExChangeMan_selectWindow( meindex, talker, 3);
}

/*-----------------------------------------
弁仿奶失件玄井日忒勻化五凶凜卞裟太請今木月〔
-------------------------------------------*/
void NPC_ExChangeManWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	/*--       及引歹曰卞中卅中午五反蔽   --*/
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) {
		return;
	}


	switch( seqno) {

		/*--白仿弘毛壅允凜及它奶件玉它--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_CLEANMSG:
		if( select == WINDOW_BUTTONTYPE_YES) {
			NPC_MsgDisp( meindex, talkerindex, 12);
		}else if(select == WINDOW_BUTTONTYPE_NO) {
		}
		break;

#ifdef _NEWEVENT
	  case CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG:
		if(select == WINDOW_BUTTONTYPE_NEXT) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD) + 1);
			NPC_MsgDisp( meindex, talkerindex, 99);
		}else if(select == WINDOW_BUTTONTYPE_PREV) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD) - 1);
			NPC_MsgDisp( meindex, talkerindex, 99);
		}
		break;
#else
	  case CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG:
		break;
#endif
		/*--伉弁巨旦玄及丐曰互午丹丟永本□斥--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK:
			if(select == WINDOW_BUTTONTYPE_NEXT) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
				NPC_MsgDisp( meindex, talkerindex, 22);
			}else if(select == WINDOW_BUTTONTYPE_PREV) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
				NPC_MsgDisp( meindex, talkerindex, 22);
			}
		break;
		
		/*--失弁本皿玄及丐曰互午丹丟永本□斥--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK:
			if(select == WINDOW_BUTTONTYPE_NEXT) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
				NPC_MsgDisp( meindex, talkerindex, 82);
			}else if(select == WINDOW_BUTTONTYPE_PREV) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
				NPC_MsgDisp( meindex, talkerindex, 82);
			}
		break;

		/*--蜇箕及奶矛件玄毛鞅戶月井--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_NOWEVENT:
		if(select == WINDOW_BUTTONTYPE_YES) {
			NPC_MsgDisp( meindex, talkerindex, 7);
		}else if(select == WINDOW_BUTTONTYPE_NO){
			NPC_MsgDisp( meindex, talkerindex, 1);
		}
		break;
		
		/*--伉弁巨旦玄及丟奶件它奶件玉它--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG:
		if(select == WINDOW_BUTTONTYPE_YES) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,0);
			NPC_MsgDisp( meindex, talkerindex, 2);
		}else if(select == WINDOW_BUTTONTYPE_NO) {
		
		}else if(select == WINDOW_BUTTONTYPE_NEXT) {
			 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
						CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
			NPC_MsgDisp( meindex, talkerindex, 55);
		}else if(select == WINDOW_BUTTONTYPE_PREV) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
			NPC_MsgDisp( meindex, talkerindex, 55);
		}
		break;

		/*--熬仃潸曰及丟奶件及它奶件玉它--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG:
		if(select == WINDOW_BUTTONTYPE_YES) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,0);
#ifdef _NEWEVENT
			NPC_MsgDisp( meindex, talkerindex, 81);
#else
			NPC_MsgDisp( meindex, talkerindex, 8);
#endif
		}else if(select == WINDOW_BUTTONTYPE_NO) {
		}else if(select == WINDOW_BUTTONTYPE_NEXT) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
			NPC_MsgDisp( meindex, talkerindex, 66);
		}else if(select == WINDOW_BUTTONTYPE_PREV) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
			NPC_MsgDisp( meindex, talkerindex, 66);
		}
		break;

#ifdef _NEWEVENT
	  case CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW:
		  if(select == WINDOW_BUTTONTYPE_OK){
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,0);
				NPC_MsgDisp( meindex, talkerindex, 8);
		  }else if(select == WINDOW_BUTTONTYPE_NEXT) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
				NPC_MsgDisp( meindex, talkerindex, 81);
		  }else if(select == WINDOW_BUTTONTYPE_PREV) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
				NPC_MsgDisp( meindex, talkerindex, 81);
		  }
		  break;
#endif
		/*--矢永玄及  毛芨尹月它奶件玉它--*/
	  case CHAR_WINDOWTYPE_WINDOWPETSKILLSHOP:
			if(CHAR_getWorkInt( talkerindex ,CHAR_WORKSHOPRELEVANTTRD) ==0) {
				NPC_EventPetSkill( meindex, talkerindex, data);
				NPC_MsgDisp( meindex, talkerindex, 25);
			}else{
				NPC_EventPetSkill( meindex, talkerindex, data);
				NPC_MsgDisp( meindex, talkerindex, 88);
			}
		break;
	}
}


/*-------------------------
 *  橢瘀毛心凶仄化中月井
 --------------------------*/
int NPC_ExChangeManEventCheck( int meindex, int talker, char *buff1)
{
	char buff2[512];
	char buff3[128];
	char buf[512];
	int i = 1,j = 1;
	int loop = 0;
	
	NPC_Util_GetStrFromStrWithDelim( buff1, "EVENT", buf, sizeof( buf));

	/*-- EVENt及橢瘀毛賜  卞葦化中仁 --*/
	while( getStringFromIndexWithDelim(buf, ",", i, buff2,sizeof( buff2))
	 !=FALSE )
	{
		i++;
		/*-- "&"互丐月及匹  醒橢瘀 --*/
		if(strstr( buff2, "&")!=NULL){
			j = 1;
			loop = 0;
			while(getStringFromIndexWithDelim(buff2, "&", j, buff3, sizeof( buff3))
			 != FALSE )
			{
				j++;
				if(NPC_EventBigSmallCheck( meindex, talker, buff3) == FALSE)
				{
					loop = 1;
					break;
				}
			}
				
			if(loop == 0){
				i--;
				return i;
			}
		/*--橢瘀互夫午勾--*/
		}else{
			if(NPC_EventBigSmallCheck( meindex, talker, buff2) == TRUE) {
				i--;
				return i;
			}
		}
	
	}

	return -1;

}

/*---------------------------------
 *   凝筏寞民尼永弁
 --------------------------------*/
BOOL NPC_EventBigSmallCheck(int meindex,int talker, char* buf)
{

	char buff2[128];
	int kosuu;
	
	/*--矢永玄及民尼永弁互反中勻化中月樺寧反域繹PET橢瘀毛民尼永弁允月 --*/
	if(strstr( buf, "PET") != NULL) {
		if(strstr( buf, "EV") != NULL) {
			if(NPC_PetLvCheck( meindex, talker, buf, 1) != FALSE) return TRUE;
		}else{
			if(NPC_PetLvCheck( meindex, talker, buf, 0) != FALSE) return TRUE;
		}
		return FALSE;
	}

	/*--動票反｝  騷及筏寞民尼永弁 --*/
	if(strstr( buf, "<") != NULL) {
		getStringFromIndexWithDelim( buf, "<", 2, buff2, sizeof( buff2));
		kosuu = atoi( buff2);
		getStringFromIndexWithDelim( buf, "<", 1, buff2, sizeof( buff2));

		if(NPC_EventFreeIfCheck( meindex, talker, buff2, kosuu, 1) == TRUE) {
			return TRUE;
		}

	}else if(strstr( buf, ">") != NULL) {
		getStringFromIndexWithDelim( buf, ">", 2, buff2, sizeof(buff2));
		kosuu = atoi(buff2);
		getStringFromIndexWithDelim( buf, ">" ,1, buff2, sizeof(buff2));

		if(NPC_EventFreeIfCheck( meindex, talker, buff2, kosuu, 2)==TRUE) {
			return TRUE;
		}

	}else if(strstr( buf, "!=" ) != NULL) {
		getStringFromIndexWithDelim( buf, "!=", 2, buff2, sizeof( buff2));
		kosuu = atoi( buff2);
		getStringFromIndexWithDelim( buf, "!=", 1, buff2, sizeof( buff2));
		if(NPC_EventFreeIfCheck( meindex, talker, buff2, kosuu, 3) == TRUE) {
			return TRUE;
		}
		
	}else if(strstr( buf, "=") != NULL) {
		getStringFromIndexWithDelim( buf, "=", 2, buff2, sizeof( buff2));
		kosuu = atoi( buff2);
		getStringFromIndexWithDelim( buf, "=", 1, buff2, sizeof( buff2));

		if(strstr( buf, "*") != NULL) {
			if(NPC_EventReduce( meindex, talker, buf) == TRUE){
				return TRUE;
			}
		}else if(NPC_EventFreeIfCheck( meindex, talker, buff2, kosuu, 0)
		== TRUE)
		{
			return TRUE;
		}
	}
	
	return FALSE;

}


/*-----------------------------------
 * 嫩  毛  凶仄化中月井及民尼永弁
 *-----------------------------------*/
BOOL NPC_EventFreeIfCheck(int meindex,int talker,char* buf,int kosuu,int flg)
{

	/*--伊矛伙民尼永弁--*/
	if(strcmp( buf, "LV") == 0) {
		if(NPC_EventLevelCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	/*--失奶  丞民尼永弁--*/
	if(strcmp( buf, "ITEM") == 0) {
		if(NPC_EventItemCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	/*--蔽  奶矛件玄民尼永弁--*/
	if(strcmp( buf, "ENDEV") == 0) {
		if(NPC_ENDEventNoCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	if(strcmp( buf, "NOWEV") == 0) {
		if(NPC_NOWEventNoCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	if(strcmp( buf, "SP" ) == 0) {
		if(NPC_SavePointCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	if(strcmp( buf, "TIME") == 0) {
		if(NPC_TiemCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
	if(strcmp(buf,"CLASS")==0){
		if(NPC_ActionClassCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif

	if(strcmp( buf, "IMAGE") == 0) {
		if(NPC_ImageCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

#ifdef _ANGEL_SUMMON
	if(strcmp(buf,"ANGEL_NOW")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_DOING, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_NOW")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_DOING, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_OVER")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_OVER")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_OUT")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_TIMEOVER, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_OUT")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_TIMEOVER, 0)==TRUE)
				return TRUE;
	}

	if(strcmp(buf,"ANGEL_I_NOW")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_DOING, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_NOW")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_DOING, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_I_OVER")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_OVER")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_I_OUT")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_TIMEOVER, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_OUT")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_TIMEOVER, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp( buf, "HEROCNT") == 0) {
		//if(NPC_EventLevelCheck( meindex, talker, kosuu, flg) == TRUE) {
		if(NPC_HeroCompleteCountCheck( meindex, talker, kosuu, flg) == TRUE)
			return TRUE;
	}

#endif
#ifdef _NPC_EXCHANGEMANTRANS
	//判斷轉生
	{
		//print("\n判斷轉生");
		if(strcmp(buf,"TRANS")==0){
			int mytrans;
			mytrans = CHAR_getInt(talker,CHAR_TRANSMIGRATION);
			if(NPC_EventBigSmallLastCheck( kosuu, mytrans, flg) == TRUE)
				return TRUE;
		}
	}
#endif
#ifdef _PROSK99
	if(strstr( buf, "PROSK" ) != NULL) {//PROSK99>16 職技等級大於等於99%的數量要超過16個
		char *p=NULL;
		int i,level=0,count=0;
		CHAR_HaveSkill *pSkil;
		if( p = strstr( buf, "PROSK" ) )
			level = atoi(p+5);
		else 
			level = 100;
		for( i = 0; i < CHAR_SKILLMAXHAVE; i ++ ){
			if( pSkil = CHAR_getCharHaveSkill( talker, i ) )
				if( SKILL_getInt( &pSkil->skill, SKILL_LEVEL) >= level )
					++count;
		}	
		if(NPC_EventBigSmallLastCheck( kosuu, count, flg) == TRUE)
			return TRUE;
	}
#endif

#ifdef _ADD_reITEM
	if(strstr( buf, "reITEM" ) != NULL) {
		int i,count=0,itemindex=-1;
		for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
			itemindex = CHAR_getItemIndex( talker , i );
			if( !ITEM_CHECKINDEX( itemindex) )
				++count;
		}
		if(NPC_EventBigSmallLastCheck( kosuu, count, flg) == TRUE)
			return TRUE;
	}
#endif

	return FALSE;

}

/*--------------------------------
 *  矢永玄及伊矛伙毛民尼永弁允月
 ---------------------------------*/
BOOL NPC_PetLvCheck(int meindex,int talker,char *buf,int mode)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buff2[16];
	char buf3[8];
	int petno;
	int baseno;
	int petlevel = 0;
	int flg=0;
	int mypetlevel = 0;
	int i = 0;
	int petindex;
	char name[512];
	int cnt = 0;
	int loop;
	int pettran=-1;

	/*--隙爛今木凶矢永玄及瓜件田□  ID)毛潸  --*/
	getStringFromIndexWithDelim( buf, "-", 2, buff2, sizeof( buff2));
	if(strstr(buff2,"*") != NULL) {
		getStringFromIndexWithDelim( buff2, "*", 1, buf3, sizeof( buf3));
		petno = atoi( buf3);
		getStringFromIndexWithDelim( buff2, "*", 2, buf3, sizeof( buf3));
		loop = atoi( buf3);

	}else{
		petno = atoi( buff2);
		loop = 1;

	}
	if(getStringFromIndexWithDelim( buf, "-", 3, buff2, sizeof( buff2)))
		{
			pettran = atoi( buff2);
		}
	/*--矢永玄  互釩中化中月井及民尼永弁--*/
	for(i=0; i < CHAR_MAXPETHAVE ; i++) {
		if(loop == cnt) return TRUE;
		
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )  continue;
		if(pettran != -1)
			{
				if(pettran!=CHAR_getInt( petindex, CHAR_TRANSMIGRATION))
					continue;
			}
		/*--矢永玄及ID毛潸  仄化中仁--*/
		baseno = CHAR_getInt( petindex, CHAR_PETID);

		/* 矢永玄及ID午｝蜇箕及乒□玉毛譬屯月 */
		if( (baseno == petno) && (CHAR_getInt( petindex, CHAR_ENDEVENT)
		 == mode))
		 {
			/*--伊矛伙毛民尼永弁允月--*/
			mypetlevel = CHAR_getInt( petindex, CHAR_LV);
			getStringFromIndexWithDelim( buf, "-", 1, buff2, sizeof( buff2));

			if(strstr( buf, "<") != NULL) {
				flg = 1;
				getStringFromIndexWithDelim( buff2, "<", 2, buf3, sizeof(buf3));
			}else if(strstr( buf, ">") != NULL) {
				getStringFromIndexWithDelim( buff2, ">", 2, buf3, sizeof(buf3));
				flg = 2;
			}else if(strstr( buf, "=") != NULL) {
				getStringFromIndexWithDelim( buff2, "=", 2, buf3, sizeof(buf3));
				flg = 0;
			}

			petlevel = atoi( buf3);

			if(NPC_EventBigSmallLastCheck( petlevel, mypetlevel, flg) == TRUE) {
				/*--矢永玄及  蟆互澀爛今木化中凶日  蟆及民尼永弁  月--*/
				if(CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC)==1) {
					char *usename;
					
					/*--澀爛白央奶伙及  心  心--*/
					if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
						print("PetLevel:GetArgStrErr");
						return FALSE;
					}

					usename = CHAR_getUseName( petindex);
					NPC_Util_GetStrFromStrWithDelim( argstr, "Pet_Name",
													name,sizeof( name));
	
					if(strcmp( name, usename) == 0)
					{
						cnt++;
						continue;
					}
				}else {
					CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);
					cnt++;
					continue;
				}
			}
		}
	}
	
	if(loop == cnt) return TRUE;
	if(i == CHAR_MAXPETHAVE) return FALSE;

	return FALSE;
}


/*--------------------------------
 *  矢永玄及伊矛伙毛民尼永弁允月
 ---------------------------------*/
int NPC_PetLvCheckType2(int petindex,int meindex,int talker,char *buf,int mode)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buff2[16];
	char buf3[8];
	int petno;
	int baseno;
	int petlevel = 0;
	int flg = 0;
	int mypetlevel = 0;
	char name[512];
	
	/*--隙爛今木凶矢永玄及瓜件田□  ID)毛潸  --*/
	getStringFromIndexWithDelim( buf, "-", 2, buff2, sizeof( buff2));
	if(strstr(buff2,"*") != NULL) {
		getStringFromIndexWithDelim( buff2, "*", 1, buf3, sizeof( buf3));
		petno = atoi( buf3);
	}else{
		petno = atoi( buff2);
	}

	
	/*--矢永玄及ID毛潸  仄化中仁--*/
	baseno = CHAR_getInt( petindex, CHAR_PETID);

	/*--矢永玄及ID午｝蜇箕及乒□玉毛譬屯月--*/
	if( (baseno == petno) && (CHAR_getInt( petindex, CHAR_ENDEVENT)
	 == mode))
	{
		/*--伊矛伙毛民尼永弁允月--*/
		mypetlevel=CHAR_getInt( petindex, CHAR_LV);
		getStringFromIndexWithDelim( buf, "-", 1,buff2,sizeof(buff2));

		if(strstr( buf, "<") != NULL) {
			flg = 1;
			getStringFromIndexWithDelim( buff2, "<", 2,
										 buf3, sizeof(buf3));
		}else if(strstr( buf, ">") != NULL) {
			getStringFromIndexWithDelim( buff2, ">", 2, buf3, sizeof(buf3));
			flg = 2;
		}else if(strstr( buf, "=") != NULL) {
			getStringFromIndexWithDelim( buff2, "=", 2, buf3, sizeof(buf3));
			flg = 0;
		}
		
		petlevel = atoi(buf3);

		if(NPC_EventBigSmallLastCheck( petlevel, mypetlevel, flg)
		 == TRUE)
		{
			/*--矢永玄及  蟆互澀爛今木化中凶日  蟆及民尼永弁  月--*/
			if(CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) == 1) {
				char *usename;

				/*--澀爛白央奶伙及  心  心--*/
				if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
					print("PetLevel:GetArgStrErr");
					return FALSE;
				}
				
				usename = CHAR_getUseName( petindex);
				NPC_Util_GetStrFromStrWithDelim(argstr,"Pet_Name",
												name, sizeof( name));
	
				if(strcmp( name, usename) == 0)
				{
					return TRUE;
				}
			}else {
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);
				return TRUE;
			}
		}
	}

	return FALSE;

}

/*------------------------
 *請  鰾及民尼永弁
 --------------------------*/
BOOL NPC_SavePointCheck(int meindex,int talker,int shiftbit,int flg)
{
	int point;

	point = CHAR_getInt( talker, CHAR_SAVEPOINT);
	
	if( (point & ( 1<< shiftbit))  == ( 1<< shiftbit)) {
		if(flg == 3) return FALSE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;
}

/*------------------------
 *奶丟□斥及民尼永弁
 --------------------------*/
BOOL NPC_ImageCheck(int meindex,int talker,int kosuu,int flg)
{
	int image;

	image = CHAR_getInt( talker, CHAR_IMAGETYPE);
	
	if(NPC_EventBigSmallLastCheck( image, kosuu, flg) == TRUE) {
		return TRUE;
	}
	return FALSE;

}


/*-----------------------------------------
 * 凜棉及民尼永弁毛墊丹(丐中引中凜棉)
 ------------------------------------------*/
BOOL NPC_TiemCheck(int meindex,int talker,int time,int flg)
{
	LSTIME		nowlstime;
	int now;

	RealTimeToLSTime( NowTime.tv_sec, &nowlstime);
	now = getLSTime( &nowlstime);

	if(NPC_EventBigSmallLastCheck( time, now, flg) == TRUE) {
		return TRUE;
	}
	return FALSE;

}

/*------------------------------
 *--Item毛民尼永弁				
 -------------------------------*/
BOOL NPC_EventItemCheck(int meindex,int talker,int itemNo,int flg)
{
	int i;
	int itemindex = -1;
	int id;

	//Change fix 不檢查穿戴在身上的道具
	for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {

		itemindex=CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX( itemindex) ) {
			id=ITEM_getInt( itemindex ,ITEM_ID );

			if(NPC_EventBigSmallLastCheck( itemNo, id, flg) == TRUE) {
				if(flg == 0)	return TRUE;
				continue;
			}else{
				if(flg == 0) continue;
				return FALSE;
			}
		}
	}

	if(flg == 3) return TRUE;

	return FALSE;

}


/*------------------------------
 * level毛民尼永弁				
 -------------------------------*/
BOOL NPC_EventLevelCheck(int meindex,int talker,int level,int flg)
{

	int mylevel;
	mylevel = CHAR_getInt(talker,CHAR_LV);

	if(NPC_EventBigSmallLastCheck( level, mylevel, flg) == TRUE)
	{
		if(flg == 3) return FALSE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;

}



/*------------------------------
 * ENDEventNo毛民尼永弁
 -------------------------------*/
BOOL NPC_ENDEventNoCheck(int meindex,int talker,int shiftbit,int flg)
{

	if(NPC_EventCheckFlg( talker, shiftbit) == TRUE) {
		if(flg == 3) return FALSE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;
}


/*------------------------------
 * NOWEventNo毛民尼永弁
 -------------------------------*/
BOOL NPC_NOWEventNoCheck(int meindex,int talker,int shiftbit,int flg)
{
	if(NPC_NowEventCheckFlg( talker, shiftbit) == TRUE) {
		if(flg == 3) return TRUE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;
}

/*---------------------------------------
 *    蔽民尼永弁
 --------------------------------------*/
BOOL NPC_EventBigSmallLastCheck(int point1,int mypoint,int flg)
{

	if(flg == 0) {
		if(point1 == mypoint) {
			return TRUE;
		}
	}else if(flg == 1) {
		if(mypoint < point1) {
			return TRUE;
		}
	}else if(flg == 2) {
		if(mypoint > point1) {
			return TRUE;
		}
	}else if(flg == 3) {
		if(point1 != mypoint) {
			return TRUE;
		}
	}
	return FALSE;
}


/*---------------------------------
 *丟永本□斥毛請  允月
 *----------------------------------*/
#ifdef _CHANNEL_MODIFY
extern int *piOccChannelMember;
#endif
void NPC_MsgDisp(int meindex,int talker,int num)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
#ifdef _NEWEVENT
	char buf[4096];
	char token2[4096];
	char buf2[4096];
	char token[4096];
	char tmp[4096];
#else
	char buf[1024*2];
	char token2[50*14];
	char buf2[512];
	char token[512];
	char tmp[32];
#endif
	int i = 1;
	int work = 0;
	int EvNo = 0;
	int fd = getfdFromCharaIndex( talker);
	int buttontype = WINDOW_BUTTONTYPE_OK;
	int windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG;
	int evch;

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return ;
	}
	if(num == 22 || num == 82 || num == 55 || num == 66) {
		int pwork;

		pwork = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
		pwork = (pwork / 100) -1 ;
		getStringFromIndexWithDelim( argstr, "EventEnd", pwork, buf,sizeof( buf));
		
	}else{
		/*澀爛白央奶伙毛坌喃允月*/
		while(getStringFromIndexWithDelim( argstr, "EventEnd", i, buf, sizeof(buf))
		 != FALSE)
		{
			i++;
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);
			
			/*--奶矛件玄瓜件田□毛必永玄--*/
			NPC_Util_GetStrFromStrWithDelim( buf, "EventNo", buf2, sizeof( buf2));
			if(strstr( buf2, "-1") != 0) {
				EvNo = -1;
			}else{
				EvNo = atoi( buf2);
			}
			EvNo = atoi( buf2);

			/*--白仿弘民尼永弁毛墊丹〔允匹卞｝仇及奶矛件玄互蔽  仄化中月樺寧反戚尺--*/
			if(NPC_EventCheckFlg( talker, EvNo) == TRUE) continue;

			/*--平□伐□玉及民尼永弁毛墊丹--*/
			if(NPC_Util_GetStrFromStrWithDelim( buf,"Pet_Name", buf2,sizeof( buf2)) 
			 != NULL )
			{
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 1);
			}

			/*--平□伐□玉及民尼永弁毛墊丹--*/
			if(NPC_Util_GetStrFromStrWithDelim( buf,"KeyWord", buf2,sizeof( buf2) ) 
			 != NULL)
			{
					if(CHAR_getWorkInt( talker ,CHAR_WORKSHOPRELEVANT) != i) continue;
			}

			evch = NPC_ExChangeManEventCheck( meindex, talker, buf);

			if(evch != -1) {

			/*--奶矛件玄橢瘀及民尼永弁--*/
		//	if(NPC_ExChangeManEventCheck( meindex, talker, buf) == TRUE) {
				break;
			}
		}
	}
	
	//print(" NPC_MsgDisp:%d ", num);
	
	switch( num) {
		  case 0:
			/*蜇箕及奶矛件玄毛鞅戶月井升丹井毛  仁丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "StopMsg", 
												buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token,sizeof( buf2), buf2);
				buttontype = WINDOW_BUTTONTYPE_YESNO;
				windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_NOWEVENT;
			}
		break;
		
		  case 1:
		  	/*--支戶月井升丹井毛  中凶今中及NO毛蠶尹凶樺寧及丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "NoStopMsg",
												 buf2, sizeof( buf2))
			 != NULL)
			{
				strcpysafe( token, sizeof(buf2), buf2);

			}
		break;
		
		  case 7:
			/*--蜇箕及奶矛件玄毛鞅戶凶午五及丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "EndStopMsg",
												 buf2, sizeof( buf2))
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
			
				/*--奶矛件玄  毛  勻化中凶樺寧反壅蛔--*/
				NPC_EventAdd( meindex, talker, 1);
			
				/*--白仿弘毛壅允--*/
				NPC_NowEventSetFlgCls( talker, EvNo);
			
				/*--蠅勻凶及匹｝    毛今僕凶--*/
				if(CHAR_getInt( talker, CHAR_CHARM) > 0) {
					CHAR_setInt( talker, CHAR_CHARM, CHAR_getInt(talker,CHAR_CHARM) -1);
					CHAR_complianceParameter( talker );
					CHAR_send_P_StatusString( talker, CHAR_P_STRING_CHARM);
					NPC_CharmStatus( meindex, talker);
				}
			}
		break;
		
		  case 2:
				/*--丐曰互午丹丟永本□斥--*/
				if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
													 buf2, sizeof( buf2) )
				 != NULL)
				{
					strcpysafe( token,sizeof( buf2), buf2);
					/*--奶矛件玄  白仿弘毛本永玄--*/
					if(NPC_EventAdd(meindex, talker, 0) != FALSE) {
						NPC_NowEventSetFlg( talker, EvNo);
					}
					else {
						return;
					}
				
				
					if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
													buf2, sizeof( buf2) )
					!=NULL)
					{
						char buf3[16];
						int k = 1;
						if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
						
						while(getStringFromIndexWithDelim(buf2 , "," ,k, 
												buf3, sizeof(buf3))
						 !=FALSE )
						{	
							k++;
							NPC_EventSetFlg( talker, atoi( buf3));
						}
					}

					if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
													 buf2, sizeof( buf2)) 
					 != NULL) {
					 	char buf3[16];
						int k = 1;

					 	k= 1 ;
						while(getStringFromIndexWithDelim(buf2 , "," , k, 
															buf3, sizeof(buf3))
						!= FALSE )
						{
							NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
							k++;
						}
					}

				
				
				}
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work + ( i * 100);
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);
				
				if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp",
										token2,sizeof( token2) )
				!= NULL){
					NPC_EventWarpNpc(meindex,token2);
				}

				/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
				if(strstr( buf, "ThanksMsg2") != NULL) {
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}

		break;

		   case 22:
				
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work % 100;

				if(work == 1){
					if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
														 buf2, sizeof( buf2) )
					 != NULL)
					{
						strcpysafe( token,sizeof( buf2), buf2);
					}

					/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
					if(strstr( buf, "ThanksMsg2") != NULL) {
					
						/*仇仇匹霜耨允月*/
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_NEXT,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}
				
				}else{
					
					sprintf( tmp, "ThanksMsg%d", work);
					if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
														 buf2, sizeof( buf2) )
					 != NULL) {
						strcpysafe( token,sizeof( buf2), buf2);
					}
	
					work++;
					sprintf( tmp, "ThanksMsg%d", work);
	
	
					/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
					if(strstr( buf, tmp) != NULL) {
						
						/*仇仇匹霜耨允月*/
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_NEXT,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}else{
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_OK,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}
				}
		break;

		  case 25:
			  /*--丐曰互午丹丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token,sizeof( buf2), buf2);
				/*--奶矛件玄  白仿弘毛本永玄--*/
				if(NPC_EventAdd(meindex, talker, 2) != FALSE) {
					NPC_NowEventSetFlg( talker, EvNo);
				}
			}
		break;


		  case 3:
			/*--  騷及丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "NomalMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe(token, sizeof( buf2), buf2);
			}
		break;

		  case 4:
		  	/*--失奶  丞互中勻天中及凜及丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "ItemFullMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe(token, sizeof( buf2), buf2);
			}
		break;

		case 5:
			/*--匙  毛允月端及丟奶件及丟永本□斥--*/
				if( NPC_Util_GetStrFromStrWithDelim( buf, "RequestMsg",
													 buf2, sizeof( buf2) )
				 != NULL)
				{
          
          if(strstr(buf2,"%8d") != NULL) {
						char buf3[64];
						int cost;
						
						NPC_Util_GetStrFromStrWithDelim( buf, "DelStone",
													 buf3, sizeof( buf3) );
						cost = NPC_EventGetCost( meindex, talker, buf3);
						
						sprintf(token,buf2,cost);

					}else{
						strcpysafe( token,sizeof( buf2), buf2);
					}
					buttontype = WINDOW_BUTTONTYPE_YESNO;
					windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG;
				}

				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work + ( i * 100);
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);

				/*--戚矢□斥互丐月樺寧反示正件它奶件玉它及正奶皿毛井尹月--*/
				if(strstr( buf, "RequestMsg2") != NULL) {
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
		break;

	   case 55:
				
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work % 100;

				if(work == 1){
					if( NPC_Util_GetStrFromStrWithDelim( buf, "RequestMsg",
														 buf2, sizeof( buf2) )
					 != NULL)
					{
						strcpysafe( token, sizeof( buf2), buf2);
					}

					/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
					if(strstr( buf, "RequestMsg2") != NULL){
						/*仇仇匹霜耨允月*/
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_NEXT,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}
				}else{
					
					sprintf( tmp, "RequestMsg%d", work);
					if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
														 buf2, sizeof( buf2) )
					 != NULL){
						strcpysafe( token,sizeof( buf2), buf2);
							
					}
					work++;
					sprintf( tmp, "RequestMsg%d", work);
					/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
					if(strstr( buf, tmp) != NULL) {
						
						/*仇仇匹霜耨允月*/
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_NEXT,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}else{
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_YESNO,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}
				}
		break;


		  case 6:
			  //print(" 6_答應? ");
			/*--熬仃潸曰及端及丟奶件及丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "AcceptMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				if(strstr(buf2,"%8d") != NULL) {
					char buf3[64];
					int cost;
						
					NPC_Util_GetStrFromStrWithDelim( buf, "DelStone",
											 buf3, sizeof( buf3) );
					cost = NPC_EventGetCost( meindex, talker, buf3);
						
					sprintf( token, buf2, cost);
				}else{
					strcpysafe( token, sizeof( buf2), buf2);
				}
				buttontype = WINDOW_BUTTONTYPE_YESNO;
				windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG;
			}
	
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work + ( i * 100);
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);

			/*--戚矢□斥互丐月樺寧反示正件它奶件玉它及正奶皿毛井尹月--*/
			if(strstr( buf, "AcceptMsg2") != NULL) {
				/*仇仇匹霜耨允月*/
				lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
							WINDOW_BUTTONTYPE_NEXT,
							CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG,
							CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
							token);
				return;
			}
		break;


	   case 66:
		   //print(" 66_答應? ");
			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work % 100;

			if(work == 1){
				if( NPC_Util_GetStrFromStrWithDelim( buf, "AcceptMsg",
													 buf2, sizeof( buf2) )
				 != NULL)
				{
					strcpysafe( token,sizeof( buf2), buf2);
				}

				/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
				if(strstr( buf, "AcceptMsg2") != NULL) {
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}else{
			
				sprintf( tmp, "AcceptMsg%d", work);
				if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
													 buf2, sizeof( buf2) )
				 != NULL){
					strcpysafe( token,sizeof( buf2), buf2);
				}
				work++;
				sprintf( tmp, "AcceptMsg%d", work);
				/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
				if(strstr( buf, tmp) != NULL) {
					
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}else{
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_YESNO,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}
		break;

		case 8:
				//print(" 8_答應? ");
			
				if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
													buf2, sizeof( buf2) )
				 != NULL)
				{
					strcpysafe( token, sizeof( buf2), buf2);
					/*--奶矛件玄  白仿弘毛本永玄--*/
					if(NPC_AcceptDel( meindex, talker,0) == FALSE){
						return ;
					}
					
					if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
						buf2, sizeof( buf2) )
						!=NULL)
					{
						char buf3[16];
						int k = 1;
						if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
						
						while(getStringFromIndexWithDelim(buf2 , "," ,k, 
							buf3, sizeof(buf3))
							!=FALSE )
						{	
							k++;
							NPC_EventSetFlg( talker, atoi( buf3));
						}
					}
					
					if( NPC_Util_GetStrFromStrWithDelim( buf, "Charm",
						buf2, sizeof( buf2))
						!=NULL)
					{
						/*--仇仇引匹  凶日｝    毛曉僕月---*/
						if( CHAR_getInt( talker, CHAR_CHARM) < 100 && EvNo > 0 ){
							if(CHAR_getInt( talker, CHAR_CHARM) + atoi(buf2) > 100){
								CHAR_setInt( talker, CHAR_CHARM,100);
								
							}else{
								CHAR_setInt( talker, CHAR_CHARM, 
									CHAR_getInt(talker,CHAR_CHARM) + atoi(buf2));
								
							}
							
							CHAR_complianceParameter( talker );
							CHAR_send_P_StatusString( talker, CHAR_P_STRING_CHARM);
							NPC_CharmStatus( meindex, talker);
						}
					}
					
					if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
						buf2, sizeof( buf2)) 
						!= NULL) {
						char buf3[16];
						int k = 1;
						
						k= 1 ;
						while(getStringFromIndexWithDelim(buf2 , "," , k, 
							buf3, sizeof(buf3))
							!= FALSE )
						{
							NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
							k++;
						}
					}
					
					
					// Robin add 清除職業
					if( NPC_Util_GetStrFromStrWithDelim( buf, "PROFESSION",
						buf2, sizeof( buf2)) != NULL)
					{
						int p_class = atoi( buf2);
						//Change fix 2004/07/05
						for( i=0; i<CHAR_STARTITEMARRAY; i++){ //檢查裝備中道具
							if( ITEM_CHECKINDEX( CHAR_getItemIndex( talker , i ) )){
								if( CHAR_getInt( talker, PROFESSION_CLASS ) != 0
									&& CHAR_getInt( talker, PROFESSION_CLASS ) != ITEM_getInt( CHAR_getItemIndex( talker , i ), ITEM_NEEDPROFESSION) 
									&& ITEM_getInt( CHAR_getItemIndex( talker , i ), ITEM_NEEDPROFESSION) > 0 ){
									p_class = 1;
									CHAR_talkToCli( talker, -1, "請先把職業裝備通通卸下！", CHAR_COLORWHITE);
									break;
								}
							}
						}
						if( p_class == 0)
						{
							print(" ====清除職業==== ");
							
							// 還原點數	
							//if( PROFESSION_RESTORE_POINT( talker ) != 1 )	return FALSE;
							
							// 設定職業歸零
							CHAR_setInt( talker, PROFESSION_CLASS, p_class );	
#ifdef _CHANNEL_MODIFY
							if(CHAR_getInt(talker,PROFESSION_CLASS) > 0){
								int i,pclass = CHAR_getInt(talker,PROFESSION_CLASS) - 1;
								for(i=0;i<getFdnum();i++){
									if(*(piOccChannelMember + (pclass * getFdnum()) + i) == talker){
										*(piOccChannelMember + (pclass * getFdnum()) + i) = -1;
										break;
									}
								}
							}
#endif
							
							// 設定職業等級 0
							CHAR_setInt( talker, PROFESSION_LEVEL, 0 );
							
							// 技能點數歸零
							CHAR_setInt(talker, PROFESSION_SKILL_POINT, 0 );
							
							// 還原抗性
							for( i=0; i<3; i++ )
								CHAR_setInt( talker, PROFESSION_FIRE_R+i, /*CHAR_getInt( toindex, PROFESSION_FIRE_R+i) + value*/0 );
							
							// 還原巫師MP上限
							CHAR_setInt( talker , CHAR_MAXMP , 100 );
							
							// 刪除所有技能
							CHAR_CHAT_DEBUG_delsk( talker, "all" );
							
							// ?? Andy add
							CHAR_setInt( talker , ATTACHPILE, 0);
							
							// 清除任務旗標
							NPC_NowEndEventSetFlgCls( talker, 145);
							NPC_NowEndEventSetFlgCls( talker, 146);
							NPC_NowEndEventSetFlgCls( talker, 147);					
							
							CHAR_sendStatusString( talker , "S");
							
							CHAR_sendCToArroundCharacter( CHAR_getWorkInt( talker , CHAR_WORKOBJINDEX ));
							
						}
						
					}

#ifdef _ANGEL_SUMMON
					// 完成召喚任務
					if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONOVER",
						buf2, sizeof( buf2)) != NULL)
					{
						int mindex;
						int mission;
						char nameinfo[64];
						
						mission = atoi( buf2);
						mindex = checkIfAngel( talker);

						if( mission == missiontable[mindex].mission )
						{
							char msg[1024];
							
							print(" ====完成召喚任務==== ");

							CHAR_setInt( talker, CHAR_HEROCNT, 
								CHAR_getInt( talker, CHAR_HEROCNT)+1 );
							getMissionNameInfo( talker, nameinfo);
							saacproto_ACMissionTable_send( acfd, MISSION_HERO_COMPLETE, 4, nameinfo, "");
							sprintf( msg, " 完成任務 i:%d m:%d %s ", mindex, mission, nameinfo);
							print( msg);
							LogAngel( msg );
						}
						
					}

					// 清除召喚任務
					if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONCLEAN",
						buf2, sizeof( buf2)) != NULL)
					{
						int mindex;
						int mission;
						char nameinfo[64];
						
						mission = atoi( buf2);
						mindex = checkIfAngel( talker);

						if( mission == missiontable[mindex].mission )
						{
							print(" ====清除召喚任務==== ");

							getMissionNameInfo( talker, nameinfo);
							saacproto_ACMissionTable_send( acfd, mindex, 3, nameinfo, "");
						}
						
					}
#endif
				}
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work + ( i * 100 );
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);
	
				if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp",
													token2,sizeof( token2) )
					!= NULL){
						NPC_EventWarpNpc(meindex,token2);
				}
				/*--戚矢□斥互丐月樺寧反示正件它奶件玉它及正奶皿毛井尹月--*/
#ifdef _NEWEVENT
#else
				if(strstr( buf, "ThanksMsg2") != NULL){
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
#endif
		break;

#ifdef _NEWEVENT
		case 81:
			//print(" 81_答應? ");

			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work % 100;
			sprintf(tmp, "ThanksMsg%d", work + 1);
			if(work == 0 && strstr(buf, tmp) == NULL)
			{
				if(NPC_Util_GetStrFromStrWithDelim(buf, "ThanksMsg",
					buf2, sizeof( buf2)) != NULL)
					strcpysafe( token,sizeof( buf2), buf2);
				if(strstr(buf, "ThanksMsg2") != NULL)
					lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
					WINDOW_BUTTONTYPE_NEXT,
					CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW,
					CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
					token);
				else
					lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
					WINDOW_BUTTONTYPE_OK,
					CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW,
					CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
					token);
				return;
			}
			else
			{
				if(NPC_Util_GetStrFromStrWithDelim(buf, tmp,
					buf2, sizeof( buf2)) != NULL)
					strcpysafe( token,sizeof( buf2), buf2);
				work++;
				sprintf(tmp, "ThanksMsg%d", work + 1);
				
				if(strstr(buf, tmp) != NULL)
				{
					lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
						WINDOW_BUTTONTYPE_NEXT,
						CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW,
						CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
						token);
					return;
				}
				else
				{
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
						WINDOW_BUTTONTYPE_OK,
						CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						token);
					return;
				}
			}
			
			break;
#endif	
	
	   case 82:
			work = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTTRD);
			work = work % 100;

			if(work == 1){
				if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
													 buf2, sizeof( buf2))
				 != NULL)
				{
					strcpysafe( token,sizeof( buf2), buf2);
				}

				/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
				if(strstr( buf, "ThanksMsg2") != NULL){
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}else{
				
				sprintf( tmp, "ThanksMsg%d", work);
				if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
													 buf2, sizeof( buf2) )
				!= NULL)
				{
					strcpysafe( token,sizeof( buf2), buf2);
				}
				
				work++;
				sprintf( tmp, "ThanksMsg%d", work);

				/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
				if(strstr( buf, tmp) != NULL) {
						
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}else{
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_OK,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}
			break;

		case 88:
			/*--失弁本皿玄及--*/
			/*--丐曰互午丹丟永本□斥--*/
			/*--矢永玄及  迕--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
												buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
				/*--奶矛件玄  白仿弘毛本永玄--*/
				if(NPC_AcceptDel( meindex, talker,1) == FALSE){
					return ;
				}
				
				if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
												buf2, sizeof( buf2) )
				!=NULL)
				{
					char buf3[16];
					int k = 1;
				
					if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
					
					while(getStringFromIndexWithDelim(buf2 , "," ,k, 
											buf3, sizeof( buf3))
					 !=FALSE )
					{	
						k++;
						NPC_EventSetFlg( talker, atoi( buf3));
					}
				}

				if( NPC_Util_GetStrFromStrWithDelim( buf, "Charm",
													buf2, sizeof( buf2))
				!=NULL)
				{
					/*--仇仇引匹  凶日｝    毛曉僕月---*/
					if( CHAR_getInt( talker, CHAR_CHARM) < 100 && EvNo > 0 ){
						if(CHAR_getInt( talker, CHAR_CHARM) + atoi(buf2) > 100){
							CHAR_setInt( talker, CHAR_CHARM,100);
						}else{
							CHAR_setInt( talker, CHAR_CHARM, 
									CHAR_getInt(talker,CHAR_CHARM) + atoi( buf2));
						}
						CHAR_complianceParameter( talker );
						CHAR_send_P_StatusString( talker, CHAR_P_STRING_CHARM);
						NPC_CharmStatus( meindex, talker);
					}
				}
			}

		break;


		  case 9:
			
		  	if( NPC_Util_GetStrFromStrWithDelim( buf, "NomalWindowMsg",
												buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
				if(EvNo != -1) NPC_NowEventSetFlg( talker, EvNo);
			}
#ifdef _NEWEVENT
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work + ( i * 100);
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);

			/*--戚矢□斥互丐月樺寧反示正件它奶件玉它及正奶皿毛井尹月--*/
			if(strstr( buf, "NomalWindowMsg2") != NULL) {
				/*仇仇匹霜耨允月*/
				lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
							WINDOW_BUTTONTYPE_NEXT,
							CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
							CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
							token);
				return;
			}
#endif
		  	break;
		  	
#ifdef _NEWEVENT
	  case 99:
			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work % 100;

			if(work == 1)
			{
				if( NPC_Util_GetStrFromStrWithDelim( buf, "NomalWindowMsg1",
									 buf2, sizeof( buf2) ) != NULL)
					strcpysafe( token,sizeof( buf2), buf2);

				/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
				if(strstr( buf, "NomalWindowMsg2") != NULL) {
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}else{
			
				sprintf( tmp, "NomalWindowMsg%d", work);
				if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
													 buf2, sizeof( buf2) )
				 != NULL){
					strcpysafe( token,sizeof( buf2), buf2);
				}
				work++;
				sprintf( tmp, "NomalWindowMsg%d", work);
				/*--戚矢□斥互丐月樺寧反示正件方它奶件玉它及正奶皿毛井尹月--*/
				if(strstr( buf, tmp) != NULL) {
					
					/*仇仇匹霜耨允月*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}else{
					int fd = getfdFromCharaIndex( talker);
					char token2[14*50];
					/*--仇仇匹｝失奶  丞卅升及馨笛毛墊丹--*/
					if(NPC_AcceptDel( meindex, talker,0) == FALSE) return;
	
					/*白仿弘毛本永玄仄化中木壬｝本永玄允月--*/
					if( EvNo != -1) NPC_NowEventSetFlg( talker, EvNo);
						
					if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp",
										token2,sizeof( token2) )!= NULL){
								NPC_EventWarpNpc(meindex,token2);
					}
					
					//白仿弘毛  化月
					if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
										buf2, sizeof( buf2) )!=NULL)
					{
						char buf3[16];
						int k = 1;
				
						if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
				
						while(getStringFromIndexWithDelim(buf2 , "," ,k, 
								buf3, sizeof(buf3)) !=FALSE )
						{	
							k++;
							NPC_EventSetFlg( talker, atoi( buf3));
						}
					}
					//奶矛件玄白仿弘毛壅允
					if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
							 buf2, sizeof( buf2)) != NULL) {
						 	char buf3[16];
							int k = 1;

						 	k= 1 ;
							while(getStringFromIndexWithDelim(buf2 , "," , k, 
																buf3, sizeof(buf3))
							!= FALSE )
							{
								NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
								k++;
							}
					}
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_YES,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}
		  break;
#endif
		  case 10:
			  
			/*--矢永玄互中勻天中及凜及丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "PetFullMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
				print(" PetFullMsg:%s ", token);
			}
			else
			{
				print(" PetFullMsg_Error:%d ", __LINE__);
			}
		break;

	  case 11:
			/*--白仿弘毛壅允午五及丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanMainMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
				buttontype = WINDOW_BUTTONTYPE_YESNO;
				windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_CLEANMSG;
			}
		break;
	  case 12:
			/*--白仿弘毛壅仄凶  及永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlgMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				char buf3[64];
				int loop=1;
				
				strcpysafe( token, sizeof( buf2), buf2);
				NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
												 buf2, sizeof( buf2) );
				while(getStringFromIndexWithDelim(buf2 , "," , loop, buf3, sizeof(buf3))
				!= FALSE )
				{
					NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
					loop++;
				}
			}
		break;

	  case 13:
			/*--雲嗯互中勻天中及丟永本□斥--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "StoneFullMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
			}
		break;
		
		 case 14:
			/*--雲嗯互凶曰卅中丟永本□斥--*/
					if( NPC_Util_GetStrFromStrWithDelim( buf, "StoneLessMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
			}
		break;
	}

	/*仇仇匹霜耨允月*/
#ifdef _NEWEVENT
	if(num != 8)
#endif
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
					buttontype,
					windowtype,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					token);

}

/*--------------------------------------
 *奶矛件玄迕及失奶  丞毛馨笛允月
 ---------------------------------------*/
BOOL NPC_EventAdd(int meindex,int talker,int mode)
{

	char buf[64];
	char buff2[128];
	int j = 1,i = 0;
	int kosuucnt = 0;
	int itemindex;
	char buff[1024*2];
	int rand_j = 0;
	int rand_flg = 0;
	int rand_cnt = 0;
	int evcnt;
	int b_flg = 0;

	/*--澀爛白央奶伙  卞踏中化丐月橢瘀毛  凶仄化中月手及毛  五請允--*/
	evcnt = NPC_EventFile( meindex, talker, buff);

	if(evcnt == -1) return FALSE;

//	if(NPC_EventFile( meindex, talker, buff) == FALSE) return FALSE;

	/*--失奶  丞及醒毛民尼永弁--*/
	if(NPC_ItemFullCheck( meindex, talker, buff, mode, evcnt) == FALSE){
		NPC_MsgDisp( meindex, talker, 4);
		return FALSE;
	}

	/*--雲嗯民尼永弁卞  月--*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "DelStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		
		stone = NPC_EventGetCost( meindex, talker, buff2);

		if( (CHAR_getInt( talker, CHAR_GOLD) - stone) < 0 ){
			NPC_MsgDisp( meindex, talker, 14);
			return FALSE;
		}
	}


	/*--矢永玄及  毛馨笛允月--*/
	if((NPC_Util_GetStrFromStrWithDelim( buff, "pet_skill", buff2, sizeof( buff2) )
	 !=NULL) && mode == 0 ){
		if(mode == 0) {
			NPC_PetSkillMakeStr( meindex, talker, 1);
			return TRUE;
		}
	}
	
	
	if(mode == 2) mode = 0;
	
	/*--矢永玄毛馨笛允月---*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "GetPet", buff2, sizeof( buff2) )
	 !=NULL)
	{	
		if(mode == 0){
			if(NPC_EventAddPet( meindex, talker, buff2,0) == FALSE) {
				NPC_MsgDisp( meindex, talker, 10);
				return FALSE;
			}
		}else if(mode == 1){
		
		}
	}

	// Robin add 從NPC取寵物蛋
	if(NPC_Util_GetStrFromStrWithDelim( buff, "GetEgg", buff2, sizeof( buff2) )
	 !=NULL)
	{	
		if(mode == 0){
			if(NPC_EventAddEgg( meindex, talker, buff2,0) == FALSE) {
				print(" AddEgg_Error:%d ", __LINE__);
				NPC_MsgDisp( meindex, talker, 10);
				return FALSE;
			}
		}else if(mode == 1){
		
		}
	}

	if(NPC_Util_GetStrFromStrWithDelim( buff, "DelItem", buff2, sizeof( buff2) ) !=NULL){
		if(strstr(buff2,"EVDEL") != NULL){
			char work[256];
			char nbuf[256];
			
			NPC_Util_GetStrFromStrWithDelim(buf, "EVENT", buff2, sizeof( buff2));
			getStringFromIndexWithDelim( buff2, ",", evcnt, work, sizeof( work));

			if(strstr(buff,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}
			
			if(NPC_Util_GetStrFromStrWithDelim( buff, "NotDel", nbuf, sizeof( nbuf)) !=  NULL) {
				NPC_EventDelItemEVDEL( meindex, talker, work, nbuf,b_flg);
			}else{
				NPC_EventDelItemEVDEL( meindex, talker, work, "-1",b_flg);
			}
			
		}else{
			if(strstr(buff,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}

			NPC_EventDelItem( meindex, talker, buff2, b_flg);
		}
	}

	/*--雲嗯毛潸月---*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "DelStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		char token[128];
		
		stone = NPC_EventGetCost( meindex, talker, buff2);

		CHAR_setInt(talker,CHAR_GOLD,CHAR_getInt( talker, CHAR_GOLD) - stone);

		/*--由仿丟□正霜曰--*/
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
		sprintf(token, "付出%d的stone。", stone);
		CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
	}


	/*--仿件母丞失奶  丞---*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "GetRandItem", buf, sizeof( buf) ) != NULL ){
		j=1;
		while( getStringFromIndexWithDelim( buf , "," , j, buff2, sizeof( buff2)) != FALSE ){
			j++;
			rand_j++;
		}
		rand_cnt = 1;
	}


	/*--  騷及失奶  丞必永玄--*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "GetItem", buf, sizeof( buf)) != NULL ){
		if(mode == 0) {
			j = 1;
			/*--窒蜊及失奶  丞毛  允井〞--*/
			while(getStringFromIndexWithDelim( buf, "," , j, buff2, sizeof(buff2)) !=FALSE ){
				j++;
			}
			
			j--;
			for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
				itemindex=CHAR_getItemIndex( talker , i );
				if( !ITEM_CHECKINDEX( itemindex) ){
					kosuucnt++;
				 }
			}
			j= j + rand_cnt;
			if( kosuucnt < j ){
				NPC_MsgDisp( meindex, talker, 4);
				return FALSE;

			}else{
				if(NPC_Util_GetStrFromStrWithDelim( buff, "GetRandItem", buff2, sizeof( buff2))	!=NULL)	{
	 				rand_flg = 1;
					NPC_RandItemGet( meindex, talker, rand_j, buff2);
	 			}
				if(NPC_EventAddItem( meindex, talker, buf) == FALSE) return FALSE;
			}
		}else if(mode == 1){
			if(strstr(buff,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}
			NPC_EventDelItem( meindex, talker, buf,b_flg);
		}
	}

	if( (NPC_Util_GetStrFromStrWithDelim( buff, "GetRandItem", buf, sizeof( buf) )
		!=NULL) && (rand_flg == 0) && mode == 0 )
	{	
		/*--民尼永弁   癲卞蟈    月井〞-*/
		for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
			itemindex=CHAR_getItemIndex( talker , i );
			if( !ITEM_CHECKINDEX( itemindex) ){
				kosuucnt++;
			 }
		}
		/*--塢五互丐月井升丹井--*/
		if( kosuucnt == 0 ){
			NPC_MsgDisp( meindex, talker, 4);
			return FALSE;
		}

	
		NPC_RandItemGet( meindex, talker, rand_j,buf);
	}
	
#ifdef _EXCHANGEMAN_REQUEST_DELPET
	if( NPC_Util_GetStrFromStrWithDelim( buff, "DelPet", buff2, sizeof( buff2) ) != NULL && mode == 0 ) {
		char buf4[32];
		char buf5[256];
		int i=1;
		char work[256];
		int j=1;
		
		if(strstr(buff2,"EVDEL") != NULL){
			NPC_Util_GetStrFromStrWithDelim( buf, "EVENT", buf5, sizeof( buf5));
			getStringFromIndexWithDelim( buf5, ",",evcnt , buff2, sizeof( buff2));	
			while(getStringFromIndexWithDelim( buff2, "&", j, work, sizeof( work))!=FALSE){
				j++;			
				if(strstr( work, "PET") != NULL) {
					if(strstr( work, "EV") != NULL) {
						if(NPC_PetLvCheck( meindex, talker, work, 1) == FALSE) return FALSE;
					}else{
						if(NPC_PetLvCheck( meindex, talker, work, 0) == FALSE) return FALSE;
					}
				}
			}
			j = 1;		
			while(getStringFromIndexWithDelim( buff2, "&", j, work, sizeof( work)) != FALSE){
				j++;
				if(strstr( work, "PET") == NULL ) continue;
	
				if(strstr(work,"*") != NULL){
					int cnt=0;
					int petcnt=0;
					int petindex;
					
					getStringFromIndexWithDelim( work, "*", 2, buf4, sizeof( buf4));
					cnt = atoi(buf4);
					
					for(i=0 ; i < CHAR_MAXPETHAVE ; i++){
						petindex = CHAR_getCharPet( talker, i);
		
						if( petindex == -1  )  continue;
		
						if(strstr( work, "EV") != NULL) {
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 1)
							== FALSE) {
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 0) == FALSE) {
								continue;
							}
						}

						if(NPC_EventDelPet( meindex, talker,i) == FALSE) return FALSE;
						
						petcnt++;
						/*--隙爛蜊醒坌及矢永玄毛壅仄凶井升丹井〞--*/
						if(cnt == petcnt) break;

					}
				}else{
					/*--矢永玄毛綽輪允月醒毛隙爛仄化卅中及匹竟癲矢永玄毛蟈化壅允--*/

					int petindex;

					for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
						petindex = CHAR_getCharPet( talker, i);

					    if( !CHAR_CHECKINDEX( petindex) )  continue;

						if(strstr( buff2, "EV") != NULL){
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 1) == FALSE){
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 0) == FALSE) {
								continue;
							}
						}
					
						if(NPC_EventDelPet( meindex, talker, i) == FALSE) return FALSE;
					}
				}
			}
		
		}else{
			/*--手丹1蘸｝矢永玄毛  勻化中月井及民尼永弁毛墊丹--*/
			while(getStringFromIndexWithDelim( buff2, ",", j, work, sizeof( work))
			!=FALSE)
			{
				j++;
				/*--矢永玄及民尼永弁互反中勻化中月樺寧反域繹PET橢瘀毛民尼永弁允月 --*/
				if(strstr( work, "PET") != NULL) {
					if(strstr( work, "EV") != NULL) {
						if(NPC_PetLvCheck( meindex, talker, work, 1) == FALSE) return FALSE;
					}else{
						if(NPC_PetLvCheck( meindex, talker, work, 0) == FALSE) return FALSE;
					}
				}
			}
			j = 1;
			
			while(getStringFromIndexWithDelim( buff2, ",", j, work, sizeof( work)) != FALSE)
			{
				j++;
				if(strstr(work,"*") != NULL)
				{
					int cnt=0;
					int petcnt=0;
					int petindex;
					
					getStringFromIndexWithDelim( work, "*", 2, buf4, sizeof( buf4));
					cnt = atoi(buf4);
					
					for(i=0 ; i < CHAR_MAXPETHAVE ; i++){
						petindex = CHAR_getCharPet( talker, i);
		
						if( petindex == -1  )  continue;
		
						if(strstr( work, "EV") != NULL) {
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 1) == FALSE) {
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 0) == FALSE) {
								continue;
							}
						}

						if(NPC_EventDelPet( meindex, talker,i) == FALSE) return FALSE;
						
						petcnt++;
						/*--隙爛蜊醒坌及矢永玄毛壅仄凶井升丹井〞--*/
						if(cnt == petcnt) break;
					}
				}else{
					/*--矢永玄毛綽輪允月醒毛隙爛仄化卅中及匹竟癲矢永玄毛蟈化壅允--*/

					int petindex;

					for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
						petindex = CHAR_getCharPet( talker, i);

					    if( !CHAR_CHECKINDEX( petindex) )  continue;

						if(strstr( buff2, "EV") != NULL){
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 1) == FALSE){
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 0) == FALSE) {
								continue;
							}
						}
						if(NPC_EventDelPet( meindex, talker, i) == FALSE) return FALSE;
					}
				}
			}
		}
	}
#endif
	return TRUE;

}


/*--------------------------------------
 *仿件母丞匹失奶  丞毛必永玄今六月
 ------------------------------------ */
BOOL NPC_RandItemGet(int meidex,int talker,int rand_j,char *buf)
{
	char buff2[64];
	int randitem;
	int ret;
	int itemindex;
	char token[128];

	if(rand_j == 0) {
		print("Event:由於０的介入，出現錯誤。");
 		return FALSE;
 	}
 	
	randitem = rand()%rand_j;
	if(randitem == 0) randitem = rand_j;

	getStringFromIndexWithDelim(buf , "," , randitem, buff2, sizeof(buff2)) ;

	itemindex = ITEM_makeItemAndRegist( atoi( buff2));

	if(itemindex == -1) return FALSE;
	
	/*失奶  丞及馨笛(  濤失奶  丞  卞中木化仄引丹  */
	ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
	if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
		print( "npc_exchangeman.c: additem error itemindex[%d]\n", itemindex);
		ITEM_endExistItemsOne( itemindex);
		return FALSE;
	}

	if(itemindex != -1) {
		LogItem(
			CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
			CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名稱
			itemindex,
#else
			ITEM_getInt( itemindex, ITEM_ID),  /* 失奶  丞  寞 */
#endif
			"EventAddItem(任務需求所得到的道具)",
			CHAR_getInt( talker,CHAR_FLOOR),
			CHAR_getInt( talker,CHAR_X ),
 			CHAR_getInt( talker,CHAR_Y ),
            ITEM_getChar( itemindex, ITEM_UNIQUECODE),
			ITEM_getChar( itemindex, ITEM_NAME),
			ITEM_getInt( itemindex, ITEM_ID)
		);
	}
					
	sprintf(token,"收下了%s",ITEM_getChar( itemindex, ITEM_NAME));
	CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);

	CHAR_sendItemDataOne( talker, ret);
	return TRUE;

}


/*----------------------------------------------------
 * 奶矛件玄
 ----------------------------------------------------*/
BOOL NPC_AcceptDel(int meindex,int talker,int mode )
{

	char buf[1024*2];
	char buff2[256];
	int rand_j = 0;
	int j = 0;
	int evcnt;
	int b_flg = 0;
	evcnt = NPC_EventFile( meindex, talker, buf);
	if(evcnt == -1) return FALSE;

	/*--澀爛白央奶伙  卞井井木化中月橢瘀毛  凶仄化中月手及毛  五請允--*/
//	if(NPC_EventFile(meindex,talker,buf)==FALSE) return FALSE;

	/*--失奶  丞及醒毛民尼永弁--*/
	if(NPC_ItemFullCheck( meindex, talker, buf, mode, evcnt) == FALSE){
		NPC_MsgDisp( meindex, talker, 4);
		return FALSE;
	}
	
	/*--雲嗯民尼永弁卞  月--*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		stone = NPC_EventGetCost( meindex, talker, buff2);

		if( (CHAR_getInt( talker, CHAR_GOLD) - stone) < 0 ) {
			NPC_MsgDisp( meindex, talker, 14);
			return FALSE;
		}
	}


	/*--雲嗯民尼永弁卞  月--*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetStone", buff2, sizeof( buff2) )!=NULL){
		int stone;
		stone = atoi(buff2);

		if( (CHAR_getInt( talker, CHAR_GOLD) + stone) >= CHAR_getMaxHaveGold( talker) ) {
			NPC_MsgDisp( meindex, talker, 13);
			return FALSE;
		}
	}


	/*--矢永玄及  毛馨笛允月--*/
	if((NPC_Util_GetStrFromStrWithDelim( buf, "pet_skill", buff2, sizeof( buff2) )
	 !=NULL) && mode == 0 ){
		if(mode == 0){
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
			NPC_PetSkillMakeStr( meindex, talker, 1);
			return TRUE;
		}
	}

	/*--矢永玄毛壅允---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelPet", buff2, sizeof( buff2) ) != NULL) {
		char buf4[32];
		char buf5[256];
		int i=1;
		char work[256];
		int j=1;
		
		if(strstr(buff2,"EVDEL") != NULL){
			NPC_Util_GetStrFromStrWithDelim( buf, "EVENT", buf5, sizeof( buf5));
			getStringFromIndexWithDelim( buf5, ",",evcnt , buff2, sizeof( buff2));
		
		
			/*--手丹1蘸｝矢永玄毛  勻化中月井及民尼永弁毛墊丹--*/
			while(getStringFromIndexWithDelim( buff2, "&", j, work, sizeof( work))
			!=FALSE)
			{
				j++;
				
				/*--矢永玄及民尼永弁互反中勻化中月樺寧反域繹PET橢瘀毛民尼永弁允月 --*/
				if(strstr( work, "PET") != NULL) {
					if(strstr( work, "EV") != NULL) {
						if(NPC_PetLvCheck( meindex, talker, work, 1) == FALSE) return FALSE;
					}else{
						if(NPC_PetLvCheck( meindex, talker, work, 0) == FALSE) return FALSE;
					}
				}
			}
			j = 1;
			
			
			while(getStringFromIndexWithDelim( buff2, "&", j, work, sizeof( work))
			!= FALSE)
			{
				j++;
				if(strstr( work, "PET") == NULL ) continue;
	
				if(strstr(work,"*") != NULL)
				{
					int cnt=0;
					int petcnt=0;
					int petindex;
					
					getStringFromIndexWithDelim( work, "*", 2, buf4, sizeof( buf4));
					cnt = atoi(buf4);
					
					for(i=0 ; i < CHAR_MAXPETHAVE ; i++){
						petindex = CHAR_getCharPet( talker, i);
		
						if( petindex == -1  )  continue;
		
						if(strstr( work, "EV") != NULL) {
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 1)
							== FALSE) {
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 0) 
							== FALSE) {
								continue;
							}
						}

						if(NPC_EventDelPet( meindex, talker,i) == FALSE) return FALSE;
						
						petcnt++;
						/*--隙爛蜊醒坌及矢永玄毛壅仄凶井升丹井〞--*/
						if(cnt == petcnt) break;

					}
				}else{
					/*--矢永玄毛綽輪允月醒毛隙爛仄化卅中及匹竟癲矢永玄毛蟈化壅允--*/

					int petindex;

					for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
						petindex = CHAR_getCharPet( talker, i);

					    if( !CHAR_CHECKINDEX( petindex) )  continue;

						if(strstr( buff2, "EV") != NULL){
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 1)
							== FALSE){
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 0)
							== FALSE) {
								continue;
							}
						}
					
						if(NPC_EventDelPet( meindex, talker, i) == FALSE) return FALSE;
					}
				}
			}
		
		}else{
		
			/*--手丹1蘸｝矢永玄毛  勻化中月井及民尼永弁毛墊丹--*/
			while(getStringFromIndexWithDelim( buff2, ",", j, work, sizeof( work))
			!=FALSE)
			{
				j++;
				/*--矢永玄及民尼永弁互反中勻化中月樺寧反域繹PET橢瘀毛民尼永弁允月 --*/
				if(strstr( work, "PET") != NULL) {
					if(strstr( work, "EV") != NULL) {
						if(NPC_PetLvCheck( meindex, talker, work, 1) == FALSE) return FALSE;
					}else{
						if(NPC_PetLvCheck( meindex, talker, work, 0) == FALSE) return FALSE;
					}
				}
			}
			j = 1;
			
			
			while(getStringFromIndexWithDelim( buff2, ",", j, work, sizeof( work))
			!= FALSE)
			{
				j++;
				if(strstr(work,"*") != NULL)
				{
					int cnt=0;
					int petcnt=0;
					int petindex;
					
					getStringFromIndexWithDelim( work, "*", 2, buf4, sizeof( buf4));
					cnt = atoi(buf4);
					
					for(i=0 ; i < CHAR_MAXPETHAVE ; i++){
						petindex = CHAR_getCharPet( talker, i);
		
						if( petindex == -1  )  continue;
		
						if(strstr( work, "EV") != NULL) {
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 1)
							== FALSE) {
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 0) 
							== FALSE) {
								continue;
							}
						}

						if(NPC_EventDelPet( meindex, talker,i) == FALSE) return FALSE;
						
						petcnt++;
						/*--隙爛蜊醒坌及矢永玄毛壅仄凶井升丹井〞--*/
						if(cnt == petcnt) break;

					}
				}else{
					/*--矢永玄毛綽輪允月醒毛隙爛仄化卅中及匹竟癲矢永玄毛蟈化壅允--*/

					int petindex;

					for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
						petindex = CHAR_getCharPet( talker, i);

					    if( !CHAR_CHECKINDEX( petindex) )  continue;

						if(strstr( buff2, "EV") != NULL){
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 1)
							== FALSE){
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 0)
							== FALSE) {
								continue;
							}
						}
					
						if(NPC_EventDelPet( meindex, talker, i) == FALSE) return FALSE;
					}
				}
			}
		}
	}
	


	/*--雲嗯毛丐僕月---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		char token[128];

		stone = atoi(buff2);
		CHAR_setInt(talker,CHAR_GOLD,CHAR_getInt( talker, CHAR_GOLD) + stone);
		/*--由仿丟□正霜曰--*/
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
	
		sprintf( token,"得到%d的stone", stone);
		CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
	}


	/*--矢永玄毛馨笛允月---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetPet", buff2, sizeof( buff2) )
	!=NULL)
	{
		if(NPC_EventAddPet( meindex, talker, buff2,1) == FALSE) {
			NPC_MsgDisp( meindex, talker, 10);
			return FALSE;
		}
	}
#ifdef _JZ_TRANSX
	if(NPC_Util_GetStrFromStrWithDelim( buf, "JZ_TRANS", buff2, sizeof( buff2) )
	!=NULL)
	{
		if(NPC_EventTrans( meindex, talker, buff2) == FALSE) {
			NPC_MsgDisp( meindex, talker, 10);
			return FALSE;
		}
	}
#endif
#ifdef _JZ_NEW_ADDMEMBERPOINT
	if(NPC_Util_GetStrFromStrWithDelim( buf, "BMM", buff2, sizeof( buff2) )
	!=NULL)
	{
		int randMM=0;
		char buff3[32];
		while(1){
			if(getStringFromIndexWithDelim( buff2, ",", randMM, buff3,sizeof( buff3))){
				randMM++;
			}else{
				break;
			}
		}
		if(NPC_EventBMM( meindex, talker, buff2,randMM) == FALSE) {
			NPC_MsgDisp( meindex, talker, 10);
			return FALSE;
		}
	}
#endif
	
	// Robin add 從NPC取寵物蛋
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetEgg", buff2, sizeof( buff2) )
	!=NULL)
	{
		if(NPC_EventAddEgg( meindex, talker, buff2,1) == FALSE) {
			print(" AddEgg_Error:%d ", __LINE__);
			NPC_MsgDisp( meindex, talker, 10);
			return FALSE;
		}
	}

	/*--失奶  丞毛壅允--*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelItem", buff2, sizeof( buff2) )	!= NULL){
		if(strstr(buff2,"EVDEL") != NULL){
			char work[256];
			char nbuf[256];
		
			NPC_Util_GetStrFromStrWithDelim(buf, "EVENT", buff2, sizeof( buff2));
			getStringFromIndexWithDelim( buff2, ",", evcnt, work, sizeof( work));

			if(strstr(buf,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}

			if(NPC_Util_GetStrFromStrWithDelim( buf, "NotDel", nbuf, sizeof( nbuf))	!= NULL ){
				NPC_EventDelItemEVDEL( meindex, talker, work, nbuf,b_flg);
			}else{
				NPC_EventDelItemEVDEL( meindex, talker, work, "-1",b_flg);
			}
		}else{
			if(strstr(buf,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}
			NPC_EventDelItem( meindex, talker, buff2,b_flg);
		}
	}


	/*--雲嗯毛潸月---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		char token[128];
		
		stone = NPC_EventGetCost( meindex, talker, buff2);

		CHAR_setInt(talker,CHAR_GOLD,CHAR_getInt( talker, CHAR_GOLD) - stone);
		/*--由仿丟□正霜曰--*/
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
		sprintf( token, "付出%d的stone。", stone);
		CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
	}


	/*--仿件母丞失奶  丞毛馨笛允月---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetRandItem", buff2, sizeof( buff2) )
	!=NULL)
	{
		char buf3[32];
		j = 1;
		while(getStringFromIndexWithDelim(buff2 , "," , j, buf3, sizeof( buf3))
		 != FALSE )
		{
			j++;
			rand_j++;
		}
		NPC_RandItemGet( meindex, talker, rand_j, buff2);
	}


	/*--失奶  丞毛馨笛允月---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetItem", buff2, sizeof( buff2) )
	!=NULL)
	{
		 NPC_EventAddItem( meindex, talker, buff2);
	}

	/*--由仿丟□正及霜耨--*/
	CHAR_complianceParameter( talker );
	CHAR_send_P_StatusString( talker ,
					CHAR_P_STRING_ATK|CHAR_P_STRING_DEF|
					CHAR_P_STRING_CHARM|CHAR_P_STRING_QUICK|
					CHAR_P_STRING_WATER|CHAR_P_STRING_FIRE|
					CHAR_P_STRING_WIND|CHAR_P_STRING_EARTH
				);
	
	return TRUE;

}


/*--------------------------
 * 奶矛件玄矢永玄毛綽輪
 ---------------------------*/
BOOL NPC_EventDelPet(int  meindex,int  talker, int petsel)
{

	int petindex;
	char szPet[128];
	int defpet;
	char msgbuf[64];

	int fd = getfdFromCharaIndex( talker );

	petindex = CHAR_getCharPet( talker, petsel);

    if( !CHAR_CHECKINDEX(petindex) ) return FALSE;

	if( CHAR_getInt( talker, CHAR_RIDEPET) == petsel ) {

		//CHAR_talkToCli( talker, -1, "騎乘中的寵物無法交出！", CHAR_COLORYELLOW );
    	//return	FALSE;

		CHAR_setInt( talker, CHAR_RIDEPET, -1);
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_RIDEPET );
		CHAR_complianceParameter( talker );
		CHAR_sendCToArroundCharacter( CHAR_getWorkInt( talker , CHAR_WORKOBJINDEX ));
		print(" DelRidePet ");
	}
	print(" EventDelPet ");

	/*--公及平乓仿互爵    卅日｝矢永玄毛丹日內蔽  --*/
	if( CHAR_getWorkInt( CONNECT_getCharaindex( fd),
   	                     CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE) return FALSE;
	/*--公及矢永玄互田玄伙井升丹井及民尼永弁--*/
	defpet = CHAR_getInt( talker, CHAR_DEFAULTPET);
	if(defpet == petsel){
		CHAR_setInt( talker, CHAR_DEFAULTPET, -1);
		lssproto_KS_send( fd, -1, TRUE);
	}

	snprintf( msgbuf,sizeof( msgbuf), "交出%s。",
									CHAR_getChar( petindex, CHAR_NAME));
	CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORWHITE);

	// 矢永玄毛澎  仄凶夫弘
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"EvnetDell(任務刪除)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);


	/*   切  井日壅允 */
	CHAR_setCharPet( talker, petsel, -1);

	CHAR_endCharOneArray( petindex );

	snprintf( szPet, sizeof( szPet ), "K%d", petsel);
	// 蕙仄中矢永玄旦  □正旦霜曰勾仃月
	CHAR_sendStatusString( talker, szPet );

	return TRUE;

}


/*----------------------------
 *  矢永玄毛馨笛允月
 ------------------------------*/
BOOL NPC_EventAddPet(int meindex, int talker, char *buff2,int mode)
{
	int	ret;
	char msgbuf[64];
	int	enemynum;
	int	enemyid;
	int	i;
	int petindex;

	/*--矢永玄  互釩中化中月井及民尼永弁--*/
	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )  break;
	}

	if(i == CHAR_MAXPETHAVE) return FALSE;


	/* 婁醒及ID井日index毛潸   */
	if(strstr(buff2,",") != NULL){
		char buf2[16];
		while(getStringFromIndexWithDelim( buff2, ",", i, buf2, sizeof( buf2))
		!= FALSE) {
			i++;
		}
		i--;
		i = rand()%i + 1;
		getStringFromIndexWithDelim( buff2, ",", i, buf2, sizeof( buf2));
		enemyid = atoi( buf2);
	}else{
		enemyid = atoi( buff2);
	}
	
	enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID) == enemyid) {
			break;
		}
	}

	if( i == enemynum ) return FALSE;

	ret = ENEMY_createPetFromEnemyIndex( talker, i);

	for( i = 0; i < CHAR_MAXPETHAVE; i ++ ){
		if( CHAR_getCharPet( talker, i ) == ret )break;
	}
	if( i == CHAR_MAXPETHAVE ) i = 0;
	if( CHAR_CHECKINDEX( ret ) == TRUE ){
		CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
	}

	petindex = CHAR_getCharPet(talker,i);

    if( !CHAR_CHECKINDEX( petindex) )return FALSE;
	CHAR_complianceParameter( petindex );
	snprintf( msgbuf, sizeof( msgbuf ), "K%d", i );
	CHAR_sendStatusString( talker, msgbuf );

	snprintf( msgbuf, sizeof( msgbuf ), "W%d", i );
	CHAR_sendStatusString( talker, msgbuf );
	snprintf( msgbuf,sizeof( msgbuf), "拿到%s。",
								CHAR_getChar(petindex,CHAR_NAME));
	CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORWHITE);

	// 矢永玄毛澎卞  木凶夫弘
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"EventGet(任務得到)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);



	if(mode == 0) {
		/*--奶矛件玄迕及矢永玄--*/
		CHAR_setInt( petindex, CHAR_ENDEVENT, 1);
	}

	return TRUE;

}

// Robin add NPC給寵物蛋
BOOL NPC_EventAddEgg(int meindex, int talker, char *buff2,int mode)
{
	int	ret;
	char msgbuf[64];
	int	enemynum;
	int	enemyid;
	int	petid;
	int raise;
	int	i;
	int petindex;
	char buf2[32];
	char buf3[32];

	print("\n 給蛋啦!!:%s ", buff2);

	/*--矢永玄  互釩中化中月井及民尼永弁--*/
	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )  break;
	}

	if(i == CHAR_MAXPETHAVE) {
		print(" AddEgg_Error:%d ", __LINE__);
		return FALSE;
	}


	/* 婁醒及ID井日index毛潸   */
	if(strstr(buff2,",") != NULL){ // 是否為隨機給與
		while(getStringFromIndexWithDelim( buff2, ",", i, buf2, sizeof( buf2))
		!= FALSE) {
			i++;
		}
		i--;
		i = rand()%i + 1;
		getStringFromIndexWithDelim( buff2, ",", i, buf2, sizeof( buf2));

	}else{
		strcpy( buf2, buff2);
	}

	getStringFromIndexWithDelim( buf2, ";", 1, buf3, sizeof( buf3));
	enemyid = atoi( buf3); // 寵物蛋ID
	getStringFromIndexWithDelim( buf2, ";", 2, buf3, sizeof( buf3));
	petid = atoi( buf3); // 孵出的寵物ID
	getStringFromIndexWithDelim( buf2, ";", 3, buf3, sizeof( buf3));
	raise = atoi( buf3); //   養次數


	// 將enemyid轉成enemybaseid
	enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID) == petid) {
			break;
		}
	}
	if( i == enemynum ) {
		print(" AddEgg_Error:%d ", __LINE__);
		return FALSE;
	}
	petid = ENEMY_getInt( i, ENEMY_TEMPNO);


	enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID) == enemyid) {
			break;
		}
	}
	if( i == enemynum ) {
		print(" AddEgg_Error:%d ", __LINE__);
		return FALSE;
	}

	ret = ENEMY_createPetFromEnemyIndex( talker, i);

	for( i = 0; i < CHAR_MAXPETHAVE; i ++ ){
		if( CHAR_getCharPet( talker, i ) == ret )break;
	}
	if( i == CHAR_MAXPETHAVE ) i = 0;
	if( CHAR_CHECKINDEX( ret ) == TRUE ){
		CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
	}

	petindex = CHAR_getCharPet(talker,i);

    if( !CHAR_CHECKINDEX( petindex) ) {
		print(" AddEgg_Error:%d ", __LINE__);
		return FALSE;
	}

	// 設為寵物蛋
	CHAR_setInt( petindex, CHAR_FUSIONBEIT, 1);
	CHAR_setInt( petindex, CHAR_FUSIONCODE, -1);
	CHAR_setInt( petindex, CHAR_FUSIONRAISE, raise);
	CHAR_setInt( petindex, CHAR_FUSIONINDEX, petid);
	CHAR_setInt( petindex, CHAR_FUSIONTIMELIMIT, (int)time( NULL));
	
	CHAR_complianceParameter( petindex );
	snprintf( msgbuf, sizeof( msgbuf ), "K%d", i );
	CHAR_sendStatusString( talker, msgbuf );

	snprintf( msgbuf, sizeof( msgbuf ), "W%d", i );
	CHAR_sendStatusString( talker, msgbuf );
	snprintf( msgbuf,sizeof( msgbuf), "拿到%s。",
								CHAR_getChar(petindex,CHAR_NAME));
	CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORWHITE);

	// 矢永玄毛澎卞  木凶夫弘
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"EventGetEgg(任務得到)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);



	if(mode == 0) {
		/*--奶矛件玄迕及矢永玄--*/
		CHAR_setInt( petindex, CHAR_ENDEVENT, 1);
	}

	return TRUE;

}


/*---------------------------
 *  奶矛件玄  毛綽輪允月
 -----------------------------**/
BOOL NPC_EventDelItem(int meindex,int talker,char *buf,int breakflg)
{

	int i = 1, j = 1, k = 1 ;
	char buff3[128];
	char buf2[32];
	int itemindex;
	char token[256];

	while(getStringFromIndexWithDelim(buf , "," , k, buff3, sizeof(buff3))
	 !=FALSE )
	{
		k++;
		if(strstr(buff3,"*") != NULL) {
			int itemno;
			int kosuu;
			int id;
			int cnt=0;
				
			getStringFromIndexWithDelim( buff3, "*", 1, buf2, sizeof( buf2));
			itemno = atoi( buf2); 
			getStringFromIndexWithDelim( buff3, "*", 2, buf2, sizeof( buf2));
			kosuu = atoi( buf2);
	 
			if( breakflg != 1 )
                NPC_ActionDoPileDelItem( talker, itemno, kosuu);//有堆疊的處理
            else{
			    for( i =0 ; i < CHAR_MAXITEMHAVE ; i++ ){
				    itemindex = CHAR_getItemIndex( talker , i );
				    if( ITEM_CHECKINDEX( itemindex) ) {
					    id = ITEM_getInt( itemindex , ITEM_ID );
					    if(itemno == id) {
						    cnt++;
						    LogItem(
							CHAR_getChar( talker, CHAR_NAME ), // 平乓仿   
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名稱
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),  // 失奶  丞  寞 
#endif
							"EventDelItem(任務需求所收回的道具)",
							CHAR_getInt( talker, CHAR_FLOOR),
							CHAR_getInt( talker, CHAR_X ),
 							CHAR_getInt( talker, CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
	     					);
	 
		    				if(breakflg == 1){
			    			    sprintf(token,"%s 壞了",ITEM_getChar( itemindex, ITEM_NAME));
							    CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
							}else{
							    sprintf(token,"交出%s",ITEM_getChar( itemindex, ITEM_NAME));
							    CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
							}
						    //--失奶  丞毛坫誚--
						    CHAR_setItemIndex( talker, i ,-1);
						    ITEM_endExistItemsOne(itemindex);
						    CHAR_sendItemDataOne( talker, i);

						    if(cnt == kosuu){
							    break;
							}
						}
					}
				}
			}
		}
		else{
			/*--薊氏分  寞及失奶  丞毛薊請---*/
			for( j = 0 ;  j < CHAR_MAXITEMHAVE ; j++){
				itemindex = CHAR_getItemIndex( talker ,j);

				if( ITEM_CHECKINDEX( itemindex)){
					if( atoi( buff3) == ITEM_getInt( itemindex, ITEM_ID)) {
						LogItem(
							CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名稱
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),  /* 失奶  丞  寞 */
#endif
							"EventDelItem(任務需求所收回的道具)",
							CHAR_getInt( talker,CHAR_FLOOR),
							CHAR_getInt( talker,CHAR_X ),
							CHAR_getInt( talker,CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
						);
						if(breakflg == 1){
								sprintf(token,"%s 壞了",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}else{
							sprintf( token, "交出%s。", 
											ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}
						/*--失奶  丞毛坫誚--*/
						CHAR_setItemIndex( talker, j ,-1);
						ITEM_endExistItemsOne( itemindex);
						/*--由仿丟□正霜耨--*/
						CHAR_sendItemDataOne( talker, j);
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL NPC_EventDelItemEVDEL(int meindex,int talker,char *buf,char *nbuf,int breakflg)
{

	int k = 1, l = 1;
	char buff3[128];
	char buf2[32];
	char buf4[32];



	while(getStringFromIndexWithDelim(buf , "&" , k, buf4, sizeof(buf4)) !=FALSE ){
		int itemno;
		k++;
		if(strstr(buf4,"ITEM") == NULL) continue;
		getStringFromIndexWithDelim(buf4 , "=" , 2, buff3, sizeof(buff3));

		if(strstr(buff3,"*") != NULL) {
			int kosuu;
#ifndef _ITEM_PILENUMS
			char token[256];
			int i, itemindex, id, cnt;
#endif
			getStringFromIndexWithDelim( buff3, "*", 1, buf2, sizeof( buf2));
			itemno = atoi( buf2); 
			getStringFromIndexWithDelim( buff3, "*", 2, buf2, sizeof( buf2));
			kosuu = atoi( buf2);
			if(strstr(nbuf,"-1") == NULL){
				l = 1;
				while(getStringFromIndexWithDelim(nbuf , "," , l, buf2, sizeof(buf2))){
					l++;
					if(itemno == atoi( buf2)){
						l = -1;
						break;
					}
				}
				if(l == -1) continue;
			}
#ifdef _ITEM_PILENUMS
			NPC_ActionDoPileDelItem( talker, itemno, kosuu);
#else
			cnt = 0;
			for( i =0 ; i < CHAR_MAXITEMHAVE ; i++ ){
				itemindex = CHAR_getItemIndex( talker , i );
				if( ITEM_CHECKINDEX( itemindex) ) {
					id = ITEM_getInt( itemindex , ITEM_ID );
					if(itemno == id) {
						cnt++;
						LogItem(
							CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名稱
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),  /* 失奶  丞  寞 */
#endif
							"EventDelItem(任務需求所收回的道具)",
							CHAR_getInt( talker, CHAR_FLOOR),
							CHAR_getInt( talker, CHAR_X ),
 							CHAR_getInt( talker, CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
						);
						if(breakflg == 1){
							sprintf(token,"%s 壞了",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}else{
							sprintf(token,"交出%s",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}
						CHAR_setItemIndex( talker, i ,-1);
						ITEM_endExistItemsOne(itemindex);
						CHAR_sendItemDataOne( talker, i);
						if(cnt == kosuu){
							break;
						}
					}
				}
			}		
#endif
		}else{
#ifndef _ITEM_PILENUMS
			int j, itemindex;
			char token[256];
#endif
			if(strstr(nbuf,"-1") == NULL){
				l = 1;
				while(getStringFromIndexWithDelim(nbuf , "," , l, buf2, sizeof(buf2))){
					l++;
					if(atoi(buff3) == atoi( buf2)){
						l = -1;
						break;
					}
				}
				if(l == -1) continue;
			}
			itemno = -1;
#ifdef _ITEM_PILENUMS
			NPC_ActionDoPileDelItem( talker, itemno, itemno);
#else
			for( j = 0 ;  j < CHAR_MAXITEMHAVE ; j++){
				itemindex = CHAR_getItemIndex( talker ,j);
				if( ITEM_CHECKINDEX( itemindex)){
					if( atoi( buff3) == ITEM_getInt( itemindex, ITEM_ID)) {
						LogItem(
							CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名稱
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),  /* 失奶  丞  寞 */
#endif
							"EventDelItem(任務需求所收回的道具)",
							CHAR_getInt( talker,CHAR_FLOOR),
							CHAR_getInt( talker,CHAR_X ),
							CHAR_getInt( talker,CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
						);
						if(breakflg == 1){
							sprintf(token,"%s 壞了",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}else{
							sprintf( token, "交出%s。", 
										ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}
						CHAR_setItemIndex( talker, j ,-1);
						ITEM_endExistItemsOne( itemindex);
						CHAR_sendItemDataOne( talker, j);
					}
				}
			}
#endif
		}
	}

	return TRUE;
}


/*----------------------
 * 奶矛件玄  毛馨笛]
 -----------------------*/
BOOL NPC_EventAddItem(int meindex,int talker,char *buf)
{

	char buff3[128];
	int i = 1;
	int itemindex;
	char buf3[32];
	int ret;
	char token[256];

	while(getStringFromIndexWithDelim(buf , "," , i, buff3, sizeof( buff3)) 
	!= FALSE )
	{
		i++;
		if(strstr( buff3, "*") != NULL) {
			
			int itemno;
			int kosuu;
			int loop = 0;

			getStringFromIndexWithDelim( buff3, "*", 1, buf3,sizeof( buf3));
			itemno = atoi( buf3);
			getStringFromIndexWithDelim( buff3, "*", 2, buf3,sizeof( buf3));
			kosuu = atoi( buf3);
	
			for(loop = 0 ; loop < kosuu ; loop++) {
				itemindex = ITEM_makeItemAndRegist( itemno);
	
				if(itemindex == -1) return FALSE;
	
				/*失奶  丞及馨笛(  濤失奶  丞  卞中木化仄引丹  */
				ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
				if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
				    print("npc_exchange:ACCEPTadditem error itemindex[%d]\n",itemindex);
					ITEM_endExistItemsOne( itemindex);
					return FALSE;
				}

				if(itemindex != -1) {
					LogItem(
						CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
						CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名稱
						itemindex,
#else
	    				ITEM_getInt( itemindex, ITEM_ID),  /* 失奶  丞  寞 */
#endif
						"EventAddItem(任務需求所得到的道具)",
						CHAR_getInt( talker, CHAR_FLOOR),
						CHAR_getInt( talker, CHAR_X ),
						CHAR_getInt( talker, CHAR_Y ),
						ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)
					);
				}
				sprintf( token, "拿到%s。", ITEM_getChar( itemindex, ITEM_NAME));
				CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);

				CHAR_sendItemDataOne( talker, ret);
			}
		}else{
			itemindex = ITEM_makeItemAndRegist( atoi( buff3));
	
			if(itemindex == -1) return FALSE;

			/*失奶  丞及馨笛(  濤失奶  丞  卞中木化仄引丹  */
			ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
			if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
				print( "npc_exchange.c: ACCEPTadditem error itemindex[%d]\n", itemindex);
				ITEM_endExistItemsOne( itemindex);
				return FALSE;
			}

			if(itemindex != -1) {
				LogItem(
				CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
				CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名稱
				itemindex,
#else
	       		ITEM_getInt( itemindex, ITEM_ID),  /* 失奶  丞  寞 */
#endif
				"EventAddItem(任務需求所得到的道具)",
				CHAR_getInt( talker, CHAR_FLOOR),
				CHAR_getInt( talker, CHAR_X ),
 				CHAR_getInt( talker, CHAR_Y ),
				ITEM_getChar( itemindex, ITEM_UNIQUECODE),
				ITEM_getChar( itemindex, ITEM_NAME),
				ITEM_getInt( itemindex, ITEM_ID)
			);
			}
			sprintf(token,"拿到%s。", ITEM_getChar( itemindex, ITEM_NAME));
			CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);

			CHAR_sendItemDataOne( talker, ret);
		}
	}

	return TRUE;
}


/*---------------------------------
 *橢瘀毛  凶仄化中月及毛  五請允
 ------------- ---------------------*/
int NPC_EventFile(int meindex,int talker,char *arg)
{

	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buff2[256];
	char buf[1024*2];
	int i=1;
	int EvNo=0;
	int evch;
	
	/*--澀爛白央奶伙毛  心  戈--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {

		print("GetArgStrErr");
		return -1;
	}

	/*澀爛白央奶伙毛坌喃允月*/
	while(getStringFromIndexWithDelim( argstr, "EventEnd", i, buf,sizeof( buf))
	!= FALSE)
	{
		CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);
		i++;
		/*--奶矛件玄瓜件田□毛必永玄--*/
		NPC_Util_GetStrFromStrWithDelim( buf, "EventNo", buff2,sizeof( buff2) );
		EvNo = atoi( buff2);

		/*--白仿弘民尼永弁毛墊丹〔允匹卞｝仇及奶矛件玄互蔽  仄化中月樺寧反戚尺--*/
		if(NPC_EventCheckFlg( talker, EvNo) == TRUE) continue;

		/*--平□伐□玉及民尼永弁毛墊丹--*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"Pet_Name", buff2,sizeof( buff2) ) 
		 != NULL)
		{
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 1);
		}
		/*--平□伐□玉及民尼永弁毛墊丹--*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"KeyWord", buff2,sizeof( buff2) ) 
		 != NULL)
		{
			if(CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) != i ) continue;
		}

		evch = NPC_ExChangeManEventCheck( meindex, talker, buf);

		if(evch != -1) {
		/*--奶矛件玄橢瘀及民尼永弁--*/
//		if(NPC_ExChangeManEventCheck( meindex, talker, buf) == TRUE) {
			strcpysafe(arg, sizeof( buf) ,buf);
			return evch;
		}
	}

	return -1;

}


/*---------------------------------
 * 失奶  丞及民尼永弁分仃毛墊丹
 -----------------------------------*/
BOOL NPC_EventReduce(int meindex,int talker,char *buf)
{

	char buf2[512];
	char buf3[256];
	int id = 0;
	int i;
	int itemindex;
	int itemno;
	int kosuu;
	int cnt = 0;
	
	getStringFromIndexWithDelim( buf, "=", 2, buf2, sizeof( buf2));
	getStringFromIndexWithDelim( buf2, "*", 1, buf3, sizeof( buf3));
	itemno = atoi( buf3);
	getStringFromIndexWithDelim( buf2, "*", 2, buf3, sizeof( buf3));
	kosuu = atoi( buf3);
	
	for( i = 0 ; i < CHAR_MAXITEMHAVE ; i++ ){
		itemindex = CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX( itemindex) ){
			id = ITEM_getInt( itemindex ,ITEM_ID);
			if(itemno == id){
				//change add 增加對堆疊的判斷
				int pilenum = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
				if( pilenum )
					cnt+=pilenum;
				else
					cnt++;

				if(cnt >= kosuu){
					return TRUE;
				}
			}
		}
	}

	return FALSE;

}

/*----------------------------------
 *矢永玄卞旦平伙毛創尹今六月
 ----------------------------------*/
void NPC_EventPetSkill( int meindex, int talker, char *data)
{

	int skill;
	int pet;
	int slot;
	int cost;
	int skillID = 0;
	char buf[64];
	char argstr[1024];
	char msg[512];
	int petindex;
	char msgbuf[128];
	int fd = getfdFromCharaIndex( talker );
	int evcnt;
	
	if( NPC_Util_CharDistance( talker, meindex ) > 2) {
		return;
	}

	evcnt = NPC_EventFile( meindex, talker, argstr);

	if(evcnt == -1) return;

	/*--澀爛白央奶伙  卞嫗井k伊  中月橢瘀毛  凶仄化中月手及毛  五請允--*/
//	if(NPC_EventFile( meindex, talker, argstr) == FALSE) return ;

	/*弁仿奶失件玄井日忒勻化仁月犯□正--*/
	/*升及  毛尹日氏分井〞(薊    寞)  升及矢永玄井〞(薊    寞)  
	升及旦夫永玄井〞(薊    寞)  襖蟾*/
	makeStringFromEscaped( data);

	/*--犯□正及蕊曰坌仃--*/
	getStringFromIndexWithDelim( data, "|", 1, buf, sizeof( buf));
	skill = atoi( buf);
	getStringFromIndexWithDelim( data, "|", 2, buf, sizeof( buf));
	pet = atoi( buf);
	getStringFromIndexWithDelim( data, "|", 3, buf ,sizeof( buf));
	slot = atoi(buf);
	getStringFromIndexWithDelim( data, "|", 4, buf, sizeof( buf));
	cost = atoi( buf);

	/*--旦平伙    毛必永玄允月--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "pet_skill", 
									msg, sizeof( msg)) != NULL) 
	{
		getStringFromIndexWithDelim( msg, ",", skill, buf ,sizeof( buf));
		skillID = atoi( buf);
	}
	slot--;


	petindex = CHAR_getCharPet( talker, pet-1);

	if(petindex == -1){
		print("PetindexErr");
		return;
	}

	/*--公及平乓仿互爵    卅日｝矢永玄毛丹日內蔽  --*/
	if( CHAR_getWorkInt( CONNECT_getCharaindex( fd),
                         CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE) return ;

	/*--矢永玄卞  毛創尹今六月--*/
	CHAR_setPetSkill( petindex, slot, skillID);
			
	/*--雲嗯毛蛹日允--*/
	CHAR_setInt( talker, CHAR_GOLD, (CHAR_getInt( talker, CHAR_GOLD) - cost));

	snprintf( msgbuf, sizeof( msgbuf ), "W%d",pet-1);

	// 蕙仄中矢永玄及  犯□正毛霜曰勾仃月
	CHAR_sendStatusString( talker, msgbuf );
			
	CHAR_sendStatusString( talker, "P");

}


/*----------------------------------------
 *  失奶  丞互中勻天中井升丹井及民尼永弁
----------- ------------------------------*/
BOOL NPC_ItemFullCheck(int meindex,int talker,char *buf,int mode,int evcnt)
{

	char buff2[256];
	int i = 1,j = 1;
	int maxitem = 0;
	int kosuucnt = 0;
	int itemindex;
	char buf3[256];
	int rand_j = 0;
	int rand_cnt = 0;
	char nbuf[256];
	char buff6[16];
	int l = 1;
	
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelItem", buff2, sizeof( buff2) )	!= NULL ){
		if(strstr(buff2,"EVDEL") != NULL){
			char buff4[64];
			char buff5[32];
			
			i = 1;
			NPC_Util_GetStrFromStrWithDelim( buf, "EVENT", buff2, sizeof( buff2));
			getStringFromIndexWithDelim(buff2, "," , evcnt, buf3, sizeof(buf3));
			
			while(getStringFromIndexWithDelim(buf3, "&" , j, buff4, sizeof(buff4))){
				j++;
				if(strstr(buff4,"ITEM") != NULL) {
					if(strstr(buff4,"*") != NULL) {
						int itemno;

						l = 1;
						getStringFromIndexWithDelim(buff4, "=" , 2, buff5, sizeof(buff5));
						getStringFromIndexWithDelim(buff5, "*" , 1, buff6, sizeof(buff6));
						itemno = atoi(buff6);
						
						if(NPC_Util_GetStrFromStrWithDelim( buf, "NotDel", nbuf, sizeof( nbuf))	!=NULL){
							while(getStringFromIndexWithDelim(nbuf , "," , l,buff6, sizeof(buff6)))
							{
								l++;
								if(itemno == atoi( buff6))
								{
									l = -1;
									break;
								}
							}
						}
						if(l == -1) continue;
						getStringFromIndexWithDelim(buff5, "*" , 2, buff6, sizeof(buff6));
						maxitem -= atoi( buff6);
					}else{
						getStringFromIndexWithDelim(buff4, "=" , 2, buff5, sizeof(buff5));
						l = 1; 

						if(NPC_Util_GetStrFromStrWithDelim( buf, "NotDel", nbuf, sizeof( nbuf))
						!=NULL){
							while(getStringFromIndexWithDelim(nbuf , "," , l,buff6, sizeof(buff6)))	{
								l++;
								if(atoi(buff5) == atoi( buff6)){
									l = -1;
									break;
								}
							}
						}
						if(l == -1) continue;
						for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {
							itemindex = CHAR_getItemIndex( talker , i );
							if( ITEM_CHECKINDEX( itemindex)) {
								if(atoi( buff5) == ITEM_getInt( itemindex, ITEM_ID)) {
									maxitem--;
								}
							}
						}
					}
				}
			}
		}else{
			char buff3[128];
			i = 1;
			while(getStringFromIndexWithDelim(buff2, "," , i, buff3, sizeof(buff3)) !=FALSE ){
				i++;
				if(strstr( buff3, "*") != NULL){
					getStringFromIndexWithDelim( buff3, "*", 2, buf3, sizeof( buf3));
					maxitem -= atoi( buf3);
				}else{
					for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {
						itemindex=CHAR_getItemIndex( talker , i );
						if( ITEM_CHECKINDEX( itemindex)) {
							if(atoi( buff3) == ITEM_getInt( itemindex, ITEM_ID)) {
							maxitem--;
							}
						}
					}
				}
			}

		}
	}

	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetRandItem", buff2, sizeof( buff2) ) !=NULL && mode == 0){
		j = 1;
		while(getStringFromIndexWithDelim(buff2 , "," , j, buf3, sizeof( buf3)) !=FALSE ){
			j++;
			rand_j++;
		}
		rand_cnt = 1;
		for( i = CHAR_STARTITEMARRAY ; i <CHAR_MAXITEMHAVE ; i++ ) {
			itemindex = CHAR_getItemIndex( talker , i );
			if( !ITEM_CHECKINDEX(itemindex) ){
				kosuucnt++;
			}
		}
		if( maxitem == 0 && kosuucnt== 0){
			return FALSE;
		}
	}


	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetItem", buff2, sizeof( buff2) )
	!= NULL && mode == 0){
		char buff3[256];
		j = 1;
		while(getStringFromIndexWithDelim(buff2 , "," , j, buff3, sizeof( buff3)) != FALSE ){
			j++;
			if(strstr(buff3,"*") !=NULL ) {
				getStringFromIndexWithDelim( buff3, "*", 2, buf3,sizeof( buf3));
				maxitem += atoi( buf3);
			}else{
				maxitem++;
			}
		}
		kosuucnt=0;
		for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
			itemindex = CHAR_getItemIndex( talker , i );
			if( !ITEM_CHECKINDEX( itemindex)){
				kosuucnt++;
			 }
		}
		maxitem = maxitem + rand_cnt;
		if( kosuucnt < maxitem){
			return FALSE;
		}
	}


	return TRUE;
}

void NPC_CharmStatus(int meindex,int talker)
{
	int i = 0;
	int petindex;
	char petsend[64];	
	for(i = 0 ; i < CHAR_MAXPETHAVE ; i++) {
    	petindex = CHAR_getCharPet( talker, i);

		if( petindex == -1  )  continue;
		if( !CHAR_CHECKINDEX( talker ) )  continue;
		CHAR_complianceParameter( petindex );
		sprintf( petsend, "K%d", i );
		CHAR_sendStatusString( talker , petsend );
	}
}

int NPC_EventGetCost(int meindex,int talker,char *arg)
{

	int cost;
	int level;
	char buf[32];
	if(strstr( arg, "LV") != NULL) {
		level = CHAR_getInt( talker, CHAR_LV);
		getStringFromIndexWithDelim( arg, "*", 2, buf,sizeof( buf));
		cost = level * atoi( buf);
	}else{
		cost = atoi( arg);
	}
	return cost;

}
#ifdef _JZ_NEW_ADDMEMBERPOINT
BOOL NPC_EventBMM(int meindex, int talker, char *buff2,int mode)
{
	int	ret;
	char msgbuf[64], buf1[256];
	int	enemynum;
	int	enemyid;
	int		i,j;
	int level=79;
	int LevelUpPoint;
	int vital, str, tgh, dex;
	float modai;
	
	int randmm;
	int mmnum,petindex;
	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )
			break;
	}

  if( i == CHAR_MAXPETHAVE )      {
		snprintf( msgbuf,sizeof( msgbuf), "寵物已滿！！");
		CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
		return FALSE;
	}
	
	if (mode>0){
		randmm = rand()%mode;
		if(randmm == 0) randmm = mode;
		getStringFromIndexWithDelim( buff2, ",", randmm, buf1,sizeof( buf1));
		if (strstr(buf1,"*")!=NULL){
			getStringFromIndexWithDelim( buf1, "*", 1, msgbuf,sizeof( msgbuf));
			randmm = atoi(msgbuf);
			getStringFromIndexWithDelim( buf1, "*", 2, msgbuf,sizeof( msgbuf));
			mmnum = atoi(msgbuf);
		}else{
			randmm = atoi(buf1);
			mmnum = 1;
		}
		if (mmnum<1)mmnum = 1;
		if (mmnum>5)mmnum = 5;
		if(randmm== 1 ){
			enemyid = 1479;
		}else if( randmm== 2){
			enemyid = 2547;
		}else{
			print("ExChangeMan err:BMM-%s\n",buf1);
		}
		enemynum = ENEMY_getEnemyNum();
		for( i = 0; i < enemynum; i ++ ) {
			if( ENEMY_getInt( i, ENEMY_ID) == enemyid) {
				break;
			}
		}
		if( i == enemynum ) return FALSE;
		for(j = 0; j < mmnum; j ++ ) {
			ret = ENEMY_createPetFromEnemyIndex( talker, i);
			if( !CHAR_CHECKINDEX( ret))return;
			vital = 50;
			str = 50;
			tgh = 50;
			dex = 50;
			LevelUpPoint = ( vital << 24 ) + ( str << 16 ) + ( tgh << 8 ) + ( dex << 0 );
			CHAR_setInt( ret, CHAR_ALLOCPOINT , LevelUpPoint );
			print("\n lvup <<%d %d %d %d>>", vital, str, tgh, dex );

			for( i = 0; i < CHAR_MAXPETHAVE; i ++ ){
				if( CHAR_getCharPet( talker, i ) == ret )break;
			}
			if( i == CHAR_MAXPETHAVE )i = 0;

			if( CHAR_CHECKINDEX( ret ) == TRUE ){
				CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
			}
				if( level > 79 ) level = 79;
				if( level < 0 ) level = 0;
				if( level > 0 ){
					int k;
					for( k = 1; k < level; k ++ ){	//升級
						CHAR_PetLevelUp( ret );
						CHAR_PetAddVariableAi( ret, AI_FIX_PETLEVELUP );
						CHAR_setInt( ret, CHAR_LV, CHAR_getInt( ret, CHAR_LV) +1 );
					}
				}
			snprintf( msgbuf, sizeof( msgbuf ), "K%d", i );
			CHAR_sendStatusString( talker, msgbuf );
			snprintf( msgbuf, sizeof( msgbuf ), "W%d", i );
			CHAR_sendStatusString( talker, msgbuf );
		}
		snprintf( msgbuf,sizeof( msgbuf), "獲得79級MM %d 只\n",mmnum,randmm);
		CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
	}
}
#endif
#ifdef _JZ_TRANSX
BOOL NPC_EventTrans(int meindex, int talker, char *buff2)
{
	int transtemp;
	int work[10];
	if(strlen(buff2))CHAR_talkToCli( talker, -1,buff2, CHAR_COLORYELLOW);
	transtemp = CHAR_getInt(	talker, CHAR_TRANSMIGRATION)+1;
	
	if( transtemp > _JZ_TRANSX){
		CHAR_talkToCli( talker, -1,"無法再轉生了", CHAR_COLORWHITE);
		return FALSE;
	}

	CHAR_setInt(talker ,CHAR_TRANSMIGRATION,transtemp);

	NPC_TransmigrationStatus(0, talker, work);
	NPC_TransmigrationFlg_CLS(0, talker);
	CHAR_setInt(talker, CHAR_LV ,1);
	CHAR_setMaxExp( talker, 0);
   
  CHAR_setInt( talker,CHAR_SKILLUPPOINT,transtemp*10);
  CHAR_Skillupsend( talker );
  CHAR_setInt( talker , CHAR_BASEIMAGENUMBER , CHAR_getInt( talker , CHAR_BASEBASEIMAGENUMBER) );
	LogTensei(
		CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
		CHAR_getChar( talker, CHAR_CDKEY),
		"Tensei(非精靈王NPC轉生)",
		work[0],
		CHAR_getInt(talker,CHAR_TRANSMIGRATION),
		work[1],
		NPC_StartpointCheck( meindex, talker),
		0, 0,
		work[2],work[3],work[4],work[5],
		work[6],work[7],work[8],work[9]
	);
	//計算人物能力
	CHAR_complianceParameter( talker );
	CHAR_setInt(talker, CHAR_HP, CHAR_getWorkInt(talker, CHAR_WORKMAXHP) );
	CHAR_setInt(talker,CHAR_LASTTALKELDER, NPC_StartpointCheck(0,talker));
	//更新客戶端
	CHAR_sendStatusString( talker, "P" );
	CHAR_sendCToArroundCharacter( CHAR_getWorkInt( talker, CHAR_WORKOBJINDEX ));
}
#endif