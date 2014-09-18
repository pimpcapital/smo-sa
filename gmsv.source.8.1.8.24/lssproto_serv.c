#include "version.h"
#include "autil.h"
#include "lssproto_serv.h"
#include "common.h"	// for StoneAge
#include "log.h"	// for StoneAge
#include "char.h"
#include <stdio.h>
#include <time.h>
#include <net.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

void lssproto_SetServerLogFiles( char *r , char *w )
{
	lssproto_strcpysafe( lssproto_writelogfilename , w , sizeof(lssproto_writelogfilename ));
	lssproto_strcpysafe( lssproto_readlogfilename , r , sizeof(lssproto_readlogfilename ));
}
int lssproto_InitServer(	int (*writefunc)(int,char*,int) ,	int worksiz )
{
	int i;
	if( (void*)writefunc == NULL){lssproto.write_func = lssproto_default_write_wrap;} else {lssproto.write_func = writefunc;}
	lssproto_AllocateCommonWork(worksiz);
	lssproto_stringwrapper = (char**)calloc( 1,sizeof(char*) * MAXLSRPCARGS);
	if(lssproto_stringwrapper ==NULL)return -1;
	memset( lssproto_stringwrapper , 0, sizeof(char*)*MAXLSRPCARGS);
	for(i=0;i<MAXLSRPCARGS;i++){
		lssproto_stringwrapper[i] = (char*)calloc( 1, worksiz );
		if( lssproto_stringwrapper[i] == NULL){
			for(i=0;i<MAXLSRPCARGS;i++){free( lssproto_stringwrapper[i]);return -1;}
		}
	}
	lssproto.ret_work = (char*)calloc( 1,sizeof( worksiz ));
	if( lssproto.ret_work == NULL ){ return -1; }
	return 0;
}
void lssproto_CleanupServer( void )
{
	int i;
	free( lssproto.work );
	free( lssproto.arraywork);
	free( lssproto.escapework );
	free( lssproto.val_str);
	free( lssproto.token_list );
	for(i=0;i<MAXLSRPCARGS;i++){free( lssproto_stringwrapper[i]);}
	free( lssproto_stringwrapper );
	free( lssproto.ret_work );
}
#ifdef _JZ_BEST
typedef struct taglssproto_func
{
    int func;
    void*	ls_func;
    void* de_func;
}lssprotofunc;

static lssprotofunc lsprofunc[]=
{	
	{LSSPROTO_W_RECV,lssproto_W_recv,lssproto_dedata_IIC},//0
	{LSSPROTO_W2_RECV,lssproto_W2_recv,lssproto_dedata_IIC},//1
	{LSSPROTO_XYD_SEND,NULL,NULL},//2
	{LSSPROTO_EV_RECV,lssproto_EV_recv,lssproto_dedata_IIIII},//3
	{LSSPROTO_EV_SEND,NULL,NULL},//4
	{LSSPROTO_EN_RECV,NULL,NULL},//5
	{LSSPROTO_DU_RECV,lssproto_DU_recv,lssproto_dedata_II},//6
	{LSSPROTO_EN_SEND,NULL,NULL},//7
	{LSSPROTO_EO_RECV,lssproto_EO_recv,lssproto_dedata_I},//8
	{LSSPROTO_BU_RECV,lssproto_BU_recv,lssproto_dedata_I},//9
	{LSSPROTO_JB_RECV,lssproto_JB_recv,lssproto_dedata_II},//10
	{LSSPROTO_LB_RECV,lssproto_LB_recv,lssproto_dedata_II},//11
	{LSSPROTO_RS_SEND,NULL,NULL},//12
	{LSSPROTO_RD_SEND,NULL,NULL},//13
	{LSSPROTO_B_RECV,lssproto_B_recv,lssproto_dedata_C},//14
	{LSSPROTO_B_SEND,NULL,NULL},//15
	{LSSPROTO_SKD_RECV,lssproto_SKD_recv,lssproto_dedata_II},//16
	{LSSPROTO_ID_RECV,lssproto_ID_recv,lssproto_dedata_IIII},//17
	{LSSPROTO_PI_RECV,lssproto_PI_recv,lssproto_dedata_III},//18
	{LSSPROTO_DI_RECV,lssproto_DI_recv,lssproto_dedata_III},//19
	{LSSPROTO_DG_RECV,lssproto_DG_recv,lssproto_dedata_III},//20
	{LSSPROTO_DP_RECV,lssproto_DP_recv,lssproto_dedata_III},//21
	{LSSPROTO_I_SEND,NULL,NULL},//22
	{LSSPROTO_MI_RECV,lssproto_MI_recv,lssproto_dedata_II},//23
	{LSSPROTO_SI_SEND,NULL,NULL},//24
	{LSSPROTO_MSG_RECV,lssproto_MSG_recv,lssproto_dedata_ICI},//25
	{LSSPROTO_MSG_SEND,NULL,NULL},//26
	{LSSPROTO_PMSG_RECV,lssproto_PMSG_recv,lssproto_dedata_IIICI},//27
	{LSSPROTO_PME_SEND,NULL,NULL},//28
	{LSSPROTO_AB_RECV,lssproto_AB_recv,NULL},//29
	{LSSPROTO_AB_SEND,NULL,NULL},//30
	{LSSPROTO_ABI_SEND,NULL,NULL},//31
	{LSSPROTO_DAB_RECV,lssproto_DAB_recv,lssproto_dedata_I},//32
	{LSSPROTO_AAB_RECV,lssproto_AAB_recv,lssproto_dedata_II},//33
	{LSSPROTO_L_RECV,lssproto_L_recv,lssproto_dedata_I},//34
	{LSSPROTO_TK_RECV,lssproto_TK_recv,lssproto_dedata_IICII},//35
	{LSSPROTO_TK_SEND,NULL,NULL},//36
	{LSSPROTO_MC_SEND,NULL,NULL},//37
	{LSSPROTO_M_RECV,lssproto_M_recv,lssproto_dedata_IIIII},//38
	{LSSPROTO_M_SEND,NULL,NULL},//39
	{LSSPROTO_C_RECV,lssproto_C_recv,lssproto_dedata_I},//40
	{LSSPROTO_C_SEND,NULL,NULL},//41
	{LSSPROTO_CA_SEND,NULL,NULL},//42
	{LSSPROTO_CD_SEND,NULL,NULL},//43
	{LSSPROTO_R_SEND,NULL,NULL},//44
	{LSSPROTO_S_RECV,lssproto_S_recv,lssproto_dedata_C},//45
	{LSSPROTO_S_SEND,NULL,NULL},//46
	{LSSPROTO_D_SEND,NULL,NULL},//47
	{LSSPROTO_FS_RECV,lssproto_FS_recv,lssproto_dedata_I},//48
	{LSSPROTO_FS_SEND,NULL,NULL},//49
	{LSSPROTO_HL_RECV,lssproto_HL_recv,lssproto_dedata_I},//50
	{LSSPROTO_HL_SEND,NULL,NULL},//51
	{LSSPROTO_PR_RECV,lssproto_PR_recv,lssproto_dedata_III},//52
	{LSSPROTO_PR_SEND,NULL,NULL},//53
	{LSSPROTO_KS_RECV,lssproto_KS_recv,lssproto_dedata_I},//54
	{LSSPROTO_KS_SEND,NULL,NULL},//55
	{LSSPROTO_AC_RECV,lssproto_AC_recv,lssproto_dedata_III},//56
	{LSSPROTO_MU_RECV,lssproto_MU_recv,lssproto_dedata_IIII},//57
	{LSSPROTO_PS_RECV,lssproto_PS_recv,lssproto_dedata_IIIC},//58
	{LSSPROTO_PS_SEND,NULL,NULL},//59
	{LSSPROTO_ST_RECV,lssproto_ST_recv,lssproto_dedata_I},//60
	{LSSPROTO_DT_RECV,lssproto_DT_recv,lssproto_dedata_I},//61
	{LSSPROTO_FT_RECV,lssproto_FT_recv,lssproto_dedata_C},//62
	{LSSPROTO_SKUP_SEND,NULL,NULL},//63
	{LSSPROTO_SKUP_RECV,lssproto_SKUP_recv,lssproto_dedata_I},//64
	{LSSPROTO_KN_RECV,lssproto_KN_recv,lssproto_dedata_IC},//65
	{LSSPROTO_WN_SEND,NULL,NULL},//66
	{LSSPROTO_WN_RECV,lssproto_WN_recv,lssproto_dedata_IIIIIC},//67
	{LSSPROTO_EF_SEND,NULL,NULL},//68
	{LSSPROTO_SE_SEND,NULL,NULL},//69
	{LSSPROTO_SP_RECV,lssproto_SP_recv,lssproto_dedata_III},//70
	{LSSPROTO_CLIENTLOGIN_RECV,lssproto_ClientLogin_recv,lssproto_dedata_CC},//71
	{LSSPROTO_CLIENTLOGIN_SEND,NULL,NULL},//72
	{LSSPROTO_CREATENEWCHAR_RECV,lssproto_CreateNewChar_recv,lssproto_dedata_ICIIIIIIIIIII},//73
	{LSSPROTO_CREATENEWCHAR_SEND,NULL,NULL},//74
	{LSSPROTO_CHARDELETE_RECV,lssproto_CharDelete_recv,lssproto_dedata_C},//75
	{LSSPROTO_CHARDELETE_SEND,NULL,NULL},//76
	{LSSPROTO_CHARLOGIN_RECV,lssproto_CharLogin_recv,lssproto_dedata_C},//77
	{LSSPROTO_CHARLOGIN_SEND,NULL,NULL},//78
	{LSSPROTO_CHARLIST_RECV,lssproto_CharList_recv,NULL},//79
	{LSSPROTO_CHARLIST_SEND,NULL,NULL},//80
	{LSSPROTO_CHARLOGOUT_RECV,lssproto_CharLogout_recv,lssproto_dedata_I},//81
	{LSSPROTO_CHARLOGOUT_SEND,NULL,NULL},//82
	{LSSPROTO_PROCGET_RECV,lssproto_ProcGet_recv,NULL},//83
	{LSSPROTO_PROCGET_SEND,NULL,NULL},//84
	{LSSPROTO_PLAYERNUMGET_RECV,lssproto_PlayerNumGet_recv,NULL},//85
	{LSSPROTO_PLAYERNUMGET_SEND,NULL,NULL},//86
	{LSSPROTO_ECHO_RECV,lssproto_Echo_recv,lssproto_dedata_C},//87
	{LSSPROTO_ECHO_SEND,NULL,NULL},//88
	{LSSPROTO_SHUTDOWN_RECV,lssproto_Shutdown_recv,lssproto_dedata_CI},//89
	{LSSPROTO_NU_SEND,NULL,NULL},//90
	{LSSPROTO_TD_RECV,lssproto_TD_recv,lssproto_dedata_C},//91
	{LSSPROTO_TD_SEND,NULL,NULL},//92
	{LSSPROTO_FM_SEND,NULL,NULL},//93
	{LSSPROTO_FM_RECV,lssproto_FM_recv,lssproto_dedata_C},//94
	{LSSPROTO_WO_SEND,NULL,NULL},//95
	{LSSPROTO_PETST_RECV,lssproto_PETST_recv,lssproto_dedata_II},//96
	{LSSPROTO_BM_RECV,lssproto_BM_recv,lssproto_dedata_I},//97   // _BLACK_MARKET

#ifdef _MIND_ICON
	{LSSPROTO_MA_RECV,lssproto_MA_recv,lssproto_dedata_III},//98
#else
	{0,NULL,NULL},
#endif

#ifdef _FIX_DEL_MAP              // WON ADD 玩家抽地圖送監獄
	{LSSPROTO_DM_RECV,lssproto_DM_recv,NULL},//99
#else
	{0,NULL,NULL},
#endif

#ifdef _ITEM_CRACKER
	{LSSPROTO_IC_SEND,NULL,NULL},//100
#else
	{0,NULL,NULL},
#endif

#ifdef _MAGIC_NOCAST             // 精靈:沉默
	{LSSPROTO_NC_SEND,NULL,NULL},//101
#else
	{0,NULL,NULL},
#endif

	{0,NULL,NULL},//102
	
#ifdef _CHECK_GAMESPEED
	{LSSPROTO_CS_RECV,lssproto_CS_recv,NULL},//103 //加速探針
	{LSSPROTO_CS_SEND,NULL,NULL},//104 //加速探針
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

	{0,NULL,NULL},//105
	
#ifdef _TEAM_KICKPARTY
	{LSSPROTO_KTEAM_RECV,lssproto_KTEAM_recv,lssproto_dedata_I},//106
#else
	{0,NULL,NULL},
#endif

#ifdef _PETS_SELECTCON
	{LSSPROTO_PETST_SEND,NULL,NULL},//107
#else
	{0,NULL,NULL},
#endif

#ifdef _NEWREQUESTPROTOCOL			// (不可開) Syu ADD 新增Protocol要求細項
	{LSSPROTO_RESIST_RECV,lssproto_RESIST_recv,NULL},//108
	{LSSPROTO_RESIST_SEND,NULL,NULL},//109
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _OUTOFBATTLESKILL			// (不可開) Syu ADD 非戰鬥時技能Protocol
	{LSSPROTO_BATTLESKILL_RECV,lssproto_BATTLESKILL_recv,lssproto_dedata_I},//110
	{LSSPROTO_BATTLESKILL_SEND,NULL,NULL},//111
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _CHATROOMPROTOCOL			// (不可開) Syu ADD 聊天室頻道
	{LSSPROTO_CHATROOM_RECV,lssproto_CHATROOM_recv,lssproto_dedata_C},//112
	{LSSPROTO_CHATROOM_SEND,NULL,NULL},//113
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

	{LSSPROTO_SPET_RECV,lssproto_SPET_recv,lssproto_dedata_I},//114		// Robin 待機寵
	{LSSPROTO_SPET_SEND,NULL,NULL},//115

#ifdef _STREET_VENDOR
	{LSSPROTO_STREET_VENDOR_RECV,lssproto_STREET_VENDOR_recv,lssproto_dedata_C},//116		// 擺攤功能
	{LSSPROTO_STREET_VENDOR_SEND,NULL,NULL},//117
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _RIGHTCLICK
	{LSSPROTO_RCLICK_RECV,lssproto_RCLICK_recv,lssproto_dedata_IC},//118
	{LSSPROTO_RCLICK_SEND,NULL,NULL},//119
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _JOBDAILY
	{LSSPROTO_JOBDAILY_SEND,NULL,NULL},//120		// CYG　任務日誌功能
	{LSSPROTO_JOBDAILY_RECV,lssproto_JOBDAILY_recv,lssproto_dedata_C},//121
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

	{0,NULL,NULL},//121
	
#ifdef _TEACHER_SYSTEM
	{LSSPROTO_TEACHER_SYSTEM_RECV,lssproto_TEACHER_SYSTEM_recv,lssproto_dedata_C},//122		// 導師功能
	{LSSPROTO_TEACHER_SYSTEM_SEND,NULL,NULL},//123
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _ADD_STATUS_2
	{LSSPROTO_S2_RECV,NULL,NULL},//124
	{LSSPROTO_S2_SEND,NULL,NULL},//125
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif
};

