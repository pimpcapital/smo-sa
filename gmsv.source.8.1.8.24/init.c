#include "version.h"
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/resource.h>

#include "configfile.h"
#include "util.h"
#include "net.h"
#include "msignal.h"
#include "buf.h"
#include "object.h"
#include "char.h"
#include "char_data.h"
#include "item.h"
#include "readmap.h"
#include "function.h"
#include "saacproto_cli.h"
#include "lssproto_serv.h"
#include "readnpc.h"
#include "log.h"
#include "handletime.h"
#include "title.h"
#include "encount.h"
#include "enemy.h"
#include "battle.h"
#include "magic_base.h"
#include "pet_skill.h"
#include "item_gen.h"
#include "petmail.h"
#include "npc_quiz.h"
#ifdef _JZ_RIDE
#include "char_base.h"
#endif
#ifdef _M_SERVER
#include "mclient.h"
#endif
#ifdef _JZ_CAPTURE_FREES
#include "battle_event.h"
#endif
#ifdef _NPCSERVER_NEW
#include "npcserver.h"
#endif

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
#include "profession_skill.h"
#endif

#ifdef _ITEM_QUITPARTY
#include "init.h"
//int itemquitparty_num = 0;
//static DisappearItem *Disappear_Item;
#endif

#define MESSAGEFILE "hoge.txt"

#define OPTIONSTRING "d:f:h"
#define usage() print( "Usage: %s ["OPTIONSTRING"]\n", getProgname() );

void printUsage( void )
{
    usage();
  /*print( "Usage: %s ["OPTIONSTRING"]\n", progname );*/
    print( "          [-d debuglevel]        default value is 0\n" );
    print( "          [-f configfilename]    default value is setup.cf\n"
        );
}

/*
 * 戊穴件玉仿奶件毛質  允月
 *
 * 婁醒
 *      argc      儂  及醒
 *      argv      儂  及    
 * 忒曰襖
 *      TRUE(1)     懇橘卅戊穴件玉仿奶件婁醒分勻凶日
 *      FALSE(0)    唱橘卅戊穴件玉仿奶件婁醒分勻凶日
 */
BOOL parseCommandLine( int argc , char** argv )
{
    int c;                          /* getopt 匹銀丹 */
    extern char* optarg;            /* getopt 匹銀丹 */


    while( ( c = getopt( argc, argv ,OPTIONSTRING )) != -1 ){
        switch( c ){
        case 'd':
        {
            int     debuglevel;
            if( !strtolchecknum( optarg, (int*)&debuglevel, 10,
                                 INT)){
                print( "Specify digit number\n" );
                return FALSE;
            }
            setDebuglevel( debuglevel );
            break;
        }
        case 'f':
            setConfigfilename( optarg );
            break;
        case 'h':
            printUsage();
            return FALSE;
            break;

        default:
            printUsage();
            return FALSE;
            break;

        }
    }
    return TRUE;
}


/*
 * 棕雁  醒毛由□旦允月
 * 婁醒
 *      env     棕雁  醒及    
 *
 * 漆及赭窒手仄卅中
 */
BOOL parseEnvironment( char** env )
{
    if( getDebuglevel() >= 3 ){
        int index=0;
        while( env[index] != NULL )print( "%s " , env[index++] );
        print( "\n" );
    }
    return TRUE;
}




/*lsgen迕及伐□平件弘田永白央及  五今*/
#define LSGENWORKINGBUFFER  65536*4



#define GOTORETURNFALSEIFFALSE(x) if(!(x))goto RETURNFALSE
/*
 * 賡渝祭伙□民件
 * 婁醒
 *      argc    argv及醒
 *      argv    戊穴件玉仿奶件婁醒
 *      env     棕雁  醒
 * 忒曰襖
 *      TRUE(1) 岳  
 *      FALSE(1) 撩  
 */
