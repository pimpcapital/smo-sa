#define __CONFIGFILE_C__
#include "version.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "util.h"
//#include "configfile.h"
//ttom
#include "lssproto_util.h"
#include "configfile.h"
#include "net.h"
//ttom end
#include "npcutil.h"
// Arminius 7.12 login announce
#include "char.h"

// CoolFish: add 
#include "lssproto_serv.h"

#include "npc_raceman.h"

/* 澀爛毛忡  允月厭瞻   */
typedef struct tagConfig
{
    /*皿夫弘仿丞  (憤  讀卞菲戶凶中仃升引分蟯  */
    char    progname[8];

    char    configfilename[32]; /* config白央奶伙   */
    unsigned char debuglevel;   /* 犯田永弘伊矛伙 */

    unsigned int  usememoryunit;    /*丟乒伉及交瓦永玄扔奶術 */
    unsigned int  usememoryunitnum; /*丟乒伉及交瓦永玄醒 */

    char    asname[32];         /*失市它件玄扔□田及  蟆*/
    unsigned short  acservport; /*失市它件玄扔□田及禾□玄  */
    char    acpasswd[32];       /*失市它件玄扔□田尺及由旦伐□玉*/
    char    gsnamefromas[32];   /*
                                 * 失市它件玄扔□田井日葦尹月
                                 * 必□丞扔□田午仄化及  蟆
                                 */
                                 
    // Arminius 7.24 manor pk
    char gsid[32];	// game server chinese id
#ifdef _SERVICE    
    // Terry 2001/10/03
    char apid[32];      // service ap id
		unsigned short apport;	//service ap port
    int  looptime;	// 幾秒後設定離開
    int  enableservice;	// 是否使用石器服務員功能
#endif    
    unsigned short allowmanorpk;	// is this server allow manor pk
                                 
    unsigned short port;        /* 扔□田及謹切熬仃禾□玄 */
	
	int				servernumber;	/* 必□丞扔□田及  寞 */
	int				reuseaddr;	/* Address already used... 互鞅引日卅中凜及凶戶卞 */
    int             do_nodelay;     /* TCP_NODELAY 卞允月井升丹井 */
    int             log_write_time; /* 踏五仇心夫弘毛允月井升丹井〔 */
    int             log_io_time;    /* I/O蟈  及凜棉反井月井升丹井〔 */
    int             log_game_time;  /* 必□丞及質  蟈  及凜棉毛反井月 */
    int             log_netloop_faster; /* netloop_faster 及夫弘 */
	int				saacwritenum;	/* 失市它件玄扔□田尺及窒諦  卞write允月井 */
	int				saacreadnum;	/* 失市它件玄扔□田井日及dispatch 毛窒莢允月井 */

    unsigned short fdnum;           /*升木分仃戊生弁撲亦件毛忡  允月井 */
    unsigned int   othercharnum;    /*  公及職及平乓仿及醒  */

    unsigned int objnum;            /* 左皮斥尼弁玄及    醒*/
    unsigned int   petcharnum;   	/* 矢永玄及醒    */
    unsigned int itemnum;           /* 失奶  丞及    醒*/
    unsigned int battlenum;         /* 田玄伙及    */

    char    topdir[64];         	/* 玄永皿犯奴伊弁玄伉  */

    char    mapdir[64];         	/* 穴永皿犯奴伊弁玄伉  */
    char    maptilefile[64];    	/* 穴永皿澀爛白央奶伙  */
    char    battlemapfile[64];    	/* 田玄伙穴永皿澀爛白央奶伙  */
    char    itemfile[64];       	/* 失奶  丞澀爛白央奶伙  */
    char    invfile[64];        	/*   襯澀爛白央奶伙  */
    char    appearfile[64];     	/* 請蜇匏  澀爛白央奶伙  */
	char	titlenamefile[64];		/* 憊寞白央奶伙   */
	char	titleconfigfile[64];	/* 憊寞澀爛白央奶伙   */
	char	encountfile[64];		/* 巨件市它件玄澀爛白央奶伙   */
	char	enemybasefile[64];		/* 襯湘  澀爛白央奶伙   */
	char	enemyfile[64];			/* 襯澀爛白央奶伙   */
	char	groupfile[64];			/* 弘伙□皿澀爛白央奶伙   */
	char	magicfile[64];			/* 熱諸澀爛白央奶伙   */
	        #ifdef __ATTACK_MAGIC


        char    attmagicfile[64];       // 攻擊性咒術

        #endif

	char	petskillfile[64];		/* 矢永玄  熱諸澀爛白央奶伙   */
    char    itematomfile[64];       /* 失奶  丞及箋    白央奶伙 */
    char    effectfile[64];     	/* 梢請澀爛白央奶伙  */
    char    quizfile[64];     		/* 弁奶術澀爛白央奶伙  */


    char    lsgenlog[64];       /*扔□田及lsgen 失它玄皿永玄白央奶伙  */

    char    storedir[64];       /*旦玄失犯奴伊弁玄伉    */
    char    npcdir[64];         /*NPC及澀爛白央奶伙毛  仁犯奴伊弁玄伉   */

    char    logdir[64];         /*
                                 * 夫弘犯奴伊弁玄伉
                                 */
    char    logconfname[64];    /*
                                 * 夫弘澀爛白央奶伙  
                                 */
    char	chatmagicpasswd[64];	/* 民乓永玄  芊由旦伐□玉 */

#ifdef _STORECHAR
    char	storechar[64];
#endif
	
    unsigned int 	chatmagiccdkeycheck;	/* 民乓永玄  芊匹CDKEY毛民尼永弁允月井 */
    
    unsigned int    filesearchnum;     /*白央奶伙毛腹綢匹五月白央奶伙及醒*/
    unsigned int    npctemplatenum;     /*NPC及  件皿伊□玄白央奶伙及醒*/
    unsigned int    npccreatenum;       /*NPC及戲遣白央奶伙及醒*/
    unsigned int    walksendinterval;   /* 洶仁及毛霜月棉厥 */
    unsigned int    CAsendinterval_ms;     /* CA毛霜月棉厥 (ms)*/
    unsigned int    CDsendinterval_ms;     /* CD毛霜月棉厥 (ms)*/
    unsigned int    Onelooptime_ms;     	/* 1伙□皿卞井仃月凜棉 */
	unsigned int	Petdeletetime;		/* 矢永玄互壅  允月凜棉 */
	unsigned int	Itemdeletetime;		/* 失奶  丞互壅  允月凜棉 */
    /* 夫弘奶件  及平乓仿及本□皮毛允月棉厥 */
    unsigned int    CharSavesendinterval;
    unsigned int    addressbookoffmsgnum;  /*
                                            * 失玉伊旦皮永弁卞左白仿奶件
                                            * 丟永本□斥毛
                                            * 窒丟永本□斥酸六月井
                                            */

    unsigned int    protocolreadfrequency;  /*
                                             * 皿夫玄戊伙毛窒立伉    
                                             * 卞  戈井
                                             */
    unsigned int    allowerrornum;          /*
                                             * 巨仿□毛窒蜊引匹袱允井
                                             */
    unsigned int    loghour;          		/*
                                             * 夫弘毛忡繡允月凜對  〞凜  
                                             */
    unsigned int    battledebugmsg;    		/*
                                             * 田玄伙  及犯田永弘丟永本□斥毛請允井〔ㄟ卅日請今卅中
                                             */
    //ttom add this because the second had this                                         
    unsigned int    encodekey;              
    unsigned int    acwbsize;             
    unsigned int    acwritesize;
    unsigned int    ErrUserDownFlg;
    //ttom end
#ifdef _GMRELOAD
	char	gmsetfile[64];				/* GM帳號、權限設定檔 */
#endif

#ifdef _AUCTIONEER
  char auctiondir[256];   // 拍賣資料目錄
#endif
#ifdef _BLACK_MARKET 
  char blackmarketfile[256];
#endif
#ifdef _M_SERVER
    char    msname[32];         
    unsigned short  msport;
#endif
#ifdef _NPCSERVER_NEW
	char    nsaddress[64];         
	unsigned short  nsport;
#endif

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
	char profession[64];
#endif

#ifdef _ITEM_QUITPARTY
    char itemquitparty[64];
#endif

#ifdef _MUSEUM
	int museum;
#endif

#ifdef _DEL_DROP_GOLD	
	unsigned int	Golddeletetime;
#endif
#ifdef _HELP_NEWHAND
	int jzitem[15];
#endif
#ifdef _JZ_NEW_CONF
	int jzpet[4];
	int jzgetridepet;
	int jzexp;
	int jzupexp;
	int jzgold;
	int jzridets;
	int jzpetlevel;
#endif

#ifdef _JZ_RIDE
	char Ridefile[64];
#endif
 	char MemberWord[64];
#ifdef _JZ_CAPTURE_FREES
	char CapFreeFile[64];
#endif
	int FMTimeLimit;
	int HouConWarTime;
	int HouFightWarTime;
	int NowEvent[5];
	int EndEvent[5];
#ifdef _JZ_CF_DELPETITEM
	int DelPet[5];
	int DelItem[5];
#endif
#ifdef _JZ_VIPCODE
	int SaveNumberMultiple;
#endif
	char ping;
	char watcallmap;
	char getpetup;
	int watchmap[5];
	int unwarpmap[5];
	int jz_maxlevel;
	int transmaxlevel;
	char expfile[64];
	int maxchar;
#ifdef _JZ_BASE_IF
	int baseif;
#endif
#ifdef _JZ_CIRC_AN
	int loopantime;
	char loopanfile[64];
#endif
#ifdef _JZ_ANGELOPEN
	char AngelOpen;
#endif
	int maxcharor;
	int ipconn;
	int evsls;
	int recvbuffer;
	int readbuffer;
	int acwritebuf;
	int runlevel;
}Config;
Config config;