#define DME() print("<DME(%d)co:%d func:%d flg:%d>",fd,__LINE__,func,lsret)

int lssproto_ServerDispatchMessage(int fd, char *encoded)
{
	typedef int(*dedata_func)(int,void*);
	typedef void (*lssv_func)(int);
	int func,fieldcount,lsret=-10;
	static char raw[65500];

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);
	
	util_DecodeMessage(raw,encoded);

	if( !util_SplitMessage(raw,SEPARATOR) ){
		print("\nDME1:package=%s\n",raw);
		util_DiscardMessage();
		lsret = -2;
		DME(); return -1;
	}
	if (!util_GetFunctionFromSlice(&func, &fieldcount)) {
		logHack(fd,HACK_GETFUNCFAIL);
		util_DiscardMessage();
		lsret = -3;
		DME(); return -1;
	}
	if(func>=arraysizeof(lsprofunc) || func < 0){
		logHack(fd,HACK_FUNCERR);
		util_DiscardMessage();
		lsret = -4;
		DME(); return -1;
	}
	if(func==LSSPROTO_CLIENTLOGIN_RECV || func ==LSSPROTO_PROCGET_RECV)
		strcpy(PersonalKey, _DEFAULT_PKEY);
	
	lssv_func lssvfunc=(lssv_func)lsprofunc[func].ls_func;
	if(lssvfunc==NULL){
		print("lsfunc is null\n");
		logHack(fd,HACK_LSFUNCNULL);
		util_DiscardMessage();
		lsret = -5;
		DME(); return -1;
	}else if(lsprofunc[func].func==func){
		dedata_func dedatafunc=(dedata_func)lsprofunc[func].de_func;
		if(dedatafunc==NULL){
			lssvfunc(fd);
			util_DiscardMessage();
			return 0;
		}else{
			lsret = dedatafunc(fd,lssvfunc);
			if(lsret<0){
				util_DiscardMessage();
				logHack(fd,HACK_CHECKSUMERROR);
				lsret = -6;
				DME(); return -1;
			}else{
				util_DiscardMessage();
				return lsret;
			}
		}
	}
	util_DiscardMessage();
	logHack(fd,HACK_NOTDISPATCHED);
	DME(); return -1;
}

int lssproto_dedata_IIC(int fd,void* pf)
{
	typedef void (*lsfunc_IIC)(int,int,int,char*);
	lsfunc_IIC pf2=(lsfunc_IIC)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	char *direction=malloc(65500);
	if(direction==NULL)return -2;
		
	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_destring(4, direction);
	util_deint(5, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(direction);
		return -1;
	}
	pf2(fd, x, y, direction);
	free(direction);
	return 0;
}

int lssproto_dedata_IIIII(int fd,void* pf)
{
	typedef void (*lsfunc_IIIII)(int,int,int,int,int,int);
	lsfunc_IIIII pf2=(lsfunc_IIIII)pf;
	int checksum=0, checksumrecv;
	int event;
	int seqno;
	int x;
	int y;
	int dir;

	checksum += util_deint(2, &event);
	checksum += util_deint(3, &seqno);
	checksum += util_deint(4, &x);
	checksum += util_deint(5, &y);
	checksum += util_deint(6, &dir);
	util_deint(7, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, event, seqno, x, y, dir);
	return 0;
}

int lssproto_dedata_II(int fd,void* pf)
{
	typedef void (*lsfunc_II)(int,int,int);
	lsfunc_II pf2=(lsfunc_II)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;

	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	util_deint(4, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, x, y);
	return 0;
}

int lssproto_dedata_I(int fd,void* pf)
{
	typedef void (*lsfunc_I)(int,int);
	lsfunc_I pf2=(lsfunc_I)pf;
	int checksum=0, checksumrecv;
	int dummy;

	checksum += util_deint(2, &dummy);
	util_deint(3, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, dummy);
	return 0;
}

int lssproto_dedata_C(int fd,void* pf)
{
	typedef void (*lsfunc_C)(int,char*);
	lsfunc_C pf2=(lsfunc_C)pf;
	int checksum=0, checksumrecv;
	char *command=malloc(65500);
	if(command==NULL)return -2;
		
	checksum += util_destring(2, command);
	util_deint(3, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(command);
		return -1;
	}		
	pf2(fd, command);
	free(command);
	return 0;
}

int lssproto_dedata_IIII(int fd,void* pf)
{
	typedef void (*lsfunc_IIII)(int,int,int,int,int);
	lsfunc_IIII pf2=(lsfunc_IIII)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	int haveitemindex;
	int toindex;

	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_deint(4, &haveitemindex);
	checksum += util_deint(5, &toindex);
	util_deint(6, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, x, y, haveitemindex, toindex);
	return 0;
}
int lssproto_dedata_III(int fd,void* pf)
{
	typedef void (*lsfunc_III)(int,int,int,int);
	lsfunc_III pf2=(lsfunc_III)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	int dir;

	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_deint(4, &dir);
	util_deint(5, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, x, y, dir);
	return 0;
}
int lssproto_dedata_ICI(int fd,void* pf)
{
	typedef void (*lsfunc_ICI)(int,int,char*,int);
	lsfunc_ICI pf2=(lsfunc_ICI)pf;
	int checksum=0, checksumrecv;
	int index;
	char *message=malloc(65500);
	int color;
	if(message==NULL)return -2;
	checksum += util_deint(2, &index);
	checksum += util_destring(3, message);
	checksum += util_deint(4, &color);
	util_deint(5, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(message);
		return -1;
	}
	pf2(fd, index, message, color);
	free(message);
	return 0;
}

int lssproto_dedata_IIICI(int fd,void* pf)
{
	typedef void (*lsfunc_IIICI)(int,int,int,int,char*,int);
	lsfunc_IIICI pf2=(lsfunc_IIICI)pf;
	int checksum=0, checksumrecv;
	int index;
	int petindex;
	int itemindex;
	char *message=malloc(65500);
	int color;
	if(message==NULL)return -2;
	checksum += util_deint(2, &index);
	checksum += util_deint(3, &petindex);
	checksum += util_deint(4, &itemindex);
	checksum += util_destring(5, message);
	checksum += util_deint(6, &color);
	util_deint(7, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(message);
		return -1;
	}
	pf2(fd, index, petindex, itemindex, message, color);
	free(message);
	return 0;
}

int lssproto_dedata_IICII(int fd,void* pf)
{
	typedef void (*lsfunc_IICII)(int,int,int,char*,int,int);
	lsfunc_IICII pf2=(lsfunc_IICII)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	char *message=malloc(65500);
	int color;
	int area;
	if(message==NULL)return -2;
	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_destring(4, message);
	checksum += util_deint(5, &color);
	checksum += util_deint(6, &area);
	util_deint(7, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(message);
		return -1;
	}
	pf2(fd, x, y, message, color, area);
	free(message);
	return 0;
}

int lssproto_dedata_IIIC(int fd,void* pf)
{
	typedef void (*lsfunc_IIIC)(int,int,int,int,char*);
	lsfunc_IIIC pf2=(lsfunc_IIIC)pf;
	int checksum=0, checksumrecv;
	int havepetindex;
	int havepetskill;
	int toindex;
	char *data=malloc(65500);
	if(data==NULL)return -2;
	checksum += util_deint(2, &havepetindex);
	checksum += util_deint(3, &havepetskill);
	checksum += util_deint(4, &toindex);
	checksum += util_destring(5, data);
	util_deint(6, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, havepetindex, havepetskill, toindex, data);
	return 0;
}

int lssproto_dedata_IC(int fd,void* pf)
{
	typedef void (*lsfunc_IC)(int,int,char*);
	lsfunc_IC pf2=(lsfunc_IC)pf;
	int checksum=0, checksumrecv;
	int havepetindex;
	char *data=malloc(65500);
	if(data==NULL)return -2;
	checksum += util_deint(2, &havepetindex);
	checksum += util_destring(3, data);
	util_deint(4, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(data);
		return -1;
	}
	pf2(fd, havepetindex, data);
	free(data);
	return 0;
}
int lssproto_dedata_IIIIIC(int fd,void* pf)
{
	typedef void (*lsfunc_IIIIIC)(int,int,int,int,int,int,char*);
	lsfunc_IIIIIC pf2=(lsfunc_IIIIIC)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	int seqno;
	int objindex;
	int select;
	char *data=malloc(65500);
	
	if(data==NULL)return -2;
		
	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_deint(4, &seqno);
	checksum += util_deint(5, &objindex);
	checksum += util_deint(6, &select);
	checksum += util_destring(7, data);
		
	util_deint(8, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(data);
		return -1;
	}
	pf2(fd, x, y, seqno, objindex, select, data);
	free(data);
	return 0;
}

int lssproto_dedata_CC(int fd,void* pf)
{
	typedef void (*lsfunc_CC)(int,char*,char*);
	lsfunc_CC pf2=(lsfunc_CC)pf;
	int checksum=0, checksumrecv;
	char *cdkey=malloc(65500);
	if(cdkey==NULL)return -3;
		
	char *passwd=malloc(65500);
	if(passwd==NULL){
		free(cdkey);
		return -2;
	}

	checksum += util_destring(2, cdkey);
	checksum += util_destring(3, passwd);
		
	util_deint(4, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(cdkey);
		free(passwd);
		return -1;
	}
	pf2(fd, cdkey, passwd);
	free(cdkey);
	free(passwd);
	return 0;
}

