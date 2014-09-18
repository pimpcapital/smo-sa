#include "version.h"
#include <string.h>
#include "object.h"
#include "char_base.h"
#include "char.h"
#include "item.h"
#include "util.h"
#include "handletime.h"
#include "npc_doorman.h"
#include "npc_door.h"
#include "npcutil.h"
#include "configfile.h"
/*
 *
 *    曰卞玉失互丐月午五卞公及玉失毛窒日井及橢瘀卞方勻化釩仁NPC.
 *  玉失及午卅曰卞create允月分仃匹｝公及玉失毛丑綜今六月仇午互匹五月〔
 *  反卅仄井仃日木凶午五卞蟈平乓仿毛腹綢仄化｝引歹曰8穴旦卞玉失互中月
 *  樺寧反公及允屯化卞覆仄化排黍允月〔孔凶勾及玉失互  凜卞釩仁仇午卞卅月〔
 *
 *  奶件正□白尼奶旦反Talk匹
 *
 * 玉失毛釩仁凶戶卞
 *
 * 1 雲嗯毛藹璋允月〔藹璋匹五凶日夫日仁     gold|100
 * 2 失奶  丞毛1蜊藹璋允月 〔藹璋匹五凶日夫日仁  item|45
 * 3 失奶  丞毛  勻化中月井升丹井譬屯月〔   勻化中凶日釩仁〔itemhave|44
 * 4 失奶  丞毛  勻化中卅中井升丹井譬屯月〔  勻化中卅井勻凶日釩仁〔
 *          itemnothave|333
 * 5 憊寞毛手勻化中月井升丹井譬屯月〔  勻化中凶日釩仁〔 titlehave|string
 * 6 憊寞毛手勻化中卅中井升丹井譬屯月〔  勻化中卅井勻凶日釩仁〔
 *      titlenothave|string
 *
 * 井卅日內撙  卞蠶尹月午釩仁〔嗯及樺寧反｝
 *＞100打□伙玉中凶分五引允互中中匹允井〞＝匹＞反中＝午中丹午100打□伙玉
 * 午日木月〔中五卅曰＞反中＝分仃蛻勻化手午日木月〔匹｝＞100打□伙玉
 * 中凶分五引仄凶〔＝午蛻歹木月〔
 *
 * 失奶  丞藹璋及樺寧反｝＞窒★毛域蜊中凶分五引允互中中匹允井〞＝午五仁〔
 *  3井日6及樺寧反｝窒井反卅仄井仃化橢瘀互公欠勻化凶日釩仁〔
 *
 *
 *
 *    旦玄及  芊
 *
 *1  玉失毛化五午丹卞  仁
 *2  仇及NPC毛贗癲卞玉失及午卅曰卞  仁〔婁醒毛 gold|100 卞允月
 *3  仇及NPC卞覆仄化｝100打□伙玉動曉手勻化中月橇謫匹＞反中＝午蛻丹
 *4  玉失互夫日中化嗯互蛹勻凶日岳  〔
 *
 */

static void NPC_DoormanOpenDoor( char *nm  );

BOOL NPC_DoormanInit( int meindex )
{
	char	arg1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *arg;
    char dname[1024];

	/* 奶矛件玄及正奶皿澀爛 */
	CHAR_setWorkInt( meindex, CHAR_WORKEVENTTYPE,CHAR_EVENT_NPC);

    CHAR_setInt( meindex , CHAR_HP , 0 );
    CHAR_setInt( meindex , CHAR_MP , 0 );
    CHAR_setInt( meindex , CHAR_MAXMP , 0 );
    CHAR_setInt( meindex , CHAR_STR , 0 );
    CHAR_setInt( meindex , CHAR_TOUGH, 0 );
    CHAR_setInt( meindex , CHAR_LV , 0 );

    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPETOWNPEOPLE );
    CHAR_setFlg( meindex , CHAR_ISOVERED , 0 );
    CHAR_setFlg( meindex , CHAR_ISATTACKED , 0 );  /*   猾今木卅中方氏 */

	arg = NPC_Util_GetArgStr( meindex, arg1, sizeof( arg1));

    if(!getStringFromIndexWithDelim( arg, "|", 3, dname, sizeof(dname ))){
        print("RINGO: 設定看門者時需要門的名字唷！:%s:\n",
              arg );
        return FALSE;
    }
    print( "RINGO: Doorman create: arg: %s dname: %s\n",arg,dname);
    CHAR_setWorkChar( meindex , CHAR_WORKDOORMANDOORNAME , dname );

    return TRUE;
}