BOOL init(int argc , char** argv , char** env )
{
#ifdef _ITEM_QUITPARTY
    FILE *f;
	int i;
	char line[256];
#endif
    srand( getpid());
    print( "This Program is compiled at %s %s by gcc %s\n",
           __DATE__ , __TIME__ , __VERSION__ );
    defaultConfig( argv[0] );
    signalset();

    GOTORETURNFALSEIFFALSE(parseCommandLine( argc , argv ));
    GOTORETURNFALSEIFFALSE(parseEnvironment( env ));

    {
        Char    aho;
        debug( sizeof( aho ), d);
        debug( sizeof( aho.data ), d);
        debug( sizeof( aho.string ), d);
        debug( sizeof( aho.flg ),d);
        debug( sizeof( aho.indexOfExistItems ), d);
        debug( sizeof( aho.haveSkill ), d);
        debug( sizeof( aho.indexOfHaveTitle ), d);
        debug( sizeof( aho.addressBook ),d);
        debug( sizeof( aho.workint ),d);
        debug( sizeof( aho.workchar ),d);
    }

    print( "SetupFileName: %s\n" , getConfigfilename() );

    GOTORETURNFALSEIFFALSE(readconfigfile( getConfigfilename() ) );
    //ttom start
    {  int iWork = setEncodeKey();
       if( iWork == 0 ){
       // 巨件戊□玉平□毛澀爛
       printf( "----------------------------------------\n" );
       printf( "-------------[encodekey] is not set in %s\n", getConfigfilename() );
       printf( "----------------------------------------\n" );
       exit( 1 );
       }else{
            // 巨件戊□玉平□毛澀爛
               printf( "encodekey = %d\n", iWork );
       }
    }
    // AcWBuff毛澀爛
    {   int iWork = setAcWBSize();
        if( iWork == 0 ){
           printf( "----------------------------------------\n" );
           printf( "-------------[acwbsize] is not set in %s\n", getConfigfilename() );
           printf( "----------------------------------------\n" );
           exit( 1 );
           }else{
                   printf( "acwbsize = %d\n", iWork );
           }
    }
    //ttom end
    if( getDebuglevel() >= 1 ){
//		print("ServerType: %d\n", getServerType() );
        print("DebugLevel: %d\n" , getDebuglevel() );
        print("MemoryUnit: %d\n"  , getMemoryunit() );
        print("MemoryUnitNum: %d\n" , getMemoryunitnum() );

        print("AccountServerName: %s\n" , getAccountservername() );
        print("AccountServerPort: %d\n" , getAccountserverport() );
        print("AccountServerRealizesGameServeras: %s\n",
              getGameservername());
        print("AccountServerLoginPasswd: %s\n", getAccountserverpasswd());

        print("Wait at Port: %d\n",  getPortnumber() );

        print("servernumber : %d\n",  getServernumber() );

        print("Reuseaddr: %d\n",  getReuseaddr() );


        print("FileDiscriptorNum ≒ about player num: %d\n",
              getFdnum() );
        print("PetCharNum: %d\n" , getPetcharnum() );
        print("OtherCharNum: %d\n"  , getOtherscharnum() );

        print("Object Number: %d\n",  getObjnum() );
        print("Item Number: %d\n",  getItemnum() );
        print("Battle Number: %d\n",  getBattlenum() );

        print("Topdirectory: %s\n"  , getTopdir());

        print("Mapdirectory: %s\n"  , getMapdir());
        print("MapTileFile: %s\n"  , getMaptilefile());
        print("ItemFile: %s\n"  , getItemfile());
        print("InvincibleFile: %s\n"  , getInvfile());
        print("AppearPositionFile: %s\n"  , getAppearfile());
        print("EffectSettingFile: %s\n"  , getEffectfile());
        print("TitleNameFile: %s\n"  , getTitleNamefile());
        print("TitleConfigFile: %s\n"  , getTitleConfigfile());
        print("EncountFile: %s\n"  , getEncountfile());
        print("EnemyBaseFile: %s\n"  , getEnemyBasefile());
        print("EnemyFile: %s\n"  , getEnemyfile());
        print("GroupFile: %s\n"  , getGroupfile());
        print("MagicFile: %s\n"  , getMagicfile());

#ifdef __ATTACK_MAGIC
        print( "AttMagicFile: %s\n" , getAttMagicfileName() );
#endif

        print("PetskillFile: %s\n"  , getPetskillfile());

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
        print("ProfessionFile: %s\n"  , getProfession());
#endif

        print("Itematomfile: %s\n", getItematomfile());
        print("Quizfile: %s\n", getQuizfile());
#ifdef _BLACK_MARKET
		print("BlackMarketFile: %s\n", getBMItemFile());
#endif
#ifdef _GMRELOAD
		print("GMSetFile: %s\n", getGMSetfile());
#endif
        print("LsgenLog: %s\n"  ,  getLsgenlogfilename() );
        print("Storedir: %s\n"  , getStoredir());
        print("Npcdir: %s\n"  , getNpcdir());
        print("Logdir: %s\n",  getLogdir());
        print("LogConfigFilename: %s\n", getLogconffile() );
        print("ChatMagicPasswd: %s\n", getChatMagicPasswd() );
        print("ChatMagicCDKeyCheck: %d\n", getChatMagicCDKeyCheck() );

        print("NPCTemplateNum: %d\n"  , getNpctemplatenum() );
        print("NPCCreateNum: %d\n"  , getNpccreatenum() );

        print("WalkSendInterval: %d\n"  , getWalksendinterval());
        print("CASendInterval: %d\n"  , getCAsendinterval_ms());
        print("CDSendInterval: %d\n"  , getCDsendinterval_ms());
        print("Onelooptime: %d\n"  , getOnelooptime_ms());
        print("Petdeletetime: %d\n"  , getPetdeletetime());
        print("Itemdeletetime: %d\n"  , getItemdeletetime());
#ifdef _DEL_DROP_GOLD
		print("Golddeletetime: %d\n"  , getGolddeletetime());
#endif
        print("CharSaveSendInterval: %d\n"  , getCharSavesendinterval());

        print("AddressBookOffMesgNum: %d\n",getAddressbookoffmsgnum());
        print("ProtocolReadFrequency: %d\n",getProtocolreadfrequency());

        print("AllowErrorNum: %d\n",getAllowerrornum());

#ifdef	_M_SERVER
		print("mServerIP: %s\n",getmservername());
		print("mServerPort: %d\n",getmserverport());
#endif
#ifdef _NPCSERVER_NEW
		print("NpcServerIP: %s\n",getnpcserveraddr());
		print("NpcServerPort: %d\n",getnpcserverport());
#endif
		print("\n");
    }

	{	//andy_add 2003/05/05 check GameServer Name
		char *GameServerName;
		GameServerName = getGameserverID();
		if( GameServerName == NULL || strlen( GameServerName) <= 0 )
			return FALSE;
		print("\n GameserverID: %s\n",  GameServerName );
	}
	
    print("Start Initialize\n" );
    
    setpriority(PRIO_PROCESS, 0, getrunlevel());
    print("Main thread pri=%d nice:%d\n",getpriority(PRIO_PROCESS, 0),getrunlevel());

		if(getping())
		{
			print("ping close\n");
			system("echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all");
		}else
		{
			print("ping open\n");
			system("echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_all");
		}
//#define DEBUG1( arg... ) if( getDebuglevel()>1 ){##arg}
    print( "Configuring Memory..." );
    GOTORETURNFALSEIFFALSE(configmem( getMemoryunit(),
                                      getMemoryunitnum() ) );
    GOTORETURNFALSEIFFALSE(memInit());
	print( "done\n" );
    if( !initConnect(getFdnum()) )
        goto MEMEND;
    while( 1 ){
        print( "Attempt to bind local address port %d... " , getPortnumber());
        bindedfd = bindlocalhost( getPortnumber() );
        if( bindedfd == -1 )
            sleep( 10 );
        else
            break;

    }
	print( "done\n" );
	print( "Configuring Object..." );
    if( !initObjectArray( getObjnum()) )
        goto CLOSEBIND;
	print( "done\n" );
	print( "Configuring character..." );
    if(!CHAR_initCharArray( getFdnum(), getPetcharnum(),getOtherscharnum()) )
        goto CLOSEBIND;
	print( "done\n" );
	print( "Configuring items:%s...", getItemfile());
    if(!ITEM_readItemConfFile( getItemfile()) )
        goto CLOSEBIND;
    if(!ITEM_initExistItemsArray( getItemnum() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Configuring battle..." );
    if(!BATTLE_initBattleArray( getBattlenum() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Configuring Function Table..." );
    if( !initFunctionTable() )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Initializing addressbook..." );
    if( !PETMAIL_initOffmsgBuffer( getAddressbookoffmsgnum() ))
        goto CLOSEBIND;
	print( "done\n" );

	print( "Reading Invincible File..." );
    if( !CHAR_initInvinciblePlace( getInvfile() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Reading AppearPosition File..." );
    if( !CHAR_initAppearPosition( getAppearfile() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Reading TitleName File..." );
    if( !TITLE_initTitleName( getTitleNamefile() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Reading TitleConfig File..." );
    if( !TITLE_initTitleConfig( getTitleConfigfile() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Reading Encount File..." );
    if( !ENCOUNT_initEncount( getEncountfile() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Reading EnemyBase File..." );
    if( !ENEMYTEMP_initEnemy( getEnemyBasefile() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Reading Enemy File..." );
    if( !ENEMY_initEnemy( getEnemyfile() ) )
        goto CLOSEBIND;
	print( "done\n" );

	print( "Reading Group File..." );
    if( !GROUP_initGroup( getGroupfile() ) )
        goto CLOSEBIND;
	print( "done\n" );
	print( "Reading Magic File..." );
    if( !MAGIC_initMagic( getMagicfile() ) )
        goto CLOSEBIND;
	print( "done\n" );

		#ifdef __ATTACK_MAGIC

	print( "Reading Attack Magic File..." );

    if( !ATTMAGIC_initMagic( getAttMagicfileName() ) )
//		if( !ATTMAGIC_initMagic( getMagicfile() ) )
        goto CLOSEBIND;

	print( "Attack magic file -->%s..." , getAttMagicfileName());
	print( "done\n" );

    #endif
 
	print( "Reading Petskill File..." );
    if( !PETSKILL_initPetskill( getPetskillfile() ) )
        goto CLOSEBIND;
	print( "done\n" );

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
	print( "Reading profession File..." );
	if( !PROFESSION_initSkill( getProfession() ) ){
		goto CLOSEBIND;
	}
	print( "done\n" );
#endif

    /* 失奶  丞及箋    毛  戈 */
	print( "Reading Itematomfile..." );
    if( !ITEM_initItemAtom( getItematomfile()) )
        goto CLOSEBIND;
	print("done\n" );

	print( "Initializing ItemIngCache..." );
    if( !ITEM_initItemIngCache() )
        goto CLOSEBIND;
	print("done\n" );
    
	print( "Initializing Itemrandtable..." );
    if( !ITEM_initRandTable() )
        goto CLOSEBIND;
	print("done\n" );
    
	print( "Reading EffectSetting File..." );
    if( !CHAR_initEffectSetting( getEffectfile() ) )
        goto CLOSEBIND;
	print( "done\n" );
	print( "Reading Quiz File..." );
    if( !QUIZ_initQuiz( getQuizfile() ) )
        goto CLOSEBIND;
	print( "done\n" );

#ifdef _BLACK_MARKET
	print( "Reading BlackMarket File..." );
	if ( !LoadBMItem(getBMItemFile()) )
		goto CLOSEBIND;
	print( "done\n" );
#endif

#ifdef _GMRELOAD
	print( "Reading GMSet File..." );
	if ( !LoadGMSet( getGMSetfile() ) )
		goto CLOSEBIND;
	print( "done\n" );
#endif

#ifdef _ANGEL_SUMMON
	print("Reading MissionList File...");
	if( !LoadMissionList( ) )
		goto CLOSEBIND;
	print("done\n");
#endif

#ifdef _CONTRACT
	print("Reading Contract File...");
	//if( !LoadMissionList( ) )
	//	goto CLOSEBIND;
	if( !ITEM_initContractTable() )
		goto CLOSEBIND;
	print("done\n");
#endif
#ifdef _JZ_RIDE
	print("Reading Ride File...");
	if(!CHAR_InitRide(getRidefile()))
		goto CLOSEBIND;
	print("done\n");
#endif
#ifdef _JZ_CAPTURE_FREES
	print("Reading CapFree File...");
	if(!BATTLE_InitCapFree(getCapFreeFile()))
		goto CLOSEBIND;
	print("done\n");
#endif
#ifdef _JOBDAILY
	print("Reading Jobdaily File...");
	if(!LoadJobdailyfile())
		goto CLOSEBIND;
	print("done\n");
#endif
#ifdef _JZ_CIRC_AN
	print("Reading loop Announce File...");
	if(!net_loadloopan(getloopanfile()))
		goto CLOSEBIND;
	print("done\n");
#endif

#ifdef _RACEMAN
	print("Reading Racepet File...");
	if(!LoadRacepetfile())
		goto CLOSEBIND;
	print("done\n");
#endif

	print( "Configuring map..." );
    if( !MAP_initReadMap( getMaptilefile() , getMapdir() ))
        goto CLOSEBIND;
	print( "done\n" );
	print( "Reading NPC Files..." );
    if( !NPC_readNPCSettingFiles( getNpcdir(), getNpctemplatenum(),
                                  getNpccreatenum() ) )
        goto CLOSEBIND;
	print( "done\n" );
	print( "Initialize rpc server ... " );
    if( lssproto_InitServer( lsrpcClientWriteFunc, LSGENWORKINGBUFFER ) < 0 )
        goto CLOSEBIND;
	print( "done\n" );
	print( "Attempt to connect to account server... " );
    acfd = connectHost( getAccountservername(), getAccountserverport());
    if(acfd == -1)
        goto CLOSEBIND;


/*
	{
		int errorcode;
		int errorcodelen;
		int qs;

		errorcodelen = sizeof(errorcode);
		qs = getsockopt( acfd, SOL_SOCKET, SO_RCVBUF , &errorcode, &errorcodelen);
		//andy_log
		print("\n\n GETSOCKOPT SO_RCVBUF: [ %d, %d, %d] \n", qs, errorcode, errorcodelen);
	}
*/

	print( "done\n" );
    initConnectOne( acfd, NULL , 0 );
    if( !CONNECT_acfdInitRB( acfd)) goto CLOSEAC;
    if( !CONNECT_acfdInitWB( acfd)) goto CLOSEAC;
    CONNECT_setCtype( acfd, AC );
		//初試化EPOLL
#ifdef _JZ_EPOLL
	print( "Initialize epoll ... " );
    if(!init_epoll(getevsls() , getFdnum() ))
        goto CLOSEAC;
	print( "done\n" );
#else
	print( "Initialize acip to long ... " );
    if(!CharIpToLongIp(getAccountservername()))
        goto CLOSEAC;
	print( "done\n" );
#endif
	print( "Initialize rpc client ... " );
    /*  rpc(client)及賡渝祭 */
    if( saacproto_InitClient( lsrpcClientWriteFunc,LSGENWORKINGBUFFER, acfd) < 0 )
        goto CLOSEAC;
	print( "done\n" );

	print( "Sending login request to account server... " );
    /*  夫弘奶件邰菲毛請允  */
    saacproto_ACServerLogin_send(acfd, getGameservername(),
                                 getAccountserverpasswd());
	print( "done\n" );

    if( isExistFile( getLsgenlogfilename() ) ){
        lssproto_SetServerLogFiles( getLsgenlogfilename(),
                                    getLsgenlogfilename() );
        saacproto_SetClientLogFiles( getLsgenlogfilename(),
                                     getLsgenlogfilename() );
    }


	print( "Initialize finished\n" );

#ifdef _MUSEUM
	if( getMuseum() )
		print("\nThis is 石器博物館!!\n");
	else
		print("\nThis is 普通星球!!\n");
#endif

	print( "Start Log\n" );
    {
        char    logconffile[512];
        snprintf( logconffile, sizeof( logconffile), "%s/%s" ,
                  getLogdir(), getLogconffile() );
        if( !initLog( logconffile ) )
            goto CLOSEAC;
    }

#ifdef _M_SERVER
	print( "Attempt to connect to m server... " );
    mfd = connectmServer( getmservername(), getmserverport());
	if (mfd ==-1 ){
		print( "Failed to connect to m server... " );
	}else{
		initConnectOne( mfd, NULL , 0 );
		print( "Successed to connect to m server... " );
	}
#endif

#ifdef _NPCSERVER_NEW
	npcfd = connectNpcServer( getnpcserveraddr(), getnpcserverport());
	if( npcfd == -1 ){
		print( "Failed to connect to NpcServer... " );
	}else{
		initConnectOne( npcfd, NULL , 0 );
		print( "Successed to connect to NpcServer... " );
		NPCS_NpcSLogin_send( npcfd);
	}
#endif
	
#ifdef _ITEM_QUITPARTY
	print( "Reading itemquitparty File..." );
    
	//讀取檔案
    f = fopen( getitemquitparty(), "r" );
	if( f != NULL ){
		while( fgets( line, sizeof( line ), f ) ){
			if( line[0] == '#' )continue;
            if( line[0] == '\n' )continue;
		    chomp( line );
			itemquitparty_num++;
		}
		if( fseek( f, 0, SEEK_SET ) == -1 ){
			print( "itemquitparty Seek Error\n" );
			fclose(f);
			goto CLOSEAC;
		}
		//配記憶體
		Disappear_Item = allocateMemory( sizeof(struct tagDisappearItem) * itemquitparty_num );
		if( Disappear_Item == NULL ){
			print( "Can't allocate Memory %d\n", sizeof(struct tagDisappearItem) * itemquitparty_num );
			fclose( f );
			goto CLOSEAC;
		}

		i = 0;
		//將道具編號存入 Disappear_Item.string
		while( fgets( line, sizeof( line ), f ) ){
			if( line[0] == '#' )continue;
			if( line[0] == '\n' )continue; 
			chomp( line );
			sprintf( Disappear_Item[i].string,"%s",line );
			print("\n道具編號:%s", Disappear_Item[i].string );
			i++;
		}
		fclose(f);
	}
#endif

    DEBUG_ADJUSTTIME = 0;
    print( "\n" );
    return TRUE;

CLOSEAC:
    close( acfd );
CLOSEBIND:
    close( bindedfd );
    endConnect();
MEMEND:
    memEnd();
RETURNFALSE:
    return FALSE;
}