int lssproto_dedata_ICIIIIIIIIIII(int fd,void* pf)
{
	typedef void (*lsfunc_ICIIIIIIIIIII)(int,int,char*,int,int,int,int,int,int,int,int,int,int,int);
	lsfunc_ICIIIIIIIIIII pf2=(lsfunc_ICIIIIIIIIIII)pf;
	int checksum=0, checksumrecv;
	int dataplacenum;
	char *charname=malloc(65500);
	if(charname==NULL)return -2;
	int imgno;
	int faceimgno;
	int vital;
	int str;
	int tgh;
	int dex;
	int earth;
	int water;
	int fire;
	int wind;
	int hometown;

	checksum += util_deint(2, &dataplacenum);
	checksum += util_destring(3, charname);
	checksum += util_deint(4, &imgno);
	checksum += util_deint(5, &faceimgno);
	checksum += util_deint(6, &vital);
	checksum += util_deint(7, &str);
	checksum += util_deint(8, &tgh);
	checksum += util_deint(9, &dex);
	checksum += util_deint(10, &earth);
	checksum += util_deint(11, &water);
	checksum += util_deint(12, &fire);
	checksum += util_deint(13, &wind);
	checksum += util_deint(14, &hometown);
	util_deint(15, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(charname);
		return -1;
	}
	pf2(fd, dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
	free(charname);
	return 0;
}

int lssproto_dedata_CI(int fd,void* pf)
{
	typedef void (*lsfunc_CI)(int,char*,int);
	lsfunc_CI pf2=(lsfunc_CI)pf;
	int checksum=0, checksumrecv;
	char *passwd=malloc(65500);
	if(passwd==NULL)return -2;
	int min;

	checksum += util_destring(2, passwd);
	checksum += util_deint(3, &min);
	util_deint(4, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(passwd);
		return -1;
	}
	pf2(fd, passwd, min);
	free(passwd);
	return 0;
}
#else
#define DME() print("<DME(%d)%d:%d>",fd,__LINE__,func)

int lssproto_ServerDispatchMessage(int fd, char *encoded)
{
	int func,fieldcount;
	char raw[65500];

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	
	util_DecodeMessage(raw,encoded);

	if( !util_SplitMessage(raw,SEPARATOR) ){
		print("\nDME1:package=%s\n",raw);
		DME(); return -1;
	}
	if (!util_GetFunctionFromSlice(&func, &fieldcount)) {
		logHack(fd,HACK_GETFUNCFAIL);
		DME(); return -1;
	}
	
	if (func==LSSPROTO_W_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char direction[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, direction);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_W_recv(fd, x, y, direction);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_W2_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char direction[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, direction);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_W2_recv(fd, x, y, direction);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_EV_RECV) {
		int checksum=0, checksumrecv;
		int event;
		int seqno;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &event);
		checksum += util_deint(3, &seqno);
		checksum += util_deint(4, &x);
		checksum += util_deint(5, &y);
		checksum += util_deint(6, &dir);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_EV_recv(fd, event, seqno, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DU_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DU_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_EO_RECV) {
		int checksum=0, checksumrecv;
		int dummy;

		checksum += util_deint(2, &dummy);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_EO_recv(fd, dummy);
		util_DiscardMessage();
		return 0;
	}
     
	if (func==LSSPROTO_BU_RECV) {
		int checksum=0, checksumrecv;
		int dummy;

		checksum += util_deint(2, &dummy);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_BU_recv(fd, dummy);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_JB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_JB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_LB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_LB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_B_RECV) {
		int checksum=0, checksumrecv;
		char command[65500];

		checksum += util_destring(2, command);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_B_recv(fd, command);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SKD_RECV) {
		int checksum=0, checksumrecv;
		int dir;
		int index;

		checksum += util_deint(2, &dir);
		checksum += util_deint(3, &index);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SKD_recv(fd, dir, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ID_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int haveitemindex;
		int toindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &haveitemindex);
		checksum += util_deint(5, &toindex);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ID_recv(fd, x, y, haveitemindex, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PI_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &dir);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PI_recv(fd, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DI_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int itemindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &itemindex);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DI_recv(fd, x, y, itemindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DG_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int amount;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &amount);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DG_recv(fd, x, y, amount);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DP_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int petindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &petindex);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DP_recv(fd, x, y, petindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MI_RECV) {
		int checksum=0, checksumrecv;
		int fromindex;
		int toindex;

		checksum += util_deint(2, &fromindex);
		checksum += util_deint(3, &toindex);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MI_recv(fd, fromindex, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MSG_RECV) {
		int checksum=0, checksumrecv;
		int index;
		char message[65500];
		int color;

		checksum += util_deint(2, &index);
		checksum += util_destring(3, message);
		checksum += util_deint(4, &color);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MSG_recv(fd, index, message, color);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PMSG_RECV) {
		int checksum=0, checksumrecv;
		int index;
		int petindex;
		int itemindex;
		char message[65500];
		int color;

		checksum += util_deint(2, &index);
		checksum += util_deint(3, &petindex);
		checksum += util_deint(4, &itemindex);
		checksum += util_destring(5, message);
		checksum += util_deint(6, &color);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PMSG_recv(fd, index, petindex, itemindex, message, color);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_AB_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AB_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DAB_RECV) {
		int checksum=0, checksumrecv;
		int index;

		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DAB_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_AAB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AAB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_L_RECV) {
		int checksum=0, checksumrecv;
		int dir;

		checksum += util_deint(2, &dir);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_L_recv(fd, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_TK_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char message[65500];
		int color;
		int area;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, message);
		checksum += util_deint(5, &color);
		checksum += util_deint(6, &area);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TK_recv(fd, x, y, message, color, area);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_M_RECV) {
		int checksum=0, checksumrecv;
		int fl;
		int x1;
		int y1;
		int x2;
		int y2;

		checksum += util_deint(2, &fl);
		checksum += util_deint(3, &x1);
		checksum += util_deint(4, &y1);
		checksum += util_deint(5, &x2);
		checksum += util_deint(6, &y2);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_M_recv(fd, fl, x1, y1, x2, y2);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_C_RECV) {
		int checksum=0, checksumrecv;
		int index;

		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_C_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_S_RECV) {
		int checksum=0, checksumrecv;
		char category[65500];

		checksum += util_destring(2, category);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_S_recv(fd, category);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FS_RECV) {
		int checksum=0, checksumrecv;
		int flg;

		checksum += util_deint(2, &flg);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FS_recv(fd, flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_HL_RECV) {
		int checksum=0, checksumrecv;
		int flg;

		checksum += util_deint(2, &flg);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_HL_recv(fd, flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PR_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int request;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &request);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PR_recv(fd, x, y, request);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_KS_RECV) {
		int checksum=0, checksumrecv;
		int petarray;
		checksum += util_deint(2, &petarray);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KS_recv(fd, petarray);
		util_DiscardMessage();
		return 0;
	}

#ifdef _STANDBYPET
	if (func==LSSPROTO_SPET_RECV) {
		int checksum=0, checksumrecv;
		int standbypet;
		checksum += util_deint(2, &standbypet);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SPET_recv(fd, standbypet);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _RIGHTCLICK
	if (func==LSSPROTO_RCLICK_RECV) {
		int checksum=0, checksumrecv;
		int type;
		char data[1024];
		checksum += util_deint(2, &type);
		checksum += util_destring(3, data);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_RCLICK_recv(fd, type, data);
		util_DiscardMessage();
		return 0;
	}
#endif

	if (func==LSSPROTO_AC_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int actionno;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &actionno);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AC_recv(fd, x, y, actionno);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MU_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int array;
		int toindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &array);
		checksum += util_deint(5, &toindex);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MU_recv(fd, x, y, array, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PS_RECV) {
		int checksum=0, checksumrecv;
		int havepetindex;
		int havepetskill;
		int toindex;
		char data[65500];

		checksum += util_deint(2, &havepetindex);
		checksum += util_deint(3, &havepetskill);
		checksum += util_deint(4, &toindex);
		checksum += util_destring(5, data);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PS_recv(fd, havepetindex, havepetskill, toindex, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ST_RECV) {
		int checksum=0, checksumrecv;
		int titleindex;

		checksum += util_deint(2, &titleindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ST_recv(fd, titleindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DT_RECV) {
		int checksum=0, checksumrecv;
		int titleindex;

		checksum += util_deint(2, &titleindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DT_recv(fd, titleindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FT_RECV) {
		int checksum=0, checksumrecv;
		char data[65500];

		checksum += util_destring(2, data);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FT_recv(fd, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SKUP_RECV) {
		int checksum=0, checksumrecv;
		int skillid;

		checksum += util_deint(2, &skillid);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SKUP_recv(fd, skillid);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_KN_RECV) {
		int checksum=0, checksumrecv;
		int havepetindex;
		char data[65500];

		checksum += util_deint(2, &havepetindex);
		checksum += util_destring(3, data);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KN_recv(fd, havepetindex, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_WN_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int seqno;
		int objindex;
		int select;
		char data[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &seqno);
		checksum += util_deint(5, &objindex);
		checksum += util_deint(6, &select);
		checksum += util_destring(7, data);
		
		util_deint(8, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_WN_recv(fd, x, y, seqno, objindex, select, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SP_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &dir);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SP_recv(fd, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CLIENTLOGIN_RECV) {
		int checksum=0, checksumrecv;
		char cdkey[65500];
		char passwd[65500];
		
		strcpy(PersonalKey, _DEFAULT_PKEY);

		checksum += util_destring(2, cdkey);
		checksum += util_destring(3, passwd);
		
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		
		lssproto_ClientLogin_recv(fd, cdkey, passwd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CREATENEWCHAR_RECV) {
		int checksum=0, checksumrecv;
		int dataplacenum;
		char charname[65500];
		int imgno;
		int faceimgno;
		int vital;
		int str;
		int tgh;
		int dex;
		int earth;
		int water;
		int fire;
		int wind;
		int hometown;

		checksum += util_deint(2, &dataplacenum);
		checksum += util_destring(3, charname);
		checksum += util_deint(4, &imgno);
		checksum += util_deint(5, &faceimgno);
		checksum += util_deint(6, &vital);
		checksum += util_deint(7, &str);
		checksum += util_deint(8, &tgh);
		checksum += util_deint(9, &dex);
		checksum += util_deint(10, &earth);
		checksum += util_deint(11, &water);
		checksum += util_deint(12, &fire);
		checksum += util_deint(13, &wind);
		checksum += util_deint(14, &hometown);
		util_deint(15, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CreateNewChar_recv(fd, dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARDELETE_RECV) {
		int checksum=0, checksumrecv;
		char charname[65500];

		checksum += util_destring(2, charname);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharDelete_recv(fd, charname);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLOGIN_RECV) {
		int checksum=0, checksumrecv;
		char charname[65500];

		checksum += util_destring(2, charname);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharLogin_recv(fd, charname);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLIST_RECV) {
		int checksum=0, checksumrecv;
#ifdef _PKSEVER_VER
		int star = 0;
		util_deint(2, &checksumrecv);
		util_deint(3, &star);
#else
		util_deint(2, &checksumrecv);
#endif
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}

//#ifdef _PKSEVER_VER
//		lssproto_CharList_recv( fd, star);
//#else
		lssproto_CharList_recv( fd);
//#endif

		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLOGOUT_RECV) {
		int checksum=0, checksumrecv;
		int Flg=1;
#ifdef _CHAR_NEWLOGOUT
		checksum += util_deint(2, &Flg);
		util_deint(3, &checksumrecv);
#else
		util_deint(2, &checksumrecv);
#endif
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharLogout_recv(fd, Flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PROCGET_RECV) {
		int checksum=0, checksumrecv;
		
		strcpy( PersonalKey, _DEFAULT_PKEY);

		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ProcGet_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PLAYERNUMGET_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PlayerNumGet_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ECHO_RECV) {
		int checksum=0, checksumrecv;
		char test[65500];

		checksum += util_destring(2, test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_Echo_recv(fd, test);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SHUTDOWN_RECV) {
		int checksum=0, checksumrecv;
		char passwd[65500];
		int min;

		checksum += util_destring(2, passwd);
		checksum += util_deint(3, &min);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_Shutdown_recv(fd, passwd, min);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_TD_RECV) {
		int checksum=0, checksumrecv;
		char message[65500];

		checksum += util_destring(2, message);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TD_recv(fd, message);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FM_RECV) {
		int checksum=0, checksumrecv;
		char message[65500];

		checksum += util_destring(2, message);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FM_recv(fd, message);
		util_DiscardMessage();
		return 0;
	}
    
	if (func==LSSPROTO_PETST_RECV) {
		int checksum=0, checksumrecv;
		int nPet;
		int sPet;		

		checksum += util_deint(2, &nPet);
		checksum += util_deint(3, &sPet);		
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_PETST_recv(fd, nPet, sPet);		
		util_DiscardMessage();
		return 0;
	}
    
	// _BLACK_MARKET
	if (func==LSSPROTO_BM_RECV) {
		int checksum=0, checksumrecv;
		int iindex;		

		checksum += util_deint(2, &iindex);		
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_BM_recv(fd, iindex);		
		util_DiscardMessage();
		return 0;
	}

#ifdef _FIX_DEL_MAP           // WON ADD 玩家抽地圖送監獄
	if (func==LSSPROTO_DM_RECV) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_DM_recv( fd );
		util_DiscardMessage();
		return 0;
	}

#endif

#ifdef _CHECK_GAMESPEED
	if (func==LSSPROTO_CS_RECV) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_CS_recv( fd );
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _TEAM_KICKPARTY
	if ( func == LSSPROTO_KTEAM_RECV ) {
		int checksum = 0, checksumrecv;
		int sindex;
		checksum += util_deint( 2, &sindex);
		util_deint( 3, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KTEAM_recv( fd, sindex);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _MIND_ICON
	if(func==LSSPROTO_MA_RECV){
		int checksum = 0, checksumrecv;
		int nMind;
		int x, y;
		
		checksum += util_deint( 2, &nMind);
		checksum += util_deint( 3, &x);
		checksum += util_deint( 4, &y);
		util_deint( 5, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_MA_recv(fd, x, y, nMind);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _CHATROOMPROTOCOL			// (不可開) Syu ADD 聊天室頻道
	if (func==LSSPROTO_CHATROOM_RECV) {
		int checksum=0, checksumrecv;
		char test[65500];

		checksum += util_destring(2, test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CHATROOM_recv(fd, test);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _NEWREQUESTPROTOCOL			// (不可開) Syu ADD 新增Protocol要求細項
	if ( func==LSSPROTO_RESIST_RECV ) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_RESIST_recv( fd );
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _OUTOFBATTLESKILL			// (不可開) Syu ADD 非戰鬥時技能Protocol
	if(func==LSSPROTO_BATTLESKILL_RECV){
		int checksum = 0, checksumrecv;
		int iNum;
		
		checksum += util_deint( 2, &iNum);
		util_deint( 3, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_BATTLESKILL_recv(fd, iNum);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _STREET_VENDOR
	if(func == LSSPROTO_STREET_VENDOR_RECV){
		int checksum = 0,checksumrecv;
		char message[65500];

		checksum += util_destring(2,message);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_STREET_VENDOR_recv(fd,message);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _JOBDAILY
	if(func == LSSPROTO_JOBDAILY_RECV){
		int checksum = 0,checksumrecv;
		char buffer[16384];

		buffer[0] = '\0';
		memset(buffer,0,16384); //kkkkkkkkk
		checksum += util_destring(2,buffer);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		print("\njobdaily:%s ",buffer);
		lssproto_JOBDAILY_recv(fd,buffer);
		util_DiscardMessage();
		return 0;

	}
#endif
#ifdef _TEACHER_SYSTEM
	if(func == LSSPROTO_TEACHER_SYSTEM_RECV){
		int checksum = 0,checksumrecv;
		char message[65500];

		checksum += util_destring(2,message);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TEACHER_SYSTEM_recv(fd,message);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _ADD_STATUS_2
	if(func == LSSPROTO_S2_RECV){
		return 0;
	}
#endif

	util_DiscardMessage();
	logHack(fd,HACK_NOTDISPATCHED);
	DME(); return -1;
}
#endif
/*
   servertoclient XYD( int x, int y, int dir );
 	潛卞爵  蔽    卞皿伊奶乩□及匏  毛  譬幫允月凶戶卞銀丹〔
*/
void lssproto_XYD_send(int fd,int x,int y,int dir)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, dir);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_XYD_SEND, buffer);
}

void lssproto_EV_send(int fd,int seqno,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, seqno);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EV_SEND, buffer);
}

void lssproto_EN_send(int fd,int result,int field)
{
	char buffer[65500];
	int checksum=0;

	//print(" EN_send ");

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, result);
	checksum += util_mkint(buffer, field);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EN_SEND, buffer);
}


void lssproto_RS_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RS_SEND, buffer);
}


void lssproto_RD_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RD_SEND, buffer);
}

void lssproto_B_send(int fd,char* command)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, command);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_B_SEND, buffer);
}

void lssproto_I_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_I_SEND, buffer);
}

void lssproto_SI_send(int fd,int fromindex,int toindex)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fromindex);
	checksum += util_mkint(buffer, toindex);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SI_SEND, buffer);
}

void lssproto_MSG_send(int fd,int aindex,char* text,int color)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, aindex);
	checksum += util_mkstring(buffer, text);
	checksum += util_mkint(buffer, color);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_MSG_SEND, buffer);
}


void lssproto_PME_send(int fd,int objindex,int graphicsno,int x,int y,int dir,int flg,int no,char* cdata)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, objindex);
	checksum += util_mkint(buffer, graphicsno);
	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, dir);
	checksum += util_mkint(buffer, flg);
	checksum += util_mkint(buffer, no);
	checksum += util_mkstring(buffer, cdata);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PME_SEND, buffer);
}