/*
 *  戊件白奴弘白央奶伙毛  戈凜卞銀丹厭瞻  
 *  xxxx=yyyy 心凶中卅及毛  戈
 */

typedef struct tagReadConf
{
    char    name[32];       /*xxxx卞丐凶月襖*/

    /*戚及2勾反NULL毛  木月午窒手質  仄卅中*/
    char  *charvalue;      /*yyyy毛公及引引醫  允月凜及醫  燮*/
    size_t  charsize;       /*charvalue及扔奶術*/

    /*
     *     酷  午仄化=及  互 "ON"分勻凶日 intvalue 卞反1毛醫  允月
     * 公木動陸反 atoi 及瑛絆
     */
    void*    value;       /*yyyy毛  晶允月醫  允月凜及醫  燮*/
    CTYPE    valuetype;
}ReadConf;

ReadConf readconf[]=
{
		
		{ "runlevel"      , NULL ,0 , (void*)&config.runlevel      ,INT},
		{ "evsls"      , NULL ,0 , (void*)&config.evsls      ,INT},
		{ "recvbuffer"      , NULL ,0 , (void*)&config.recvbuffer      ,INT},
		{ "readbuffer"      , NULL ,0 , (void*)&config.readbuffer      ,INT},
		{ "acwritebuf"      , NULL ,0 , (void*)&config.acwritebuf      ,INT},
		{ "ipconn"      , NULL ,0 , (void*)&config.ipconn      ,INT},
		{ "maxcharor"      , NULL ,0 , (void*)&config.maxcharor      ,INT},
#ifdef _JZ_ANGELOPEN
		{ "AngelOpen"      , NULL ,0 , (void*)&config.AngelOpen      ,CHAR},
#endif
#ifdef _JZ_CIRC_AN
		{ "ANNOUNCETIME"      , NULL ,0 , (void*)&config.loopantime      ,INT},
		{ "ANNOUNCEPATH"  , config.loopanfile,sizeof(config.loopanfile),NULL,0},
#endif
		{ "baseif"      , NULL ,0 , (void*)&config.baseif      ,INT},
		{ "maxchar"      , NULL ,0 , (void*)&config.maxchar      ,INT},
		{ "expfile"  , config.expfile,sizeof(config.expfile),NULL,0},
		{ "transmaxlevel"      , NULL ,0 , (void*)&config.transmaxlevel      ,INT},
		{ "maxlevel"      , NULL ,0 , (void*)&config.jz_maxlevel      ,INT},
		{ "unwarpmap1"      , NULL ,0 , (void*)&config.unwarpmap[0]      ,INT},
		{ "unwarpmap2"      , NULL ,0 , (void*)&config.unwarpmap[1]      ,INT},
		{ "unwarpmap3"      , NULL ,0 , (void*)&config.unwarpmap[2]      ,INT},
		{ "unwarpmap4"      , NULL ,0 , (void*)&config.unwarpmap[3]      ,INT},
		{ "unwarpmap5"      , NULL ,0 , (void*)&config.unwarpmap[4]      ,INT},
		{ "watchmap1"      , NULL ,0 , (void*)&config.watchmap[0]      ,INT},
		{ "watchmap2"      , NULL ,0 , (void*)&config.watchmap[1]      ,INT},
		{ "watchmap3"      , NULL ,0 , (void*)&config.watchmap[2]      ,INT},
		{ "watchmap4"      , NULL ,0 , (void*)&config.watchmap[3]      ,INT},
		{ "watchmap5"      , NULL ,0 , (void*)&config.watchmap[4]      ,INT},
		{ "getpetup"      , NULL ,0 , (void*)&config.getpetup      ,CHAR},
		{ "watcallmap"      , NULL ,0 , (void*)&config.watcallmap      ,CHAR},
		{ "ping"      , NULL ,0 , (void*)&config.ping      ,CHAR},
#ifdef _JZ_VIPCODE
		{ "online"      , NULL ,0 , (void*)&config.SaveNumberMultiple      ,INT},
#endif
#ifdef _JZ_CF_DELPETITEM
		{ "delitem1"      , NULL ,0 , (void*)&config.DelItem[0]      ,INT},
		{ "delitem2"      , NULL ,0 , (void*)&config.DelItem[1]      ,INT},
		{ "delitem3"      , NULL ,0 , (void*)&config.DelItem[2]      ,INT},
		{ "delitem4"      , NULL ,0 , (void*)&config.DelItem[3]      ,INT},
		{ "delitem5"      , NULL ,0 , (void*)&config.DelItem[4]      ,INT},
		{ "delpet1"      , NULL ,0 , (void*)&config.DelPet[0]      ,INT},
		{ "delpet2"      , NULL ,0 , (void*)&config.DelPet[1]      ,INT},
		{ "delpet3"      , NULL ,0 , (void*)&config.DelPet[2]      ,INT},
		{ "delpet4"      , NULL ,0 , (void*)&config.DelPet[3]      ,INT},
		{ "delpet5"      , NULL ,0 , (void*)&config.DelPet[4]      ,INT},
#endif
		{ "endevent1"      , NULL ,0 , (void*)&config.EndEvent[0]      ,INT},
		{ "endevent2"      , NULL ,0 , (void*)&config.EndEvent[1]      ,INT},
		{ "endevent3"      , NULL ,0 , (void*)&config.EndEvent[2]      ,INT},
		{ "endevent4"      , NULL ,0 , (void*)&config.EndEvent[3]      ,INT},
		{ "endevent5"      , NULL ,0 , (void*)&config.EndEvent[4]      ,INT},
		{ "nowevent1"      , NULL ,0 , (void*)&config.NowEvent[0]      ,INT},
		{ "nowevent2"      , NULL ,0 , (void*)&config.NowEvent[1]      ,INT},
		{ "nowevent3"      , NULL ,0 , (void*)&config.NowEvent[2]      ,INT},
		{ "nowevent4"      , NULL ,0 , (void*)&config.NowEvent[3]      ,INT},
		{ "nowevent5"      , NULL ,0 , (void*)&config.NowEvent[4]      ,INT},
		{ "HouseholdFightWarTime"      , NULL ,0 , (void*)&config.HouFightWarTime      ,INT},	
		{ "HouseholdConfirmWarTime"      , NULL ,0 , (void*)&config.HouConWarTime      ,INT},	
		{ "JOINFAMILYTIME"      , NULL ,0 , (void*)&config.FMTimeLimit      ,INT},	
#ifdef _JZ_CAPTURE_FREES
		{ "CAPTUREFREESFILE"  , config.CapFreeFile,sizeof(config.CapFreeFile),NULL,0},
#endif
		{ "MEMBER"  , config.MemberWord,sizeof(config.MemberWord),NULL,0},
#ifdef _JZ_RIDE
		{ "Ridefile"  , config.Ridefile,sizeof(config.Ridefile),NULL,0},
#endif
#ifdef _JZ_NEW_CONF
		{ "petlevel"      , NULL ,0 , (void*)&config.jzpetlevel      ,INT},	
		{ "pet1"      , NULL ,0 , (void*)&config.jzpet[0]      ,INT},	
		{ "pet2"      , NULL ,0 , (void*)&config.jzpet[1]      ,INT},	
		{ "pet3"      , NULL ,0 , (void*)&config.jzpet[2]      ,INT},	
		{ "pet4"      , NULL ,0 , (void*)&config.jzpet[3]      ,INT},	
		{ "getridepet"      , NULL ,0 , (void*)&config.jzgetridepet      ,INT},	
		{ "exp"      , NULL ,0 , (void*)&config.jzexp      ,INT},	
		{ "upexp"      , NULL ,0 , (void*)&config.jzupexp      ,INT},	
		{ "gold"      , NULL ,0 , (void*)&config.jzgold      ,INT},	
		{ "ridets"      , NULL ,0 , (void*)&config.jzridets      ,INT},	
#endif
#ifdef _HELP_NEWHAND
		{ "item1"      , NULL ,0 , (void*)&config.jzitem[0]      ,INT},
		{ "item2"      , NULL ,0 , (void*)&config.jzitem[1]      ,INT},
		{ "item3"      , NULL ,0 , (void*)&config.jzitem[2]      ,INT},
		{ "item4"      , NULL ,0 , (void*)&config.jzitem[3]      ,INT},
		{ "item5"      , NULL ,0 , (void*)&config.jzitem[4]      ,INT},
		{ "item6"      , NULL ,0 , (void*)&config.jzitem[5]      ,INT},
		{ "item7"      , NULL ,0 , (void*)&config.jzitem[6]      ,INT},
		{ "item8"      , NULL ,0 , (void*)&config.jzitem[7]      ,INT},
		{ "item9"      , NULL ,0 , (void*)&config.jzitem[8]      ,INT},
		{ "item10"      , NULL ,0 , (void*)&config.jzitem[9]      ,INT},
		{ "item11"      , NULL ,0 , (void*)&config.jzitem[10]      ,INT},
		{ "item12"      , NULL ,0 , (void*)&config.jzitem[11]      ,INT},
		{ "item13"      , NULL ,0 , (void*)&config.jzitem[12]      ,INT},
		{ "item14"      , NULL ,0 , (void*)&config.jzitem[13]      ,INT},
		{ "item15"      , NULL ,0 , (void*)&config.jzitem[14]      ,INT},
#endif
    { "debuglevel"      , NULL ,0 , (void*)&config.debuglevel      ,CHAR},

    { "usememoryunit"   , NULL ,0 , (void*)&config.usememoryunit   ,INT},
    { "usememoryunitnum", NULL ,0 , (void*)&config.usememoryunitnum,INT},

    { "acserv",			config.asname,sizeof(config.asname) ,NULL , 0},
    { "acservport",		NULL ,0 , (void*)&config.acservport     ,SHORT},
    { "acpasswd",		config.acpasswd,sizeof( config.acpasswd),NULL,0},
    { "gameservname",	config.gsnamefromas,sizeof(config.gsnamefromas),
     NULL,0},

    // Arminius 7.24 manor pk
    { "gameservid", config.gsid, sizeof(config.gsid), NULL, 0},
#ifdef _SERVICE    
    // Terry 2001/10/03 service ap
    { "apid", config.apid, sizeof(config.apid), NULL, 0},
		{ "apport",	NULL ,0 ,(void*)&config.apport ,SHORT},
    { "looptime",NULL,0,(void*)&config.looptime,INT},
    { "enableservice",NULL,0,(void*)&config.enableservice,INT},
#endif    
    { "allowmanorpk", NULL, 0, (void*)&config.allowmanorpk, SHORT},

    { "port",			NULL ,0 , (void*)&config.port           ,SHORT},
    { "servernumber",	NULL ,0 , (void*)&config.servernumber           ,INT},

    { "reuseaddr",			NULL ,0 , (void*)&config.reuseaddr  ,		INT},
    { "nodelay",			NULL , 0 , (void*)&config.do_nodelay ,		INT},
    { "log_write_time", 	NULL, 0 , (void*)&config.log_write_time,	INT},
    { "log_io_time", 		NULL, 0 , (void*)&config.log_io_time, 		INT},
    { "log_game_time",		NULL, 0 , (void*)&config.log_game_time,		INT},
    { "log_netloop_faster", NULL,0,(void*)&config.log_netloop_faster,	INT},
    { "saacwritenum",		NULL,0,(void*)&config.saacwritenum, 		INT},
    { "saacreadnum",		NULL,0,(void*)&config.saacreadnum, 			INT},
    { "fdnum",				NULL ,0 , (void*)&config.fdnum,				SHORT},
    { "petnum",				NULL ,0 , (void*)&config.petcharnum,		INT},
    { "othercharnum",		NULL ,0 , (void*)&config.othercharnum,		INT},

    { "objnum",			NULL ,0 , (void*)&config.objnum,				INT},
    { "itemnum",		NULL ,0 , (void*)&config.itemnum,				INT},
    { "battlenum",		NULL ,0 , (void*)&config.battlenum,				INT},

    { "topdir"          , config.topdir,sizeof(config.topdir),NULL,0},
    { "mapdir"          , config.mapdir,sizeof(config.mapdir),NULL,0},
    { "maptilefile"     , config.maptilefile,sizeof(config.maptilefile),NULL,0},
    { "battlemapfile"   , config.battlemapfile,sizeof(config.battlemapfile),NULL,0},

#ifdef _ITEMSET6_TXT
	{ "itemset6file",	config.itemfile,	sizeof(config.invfile),	NULL,	0},
#else
#ifdef _ITEMSET5_TXT
	{ "itemset5file",	config.itemfile,	sizeof(config.invfile),	NULL,	0},
#else
#ifdef _ITEMSET4_TXT
	{ "itemset4file"  , config.itemfile,sizeof(config.invfile),NULL,0},
#else
#ifdef _ITEMSET3_ITEM
	{ "itemset3file"  , config.itemfile,sizeof(config.invfile),NULL,0},
#endif
#endif
#endif
#endif
    { "invinciblefile"  , config.invfile,sizeof(config.invfile),NULL,0},
    { "appearpositionfile"  , config.appearfile,sizeof(config.appearfile),NULL,0},
	{ "titlenamefile", config.titlenamefile, sizeof( config.titlenamefile),NULL,0},
	{ "titleconfigfile", config.titleconfigfile, sizeof( config.titleconfigfile),NULL,0},
	{ "encountfile", config.encountfile, sizeof( config.encountfile),NULL,0},
	{ "enemyfile", config.enemyfile, sizeof( config.enemyfile),NULL,0},
	{ "enemybasefile", config.enemybasefile, sizeof( config.enemybasefile),NULL,0},
	{ "groupfile", config.groupfile, sizeof( config.groupfile),NULL,0},
	{ "magicfile", config.magicfile, sizeof( config.magicfile),NULL,0},
#ifdef __ATTACK_MAGIC
	{ "attmagicfile" , config.attmagicfile , sizeof( config.attmagicfile )  , NULL , 0 },
#endif

#ifdef _PETSKILL2_TXT
	{ "petskillfile2", config.petskillfile, sizeof( config.petskillfile),NULL,0},
#else
	{ "petskillfile1", config.petskillfile, sizeof( config.petskillfile),NULL,0},
#endif

    { "itematomfile" , config.itematomfile, sizeof( config.itematomfile),NULL,0},
    { "effectfile"  , config.effectfile,sizeof(config.effectfile),NULL,0},
    { "quizfile"  , config.quizfile,sizeof(config.quizfile),NULL,0},

    { "lsgenlogfilename", config.lsgenlog,sizeof(config.lsgenlog),NULL,0},
#ifdef _GMRELOAD
	{ "gmsetfile", config.gmsetfile, sizeof( config.gmsetfile),NULL,0},
#endif

    { "storedir"        ,config.storedir,sizeof(config.storedir),NULL,0},
    { "npcdir"          ,config.npcdir,sizeof(config.npcdir),NULL,0},
    { "logdir"          ,config.logdir,sizeof(config.logdir),NULL,0},
    { "logconfname"     ,config.logconfname,sizeof(config.logconfname),NULL,0},
    { "chatmagicpasswd", config.chatmagicpasswd, sizeof( config.chatmagicpasswd),NULL,0},
#ifdef _STORECHAR
    { "storechar", config.storechar, sizeof( config.storechar),NULL,0},
#endif
    { "chatmagiccdkeycheck",  NULL,0, &config.chatmagiccdkeycheck,INT},
    { "filesearchnum",  NULL,0, &config.filesearchnum,INT},
    { "npctemplatenum",  NULL,0, &config.npctemplatenum,INT},
    { "npccreatenum",    NULL,0, &config.npccreatenum,INT},
    { "walkinterval" ,NULL,0,(void*)&config.walksendinterval,INT},
    { "CAinterval" ,NULL,0,(void*)&config.CAsendinterval_ms,INT},
    { "CDinterval" ,NULL,0,(void*)&config.CDsendinterval_ms,INT},
    { "CharSaveinterval" ,NULL,0,(void*)&config.CharSavesendinterval,INT},
    { "Onelooptime" ,NULL,0,(void*)&config.Onelooptime_ms,INT},
    { "Petdeletetime" ,NULL,0,(void*)&config.Petdeletetime,INT},
    { "Itemdeletetime" ,NULL,0,(void*)&config.Itemdeletetime,INT},
	 { "addressbookoffmesgnum" ,NULL,0,
      (void*)&config.addressbookoffmsgnum,INT},

    { "protocolreadfrequency" ,NULL,0,
      (void*)&config.protocolreadfrequency,INT},

    { "allowerrornum" ,NULL,0,(void*)&config.allowerrornum,INT},
    { "loghour" ,NULL,0,(void*)&config.loghour,INT},
    { "battledebugmsg" ,NULL,0,(void*)&config.battledebugmsg,INT},
    //ttom add because the second had
    { "encodekey" ,NULL,0,(void*)&config.encodekey,INT},
    { "acwritesize" ,NULL,0,(void*)&config.acwritesize,INT},
    { "acwbsize" ,NULL,0,(void*)&config.acwbsize,INT},
    { "erruser_down" ,NULL,0,(void*)&config.ErrUserDownFlg,INT},    
    //ttom end
#ifdef _AUCTIONEER
    { "auctiondir" , config.auctiondir, sizeof(config.auctiondir),NULL,0},
#endif
#ifdef _BLACK_MARKET
	{ "blackmarketfile", config.blackmarketfile, sizeof(config.blackmarketfile), NULL, 0},
#endif
#ifdef _M_SERVER
    { "msname",		config.msname,sizeof(config.msname) ,NULL , 0},
    { "msport",		NULL ,0 , (void*)&config.msport     ,SHORT},
#endif
#ifdef _NPCSERVER_NEW
    { "npcaddress",		config.nsaddress, sizeof(config.nsaddress) ,NULL , 0},
    { "nsport",		NULL, 0 , (void*)&config.nsport     ,SHORT},
#endif

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
    { "profession",		config.profession, sizeof(config.profession) ,NULL , 0},
#endif

#ifdef _ITEM_QUITPARTY
    { "itemquitparty",	config.itemquitparty, sizeof(config.itemquitparty) ,NULL , 0},
#endif

#ifdef _MUSEUM
    { "museum",			NULL , 0 , (void*)&config.museum ,		INT},
#endif

#ifdef _DEL_DROP_GOLD
	{ "Golddeletetime" ,NULL,0,(void*)&config.Golddeletetime,	INT},
#endif

};

