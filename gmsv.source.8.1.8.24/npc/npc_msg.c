#include "version.h"
#define _NPC_MSG_C_

#include "char.h"
#include "char_base.h"
#include "npcutil.h"


/*
  Åµ·¿Åª¤Ê´Ç  ¡e  Áü¤Ï²¿¤Ç¤â¤è¤¯¡e
  ¤È¤ê¤¢¤¨¤º¤É¤Î  ¸þ¤«¤é¤Ç¤â¸«¤¨¤ë¤è¤¦¤Ë¤¹¤ë¡e

  init , looked ¤Î¤ß¤Ë  ±þ

  */  
BOOL NPC_MsgInit( int meindex )
{
    //CHAR_setInt( meindex , CHAR_HP , 0 );

    //CHAR_setInt( meindex , CHAR_MP , 0 );
    //CHAR_setInt( meindex , CHAR_MAXMP , 0 );

    //CHAR_setInt( meindex , CHAR_STR , 0 );
    //CHAR_setInt( meindex , CHAR_TOUGH, 0 );

    //CHAR_setInt( meindex , CHAR_LV , 0 );

    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPETOWNPEOPLE );
    //CHAR_setFlg( meindex , CHAR_ISOVERED , 0 );
    //CHAR_setFlg( meindex , CHAR_ISATTACKED , 0 );  /*   ·â¤µ¤ì¤Ê¤¤¤è¤ó */

    return TRUE;
}


/*
 * ¸«¤é¤ì¤¿¡e¤Ï¤Ê¤¹¤À¤±¡e
 */
void NPC_MsgLooked( int meindex , int lookedindex )
{
	char	arg1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *arg;
    
	arg = NPC_Util_GetArgStr( meindex, arg1, sizeof( arg1));
    CHAR_talkToCli( lookedindex , -1 , arg , CHAR_COLORWHITE );

}