void lssproto_AB_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_AB_SEND, buffer);
}


void lssproto_ABI_send(int fd,int num,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, num);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ABI_SEND, buffer);
}

void lssproto_TK_send(int fd,int index,char* message,int color)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, index);
	checksum += util_mkstring(buffer, message);
	checksum += util_mkint(buffer, color);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_TK_SEND, buffer);
}

void lssproto_MC_send(int fd,int fl,int x1,int y1,int x2,int y2,int tilesum,int objsum,int eventsum,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fl);
	checksum += util_mkint(buffer, x1);
	checksum += util_mkint(buffer, y1);
	checksum += util_mkint(buffer, x2);
	checksum += util_mkint(buffer, y2);
	checksum += util_mkint(buffer, tilesum);
	checksum += util_mkint(buffer, objsum);
	checksum += util_mkint(buffer, eventsum);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_MC_SEND, buffer);
}


void lssproto_M_send(int fd,int fl,int x1,int y1,int x2,int y2,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fl);
	checksum += util_mkint(buffer, x1);
	checksum += util_mkint(buffer, y1);
	checksum += util_mkint(buffer, x2);
	checksum += util_mkint(buffer, y2);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_M_SEND, buffer);
}

/*
   servertoclient C( string data );
       (Characters)
       弁仿奶失件玄反 CA 互  化｝公及平乓仿卞勾中化譬卅井勻凶凜反｝ C 
       匹仇及楮醒毛邰菲匹五月〔扔□田反民尼永弁仄卅中〔
       
 	string data
 		  白巧□穴永玄反 動票及3潘  及嫩  毛戊件穴匹卅日屯凶
 	    手及匹丐月.公木冗木及嫩  及  褥反今日卞凶化徇'|'匹仁亢日木
 	    化中月〔
 	    
 	      凶化徇匹仁亢日木凶玄□弁件互12蜊及樺寧
 		  玄□弁件及    反
 
 		  WHICHTYPE|CHARINDEX|X|Y|DIR|BASEIMG|LEVEL|NAMECOLOR|NAME|SELFTITLE|WALKABLE|HEIGHT|POPUPNAMECOLOR
 		  仇及左皮斥尼弁玄反平乓仿弁正□匹丐月午中丹仇午匹丐月〔
 		  
 		  WHICHTYPE 反〕仇及平乓仿弁正□互升丹中勻凶潘  及手及
 		  匹丐月井〔仇木反扔□田□匹反動票及方丹卞爛聒今木化中月〔
 		  
 		typedef enum
 		{
 		    CHAR_TYPENONE,          窒匹手卅中
 		    CHAR_TYPEPLAYER,        皿伊奶乩□
 		    CHAR_TYPEENEMY,         襯
 		    CHAR_TYPEPET,           矢永玄
 		    CHAR_TYPEDOOR,          玉失
 		    CHAR_TYPEBOX ,              
 		    CHAR_TYPEMSG ,          棗  
 		    CHAR_TYPEWARP ,         伐□皿札□件
 		    CHAR_TYPESHOP ,         饕
 		    CHAR_TYPEHEALER ,       甲□仿□
 		    CHAR_TYPEOLDMAN ,       贏  
 		    CHAR_TYPEROOMADMIN,     尕  遣盒
 		    CHAR_TYPETOWNPEOPLE,    引切及夫午
 		    CHAR_TYPEDENGON,        鰻蛻  
 		    CHAR_TYPEADM,           鰻蛻  
 		    CHAR_TYPETEMPLE,        Temple master
 		    CHAR_TYPESTORYTELLER,   賄曰  
 		    CHAR_TYPERANKING,       尕  遣仿件平件弘  憎      
 		    CHAR_TYPEOTHERNPC,      公及職及腹綢覆擂卞卅日卅中NPC
 		    CHAR_TYPEPRINTPASSMAN,  玉失及由旦伐□玉  憎允月NPC
 		    CHAR_TYPENPCENEMY,      裔爛襯
 		    CHAR_TYPEACTION,        失弁撲亦件卞  殺允月NPC
 		    CHAR_TYPEWINDOWMAN,     它奴件玉它  憎允月NPC    旦玄井手)
 		    CHAR_TYPESAVEPOINT,     本□皮禾奶件玄
 		    CHAR_TYPEWINDOWHEALER,  它奶件玉它正奶皿及甲□仿□
 		    CHAR_TYPEITEMSHOP,	    雲饕
 		    CHAR_TYPESTONESHOP,	    檗  盒  矢永玄及  盒  
 		    CHAR_TYPEDUELRANKING,   DUEL仿件平件弘NPC
 		    CHAR_TYPEWARPMAN,	    伐□皿穴件NPC
 		    CHAR_TYPEEVENT,	    奶矛件玄NPC
 		    CHAR_TYPEMIC,	    奶矛件玄NPC
 		    CHAR_TYPELUCKYMAN,	    奶矛件玄NPC
 		    CHAR_TYPEBUS,	    穴件乒旦田旦
 		    CHAR_TYPECHARM,	    奶矛件玄NPC
 		    CHAR_TYPENUM,
 		}CHAR_TYPE;
 		  
 		  公木反職及皿伊奶乩□支NPC支襯匹丐月〔扔□田□互霜耨
 		  允月午五反葦尹月  區卞勾中化蟈  霜耨允月〔勾引
 		  曰弁仿奶失件玄反｝仇及由弗永玄毛熬仃午勻凶午五卞仇及
 		  由弗永玄卞踏井木化中卅中平乓仿毛  勻化中凶日壅仄化仄
 		  引勻化方中午中丹仇午匹丐月〔引凶｝穴它旦市□末伙毛丐
 		  歹六凶午五卞  憎允月樹  反仇及樹  及心卞湘中化中月〔
 		  分井日｝穴它旦市□末伙毛丐歹六凶午五卞  憎允月仇午互
 		    凳今木凶樺寧反｝扔□田□反｝仇及楮醒毛    讀卞裟太
 		  分今卅仃木壬卅日卅中〔 SELFTITLE卞勾中化反｝犯伉立正
 		  匹丐月凶化徇毛孔仁引卅中方丹卞巨旦弗□皿今木化中月〔
 		  弁仿奶失件玄反失弁撲亦件及戊穴件玉(CA)互仁月引匹反  
 		  切匹  憎允月〔CHARINDEX扔□田□  及域啦卞平乓仿毛  
 		  爛匹五月  寞｝BASEIMG反  憎及凶戶及  寞｝LEVEL反平乓
 		  仿及伊矛伙(0卅日  憎仄卅中〔仇及襖反NPC卅升卞銀丹〔)
 		  WALKABLE反1及午五公及曉毛騷聊允月仇午互匹五｝0卅日騷
 		  聊允月仇午互匹五卅中〔HEIGHT反嫖今毛手勾手及井公丹匹
 		  卅中及井及隙爛〔
 		  
 		  平乓仿弁正□及  蟆午憤  憊寞反｝<a href="#escaping">
 		  巨旦弗□皿今木卅仃木壬卅日卅中〔'|'匹玄□弁件毛
 		  潸曰分仄化井日巨旦弗□皿毛荸輪允月〔巨旦弗□皿允月午'
 		  |'互幻井及  儂卞雲五井歹月及匹｝  賡反簽賬卞'|'毛犯
 		  伉立正午仄化方中〔霜耨允月  手｝  蟆午憤  憊寞毛巨旦
                   弗□皿仄化井日凶化徇匹勾卅中匹井日霜耨允月〔
                   引凶〕矢永玄及樺寧反憤  憊寞及醫歹曰卞交□扒□互澀爛
                   仄凶矢永玄及  蟆互霜耨今木化仁月〔
 		  
 	      凶化徇匹仁亢日木凶玄□弁件互6蜊及樺寧
 		  玄□弁件及    反
 		  INDEX|X|Y|BASEIMG|LEVEL|ITEM1LINEINFO
 		  匹嘩  卞  切化中月失奶  丞卞勾中化及樹  匹丐月〔
 		  INDEX反平乓仿及奶件犯弁永旦午井少日卅中INDEX匹丐月〔
 		  公及失奶  丞毛壅允凜卞銀迕允月〔X,Y反失奶  丞及弘夫□
 		  田伙匏  〔BASEIMG反  颶及  寞〔ITEM1LINEINFO反1墊
 		  info卞  憎允月凶戶及樹  匹丐月〔失奶  丞它奶件玉它  
 		  及  憎迕及樹  反  及  芊匹迕啦允月〔失奶  丞卞楮仄化
 		  反CA反  卅中〔ITEM1LINEINFO反<a href="#escape">巨旦弗□
 		  皿今木月〔仇及巨旦弗□皿及  芊反曉及嫩  毛輔寰〔
 
 	      凶化徇匹仁亢日木凶玄□弁件互4蜊及樺寧
 		  玄□弁件及    反
 		  INDEX|X|Y|VALUE
 		  匹嘩  卞  切化中月雲嗯卞勾中化及樹  匹丐月〔    反允
 		  屯化醒儂〔INDEX,X,Y 反失奶  丞午  元〔VALUE 反升木分
 		  仃及洶井午中丹儀匹丐月〔失奶  丞卞勾中化及樹  匹丐月〔
 	      凶化徇匹仁亢日木凶玄□弁件互1蜊及樺寧
 		  INDEX
 		  仇及平乓仿及C反項尹日木卅中〔
*/
void lssproto_C_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_C_SEND, buffer);
}