// Arminius 7.12 login announce
char announcetext[8192];
void AnnounceToPlayer(int charaindex)
{
  char *ptr,*qtr;
  
  ptr=announcetext;
  while ((qtr=strstr(ptr,"\n"))!=NULL) {
    qtr[0]='\0';
//    printf("ptr=%s\n",ptr);
    CHAR_talkToCli(charaindex, -1, ptr, CHAR_COLORYELLOW);
    qtr[0]='\n';
    ptr=qtr+1;
  }
  CHAR_talkToCli(charaindex, -1, ptr, CHAR_COLORYELLOW);
  
}

// Robin 0720
void AnnounceToPlayerWN(int fd)
{
	char buf[8192];
	lssproto_WN_send( fd , WINDOW_MESSAGETYPE_LOGINMESSAGE,
		WINDOW_BUTTONTYPE_OK,
		-1, -1,
		makeEscapeString( announcetext, buf, sizeof(buf))
		);
}


void LoadAnnounce(void)
{
    FILE *f;

    memset(announcetext, 0, sizeof(announcetext));
    if ((f=fopen("./announce.txt","r"))!=NULL) {
      fread(announcetext, sizeof(announcetext), 1, f);
      announcetext[sizeof(announcetext)-1]='\0';
      fclose(f);
    }
}
#ifdef _PET_TALKPRO
PTALK pettalktext[PETTALK_MAXID];