void NPC_DoormanTalked( int meindex , int talkerindex , char *msg ,
                     int color )
{
    char mode[128];
    char opt[256];
    char	arg1[NPC_UTIL_GETARGSTR_BUFSIZE];
    char *arg;

    /* 皿伊奶乩□互玉失穴件及1弘伉永玉動  卅日反氏及丹 */
    if(NPC_Util_CharDistance( talkerindex, meindex ) > 1)return;

	arg = NPC_Util_GetArgStr( meindex, arg1, sizeof( arg1));

    if( !getStringFromIndexWithDelim( arg, "|", 1, mode, sizeof( mode )))
        return;

    if( !getStringFromIndexWithDelim( arg, "|", 2, opt, sizeof( opt ) ))
        return;

    if( strcmp( mode , "gold" ) == 0 ){
        int g = atoi( opt );
        int yn = NPC_Util_YN( msg );
        /*char *nm = CHAR_getChar( meindex , CHAR_NAME );*/
        char msg[256];

        if( g > 0 && yn < 0 ){
            snprintf( msg ,sizeof( msg ) ,
                      "打開門需要給我%d的金子這樣可以嗎？", g );
            CHAR_talkToCli( talkerindex, meindex , msg, CHAR_COLORWHITE );
        } else if( g > 0 && yn == 0 ){
            snprintf( msg , sizeof( msg ),
                      "打開門 %d的金子是必要的。", g );
        } else if( g > 0 && yn == 1 ){
            int now_g = CHAR_getInt( talkerindex, CHAR_GOLD );
            if( now_g < g ){
                snprintf( msg , sizeof( msg ) ,
                          "打開門 %d的金子是必要的。", g );
            	CHAR_talkToCli( talkerindex, meindex , msg, CHAR_COLORWHITE );
            } else {
                snprintf( msg , sizeof( msg ),
                          "%d 收到金子了。現在就來開門。", g );
            	CHAR_talkToCli( talkerindex, meindex , msg, CHAR_COLORWHITE );

                /* 雲嗯毛必永玄 */
                now_g -= g;
                CHAR_setInt( talkerindex , CHAR_GOLD , now_g );
                /* 丐凶日仄中旦  □正旦毛霜耨 */
                CHAR_send_P_StatusString(talkerindex, CHAR_P_STRING_GOLD);

                /* 玉失夫日仁 */
                NPC_DoormanOpenDoor(
                    CHAR_getWorkChar( meindex, CHAR_WORKDOORMANDOORNAME));
            }
        }
    } else if( strcmp( mode , "item" ) == 0 ){
        CHAR_talkToCli( talkerindex, meindex ,
                        "尚在未支援模式。",
                        CHAR_COLORWHITE);
    } else if( strcmp( mode , "itemhave" ) == 0 ){
        CHAR_talkToCli( talkerindex, meindex ,
                        "尚在未支援模式。",
                        CHAR_COLORWHITE);
    } else if( strcmp( mode , "itemnothave" ) == 0 ){
        CHAR_talkToCli( talkerindex, meindex ,
                        "尚在未支援模式。",
                        CHAR_COLORWHITE);
    } else if( strcmp( mode , "titlehave" ) == 0 ){
        CHAR_talkToCli( talkerindex, meindex ,
                        "尚在未支援模式。",
                        CHAR_COLORWHITE);

    } else if( strcmp( mode , "roomlimit" ) == 0 ){

		/*   盒及諦醒孺蜃互丐月樺寧 */
		char szOk[256], szNg[256], szBuf[32];
		int checkfloor;
		int maxnum, i, iNum;

	    if( !getStringFromIndexWithDelim( arg, "|", 2, szBuf, sizeof( szBuf ) ))
    	    return;

		/* 譬屯月白夫失午    諦醒 */
		if( sscanf( szBuf, "%d:%d", &checkfloor, &maxnum ) != 2 ){
			return;
		}

		for( iNum = 0,i = 0; i < getFdnum(); i ++ ){
			/* 皿伊奶乩□動陸卞反飭  互  中 */
			if( CHAR_getCharUse( i ) == FALSE )continue;
			if( CHAR_getInt( i, CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER )continue;
			/* 隙爛及白夫失動陸卞飭  互  中 */
			if( CHAR_getInt( i, CHAR_FLOOR ) != checkfloor )continue;
			iNum++;
		}
	    if( !getStringFromIndexWithDelim( arg, "|", 5, szNg, sizeof( szNg ))){
   			strcpy( szNg, "。。。。" );	/* 鳳傘卅仄及本伉白 */
		}
    	if( !getStringFromIndexWithDelim( arg, "|", 4, szOk, sizeof( szOk ))){
   			strcpy( szOk, "開門吧。。。" );	/* 鳳傘丐曰及本伉白 */
   		}

		if( iNum >= maxnum ){
			/*     毛譯尹化中月樺寧 */
	        CHAR_talkToCli( talkerindex, meindex ,szNg, CHAR_COLORWHITE);
		}else{
			/*     卞  凶卅中樺寧 */
	        CHAR_talkToCli( talkerindex, meindex ,szOk, CHAR_COLORWHITE);
            NPC_DoormanOpenDoor(
                    CHAR_getWorkChar( meindex, CHAR_WORKDOORMANDOORNAME));
		}

    } else if( strcmp( mode , "titlenothave" ) == 0 ){
        CHAR_talkToCli( talkerindex, meindex ,
                        "尚在未支援模式。",
                        CHAR_COLORWHITE);
    }
}

/*
 *    蟆匹腹綢仄化甲永玄仄凶及毛允屯化釩仁〔
 *
 */
static void NPC_DoormanOpenDoor( char *nm)
{
    int doori = NPC_DoorSearchByName( nm );
    print( "RINGO: Doorman's Door: index: %d\n", doori );

    NPC_DoorOpen( doori , -1 );

}