/*
   servertoclient CA( string data );
       (CharacterAction)
       葦尹月  區卞中月平乓仿及失弁撲亦件橇謫毛凳蕙允月〔
       扔□田□井日弁仿奶失件玄卞域  讀卞霜耨允月〔
       備平乓仿及1失弁撲亦件仍午卞霜耨〔扔□田□反失弁撲亦件毛匹五月分
       仃做諒允月仇午〔
       
 	string data
 	CHARINDEX|X|Y|ACTION|PARAM1|PARAM2|PARAM3|PARAM4|....毛戊
 	    件穴匹仁亢勻兇手及卞允月〔PARAM反失弁撲亦件仍午卞蜊醒手
 	    銀中井兇手仇午卅月〔動票反失弁撲亦件域  〔X,Y反匏  匹升
 	    及失弁撲亦件匹手｝丐凶日仄中匏  毛隙爛允月〔
 	    仇及  儂  反巨旦弗□皿今木卅中〔
 	  
 	        ACTION  PARAM1  PARAM2 
 	       PARAM3  PARAM4  
 	        Stand:0    輊0~7        
 	        Walk:1    輊0~7        
 	        Attack:2    輊0~7        
 	        Throw:3    輊0~7        
 	        Damage:4    輊0~7      
 	        Dead:5    輊0~7        
 	        UseMagic:6    輊0~7  
 	        UseItem:7    輊0~7  
 	        Effect:8    輊0~7  巨白尼弁玄  寞  
 	        Down:10 (邐木月)    輊0~7  
 	        Sit:11 (甄月)    輊0~7  
 	        Hand:12 (澎毛蕊月)    輊0~7  
 	        Pleasure:13 (減少)    輊0~7  
 	        Angry:14 (變月)    輊0~7  
 	        Sad:15 (  仄戈)    輊0~7  
 	        Guard:16 (布□玉)    輊0~7  
 	        actionwalk:17 (失弁撲亦件迕洶五)    輊0~7  
 	        nod:18 (丹卅內仁)    輊0~7  
 	        actionstand:19 (失弁撲亦件迕  切禾□術)    輊0~7  
 	        Battle:20 (爵  樹  )    輊0~7  BattleNo(-1 卅日  憎壅允    SideNo  HelpNo  ㄠ卅日踞仃毛裟少CA  憎〕ㄟ卅日壅允〕引凶反  仄    
 	        Leader:21 (伉□母□樹  )    輊0~7  0:  憎壅允 1:  憎  
 	        Watch:22 (爵  棋爵)    輊0~7  0:  憎壅允 1:  憎  
 	        namecolor:23(  蟆及縉樹  )    輊0~7    蟆及縉  寞  
 	        Turn:30(  輊  晶)    輊0~7    
 	        Warp:31(伐□皿)    輊0~7    
 	      
 	ACTION及襖反幫醒匹｝    反曉及  及爾羹及嫩  匹丐月〔
*/
void lssproto_CA_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CA_SEND, buffer);
}

/*
   servertoclient CD( string data );
       (CharacterDelete)
 	data 反犯伉立正  ,  匹嗉濠日木凶奶件犯永弁旦〔
 	  醒瘀霜月儀互請  月〔
       仇及ID毛  勻化中月平乓仿弁正互壅尹凶凜卞扔□田井日弁仿奶失件玄尺
       午鰻歹月〔
*/
void lssproto_CD_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CD_SEND, buffer);
}

/*
   <a name="R">servertoclient R( string data );
       (Radar)
       伊□母□及    毛弁仿奶失件玄卞霜耨允月〔弁仿奶失件玄反仇及霜耨毛
       邰菲允月仇午反卅中〔扔□田□互贗癲卅正奶立件弘匹霜耨允月〔
       凶午尹壬10洶丐月仁仍午午井｝1坌仍午午井〔
 
 	string data
 	x ,y, kind ,及賜卞襖毛'|'匹仁亢勻化卅日屯兇手及毛
 	    今日卞'|'匹卅日屯兇手及〔
 	    x,y反平乓仿井日及濮覆匏  〔kind反動票卞仄戶允醒襖匹反卅中
 	    筏寞〔伊□母□卞丹勾月  及潘  毛隙爛允月〔霜日木化仁月手及
 	    及潘  反｝公木引匹卞儒尹凶  芊午井｝旦平伙匹  祭仄｝公木反
 	    扔□田□互嗶尹月〔
 	 
 	        kind及襖        
 	        E  襯  
 	        P  皿伊奶乩□  
 	        S  熔  
 	        G  雲嗯  
 	        I  失奶  丞(熔動陸)  
   
   data及  「"12|22|E|13|24|P|14|28|P"
   仇及  儂  反巨旦弗□皿今木卅中〔
*/
void lssproto_R_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_R_SEND, buffer);
}