void LoadPetTalk(void)
{
	FILE *fp;
	char fn[256];
	char line[ 4096];
	char talkmem[4096];
	int maxid=0;
	char buf1[256], buf2[256], buf3[256];
	int talkNO=-1, mark=-1, i;
	int len = sizeof( talkmem);
	
	memset(talkmem, 0, sizeof(talkmem));
	sprintf(fn, "%s/pettalk/pettalk.menu", getNpcdir());

	for( i=0;i<PETTALK_MAXID;i++)	{
		pettalktext[i].ID = -1;
		strcpy( pettalktext[i].DATA, "\0");
	}

	print("\n LoadPetTalk file:%s", fn);
	fp = fopen( fn, "r");
    if( fp != NULL ) {
		while( fgets( line, sizeof( line), fp)) {
			if( strlen( talkmem) != 0 ) {
				if( talkmem[strlen( talkmem) -1] != '|' ) {
					strcatsafe( talkmem, len, "|");		
				}
			}
			chompex( line);
			strcatsafe( talkmem,len,  line);
		}
		fclose( fp);
    }else	{
		print("...err:not found !");
	}

	talkNO=1;
	while( getStringFromIndexWithDelim( talkmem,"END",talkNO, buf1, sizeof( buf1)) != FALSE	){
		talkNO++;
		if( NPC_Util_GetStrFromStrWithDelim( buf1, "PETTEMPNO", buf2, sizeof( buf2)) == NULL  )
			continue;
		mark=1;
		strcpy( fn,"\0");

		if( getStringFromIndexWithDelim( buf2,",", mark+1,buf3,sizeof( buf3)) != FALSE )	{
			pettalktext[maxid].ID = atoi( buf3);
			if( getStringFromIndexWithDelim( buf2,",", mark,buf3,sizeof( buf3)) != FALSE )	{
				sprintf(fn, "%s/pettalk/%s", getNpcdir(), buf3);
				//print("\n ...file:%s", fn);
				fp = fopen( fn, "r");
				if( fp != NULL )	{
					char line[4096];
					while( fgets( line, sizeof( line), fp ) ) {
						if( strlen( pettalktext[maxid].DATA) != 0 ) {
							if( pettalktext[maxid].DATA[strlen( pettalktext[maxid].DATA) -1] != '|' ) {
								strcatsafe( pettalktext[maxid].DATA, sizeof( pettalktext[maxid].DATA), "|");
							}
						}
						chompex( line);
						strcatsafe( pettalktext[maxid].DATA, sizeof( pettalktext[maxid].DATA), line);
					}
					maxid++;
					fclose( fp);
				}else	{
					print("... err:[%s] not found!", fn);
					pettalktext[maxid].ID=-1;
				}
			}else	{
				pettalktext[maxid].ID=-1;
			}
		}
		print(".");
		if( maxid >= PETTALK_MAXID )
			break;
	}
	print("\n.......maxid=%d", maxid);
	{
		int haveid=0;
		for( i=0;i<PETTALK_MAXID;i++)	{
			if( pettalktext[i].ID >= 0 )	{
				haveid++;
			}
		}
		print("...haveid=%d", haveid);
	}

}

#else
char pettalktext[4096];
void LoadPetTalk(void)
{
  FILE *fp;
  char fn[256];
  char	line[ 4096];
  int len = sizeof( pettalktext);
  
  memset(pettalktext, 0, sizeof(pettalktext));
  sprintf(fn, "%s/pettalk/pettalk.mem", getNpcdir());
  
  fp = fopen( fn, "r");
    if( fp != NULL ) {
		print("\n\n READ pettalk.mem");
		while( fgets( line, sizeof( line), fp)) {
			if( strlen( pettalktext) != 0 ) {
				if( pettalktext[strlen( pettalktext) -1] != '|' ) {
					strcatsafe( pettalktext, len, "|");		
				}
			}
			chompex( line);
			strcatsafe( pettalktext,len,  line);
		}
		fclose( fp);
		print("\n %s", pettalktext);
    }else	{
		print("\n Could't Found pettalk.mem");
	}
}
#endif

#ifdef _GAMBLE_BANK
GAMBLEBANK_ITEMS GB_ITEMS[GAMBLEBANK_ITEMSMAX];

void Load_GambleBankItems( void)
{

	FILE *fp;
	char filename[256];
	char buf1[256];
	char name[128];
	int num,ID,type;
	int i=0;
	sprintf(filename, "./data/gambleitems.txt" );
	print("\n Load GambleItems file:%s ...", filename);
	fp = fopen( filename, "r");
    if( fp != NULL ) {
		while( fgets( buf1, sizeof( buf1), fp) != NULL )	{
			if( strstr( buf1, "#") != 0 ) continue;
			sscanf( buf1,"%s %d %d %d", name, &ID, &num , &type);
			strcpy( GB_ITEMS[i].name, name);
			GB_ITEMS[i].Gnum = num;
			GB_ITEMS[i].ItemId = ID;
			GB_ITEMS[i].type = type;
			i++;
		}
		print("..maxID: %d ", i);
		fclose( fp);
    }else	{
		print("err not found %s", filename);
	}

}
#endif


#ifdef _CFREE_petskill
PETSKILL_CODES Code_skill[PETSKILL_CODE];
void Load_PetSkillCodes( void)
{
	FILE *fp;
	char filename[256];
	char buf1[256];
	char name[128];
	char type[256];
	int num,ID;
	int i=0;
	sprintf(filename, "./data/skillcode.txt" );
	print("\n Load PetSKill Code file:%s", filename);
	fp = fopen( filename, "r");
    if( fp != NULL ) {
		while( fgets( buf1, sizeof( buf1), fp) != NULL )	{
			sscanf( buf1,"%s %d %d %s", name, &num, &ID, type);
			strcpy( Code_skill[i].name, name);
			Code_skill[i].TempNo = num;
			Code_skill[i].PetId = ID;
			strcpy( Code_skill[i].Code, type);
			//print("\n %s|%d|%d|%s|", Code_skill[i].name, Code_skill[i].TempNo, 
			//	Code_skill[i].PetId, Code_skill[i].Code);
			i++;
			if( i >= PETSKILL_CODE ) break;
		}
		fclose( fp);
    }else	{
		print("...not found %s", filename);
	}
	print("\n");
}
#endif

