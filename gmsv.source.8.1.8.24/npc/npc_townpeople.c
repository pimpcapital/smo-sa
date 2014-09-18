#include "version.h"
#include "object.h"
#include "char_base.h"
#include "char.h"
#include "util.h"
#include "handletime.h"
#include "anim_tbl.h"
#include "npc_door.h"
#include "lssproto_serv.h"
#include "npcutil.h"


/*
 *  ¤Þ¤Á¤Î¤Ò¤È¤Ó¤È  by nakamura
 *  ¥¿¥¤¥×  ¡uTownPeople
 *    ¤·¤«¤±¤é¤ì¤¿¤È¤­¤ËNPCARGUMENT¤Î  »ú  ¤ò  ¤·¤«¤±¤Æ¤­¤¿¿Í¤Ë  ¤¹¡e
 *
 *  ¤Þ¤¿¡bnpcarg¤Ë ¤³¤ó¤Ë¤Á¤Ï,º£  ¤Ï¤Æ¤ó¤­¤¬¤¤¤¤¤Ç¤¹¤Í
 *  ¤Î¤è¤¦¤Ë  ³Ñ¥³¥ó¥Þ¤ò¥Ç¥ê¥ß¥¿¤È¤·¤Æ  ¿ô¤Î¥á¡¼¥»¡¼¥¸¤ò
 *  ½ñ¤¯¤³¤È¤¬¤Ç¤­¡b¤½¤Î¾ì¹ç¥é¥ó¥À¥à¤Ç¤½¤Î  ¤«¤é¤·¤ã¤Ù¤ë¡e
 *  npcgen.perl ¤Ç¤Ï¡b MAN¤Ç¤¢¤ë¡e MSG¤È¤Û¤È¤ó¤É¶³¤Ê¤¸¤À¤Ê¤¢
 *
 */

/*
 *   ¤·¤«¤±¤é¤ì¤¿¤È¤­¤ÏNPCARGUMENT¤ò¤½¤Î¤Þ¤Þ  ¤¹¡e
 */
void NPC_TownPeopleTalked( int index, int talker, char *msg, int color )
{
	char arg1[NPC_UTIL_GETARGSTR_BUFSIZE], token[NPC_UTIL_GETARGSTR_LINEMAX];
	char *arg;
    int i, tokennum;

    /* 3¥°¥ê¥Ã¥É°Ê  ¤Î¾ì¹ç¤À¤±ÊÖÅú¤¹¤ë */

	if( CHAR_getInt(talker,CHAR_WHICHTYPE) == CHAR_TYPEPLAYER 
        && NPC_Util_charIsInFrontOfChar( talker, index, 3 ) ){

        arg = NPC_Util_GetArgStr( index, arg1, sizeof( arg1));

        tokennum = 1;
        /* ¥³¥ó¥Þ¤Ç¶èÀÚ¤é¤ì¤¿¥È¡¼¥¯¥ó¤¬²¿¤³¤¢¤ë¤«¿ô¤¨¤ë */
        for( i=0;arg[i]!='\0';i++ ){
            if( arg[i] == ',' ) tokennum++;
        }

        /* ¥é¥ó¥À¥à¤Ç¤É¤ì¤òÃý¤ë¤«·è¤á¤Æ¡b¤½¤Î¥È¡¼¥¯¥ó¤ò¼è¤ê¤À¤¹ */
        getStringFromIndexWithDelim( arg,",",
                                     rand()%tokennum+1,token, sizeof(token));

        CHAR_talkToCli( talker, index, token, CHAR_COLORWHITE );
    }
}

/*
 * ½é´ü²½¤¹¤ë¡e
 */
BOOL NPC_TownPeopleInit( int meindex )
{

    //CHAR_setInt( meindex , CHAR_HP , 0 );
    //CHAR_setInt( meindex , CHAR_MP , 0 );
    //CHAR_setInt( meindex , CHAR_MAXMP , 0 );
    //CHAR_setInt( meindex , CHAR_STR , 0 );
    //CHAR_setInt( meindex , CHAR_TOUGH, 0 );
    //CHAR_setInt( meindex , CHAR_LV , 0 );

    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPETOWNPEOPLE );
    //CHAR_setFlg( meindex , CHAR_ISOVERED , 1 );
    //CHAR_setFlg( meindex , CHAR_ISATTACKED , 0 );  /*   ·â¤µ¤ì¤Ê¤¤¤è¤ó */
    
    return TRUE;
}