/*
   servertoclient S( string data );
       (Status)
       平乓仿及旦  □正旦毛霜耨允月〔
       犯□正反 市  打伉筏寞  儂(域  儂)     午卅勻化中月〔勾引曰  賡及
       1  儂毛葦木壬窒及旦  □正旦井坌月〔    反2  儂  井日匹丐月〔
           反動票及白巧□穴永玄卞仄凶互丹〔凶化徇筏寞'|'互犯伉立正匹丐月〔
       2蜊  動嫦及玄□弁件互    匹丐月〔
       
 	P 蟈由仿丟□正
 	    kubun hp maxhp mp maxmp str tough exp maxexp exp 
 	    level attackpower deffencepower 
 	    fixdex fixcharm fixluck fixfireat fixwaterat fixearthat fixwindat
 	    gold 尥仃化中月
 	    憊寞及index   蟆 憤裘憊寞 
 
 	    犯伉立正反 '|' 匹丐月〔引凶  蟆午憤裘憊寞反｝
 	    <a href="#escaping">巨旦弗□皿仄兇手及毛
 	    凶化徇匹勾卅僕兇手及互騷耨今木月〔
 	    公木冗木及襖及滇反動票〔
 	    kubun 卞ㄠ互  勻化中月午蟆由仿丟□正毛霜耨〕
 	    ㄡ動曉分午〕備申永玄及  勻化中月手及及由仿丟□正互賜  卞  勻化霜耨今木化中月〔
 	     尹壬〕ㄢ互  凶日hp午Maxhp互kubun 動嫦及玄□弁件卞  戶日木化中月〔
 	    
 	        kunun  int  
 	        Hp(    )hp  int  
 	        MaxHp  int  
 	        Mp(熱諸由伐□)  int  
 	        MaxMp  int  
 	        Vital(    )  int  
 	        Str(    )  int  
 	        Tough(暹屺今)  int  
 	        Dex(豳鍍今)  int  
 	        Exp(煩董襖)exp  int  
 	        MaxExp(戚引匹及煩董襖)  int  
 	        Level(伊矛伙)  int  
 	        Attack(  猾  )  int  
      	        Defense(潮    )  int  
 	        fQuick(  蔽豳鍍今)  int  
 	        fCharm(    )  int  
 	        fLuck(捫)  int  
 	        fEarth(嘩)fixearthat  int  
 	        fWater(  )  int  
 	        fFire(紹)  int  
 	        fWid(氘)fixwindat  int  
 	        Gold(雲嗯)  int  
 	        憊寞及index  int  
 	        犯亙巨伙禾奶件玄  int  
 	        鱉戲莢醒  int  
 	          蟆    儂    
 	        憤裘憊寞    儂    
 	    
 	    填    ( char 及    公及引引踏仁 )
 	    
 	    P10|20|10|20|10|10|10|1|2|13|13|1|100|10|尺氏天|abc|def
 	
 	C 甄  
 	    floor maxx maxy x y
 	    毛  | 匹嗉濠勻化霜月〔
 	    填    ( char 及    公及引引踏仁 )
 	    
 	    C1024|100|100|10|20
 	    
 	    曉筏及  匹反｝白夫失ID1024, 扔奶術100x100 X 10,Y 20及午仇
 	    欠卞平乓仿弁正□互中月午中丹    卞卅月〔仇及穴奶平乓仿及白
 	    夫失手孔仁戶凶甄  毛弁仿奶失件玄互襞月仇午互匹五月及反仇及
 	    戊穴件玉及心匹丐月〔洶五及瑛絆反白夫失  及匏  仄井歹井日卅
 	    中〔方勻化｝必□丞釩銨凜｝閉蟾銀迕及午五｝伐□皿及午五卅升
 	    反｝仇及戊穴件玉毛勾井勻化懇仄中襖毛霜耨仄卅仁化反卅日卅中〔
 	    
 	I 蟈失奶  丞
 	    (失奶  丞1)|(失奶  丞2)...(失奶  丞n)
 	    失奶  丞及    反動票及  芊匹由永弁允月〔
 	    夫午勾及失奶  丞反｝井卅日內動票及6勾及玄□弁件及本永玄匹
 	    霜耨今木月〔夫午勾夫午勾及    反｝
 	    
 	      蟆|  蟆2|旦  □斥|失奶  丞及    |失瓦丟  寞|銀迕第  樺赭|銀迕覆擂|失奶  丞伊矛伙|白仿弘  
 	    
 	      失奶  丞  反摹  伊矛伙匹憤  讀卞  凳今木月〔
 	        蟆2反摹  伊矛伙卞方勻化｝失奶  丞及    互  月  爛〔
 		  弁仿奶失件玄及失奶  丞  及2墊  卞  木月儀〔
 	      旦  □斥反  蟆及縉毛贅月及卞銀丹〔
 	      失奶  丞及    〔
 	      失瓦丟  寞反｝  颶  寞〔
 	      銀迕第  樺赭午反〕仇及失奶  丞互升仇匹銀迕允月儀互第  井互  勻化中月〔
 	          仇木反〕扔□田□匹反動票及方丹卞爛聒今木化中月〔
 	          
 			typedef enum
 			{
 				ITEM_FIELD_ALL,			允屯化及樺赭匹銀尹月
 				ITEM_FIELD_BATTLE,		爵    及心
 				ITEM_FIELD_MAP,			騷橘穴永皿曉及心
 
 			}ITEM_FIELDTYPE;
 	          
 	      銀迕覆擂午反〕仇及失奶  丞毛銀迕允月仇午及請  月覆擂互  勻化中月〔
 	          扔□田□匹反動票及方丹卞爛聒今木化中月〔
 	          
 			typedef enum
 			{
 				ITEM_TARGET_MYSELF,		憤坌及心
 				ITEM_TARGET_OTHER,		職及諦  憤坌殖戈)
 				ITEM_TARGET_ALLMYSIDE,		    蟈  
 				ITEM_TARGET_ALLOTHERSIDE,	鍬澎礎蟈  
 				ITEM_TARGET_ALL,		蟈化
 			}ITEM_TARGETTYPE;
 		
   	        <font size=+1>仇及醒儂卞100毛簫允午〕韶氏匹中月樊手覆擂午卅月〔
 	      失奶  丞伊矛伙〔仇及伊矛伙動曉及樊匹卅中午隸  請  卅中〔
 	      白仿弘  〔縉氏卅白仿弘毛霜耨允月〔備申永玄及覆殺反動票及騷曰〔

 		      0bit    矢永玄丟□伙匹霜耨第  卅失奶  丞井升丹井〔霜耨第  分午ㄠ〔ㄟ分午霜耨匹五卅中〔  
 		      1Bit    仇及失奶  丞互寧岳請  月井升丹井〔  
 		      2Bit    仇及失奶  丞互    井升丹井〔1分午    匹丐月〔  
 	    
 	    仇仇及  匹反<a href="#escaping">巨旦弗□皿允月〔
 	    填    ( char 及    公及引引踏仁 )
 	    
 	    I丐幻 卅 閡|str+2|1|丐幻\|中元|10|2|0
 	    
 	    旦  □斥及襖及啦  反｝幫醒襖匹
 	    
 	        1  旦  □斥A  
 	        2  旦  □斥B  	      
 	        3  旦  □斥C  
 	        4  旦  □斥D  
 	    
 	    午允月〔引凶｝失奶  丞  及塢及  坌卞勾中化反｝5蜊及玄□弁件互
 	    塢匹｝勾引曰凶化徇互卅日氏分橇謫匹霜耨今木化仁月〔
 	
 	S 蟈旦平伙
 	    (旦平伙0)|(旦平伙1)|(旦平伙2) ... (旦平伙n)
 	    憊寞午  元匹｝平乓仿互手化月    旦平伙醒坌毛霜耨允月〔勾引
 	    曰卅中手及反 ||| 午卅月及匹丐月〔
 	    備巨件玄伉及    反｝
 	    旦平伙及潘  戊□玉|伊矛伙
 	    匹丐月〔
 	    填    ( char 及    公及引引踏仁 )
 	    
 	    S10|2|20|2|||
 	
 	T 憊寞
 	    (憊寞0)|(憊寞1)|(憊寞2) ... (憊寞n)
 	    憊寞及巨件玄伉互塢及樺寧反 '|' 互  糧允月及匹｝  糧仄兇手
 	    及毛    仄化旦平乓件仄化反卅日卅中〔井卅日內    蜊醒坌霜耨
 	    允月〔
 	    夫午勾｝夫午勾及    反｝
 	      蟆
 	    及心匹丐月〔
 	    填    ( 霜日木化仁月 char 及    公及引引匹丐月 )
 	    
 	    T丐幻|方勻天日中
 	    
 	    仇木手<a href="#escaping">巨旦弗□皿允月〔
 
 	M   坌由仿丟□正
 	    平乓仿弁正□及旦  □正旦及丹切｝    卞  凳今木月手及(蜇箕
 	    襖)午公丹匹卅中手及(    襖卅升)互丐月〔升木井及旦  □正旦
 	    互  凳今木月凶太卞  莢蟈  及由仿丟□正毛霜耨允月及反生永玄
 	    伐□弁玄仿白奴永弁毛  蟯卞銀丹仇午卞卅月及匹｝  卞HP ,
 	    MP,EXP及襖分仃卞仄廿勻凶由仿丟□正霜耨戊穴件玉毛迕啦允月〔
 	    公木互仇及M戊穴件玉匹丐月〔2  儂  動嫦及  域玄□弁件反HP及
 	    蜇箕襖 ,     玄□弁件反MP｝  趕玄□弁件反EXP匹丐月〔
 	    動票卞填    毛仄戶允〔
 	    
 	    M54|210|8944909
 	    
 	    仇及  匹反HP及蜇箕襖互54｝MP及蜇箕襖互210｝煩董襖互8944909
 	    卞卅勻化中月及分〔仇及3潘  反  卞  凳  蘸互嫖中午  歹木月
 	    凶戶    仄凶戊穴件玉祭仄凶互｝手切欠氏仇及M戊穴件玉及井歹
 	    曰卞P戊穴件玉匹蟈由仿丟□正毛霜耨仄化手方中〔凶分仄｝  贗
 	    卞生永玄伐□弁毛銀丹凶戶卞反｝仇及戊穴件玉毛銀丹仇午互蹂儔
 	    今木月〔
 	    
         D 平乓仿及ID
 	    夫弘奶件凜卞分仃霜月平乓仿及index〕公木午凜棉毛霜月〔
 	    D1000|912766409午井匹霜日木月〔
 	
 	E 巨件市它件玄割    票蜃)|(曉蜃)
 	    巨件市它件玄  毛霜耨允月〔割  反n/100
 	    弁仿奶失件玄反〕票蜃及襖井日旦正□玄仄化〕ㄠ洶洶仁  卞+1仄凶割  匹巨件市它件玄允月井毛  蠅允月〔
 	    凶分仄〕曉蜃及襖毛旋尹卅中方丹卞允月〔
 	    戚莢卞仇木互霜日木化仁月引匹仇及割  匹巨件市它件玄毛煌遙允月儀〔
 	
 	J0   J6 銀尹月熱諸及    
 	    銀尹月熱諸及    毛霜耨允月〔
 	    0   6 反公木冗木失奶  丞及隸  組赭卞覆殺仄化中月〔域  及隸  組赭    〕  〕澎〕隸澡    
 	    仄井隸  請  卅中  爛分互〕漆  傀艦及啃卞蟈化及隸  組赭及樹  毛霜月
 	    J0|kubun|mp|field|target|name|comment
 	    午中丹白巧□穴永玄卞卅勻化中月〔
 	    kubun 反公及  互丐月井  中井〔ㄟ分午  中〔公木動嫦卞玄□弁件允日卅中〔ㄠ分午丐月〔
 	    mp反壅  竣  毛  允〔
 	    field反升及樺赭匹銀尹月井〔扔□田□匹反動票及  卞爛聒今木化中月〔
 	    
 		typedef enum
 		{
 			MAGIC_FIELD_ALL,		允屯化及樺赭匹銀尹月
 			MAGIC_FIELD_BATTLE,		爵    及心
 			MAGIC_FIELD_MAP,		騷橘穴永皿曉及心
 			
 		}MAGIC_FIELDTYPE;
 	    
 	    target反升木毛覆擂卞請  月井〔扔□田□匹反動票及方丹卞爛聒今木化中月〔
 	    
 		typedef enum
 		{
 			MAGIC_TARGET_MYSELF,		憤坌及心
 			MAGIC_TARGET_OTHER,		職及諦  憤坌殖戈)
 			MAGIC_TARGET_ALLMYSIDE,		    蟈  
 			MAGIC_TARGET_ALLOTHERSIDE,	鍬澎礎蟈  
 			MAGIC_TARGET_ALL,		蟈化
 			MAGIC_TARGET_NONE,		簿手薊  請  卅中〔  豢支凶戶及凜
 			MAGIC_TARGET_OTHERWITHOUTMYSELF,職及諦  憤坌殖引卅中)
 			MAGIC_TARGET_WITHOUTMYSELFANDPET, 憤坌午矢永玄動陸
 			MAGIC_TARGET_WHOLEOTHERSIDE,        及扔奶玉蟈  
 		}MAGIC_TARGETTYPE;
 	    
 	    <font size=+1>仇及醒儂卞100毛簫允午〕韶氏匹中月樊手覆擂午卅月〔
 	    
 	    name反熱諸  〔
 	    comment反仇及熱諸及    〔
 	
 	N0    N3 醮棉及由仿丟□正
 	    醮棉卞卅勻化中月諦及樹  毛霜耨允月〔
 	    N0|kubun|level|charaindex|maxhp|hp|mp|name
 	    
 	    kubun 反公及  互丐月井  中井〔ㄟ分午  中〔公木動嫦卞玄□弁件允日卅中〔ㄠ蟈由仿丟□正〔
 	    2動曉分午〕備申永玄及  勻化中月手及及由仿丟□正  1bit    level 2bit   charaindex卅升  
 	    互霜耨今木月〔
 	    charaindex 反醮棉及charaindex
 	    level反公及諦及伊矛伙〔
 	    maxhp反公及諦及MAXHP
 	    hp反公及諦及蜇箕及HP
 	    mp反公及諦及竣  
 	    name 反公及諦及  蟆〔
 	
 	K0  K4 雯  由仿丟□正
 	    hp maxhp mp maxmp str tough exp 
 	    level attackpower deffencepower 
 	    fixdex fixcharm fixluck fixfireat fixwaterat fixearthat fixwindat
 	      蟆 status
 	    K及  卞0  4毛隙爛仄化｝升及雯  井毛隙爛允月仇午〔
 	    公及戚及玄□弁件卞0互  凶日公及矢永玄  反  中午蛻丹儀〔
 	    丐月樺寧反ㄠ匹丐月〔1分午蟈由仿丟□正〔
 	    2動曉分午〕備申永玄及  勻化中月手及及由仿丟□正  1bit    hp  2bit   maxhp卅升  
 	    互霜耨今木月〔
 	    犯伉立正反 '|' 匹丐月〔引凶  蟆午憤裘憊寞反｝
 	    <a href="#escaping">巨旦弗□皿仄兇手及毛
 	    凶化徇匹勾卅僕兇手及互騷耨今木月〔
 	    公木冗木及襖及滇反動票〔
 	    
 	        No.(  寞)  int  
 	        islive(戲韶)  int  
 	        GRA(  颶  寞)  int  
 	        Hp(覲菁  )  int  
 	        MaxHp(    覲菁  )  int  
 	        Mp  int  
 	        MapMp  int  
 	        Exp(煩董襖)  int  
 	        MaxExp(戚引匹及煩董襖)  int  
 	        Level(伊矛伙)  int  
 	        Attack(  猾  )  int  
 	        Defense(潮    )  int  
 	        Quick(豳鍍今)  int  
 	        Ai(鏑擦蘸)  int  
 	        fEarth(嘩)  int  
 	        fWater(  )  int  
 	        fFire(紹)  int  
 	        fWid(氘)  int  
 	        Slot(    檗  醒)  int  
 	          蟆  凳袱第白仿弘  int  
 	          蟆    儂    
 	        交□扒□矢永玄      儂    
 	    
 	      蟆  凳袱第白仿弘午反〕仇及矢永玄及  蟆毛  凳仄化方中井升丹井及白仿弘匹〕
 	    1 分午  凳    〕ㄟ分午  凳尕第午卅月〔
 	    
 	    填    ( char 及    公及引引踏仁 )
 	    
 	    P10|20|10|20|10|10|10|1|2|13|13|1|100|10|尺氏天|PC
 
 	W0  W4 矢永玄及  犯□正
 	  W0|skillid|field|target|name|comment| x 7
 	  W0    W4 反公木冗木及矢永玄卞覆殺仄化中月〔
 	  petskillid 反〕矢永玄及  及  寞〔pet_skillinfo.h卞爛聒今木化中月〔
 	  field 反公及  互升仇匹銀迕匹五月井〔扔□田□匹反動票及方丹卞爛聒今木化中月〔
 	  
 		typedef enum
 		{
 			PETSKILL_FIELD_ALL,		允屯化及樺赭匹銀尹月
 			PETSKILL_FIELD_BATTLE,		爵    及心
 			PETSKILL_FIELD_MAP,		騷橘穴永皿曉及心
 
 		}PETSKILL_FIELDTYPE;
 	  
 	  target 反公及  及覆擂互升丹中丹手及井〔扔□田□匹反戚及方丹卞爛聒今木化中月〔
 	  
 		typedef enum
 		{
 			PETSKILL_TARGET_MYSELF,		憤坌及心
 			PETSKILL_TARGET_OTHER,		職及諦  憤坌殖戈)
 			PETSKILL_TARGET_ALLMYSIDE,	    蟈  
 			PETSKILL_TARGET_ALLOTHERSIDE,	鍬澎礎蟈  
 			PETSKILL_TARGET_ALL,		蟈化
 			PETSKILL_TARGET_NONE,		簿手薊  請  卅中〔  豢支凶戶及凜
 			PETSKILL_TARGET_OTHERWITHOUTMYSELF,職及諦  憤坌殖引卅中) 
 			PETSKILL_TARGET_WITHOUTMYSELFANDPET, 憤坌午矢永玄動陸
 		}PETSKILL_TARGETTYPE;
 	  
 	  name 反  及  蟆〔
 	  comment反公及  卞覆允月    〔
 	  target|name|comment| 反  及醒及坌ㄠ墊匹霜日木化仁月〔
 	    反域殺ㄦ勾引匹〔    匹  互  仃化中月  "|"及心互糧仁  樺寧
 	  反弁仿奶失件玄匹腴戶化  憎允月儀〔
*/
void lssproto_S_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_S_SEND, buffer);
}