#ifdef _BLACK_MARKET
BOOL LoadBMItem( char* filename)
{
	FILE *fp;	
	int  i, j, k;
	char line[512]="", cTmp[256]="";
	char *ip=NULL, *gp=NULL;	
    
	for(i=0; i<BMIMAX; i++){		
		BMItem[i].iGraphicsNum      = 0;
		BMItem[i].GCondition        = 0;
		for(j=0; j<4; j++){			
		    BMItem[i].iCondition[j] = 0;
			for(k=0; k<3; k++){			
				BMItem[i].iId[j][k] = 0;
			}
		}
		strcpy( BMItem[i].iName, "");
	}	
	for(i=0; i<12; i++) BMSellList[i] = -1;

	fp = fopen( filename, "r");

	if(fp==NULL){
		print("\nFault!! Can't Open File:%s ...\n", filename);
		return FALSE;
	}else{
		while(fgets( line, sizeof(line), fp)!=NULL){			
			char cTmp1[256]="", cTmp2[256]="", cTmp3[256]="";
			char iTmp1[128]="", iTmp2[128]="", iTmp3[128]="", iTmp4[128]="", iTmp5[128]="";

			if(BMINum>=BMIMAX){
				print("\nWarning!! To beyond the scope of the itemnum(%d).", BMIMAX);
				break;
			}
			sscanf( line, "%s %d %s %s %s %s %s", 
				    BMItem[BMINum].iName,
					&BMItem[BMINum].iGraphicsNum,
				    iTmp1, iTmp2, iTmp3, iTmp4,	cTmp);
			
            for(i=0; i<3; i++){
				if(getStringFromIndexWithDelim( iTmp1, ",", i+1, iTmp5, sizeof( iTmp5))!=FALSE)
					BMItem[BMINum].iId[0][i] = atoi(iTmp5);
				if(getStringFromIndexWithDelim( iTmp2, ",", i+1, iTmp5, sizeof( iTmp5))!=FALSE)
					BMItem[BMINum].iId[1][i] = atoi(iTmp5);
				if(getStringFromIndexWithDelim( iTmp3, ",", i+1, iTmp5, sizeof( iTmp5))!=FALSE)
					BMItem[BMINum].iId[2][i] = atoi(iTmp5);
				if(getStringFromIndexWithDelim( iTmp4, ",", i+1, iTmp5, sizeof( iTmp5))!=FALSE)
					BMItem[BMINum].iId[3][i] = atoi(iTmp5);
			}

			ip = strstr( cTmp, "I");
			gp = strstr( cTmp, "G");	

			if( ip && gp && gp>ip){
				strncpy( cTmp1, ip+1, gp-ip-1);				
				for(i=0; i<4; i++)
					if(getStringFromIndexWithDelim( cTmp1, ",", i+1, cTmp3, sizeof( cTmp3))!=FALSE)
						BMItem[BMINum].iCondition[i] = atoi(cTmp3);			
				strcpy( cTmp2, gp+1);
				BMItem[BMINum].GCondition = atoi(cTmp2);	
			}else if( ip && gp && gp<ip){
				strcpy( cTmp1, ip+1);				
				for(i=0; i<4; i++)
					if(getStringFromIndexWithDelim( cTmp1, ",", i+1, cTmp3, sizeof( cTmp3))!=FALSE)
						BMItem[BMINum].iCondition[i] = atoi(cTmp3);				
				strncpy( cTmp2, gp+1, ip-gp-1);
				BMItem[BMINum].GCondition = atoi(cTmp2);
			}else if( gp && !ip){
				strcpy( cTmp2, gp+1);
				BMItem[BMINum].GCondition = atoi(cTmp2);
			}else if( !gp && ip){
				strcpy( cTmp1, ip+1);
				for(i=0; i<4; i++)
					if(getStringFromIndexWithDelim( cTmp1, ",", i+1, cTmp3, sizeof( cTmp3))!=FALSE)
						BMItem[BMINum].iCondition[i] = atoi(cTmp3);
			}else{
				print("\nWarning!! There is not item or gold condition..");
				continue;
			}
			BMINum++;
		}
	}
	for(i=0; i<12; i++)	BMSellList[i] = RAND(0, BMINum-1);		
	fclose(fp);
	return TRUE;
}
#endif

#ifdef _GMRELOAD
BOOL LoadGMSet( char* filename )
{
	FILE* fp;
	int i = 0, gm_num = 0;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		print("File Open Error\n");
		return FALSE;
	}
	for (i = 0; i < GMMAXNUM; i++)
	{
		strcpy(gminfo[i].cdkey, "");
		gminfo[i].level = 0;
	}
	char	line[64], cdkey[64], level[64];
	while(1){
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		chop(line);
		//change 使gmset.txt可以增加註解*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		//*************************************
		gm_num = gm_num + 1;
		if (gm_num > GMMAXNUM)	break;
		easyGetTokenFromString(line, 1, cdkey, sizeof(cdkey));
		if (strcmp(cdkey, "") == 0)	break;
		strncpy(gminfo[gm_num].cdkey, cdkey, sizeof(gminfo[gm_num].cdkey));
		easyGetTokenFromString(line, 2, level, sizeof(level));
		if (strcmp(level, "") == 0)	break;
		gminfo[gm_num].level = atoi(level);
//		print("\ncdkey:%s, level:%d", gminfo[gm_num].cdkey, gminfo[gm_num].level);
	}
	fclose(fp);
	return TRUE;
}
#endif

/*------------------------------------------------------------
 * 皿夫弘仿丞  毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  cahr*
 ------------------------------------------------------------*/
char* getProgname( void )
{
    return config.progname;
}
/*------------------------------------------------------------
 * configfilename 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getConfigfilename( void )
{
    return config.configfilename;
}
/*------------------------------------------------------------
 * configfilename 毛澀爛允月〔
 * 婁醒
 *  newv    char*   蕙仄中襖
 * 忒曰襖
 *  卅仄
 ------------------------------------------------------------*/
void setConfigfilename( char* newv )
{
    strcpysafe( config.configfilename, sizeof( config.configfilename ),
                newv );
}

/*------------------------------------------------------------
 * 犯田永弘伊矛伙毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getDebuglevel( void )
{
    return config.debuglevel;
}
/*------------------------------------------------------------
 * 犯田永弘伊矛伙毛澀爛允月
 * 婁醒
 *  newv    int     蕙仄中襖
 * 忒曰襖
 *  unsigned int    檣及襖
 ------------------------------------------------------------*/
unsigned int setDebuglevel( unsigned int newv )
{
    int old;
    old = config.debuglevel;
    config.debuglevel = newv;
    return old;
}
/*------------------------------------------------------------
 * memoryunit 毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getMemoryunit( void )
{
    return config.usememoryunit;
}
/*------------------------------------------------------------
 * memoryunitnum 毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getMemoryunitnum( void )
{
    return config.usememoryunitnum;
}

/*------------------------------------------------------------
 * 失市它件玄扔□田及失玉伊旦毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char*   getAccountservername( void )
{
    return config.asname;
}
/*------------------------------------------------------------
 * 失市它件玄扔□田及禾□玄毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned short
 ------------------------------------------------------------*/
unsigned short   getAccountserverport( void )
{
    return config.acservport;
}
/*------------------------------------------------------------
 * 失市它件玄扔□田尺及由旦伐□玉毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned short
 ------------------------------------------------------------*/
char*   getAccountserverpasswd( void )
{
    return config.acpasswd;
}
/*------------------------------------------------------------
 * 失市它件玄扔□田井日葦尹月必□丞扔□田午仄化及  蟆毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned short
 ------------------------------------------------------------*/
char*   getGameservername( void )
{
    return config.gsnamefromas;
}

// Arminius 7.24 manor pk
char* getGameserverID( void )
{
    if (config.gsid[strlen(config.gsid)-1]=='\n')
      config.gsid[strlen(config.gsid)-1]='\0';
      
    return config.gsid;
}

#ifdef _SERVICE
// Terry 2001/10/03
char* getApID(void)
{
  return config.apid;
}

unsigned short getApPort(void)
{
  return config.apport; 
}

int getLoopTime(void)
{
  return config.looptime;
}

int getEnableService(void)
{
  return config.enableservice;
}
#endif
  
unsigned short getAllowManorPK( void )
{
    return config.allowmanorpk;
}

unsigned short getPortnumber( void )
{
    return config.port;
}
/*------------------------------------------------------------
 * 必□丞扔□田及  寞   寞毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned short
 ------------------------------------------------------------*/
int getServernumber( void )
{
    return config.servernumber;
}
/*------------------------------------------------------------
 * reuseaddr 及襖毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned short
 ------------------------------------------------------------*/
int getReuseaddr( void )
{
    return config.reuseaddr;
}

int getNodelay( void )
{
    return config.do_nodelay;
}
int getLogWriteTime(void)
{
    return config.log_write_time;
}
int getLogIOTime( void)
{
    return config.log_io_time;
}
int getLogGameTime(void)
{
    return config.log_game_time;
}
int getLogNetloopFaster(void)
{
    return config.log_netloop_faster;
}

/*------------------------------------------------------------
 * saacwritenum 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *   int
 ------------------------------------------------------------*/
int getSaacwritenum( void )
{
    return config.saacwritenum;
}
/*------------------------------------------------------------
 * saacwritenum 毛澀爛允月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *   int
 ------------------------------------------------------------*/
void setSaacwritenum( int num )
{
    config.saacwritenum = num;
}
/*------------------------------------------------------------
 * saacreadnum 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *   int
 ------------------------------------------------------------*/
int getSaacreadnum( void )
{
    return config.saacreadnum;
}
/*------------------------------------------------------------
 * saacreadnum 毛澀爛允月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *   int
 ------------------------------------------------------------*/
void setSaacreadnum( int num )
{
    config.saacreadnum = num;
}
/*------------------------------------------------------------
 * fdnum 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getFdnum( void )
{
    return config.fdnum;
}
/*------------------------------------------------------------
 * petcharanum 毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getPetcharnum( void )
{
    return config.petcharnum;
}


/*------------------------------------------------------------
 * othercharnum 毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getOtherscharnum( void )
{
    return config.othercharnum;
}

/*------------------------------------------------------------
 * objnum 毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getObjnum( void )
{
    return config.objnum;
}

/*------------------------------------------------------------
 * itemnum 毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getItemnum( void )
{
    return config.itemnum;
}


/*------------------------------------------------------------
 * battlenum 毛  月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getBattlenum( void )
{
    return config.battlenum;
}


/*------------------------------------------------------------
 * topdir 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getTopdir( void )
{
    return config.topdir;
}
/*------------------------------------------------------------
 * mapdir 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getMapdir( void )
{
    return config.mapdir;
}
/*------------------------------------------------------------
 * maptilefile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getMaptilefile( void )
{
    return config.maptilefile;
}
/*------------------------------------------------------------
 * battlemapfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getBattleMapfile( void )
{
    return config.battlemapfile;
}
/*------------------------------------------------------------
 * itemfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getItemfile( void )
{
    return config.itemfile;
}
/*------------------------------------------------------------
 * invfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getInvfile( void )
{
    return config.invfile;
}
/*------------------------------------------------------------
 * appearfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getAppearfile( void )
{
    return config.appearfile;
}
/*------------------------------------------------------------
 * effectfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getEffectfile( void )
{
    return config.effectfile;
}
/*------------------------------------------------------------
 * titlenamefile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getTitleNamefile( void )
{
    return config.titlenamefile;
}
/*------------------------------------------------------------
 * titleconfigfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getTitleConfigfile( void )
{
    return config.titleconfigfile;
}
/*------------------------------------------------------------
 * encountfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getEncountfile( void )
{
    return config.encountfile;
}
/*------------------------------------------------------------
 * enemyfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getEnemyfile( void )
{
    return config.enemyfile;
}
/*------------------------------------------------------------
 * enemybasefile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getEnemyBasefile( void )
{
    return config.enemybasefile;
}
/*------------------------------------------------------------
 * groupfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getGroupfile( void )
{
    return config.groupfile;
}
/*------------------------------------------------------------
 * magicfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getMagicfile( void )
{
    return config.magicfile;
}

#ifdef __ATTACK_MAGIC

/*------------------------------------------------------------
 * 取得攻擊性的咒術
 * 參數
 * None
 * 返回值
 * char*
 ------------------------------------------------------------*/