/*
   servertoclient D( int category , int dx , int dy , string data );
       (Display)
           卞窒井  憎允月隙  〔
 
       
 	int category
 	窒毛  憎允月井〔
 	    
 	        襖        
 	        1  憤坌動陸卞芨尹凶母丟□斥〔data反  儂
 		    卞卅勻凶襖〔  
 		    2  憤坌互丹仃凶母丟□斥〔data反  儂  卞
 		      卅勻凶襖  
 		
 	int dx
 	穴奶平乓仿井日及弘伉永玉鍬覆匏  X〔域  讀卞反奶矛件玄互  戲
 	    仄凶匏  〔弁仿奶失件玄反仇及襖井日贗給贗濠卅匏  毛煌遙仄化
 	        允月〔
 	int dy
 	鍬覆匏  Y
 	string data 
 	  憎允月    〔    反category卞方勻化瑁引月〔
 	    仇及  儂  反<a href="#escaping">巨旦弗□皿今木卅仃木壬
 	    卅日卅中〔
*/
void lssproto_D_send(int fd,int category,int dx,int dy,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, category);
	checksum += util_mkint(buffer, dx);
	checksum += util_mkint(buffer, dy);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_D_SEND, buffer);
}

/*
   servertoclient FS( int flg);
       (FlgSet)PS及及殺蠶〔引凶反夫弘奶件凜卅升卞憤坌及橇謫午仄化霜日木化仁月〔
       
         int flg
          0 bit  0: 醮棉Off                  1: 醮棉On
                蜇箕  銀迕  1 bit  0: 爵      輔笛off          1: 爵      輔笛On    
              2 bit  0: DUEL off                 1: DUEL On
              3 bit  0:   騷及民乓永玄乒□玉     1: 由□  奴卞仄井民乓永玄互  壬卅中乒□玉
              4 bit  0:   銅跟晶OK               1:   銅跟晶蛐  
*/
void lssproto_FS_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FS_SEND, buffer);
}

/*
   servertoclient HL( int flg);
       (HeLp)HL及及殺蠶〔引凶反由□  奴及醮棉互爵  及雲踞仃乒□玉毛  凳仄凶樺寧卞霜日木化仁月〔
       
         int flg
          0: 雲踞仃乒□玉Off                  1: 雲踞仃乒□玉On
*/
void lssproto_HL_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_HL_SEND, buffer);
}

/*
   servertoclient PR( int request, int result);
       (PartyRequest)醮棉邰菲及殺蠶〔PR毛弁仿奶失件玄互霜勻化中卅仁化手仇木毛熬仃潸月凜互丐月〔
        由□  奴互  螺荸辣  褪互  仃凶  卅升及凜〔
       
       	int request
       	0: 輪邇 1:  邇
         int result
         0: 撩   1: 岳  
*/
void lssproto_PR_send(int fd,int request,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, request);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PR_SEND, buffer);
}

#ifdef _PETS_SELECTCON
void lssproto_PETS_send(int fd,int petarray,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, petarray);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PETST_SEND, buffer);
}
#endif

void lssproto_KS_send(int fd,int petarray,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, petarray);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_KS_SEND, buffer);
}

void lssproto_SPET_send(int fd, int standbypet, int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, standbypet);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SPET_SEND, buffer);
}

/*
   servertoclient PS( int result, int havepetindex, int havepetskill, int toindex);
       (PetSkill use result)矢永玄及  毛銀勻凶瑛絆弁仿奶失件玄及PS卞覆殺仄化霜日木化仁月〔
       result 動陸反弁仿奶失件玄及PS卞覆殺仄化中月〔白奴□伙玉曉井日仄井裟壬木卅中〔
       
 	int result
 	瑛絆〔0: 撩   1:岳  
         int havepetindex
         窒    及矢永玄互銀迕仄凶井〔
         int havepetskill
         窒    及  毛銀迕仄凶井〔
 	int toindex
 	 簿卞  芊毛銀迕仄凶井〔仇木反左皮斥尼弁玄支平乓仿及index匹反卅中〔動票及  卞卅勻化中月〔
 	
 	  憤坌    = 0
 	  矢永玄  = 1   5
 	  醮棉    = 6   10   S N 及0  4卞覆殺〔憤坌憤褥手殖引木化中月  
 	
 	覆擂互蟈夠〕午井坌井日卅中〕午井及樺寧反-1匹霜耨允月〔
*/
void lssproto_PS_send(int fd,int result,int havepetindex,int havepetskill,int toindex)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, result);
	checksum += util_mkint(buffer, havepetindex);
	checksum += util_mkint(buffer, havepetskill);
	checksum += util_mkint(buffer, toindex);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PS_SEND, buffer);
}

/*
   servertoclient SKUP( int point );
       (SKillUP)
       旦平伙失永皿互匹五月儀毛扔□田互騷襞允月〔中仁勾曉僕日木月井毛隙爛允月〔
*/
void lssproto_SKUP_send(int fd,int point)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, point);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SKUP_SEND, buffer);
}

/*
   servertoclient WN( int windowtype, int buttontype, int seqno, int objindex, string data );
       (Window)
       它奴件玉它毛  憎六方午扔□田□互弁仿奶失件玄卞騷襞允月〔
       
         int windowtype
         它奴件玉它正奶皿〔升及方丹卅溥摯及它奴件玉它毛  憎允月井〔仇木午buttontype 及賺心寧歹六匹
             它奴件玉它互瑁爛允月〔
             扔□田□匹反動票及  卞澀爛今木化中月〔
           
 		typedef enum
 		{
 			WINDOW_MESSAGETYPE_MESSAGE,			丟永本□斥及心
 			WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,		丟永本□斥午域墊    
 			WINDOW_MESSAGETYPE_SELECT,			薊  它奴件玉它
 			WINDOW_MESSAGETYPE_PETSELECT,			矢永玄薊  它奴件玉它
 			WINDOW_MESSAGETYPE_PARTYSELECT,			醮棉薊  它奴件玉它
 			WINDOW_MESSAGETYPE_PETANDPARTYSELECT,		矢永玄〕醮棉薊  它奴件玉它
 			WINDOW_MESSAGETYPE_ITEMSHOPMENU,		雲饕及丟瓦亙□它奶件玉它
 			WINDOW_MWSSAGETYPE_ITEMSHOPYMAIN,		雲饕及丟奶件它奶件玉它
 			WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN,		  中潸曰毀  失奶  丞盒及丟奶件它奶件玉它
 			WINDOW_MESSAGETYPE_PETSKILLSHOP,		矢永玄及  盒今氏它奶件玉它
 			WINDOW_MESSAGETYPE_WIDEMESSAGE,			丟永本□斥及心    五中    
 			WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT,	丟永本□斥午域墊        五中    
 
 		}WINDOW_MESSAGETYPE;
 
         int buttontype
         示正件及溥摯毛隙爛允月〔扔□田□匹反動票及迕卞爛聒今木化中月〔
             
 		#define		WINDOW_BUTTONTYPE_NONE		(0)
 		#define		WINDOW_BUTTONTYPE_OK		(1 << 0)
 		#define		WINDOW_BUTTONTYPE_CANCEL	(1 << 1)
 		#define		WINDOW_BUTTONTYPE_YES		(1 << 2)
 		#define		WINDOW_BUTTONTYPE_NO		(1 << 3)
 		#define		WINDOW_BUTTONTYPE_PREV		(1 << 4)
 		#define		WINDOW_BUTTONTYPE_NEXT		(1 << 5)
             
             仇木日及賺心寧歹六匹霜耨允月〔  尹壬〕YES示正件午NO示正件互  仄中凜反
             WINDOW_BUTTONTYPE_YES | WINDOW_BUTTONTYPE_NO   (=12)
             匹霜月〔
             
         int seqno
         仇及它奴件玉它及  寞毛憎允〔扔□田□互棟  允月〔
             弁仿奶失件玄反WN卞化仇及它奴件玉它及丑綜瑛絆毛忒允及卞〕仇及  寞毛驕尹化忒蠶允月〔
             仇木卞方勻化扔□田□反升及NPC及升及樺  匹及它奴件玉它井毛  蠅請  月方丹卞允月〔
         int objindex
         仇及它奴件玉它毛請六午蛻勻凶NPC卅升及index互傘  今木化中月〔
             撲旦  丞互請六午蛻勻凶樺寧反-1卅升互  勻化中月〔
             弁仿奶失件玄反〕它奴件玉它      及WN皿夫玄戊伙匹仇及醒襖毛公及引引忒六壬  中〔
         string data
         丟永本□斥    毛憎允〔    反巨旦弗□皿允月〔"\n"匹嗉濠月午荼墊午中丹啦  午允月〔
             引凶〕弁仿奶失件玄及它奴件玉它匹  憎請  月產汔毛旋尹凶端反憤  讀卞荼墊今木月〔
             薊  餉及丐月它奴件玉它匹反〕"\n"匹嗉濠日木凶  賡及玄□弁件互丟永本□斥及墊醒
             午卅曰〕戚卞丟永本□斥及玄□弁件互  賡及玄□弁件匹隙爛今木凶蜊醒糧五〕
             公及丐午及玄□弁件  互ㄠ勾內勾及薊  餉午卅月〔引凶蟆井日賜卞 1 井日
               寞毛喃曰癲化〕薊  仄凶樺寧及WN匹及忒蠶及凜卞忒允〔
         string data(雲饕迕  
         丟永本□斥    毛憎允〔    反巨旦弗□皿允月〔"\n"匹嗉濠月午荼墊午中丹啦  午允月〔
             引凶〕弁仿奶失件玄及它奴件玉它匹  憎請  月產汔毛旋尹凶端反憤  讀卞荼墊今木月〔
             犯□正  反嫩    卞"  "匹嗉濠日木化中引允〔
    Ｕ  丹Ｖ
  	  中  月白仿弘    丹ㄟ「  月ㄠ    蟆及犯□正銀丹井白仿弘  銀丹ㄟ「銀歹卅中「1    
 		饕及  蟆  丟永本□斥  饕丟永本□斥  蜊醒薊  丟永本□斥  伊矛伙簫曰卅中丟永本□斥  割  丟永本□斥  
 		失奶  丞互中勻天中丟永本□斥  失奶  丞      尹月  尹卅中白仿弘    尹月「ㄟ  尹卅中「ㄠ    失奶  丞伊矛伙  
 		襖蟾    颶  寞        失奶  丞      尹月  尹卅中白仿弘  失奶  丞伊矛伙  襖蟾    颶  寞      
       		
       		0|1|扔丞幼伙及  湛盒|中日勻仄扎中｝窒及方丹分中|中日勻仄扎中升木卞允月|窒蜊  邰分中|公木毛隸  允月卞反伊矛伙互
       		簫曰卅中仃升中中及井中|  癲卞中中氏分友|雲中雲中失奶  丞互中勻天中元扎友尹井〞  皮□丟仿件|
       		ㄟ|ㄠㄟㄟ|ㄠㄥㄟㄟㄟ|產域  卞  猾\n躲絆  歇|泔闋|ㄟ|ㄤㄟ|ㄠㄥㄟㄟㄡ|    ㄡㄟ莢汊\n  萵卞反勾五  \n兜中  
 
         Ｕ  月Ｖ
 		  中  月白仿弘    丹ㄟ「  月ㄠ    蟆及犯□正銀丹井白仿弘  銀丹ㄟ「銀歹卅中「1    
 		饕及  蟆  丟永本□斥  饕丟永本□斥  雲嗯互中勻天中卞卅勻切扎丹丟永本□斥|割  丟永本□斥  
 		失奶  丞      木月  木卅中白仿弘  襖蟾    颶  寞        失奶  丞      木月  木卅中白仿弘  
 		襖蟾    颶  寞        
       
       		ㄠ|1|扔丞幼伙及  填盒|中日勻仄扎中｝升月允月〞  中日勻仄扎中升木毛  勻化仁木月〔|  癲卞中中氏分友|
       		公木分午｝雲嗯互左□田□仄切扎丹友  皮□丟仿件|ㄟ|ㄡㄟ|ㄠㄥㄟㄟㄟ|
       		產域  卞  猾\n躲絆  歇|泔闋|ㄟ|ㄠㄟ|ㄠㄥㄟㄟㄡ|    ㄡㄟ莢汊\n  萵卞反勾五  \n兜中  
         Ｕ失奶  丞  仃月Ｖ
 		白仿弘    月=ㄟ    丐午窒蜊  仃日木月井  
 		饕及  蟆  
 		  仃月丟永本□斥  仇木動曉  仃日木卅中丟永本□斥  割  丟永本□斥  
 		  蟆    仃日木月井白仿弘  第  ＞ㄟ〕尕第＞ㄠ    襖蟾    颶  寞  域墊奶件白巧  失奶  丞    寞  ㄠ井日  
 		  蟆    仃日木月井白仿弘  第  ＞ㄟ〕尕第＞ㄠ    襖蟾    颶  寞  域墊奶件白巧  失奶  丞    寞  ㄠ井日  
         Ｕ失奶  丞婁五潸月Ｖ
 		白仿弘  婁五潸月=ㄠ    
 		饕及  蟆  婁五請允丟永本□斥  失奶  丞互域  及凜及丟永本□斥|割  丟永本□斥  
 		  蟆  婁五請六月井升丹井白仿弘  第  ＞ㄟ〕尕第＞ㄠ    失奶  丞伊矛伙  襖蟾    颶  寞  域墊奶件白巧  
 		  蟆  婁五請六月井升丹井白仿弘  第  ＞ㄟ〕尕第＞ㄠ    失奶  丞伊矛伙  襖蟾    颶  寞  域墊奶件白巧  
 
         Ｕ公及職Ｖ
 		饕及  蟆  丟永本□斥
       
       		扔丞幼伙及  填盒|中日勻仄扎中｝升月允月〞
 	
         string data(矢永玄及  盒今氏迕  
         丟永本□斥    毛憎允〔    反巨旦弗□皿允月〔"\n"匹嗉濠月午荼墊午中丹啦  午允月〔
             引凶〕弁仿奶失件玄及它奴件玉它匹  憎請  月產汔毛旋尹凶端反憤  讀卞荼墊今木月〔
             犯□正  反嫩    卞"  "匹嗉濠日木化中引允〔
 	   Ｕ矢永玄及  潸  Ｖ
 		蟆及樹  銀丹井升丹井〞  銀丹「0｛銀歹卅中「1    饕及  蟆  丟奶件丟永本□斥  
 		      襖蟾                襖蟾        
 	
 		1  扔丞幼伙及  湛盒  中日勻仄扎中引勻六〔允壬日仄中  壬勻井匹允方  
 		    及諷公及迭  500    猾  ㄢㄟ⊙    ｛  豢  ㄢㄟ⊙          
 		嘩萄域蔥  1000  蕙仄中  葦互丐月井手  贗癲    	
*/
void lssproto_WN_send(int fd,int windowtype,int buttontype,int seqno,int objindex,char* data)
{
	char buffer[65500];
	int checksum=0;
#ifdef _DEBUG_JZ
	print("WN_SEND:START");
#endif
#ifdef _NO_WARP
	// shan hanjj add Begin
    CONNECT_set_seqno(fd,seqno);
    CONNECT_set_selectbutton(fd,buttontype);    
	// shan End
#endif

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, windowtype);
	checksum += util_mkint(buffer, buttontype);
	checksum += util_mkint(buffer, seqno);
	checksum += util_mkint(buffer, objindex);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_WN_SEND, buffer);
	
}

/*
   servertoclient EF( int effect, int level, string option );
       (EFfect)
       濕支惚毛嫦日仄凶曰〔弁仿奶失件玄卞蟈  讀卅躲絆  蜇六方午霜月〔
       
         int effect
         躲絆  寞〔公木冗木毛簫仄遙允月午〕    午手及躲絆互  日木引允〔ㄢ卞允月午惚午濕互嫦勻凶曰午井仄引允〔
           
             1:惚
             2:濕
             4:餅蹄濕    爛  
           
         int level
         躲絆及雄今〔ㄟ反壅允〔ㄠ  ㄤ反  蜇及雄今毛  允〔
         string option
         漆  傀艦迕
*/
/* Arminius
  新增給丟骰子的動畫
  int effect = 10   丟骰子
      level dont care
      option = "pet1x|pet1y|dice1|pet2x|pet2y|dice2" (no makeEscapeString)
*/
void lssproto_EF_send(int fd,int effect,int level,char* option)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, effect);
	checksum += util_mkint(buffer, level);
	checksum += util_mkstring(buffer, option);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EF_SEND, buffer);
}

/*
   servertoclient SE( int x, int y, int senumber, int sw );
       (SoundEffect)
 	弁仿奶失件玄卞SE毛  日允方丹卞隙憎允月〔
       
         int x,y
         SE毛  耨葭及甄  〔
         int senumber
             及  寞
         int sw
             毛  日允井鞅戶月井〔
           
             0:鞅戶月
             1:  日允
*/
void lssproto_SE_send(int fd,int x,int y,int senumber,int sw)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, senumber);
	checksum += util_mkint(buffer, sw);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SE_SEND, buffer);
}

/*
   servertoclient ClientLogin(string result);
       ClientLogin及忒蠶〔
       
 	string result
 	"ok" 午中丹  儂  〔仇及  儂  反巨旦弗□皿今木卅中〔
*/
void lssproto_ClientLogin_send(int fd,char* result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CLIENTLOGIN_SEND, buffer);
}

/*
   servertoclient CreateNewChar(string result,string data);
       CreateNewChar及忒蠶〔
       
 	string result
 	"successful" 井 "failed" 及中內木井〔仇及  儂  反巨旦弗□
 	    皿仄卅中〔
 	string data
 	"failed" 及凜反    毛憎允諦棉及葦化坌月  儂
 	      匹丐月〔失市它件玄扔□田井日及忒蠶公及引引匹丐月〔
 	    動票及  儂  

 	    "failed bad parameter"

 	    及樺寧反｝平乓仿綜岳及午五卞筋爛及由仿丟□正及  區毛旋尹化
 	    中月午中丹午毛啦  允月〔仇木反必□丞扔□田□互請  允月丟永
 	    本□斥匹丐月〔仇及  儂  反巨旦弗□皿仄卅中〔
*/
void lssproto_CreateNewChar_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CREATENEWCHAR_SEND, buffer);
}

/*
   servertoclient CharDelete(string result,string data);
       CharDelete 及忒蠶〔
       
 	string result
 	"successful" 井 "failed" 及中內木井〔巨旦弗□皿仄卅中〔
 	string data
 	"failed" 及凜反    毛憎允諦棉及葦化坌月  儂
 	      匹丐月〔失市它件玄扔□田井日及忒蠶公及引引匹丐月〔
 	    犯伉立正毛孔仁引卅中及匹｝巨旦弗□皿仄卅中〔
*/
void lssproto_CharDelete_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARDELETE_SEND, buffer);
}

/*
   servertoclient CharLogin(string result,string data);
       CharaLogin及忒蠶〔
       
 	string result
 	"successful" 井 "failed" 及中內木井〔巨旦弗□皿仄卅中〔
 	string data
 	"failed" 及凜反 公及    及  儂  〔巨旦弗□皿仄卅中〔
*/
void lssproto_CharLogin_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;
	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);

	util_SendMesg(fd, LSSPROTO_CHARLOGIN_SEND, buffer);
}
/*
   servertoclient CharList(string result,string data);
       CharList及忒蠶〔
       
 	string result
 	"successful" 井 "failed" 及中內木井〔巨旦弗□皿仄卅中
 	string data
 	result互"successful"及凜反｝失市它件玄扔□田□卞忡繡今木化
 	    中月允屯化及平乓仿及  蟆｝左皿撲亦件毛毛旦矢□旦匹嗉濠勻凶
 	    域蜊及  儂  〔仇及  儂  毛綜勻化中月及反｝失市它件玄扔□田〔
 	    result 互 "failed" 及凜反    毛憎允諦棉及葦化坌月  儂  匹
 	    丐月〔岳  及凜及左皿撲亦件及  褥反動票及午雲曰匹丐月〔
 	    dataplace|faceimage|level|maxhp|atk|def|quick|charm|earth|water|fire|wind|logincount|name|place
 	    
 	      dataplace
 	      本□皮犯□正及窒    井〔仇木匹平乓仿弁正□伉旦玄及窒    井毛瑁爛允月〔
 	      faceimage
 	      湧及  颶  寞
 	      level
 	      平乓仿及伊矛伙
 	      maxhp,atk,def,quick,charm
 	      備由仿丟□正〔
 	      earth.water,fire,wind
 	      備簞嶺襖
 	      logincount
 	      夫弘奶件市它件玄
 	      name
 	      平乓仿及  蟆
 	      place
 	      平乓仿及中月樺赭
 	    
 	    "|" 匹嗉濠日木化中月〔 公木冗木及嫩  反｝<a
 	    href="#escaping">巨旦弗□皿今木化中月〔公及丐午凶化徇
 	    匹勾卅僕月〔
*/
void lssproto_CharList_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLIST_SEND, buffer);
	IsSucceed(fd,result);
}

/*
   servertoclient CharLogout(string result , string data);
       Logout卞覆允月忒蠶〔
       
 	string result
 	"successful" 井 "failed" 及中內木井〔巨旦弗□皿仄卅中〔
 	string data
 	"failed" 及凜卞及心啦  互丐曰｝撩  及    (橇謫)毛憎允諦棉
 	    及葦化坌月  儂  匹丐月〔巨旦弗□皿仄卅中〔
*/
void lssproto_CharLogout_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLOGOUT_SEND, buffer);
}

/*
   servertoclient ProcGet( string data);
 	ProcGet及忒蠶〔
       
 	string data
 	巨旦弗□皿允月〔必□丞扔□田□及    樹  毛霜耨允月〔    反log/proc白央奶伙卞踏井木月    午  元〔
*/
void lssproto_ProcGet_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");
	
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PROCGET_SEND, buffer);
}

/*
   servertoclient PlayerNumGet( int logincount, int player);
 	PlayerNumGet及忒蠶〔
       
 	int logincount,player
*/
void lssproto_PlayerNumGet_send(int fd,int logincount,int player)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, logincount);
	checksum += util_mkint(buffer, player);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PLAYERNUMGET_SEND, buffer);
}

/*
   servertoclient Echo( string test );
       Echo卞覆允月忒蠶〔
       
 	string test
 	今五幻升    今木凶  儂  〔巨旦弗□皿仄卅中〔
*/
void lssproto_Echo_send(int fd,char* test)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, test);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ECHO_SEND, buffer);
}

// CoolFish: Trade 2001/4/18

void lssproto_TD_send(int fd, int index, char* message)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_TD_SEND, buffer);
}
#ifdef _CHATROOMPROTOCOL			// (不可開) Syu ADD 聊天室頻道
void lssproto_CHATROOM_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHATROOM_SEND, buffer);
}
#endif
#ifdef _NEWREQUESTPROTOCOL			// (不可開) Syu ADD 新增Protocol要求細項
void lssproto_RESIST_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RESIST_SEND, buffer);
}
#endif
#ifdef _OUTOFBATTLESKILL			// (不可開) Syu ADD 非戰鬥時技能Protocol
void lssproto_BATTLESKILL_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_BATTLESKILL_SEND, buffer);
}
#endif
void lssproto_NU_send(int fd, int nu)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, nu);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NU_SEND, buffer);
}


void lssproto_FM_send(int fd, char* message)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FM_SEND, buffer);
}


void lssproto_WO_send(int fd,int effect)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, effect);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_WO_SEND, buffer);
}
#ifdef _ITEM_CRACKER
void lssproto_IC_send(int fd, int x, int y)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_IC_SEND, buffer);
}
#endif
#ifdef _MAGIC_NOCAST             // 精靈:沉默
void lssproto_NC_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NC_SEND, buffer);
}
#endif

#ifdef _CHECK_GAMESPEED
void lssproto_CS_send( int fd, int deltimes)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint( buffer, deltimes);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CS_SEND, buffer);
}
#endif

#ifdef _STREET_VENDOR
void lssproto_STREET_VENDOR_send(int fd,char *message)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,message);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_STREET_VENDOR_SEND,buffer);
}
#endif

#ifdef _RIGHTCLICK
void lssproto_RCLICK_send(int fd, int type, char* data)
{
	char buffer[65500];
	int checksum=0;

	print("\n RCLICK_send( type=%d data=%s) ", type, data );

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, type);
	checksum += util_mkint(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RCLICK_SEND, buffer);
}
#endif

#ifdef _JOBDAILY
void lssproto_JOBDAILY_send(int fd,char *data)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	//print("tocli_data:%s ",data);
	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	//print("tocli_buffer:%s ",buffer);
	util_SendMesg(fd,LSSPROTO_JOBDAILY_SEND,buffer);
}
#endif

#ifdef _TEACHER_SYSTEM
void lssproto_TEACHER_SYSTEM_send(int fd,char *data)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_TEACHER_SYSTEM_SEND,buffer);
}
#endif

#ifdef _ADD_STATUS_2
void lssproto_S2_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_S2_SEND, buffer);
}
#endif