char* getAttMagicfileName( void )
{
    return config.attmagicfile;
}

#endif


char* getPetskillfile( void )
{
    return config.petskillfile;
}

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
char* getProfession( void )
{
    return config.profession;
}
#endif

#ifdef _ITEM_QUITPARTY
char* getitemquitparty( void )
{
    return config.itemquitparty;
}
#endif

char *getItematomfile( void )
{
    return config.itematomfile;
}


char* getQuizfile( void )
{
    return config.quizfile;
}

/*------------------------------------------------------------
 * lsgenlogfile 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getLsgenlogfilename( void )
{
    return config.lsgenlog;
}

#ifdef _BLACK_MARKET
char* getBMItemFile(void){
	return config.blackmarketfile;
}
#endif

#ifdef _GMRELOAD
char* getGMSetfile( void )
{
    return config.gmsetfile;
}
#endif

/*------------------------------------------------------------
 * storedir 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getStoredir( void )
{
    return config.storedir;
}
#ifdef _STORECHAR
/*------------------------------------------------------------
 ------------------------------------------------------------*/
char* getStorechar( void )
{
    return config.storechar;
}
#endif

#ifdef _AUCTIONEER
char* getAuctiondir(void)
{
  return config.auctiondir;
}
#endif

/*------------------------------------------------------------
 * NPC 迕及犯奴伊弁玄伉毛  化仁月楮醒
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getNpcdir( void )
{
    return config.npcdir;
}
/*------------------------------------------------------------
 * 夫弘犯奴伊弁玄伉毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getLogdir( void )
{
    return config.logdir;
}

/*------------------------------------------------------------
 * 夫弘澀爛白央奶伙  毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getLogconffile( void )
{
    return config.logconfname;
}
/*------------------------------------------------------------
 * 民乓永玄  芊由旦伐□玉 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
char* getChatMagicPasswd( void )
{
    return config.chatmagicpasswd;
}
/*------------------------------------------------------------
 * 犯田永弘民乓永玄  芊匹及CDKEY民尼永弁毛允月井升丹井毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  char*
 ------------------------------------------------------------*/
unsigned getChatMagicCDKeyCheck( void )
{
    return config.chatmagiccdkeycheck;
}

/*------------------------------------------------------------
 * filesearchnum毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getFilesearchnum( void )
{
    return config.filesearchnum;
}
/*------------------------------------------------------------
 * npctemplatenum毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getNpctemplatenum( void )
{
    return config.npctemplatenum;
}
/*------------------------------------------------------------
 * npccreatenum毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getNpccreatenum( void )
{
    return config.npccreatenum;
}

/*------------------------------------------------------------
 * walksendinterval毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getWalksendinterval( void )
{
    return config.walksendinterval;
}
/*------------------------------------------------------------
 * walksendinterval毛本永玄允月〔
 * 婁醒
 *  unsigned int 	interval	凜棉  立伉    
 * 忒曰襖
 *  void
 ------------------------------------------------------------*/
void setWalksendinterval( unsigned int interval )
{
    config.walksendinterval = interval;
}
/*------------------------------------------------------------
 * CAsendinterval毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCAsendinterval_ms( void )
{
    return config.CAsendinterval_ms;
}
/*------------------------------------------------------------
 * CAsendinterval毛本永玄允月〔
 * 婁醒
 *  unsigned int interval
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
void setCAsendinterval_ms( unsigned int interval_ms )
{
    config.CAsendinterval_ms = interval_ms;
}
/*------------------------------------------------------------
 * CDsendinterval毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCDsendinterval_ms( void )
{
    return config.CDsendinterval_ms;
}
/*------------------------------------------------------------
 * CDsendinterval毛本永玄允月〔
 * 婁醒
 *  interval		unsigned int
 * 忒曰襖
 * void
 ------------------------------------------------------------*/
void setCDsendinterval_ms( unsigned int interval_ms )
{
	config.CDsendinterval_ms = interval_ms;
}
/*------------------------------------------------------------
 * Onelooptime毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getOnelooptime_ms( void )
{
    return config.Onelooptime_ms;
}
/*------------------------------------------------------------
 * Onelooptime毛本永玄允月〔
 * 婁醒
 *  interval		unsigned int
 * 忒曰襖
 * void
 ------------------------------------------------------------*/
void setOnelooptime_ms( unsigned int interval_ms )
{
	config.Onelooptime_ms = interval_ms;
}
/*------------------------------------------------------------
 * Petdeletetime毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getPetdeletetime( void )
{
    return config.Petdeletetime;
}
/*------------------------------------------------------------
 * Petdeletetime毛本永玄允月〔
 * 婁醒
 *  interval		unsigned int
 * 忒曰襖
 * void
 ------------------------------------------------------------*/
void setPetdeletetime( unsigned int interval )
{
	config.Petdeletetime = interval;
}
/*------------------------------------------------------------
 * Itemdeletetime毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getItemdeletetime( void )
{
    return config.Itemdeletetime;
}
/*------------------------------------------------------------
 * Itemdeletetime毛本永玄允月〔
 * 婁醒
 *  interval		unsigned int
 * 忒曰襖
 * void
 ------------------------------------------------------------*/
void setItemdeletetime( unsigned int interval )
{
	config.Itemdeletetime = interval;
}

/*------------------------------------------------------------
 * CharSavesendinterval毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCharSavesendinterval( void )
{
    return config.CharSavesendinterval;
}
/*------------------------------------------------------------
 * CharSavesendinterval毛本永玄允月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
void setCharSavesendinterval( unsigned int interval)
{
	config.CharSavesendinterval = interval;
}

/*------------------------------------------------------------
 * Addressbookoffmsgnum 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getAddressbookoffmsgnum( void )
{
    return config.addressbookoffmsgnum;
}
/*------------------------------------------------------------
 * Protocolreadfrequency 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getProtocolreadfrequency( void )
{
    return config.protocolreadfrequency;
}

/*------------------------------------------------------------
 * Allowerrornum 毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getAllowerrornum( void )
{
    return config.allowerrornum;
}

/*------------------------------------------------------------
 * 夫弘毛潸  允月凜對毛  月〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getLogHour( void )
{
    return config.loghour;
}

/*------------------------------------------------------------
 * 田玄伙  及犯田永弘丟永本□斥毛請允井〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int 1卅日請允
 ------------------------------------------------------------*/
unsigned int getBattleDebugMsg( void )
{
    return config.battledebugmsg;
}
/*------------------------------------------------------------
 * 田玄伙  及犯田永弘丟永本□斥毛請允井〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int 1卅日請允
 ------------------------------------------------------------*/
void setBattleDebugMsg( unsigned int num )
{
    config.battledebugmsg = num;
}



/*
 * Config及犯白巧伙玄襖毛瑁戶月楮醒
 * 婁醒
 *  argv0   char*   戊穴件玉仿奶件婁醒及  賡
 */
void  defaultConfig( char* argv0 )
{
    char* program;                  /* program  毛菲戶月及卞銀丹 */

    /* 犯白巧伙玄襖毛  木月 */

    /*皿夫弘仿丞  */
    program = rindex(argv0, '/');
    if (program == NULL)
        program = argv0;
    else
        program++;   /* "/"及戚井日卞仄凶中及匹++允月*/
    strcpysafe( config.progname , sizeof( config.progname ) ,program );

    /*澀爛白央奶伙  */
    strcpysafe( config.configfilename,
                sizeof( config.configfilename ),"setup.cf" );

}

/*
 * 澀爛白央奶伙  毛  氏分  匹及質  毛墊丹〔
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  卅仄
 */
void lastConfig( void )
{
    char    entry[256];
    /*  穴永皿犯奴伊弁玄伉及澀爛    */
    snprintf(entry, sizeof(entry), "%s/%s", config.topdir, config.mapdir);
    strcpysafe(config.mapdir, sizeof(config.mapdir), entry);

    /*  穴永皿澀爛白央奶伙  及澀爛    */
    snprintf(entry,sizeof(entry),"%s/%s",
             config.topdir,config.maptilefile);
    strcpysafe(config.maptilefile, sizeof(config.maptilefile), entry);

    /*  田玄伙穴永皿澀爛白央奶伙  及澀爛    */
    snprintf(entry,sizeof(entry),"%s/%s",
             config.topdir,config.battlemapfile);
    strcpysafe(config.battlemapfile, sizeof(config.battlemapfile), entry);

    /*  失奶  丞澀爛白央奶伙  及澀爛    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.itemfile);
    strcpysafe(config.itemfile, sizeof(config.itemfile), entry);

    /*    襯澀爛白央奶伙  及澀爛    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.invfile);
    strcpysafe(config.invfile, sizeof(config.invfile), entry);

    /*  請蜇匏  澀爛白央奶伙  及澀爛    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.appearfile);
    strcpysafe(config.appearfile, sizeof(config.appearfile), entry);

    /*  梢請澀爛白央奶伙  及澀爛    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.effectfile);
    strcpysafe(config.effectfile, sizeof(config.effectfile), entry);

    /*  弁奶術澀爛白央奶伙  及澀爛    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.quizfile);
    strcpysafe(config.quizfile, sizeof(config.quizfile), entry);

    /*  憊寞  白央奶伙  及澀爛    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.titlenamefile);
    strcpysafe(config.titlenamefile, sizeof(config.titlenamefile), entry);

    /*  lsgen 失它玄皿永玄白央奶伙      */
    snprintf(entry,sizeof(entry),"%s/%s", config.topdir,config.lsgenlog);
    strcpysafe(config.lsgenlog, sizeof(config.lsgenlog), entry);

    /*  旦玄失犯奴伊弁玄伉及澀爛    */
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.storedir);
    strcpysafe(config.storedir, sizeof(config.storedir), entry);

    /*  NPC澀爛玄永皿犯奴伊弁玄伉及澀爛    */
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.npcdir);
    strcpysafe(config.npcdir, sizeof(config.npcdir), entry);

#ifdef _STORECHAR
    /*   */
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.storechar);
    strcpysafe(config.storechar, sizeof(config.storechar), entry);
#endif

}


/*
 * 禾奶件正□午贗濠卅滇樹  毛  勻化醫  毛允月楮醒
 * 婁醒
 *  to      void*   襖毛醫  允月禾奶件正
 *  type    CTYPE   to及滇毛瑁戶月
 *  value   double  to卞醫  允月襖
 * 忒曰襖
 *  卅仄
 */
void substitutePointerFromType( void* to , CTYPE type ,double value)
{
    switch( type  ){
    case CHAR:
        *(char*)to = (char)value;
        break;
    case SHORT:
        *(short*)to = (short)value;
        break;
    case INT:
        *(int*)to = (int)value;
        break;
    case DOUBLE:
        *(double*)to = (double)value;
        break;
    }
}


/*------------------------------------------------------------
 * 澀爛白央奶伙毛  戈
 * 婁醒
 *      filename            白央奶伙  
 * 忒曰襖
 *      TRUE(1)     岳  
 *      FALSE(0)    撩      -> 白央奶伙及左□皿件卞撩  仄凶
 ------------------------------------------------------------*/
BOOL readconfigfile( char* filename )
{
    FILE* f=NULL;
    char linebuf[256];                  /* 域墊  心  戈田永白央 */
    int linenum=0;                      /* 墊醒毛醒尹月 */
    char    realopenfilename[256];      /*    端卞open 允月白央奶伙  */

    char    hostname[128];

    /*  石旦玄  毛菲戶月    */
    if( gethostname( hostname, sizeof(hostname) ) != -1 ){
        char*   initdot;
        initdot = index( hostname, '.' );
        if( initdot != NULL )
            *initdot = '\0';
        snprintf( realopenfilename, sizeof(realopenfilename),
                  "%s.%s" , filename, hostname);

        /* 白央奶伙及左□皿件 */
        f=fopen( realopenfilename, "r" );
        if( f == NULL )
            print( "Can't open %s.  use %s instead\n", realopenfilename,
                   filename );
    }
    if( f == NULL ){
        f=fopen( filename , "r" );          /* 白央奶伙及左□皿件 */
        if( f == NULL ){
            print( "Can't open %s\n", filename );
            return FALSE;
        }
    }

    /* 域墊勿勾  心  戈 */
    while( fgets( linebuf , sizeof( linebuf ), f ) ){
        char firstToken[256];       /*1    及  儂  */
        int i;                      /*伙□皿  醒*/
        int ret;                    /*伉正□件戊□玉*/

        linenum ++;

        deleteWhiteSpace(linebuf);          /* remove whitespace    */

        if( linebuf[0] == '#' )continue;        /* comment */
        if( linebuf[0] == '\n' )continue;       /* none    */

        chomp( linebuf );                    /* remove tail newline  */

        /* delim "=" 匹  賡(1)及玄□弁件毛  月*/
        ret = getStringFromIndexWithDelim( linebuf , "=",  1, firstToken,
                                           sizeof(firstToken) );
        if( ret == FALSE ){
            print( "Find error at %s in line %d. Ignore\n",
                     filename , linenum);
            continue;
        }

        /* readconf 及扔奶術匹伙□皿 */
        char secondToken[256];      /*2    及  儂  */
        for( i = 0 ; i < arraysizeof( readconf ) ; i ++ ){
            if( strcmp( readconf[i].name ,firstToken ) == 0 ){
                /* match */
                /* delim "=" 匹2    及玄□弁件毛  月*/
                ret = getStringFromIndexWithDelim( linebuf , "=" , 2
                                                   , secondToken ,
                                                   sizeof(secondToken) );

                /* NULL  儂井升丹井毛譬屯月 */
                if( ret == FALSE ){
                    print( "Find error at %s in line %d. Ignore",
                           filename , linenum);
                    break;
                }


                /*NULL分勻凶日醫  仄卅中*/
                if( readconf[i].charvalue != NULL )
                    strcpysafe( readconf[i].charvalue
                                ,readconf[i].charsize, secondToken);

                /*NULL分勻凶日醫  仄卅中*/
                if( readconf[i].value != NULL ) {
                    if( strcmp( "ON" ,secondToken ) == 0 ) {
                        /*ON分勻凶日1毛  木月*/
                        substitutePointerFromType( readconf[i].value,
                                                   readconf[i].valuetype,
                                                   1.0);

                    }else if( strcmp( "OFF" ,secondToken ) == 0 ) {
                        /*OFF分勻凶日1毛  木月*/
                        substitutePointerFromType( readconf[i].value,
                                                   readconf[i].valuetype,
                                                   1.0);
                    }else {
                        strtolchecknum(secondToken,
                                       (int*)readconf[i].value,
                                       10, readconf[i].valuetype);
					}
				}
                break;
            }
        }
    }
    if (config.jzupexp)config.jzupexp = (config.jzupexp-1)*100;

    fclose( f );
    lastConfig();
    return TRUE;
}
//ttom add this becaus the second had this function 
/*------------------------------------------------------------
 * 巨件戊□玉平□毛澀爛允月
 * 婁醒
 *  卅仄
 * 忒曰襖
 *  unsigned int 平□毛忒允
------------------------------------------------------------*/
unsigned int setEncodeKey( void )
{
   JENCODE_KEY = config.encodekey;
   return JENCODE_KEY;
}
/*------------------------------------------------------------
* 失市它件玄扔□田□卞踏五  戈田永白央及扔奶術毛澀爛允月
* 婁醒
*  卅仄
* 忒曰襖
*  unsigned int 平□毛忒允
------------------------------------------------------------*/
unsigned int setAcWBSize( void )
{
    AC_WBSIZE = config.acwbsize;
    return AC_WBSIZE;
}
unsigned int getAcwriteSize( void )
{
    return config.acwritesize;
}
unsigned int getErrUserDownFlg( void )
{
    return config.ErrUserDownFlg;
}
    
    
//tom end


#ifdef _M_SERVER
char* getmservername(void)
{
	return config.msname;
}
void setmservername( char *msname)
{
	sprintf( config.msname, "%s", msname);
}
unsigned int getmserverport(void)
{
	return config.msport;
}
void setmserverport( int port)
{
	config.msport = port;
}
#endif

#ifdef _NPCSERVER_NEW
char *getnpcserveraddr(void)
{
	return config.nsaddress;
}

unsigned int getnpcserverport(void)
{
	return config.nsport;
}
void NS_setAddressAndPort( char *address, int nport)
{
	sprintf( config.nsaddress, "%s", address);
	config.nsport = nport;
}
#endif

#ifdef _MUSEUM
int   getMuseum( void )
{
    return config.museum;
}
#endif

#ifdef _DEL_DROP_GOLD
unsigned int getGolddeletetime( void )
{
    if( config.Golddeletetime > 0 )
		return config.Golddeletetime;
	else
		return config.Itemdeletetime;
}
void setIGolddeletetime( unsigned int interval )
{
	config.Golddeletetime = interval;
}
#endif

#ifdef _ANGEL_SUMMON

extern int mission_num;

BOOL LoadMissionList( )
{
	FILE* fp;
	int i = 0;

	mission_num = 0;

	fp = fopen(".//data//mission.txt", "r");
	if (fp == NULL)
	{
		print("Mission File Open Error\n");
		return FALSE;
	}
	
	memset( missionlist, 0, sizeof(missionlist));

	while(1){
		char	line[1024], level[64];
		char	token[1024];
		int		mindex;
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		print("\n %s ", line);
		chop(line);
		// 以#為註解*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		//*************************************

		//格式 #任務編號,必要等級,任務說明,獎品ID,限制時間(小時)
		
		getStringFromIndexWithDelim(line, ",", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		mindex = atoi( token);

		if( mindex <= 0 || mindex >= MAXMISSION) 
			break;

		missionlist[mindex].id = mindex;

		getStringFromIndexWithDelim(line, ",", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missionlist[mindex].level = atoi( token);

		getStringFromIndexWithDelim(line, ",", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missionlist[mindex].eventflag, token);

		getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missionlist[mindex].detail, token);

		//getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		//if (strcmp(token, "") == 0)	break;
		//strcpy( missionlist[mindex].bonus, token);

		getStringFromIndexWithDelim(line, ",", 5, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missionlist[mindex].limittime = atoi( token);

		/*print("\nMISSION[%d] lv:%d ef:%s detail:%s limit:%d ", mindex,
			missionlist[mindex].level, missionlist[mindex].eventflag,
			missionlist[mindex].detail, missionlist[mindex].limittime );
		*/
		mission_num++;
		//if (mission_num > MAXMISSION)	break;
	}
	fclose(fp);
	return TRUE;
}



BOOL LoadMissionCleanList( )
{
	// 格式... 使者,勇者,任務,獎賞
	FILE* fp;
	int	listindex =0;
	int i = 0;

	memset( missiontable, 0, sizeof(missiontable));
	fp = fopen(".//data//missionclean.txt", "r");
	if (fp == NULL)
	{
		print("MissionClean File Open Error\n");
		return FALSE;
	}

	while(1){
		char	line[1024], angelinfo[128], heroinfo[128];
		char	token[1024];
		
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		print("\n %s ", line);
		chop(line);
		// 以#為註解*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		
		getStringFromIndexWithDelim(line, ",", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missiontable[listindex].angelinfo, token);

		getStringFromIndexWithDelim(line, ",", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missiontable[listindex].heroinfo, token);

		getStringFromIndexWithDelim(line, ",", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missiontable[listindex].mission = atoi( token);
		
		getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		//if (strcmp(token, "") == 0)	break;
		missiontable[listindex].flag = atoi( token);

		getStringFromIndexWithDelim(line, ",", 5, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missiontable[listindex].time = atoi( token);

		print("\nMISSIONCLEAN[%d] %s %s %d %d %d", listindex,
			missiontable[listindex].angelinfo,
			missiontable[listindex].heroinfo,
			missiontable[listindex].mission,
			missiontable[listindex].flag,
			missiontable[listindex].time );

		listindex++;
		if ( listindex >= MAXMISSIONTABLE)	break;
	}
	fclose(fp);
	return TRUE;
}


#endif

#ifdef _JOBDAILY
extern  DailyFileType dailyfile[MAXDAILYLIST];
BOOL LoadJobdailyfile(void)
{
	char	line[256];
	char	token[256];
	int		listindex =0;
	int     i;
	FILE* fp;

	fp = fopen("./data/jobdaily.txt", "r");
	if (fp == NULL)
	{
		print("Jobdaily File Open Error\n");
		return FALSE;
	}

	memset( dailyfile, 0, sizeof(dailyfile));

	while(1){		
		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		//print("\n %s ", line);
		chop(line);

		// #為註解
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		
		getStringFromIndexWithDelim(line, "|", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( dailyfile[listindex].jobid, token);

		getStringFromIndexWithDelim(line, "|", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( dailyfile[listindex].rule, token);

		getStringFromIndexWithDelim(line, "|", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		if(strlen(token)>96){
			print("任務說明過長:%d",strlen(token));
			return FALSE;
		}
		strcpy( dailyfile[listindex].explain, token);
		
		getStringFromIndexWithDelim(line, "|", 4, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( dailyfile[listindex].state, token);

		/*print("\ndailyfile[%d] %s %s %s %s", listindex,
			dailyfile[listindex].jobid,
			dailyfile[listindex].rule,
			dailyfile[listindex].explain,
			dailyfile[listindex].state);	
		*/
		listindex++;
		if ( listindex >= MAXDAILYLIST)	break;
	}
	fclose(fp);
	return TRUE;
}
#endif

#ifdef _RACEMAN
//extern int petflgtable[640];
//extern struct ASKTABLE asktable[400];
extern int asktotal;
BOOL LoadRacepetfile(void)
{
	char line[1000];
	char token[64];
	int  i;
	FILE* fp;

	asktotal = 0;
	memset(asktable,0,sizeof(asktable));

	fp = fopen(".//data//raceman.txt", "r");
	if (fp == NULL)
	{
		print("Racepet File Open Error\n");
		return FALSE;
	}
	
	while(1){		
		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL)	break;		
		chop(line);

		// #為註解
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		getStringFromIndexWithDelim(line, "|", 1 , token, sizeof(token));
		if( atoi(token) <= 0 )	return FALSE;
		asktable[asktotal].no = atoi(token);

		getStringFromIndexWithDelim(line, "|", 2 , token, sizeof(token));
		if( strlen(token) == 0 ) return FALSE;
		strcpy( asktable[asktotal].petname , token );

		getStringFromIndexWithDelim(line, "|", 3 , token, sizeof(token));
		if( atoi(token) <= 0 )	return FALSE;
		asktable[asktotal].bbi = atoi(token);

		getStringFromIndexWithDelim(line, "|", 4 , token, sizeof(token));
		if( atoi(token) <= 0 )	return FALSE;
		asktable[asktotal].lowlv = atoi(token);

		getStringFromIndexWithDelim(line, "|", 5 , token, sizeof(token));
		if( atoi(token) < 0 || atoi(token) > 140 )	return FALSE;
		asktable[asktotal].highlv = atoi(token);
		asktotal++;
	}
	fclose(fp);
	return TRUE;

	/*
	for( k=0,j=0;j<ENEMYTEMP_enemynum;j++ ) {
		if( ENEMYTEMP_getInt( j, E_T_PETFLG ) == 1 ){
			petflgtable[k] = ENEMYTEMP_getInt(j,E_T_IMGNUMBER) ;
			k++;
			if(k>=640) {
				print( "Valid petflag Num is %d...", k );
				break;
			}
		}
		//if( CHAR_getInt( petindex, CHAR_PETID) == ENEMYTEMP_getInt( j, E_T_TEMPNO ) ) {//ENEMY_getInt( j, ENEMY_TEMPNO)
	}
	*/
}

#endif

#ifdef _HELP_NEWHAND
int getCreatePitem( int num )
{
	return config.jzitem[num];
}
#endif

#ifdef _JZ_NEW_CONF
int getCreatePpetlevel(void)
{
	if (config.jzpetlevel <=0 )return 1;
	if (config.jzpetlevel >=140 )return 140;
	return config.jzpetlevel;
}
int getCreatePpet(int num)
{
	return config.jzpet[num];
}
int getgetridepet(void)
{
	return config.jzgetridepet;
}
int getPexp(void)
{
	return config.jzexp;
}
int getPupexp(void)
{
	return config.jzupexp;
}
int getCreatePgold(void)
{
	return config.jzgold;
}
int getridets(void)
{
	return config.jzridets;
}
#endif

#ifdef _JZ_RIDE
char *getRidefile(void)
{
	return config.Ridefile;
}
#endif

char *getMemberWord(void)
{
	return config.MemberWord;
}

#ifdef _JZ_CAPTURE_FREES
char *getCapFreeFile(void)
{
	return config.CapFreeFile;
}
#endif

int getFMTimeLimit(void)
{
	return config.FMTimeLimit;
}

int getHouConWarTime(void)
{
	return config.HouConWarTime;
}

int getHouFightWarTime(void)
{
	if(config.HouFightWarTime<1)config.HouFightWarTime = 1;
	if(config.HouFightWarTime>24)config.HouFightWarTime = 24;
	return config.HouFightWarTime;
}

int getNowEvent(int Num)
{
	int i = config.NowEvent[Num];
	if (i <= 0)return -1;
	return i;
}

int getEndEvent(int Num)
{
	int i = config.EndEvent[Num];
	if (i <= 0)return -1;
	return i;
}

#ifdef _JZ_CF_DELPETITEM
int getDelPet(int Num)
{
	return config.DelPet[Num];
}

int getDelItem(int Num)
{
	return config.DelItem[Num];
}
#endif

#ifdef _JZ_VIPCODE
int getSaveNumberMultiple(void)
{
	return config.SaveNumberMultiple;
}
#endif

char getping(void)
{
	return config.ping;
}

char getwatcallmap(void)
{
	return config.watcallmap;
}

char getgetpetup(void)
{
	return config.getpetup;
}

int getwatchmap(int num)
{
	return config.watchmap[num];
}

int getunwarpmap(int num)
{
	return config.unwarpmap[num];
}

int getjz_maxlevel(void)
{
	return config.jz_maxlevel;
}

int gettransmaxlevel(void)
{
	return config.transmaxlevel;
}

char *getexpfile(void)
{
	return config.expfile;
}

int getmaxchar(void)
{
	return config.maxchar;
}

#ifdef _JZ_BASE_IF
int getbaseif(void)
{
	return config.baseif;
}
#endif

#ifdef _JZ_CIRC_AN
int getloopantime(void)
{
	return config.loopantime;
}

char *getloopanfile(void)
{
	return config.loopanfile;
}
#endif

#ifdef _JZ_ANGELOPEN
char getAngelOpen(void)
{
	return config.AngelOpen;
}
#endif

int getmaxcharor(void)
{
	return config.maxcharor;
}

int getipconn(void)
{
	return config.ipconn;
}
int getevsls(void)
{
	return config.evsls;
}

int getrecvbuffer(void)
{
	return config.recvbuffer;
}

int getreadbuffer(void)
{
	return config.readbuffer;
}

int getacwritebuf(void)
{
	return config.acwritebuf;
}

int getrunlevel(void)
{
	return config.runlevel;
}

