#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "npc_action.h"

/* 
 * 皿伊奶乩□及失弁撲亦件卞  殺允月NPC〔
 * 簽卞韻曰忒仄凶曰允月分仃分互〔
 * 洶仁〕引凶反  勻化中月及失弁撲亦件卞反  殺仄卅中〔
 *
 * 婁醒「
 *      msgcol:		丟永本□斥及縉〔犯白巧伙玄反疵縉
 *      normal:		  騷卞韻勻化五凶凜支  躲卅失弁撲亦件卞覆仄化及忒蠶
 *		attack:		  猾失弁撲亦件卞覆允月忒蠶
 *		damage:		母丟□斥毛熬仃凶失弁撲亦件卞覆允月忒蠶
 *		down:		邐木月失弁撲亦件卞覆允月忒蠶
 *		sit:		甄月失弁撲亦件卞覆仄化及忒蠶
 *		hand:		澎毛蕊月失弁撲亦件卞覆仄化及忒蠶
 *		pleasure:	減少失弁撲亦件卞覆仄化及忒蠶
 *		angry:		變月失弁撲亦件卞覆仄化及忒蠶
 *		sad:		  仄戈失弁撲亦件卞覆仄化及忒蠶
 *		guard:		布□玉允月失弁撲亦件卞覆仄化及忒蠶
 */
 

#define		NPC_ACTION_MSGCOLOR_DEFAULT		CHAR_COLORYELLOW

enum {
	CHAR_WORK_MSGCOLOR	= CHAR_NPCWORKINT1,
};

/*********************************
* 賡渝質  
*********************************/
BOOL NPC_ActionInit( int meindex )
{
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	int		tmp;
	
	tmp = NPC_Util_GetNumFromStrWithDelim( argstr, "msgcol");
	if( tmp == -1 ) tmp = NPC_ACTION_MSGCOLOR_DEFAULT;
	CHAR_setWorkInt( meindex, CHAR_WORK_MSGCOLOR, tmp);
    
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEACTION );
	
	
    return TRUE;
}




/*********************************
*   仄井仃日木凶凜及質  
*********************************/
void NPC_ActionTalked( int meindex , int talkerindex , char *szMes ,
                     int color )
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buf[64];

    /* 皿伊奶乩□卞覆仄化分仃  殺允月 */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }
	/* ㄠ弘伉永玉動  及心 */
	if( !NPC_Util_charIsInFrontOfChar( talkerindex, meindex, 1 )) return; 

	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));

	if( NPC_Util_GetStrFromStrWithDelim( argstr, 
										"normal",
										buf, sizeof( buf))
    	!= NULL )
	{
    	CHAR_talkToCli( talkerindex, meindex ,buf , 
    					CHAR_getWorkInt( meindex, CHAR_WORK_MSGCOLOR ));
	}
	
}
/*********************************
* watch質  
*********************************/
void NPC_ActionWatch( int meobjindex, int objindex, CHAR_ACTION act,
                    int x,int y,int dir, int* opt,int optlen )
{
	int		meindex;
	int		index;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buf[64];
	int		i;
    struct  {
    	CHAR_ACTION		act;
    	char			*string;
    }searchtbl[] = {
	    { CHAR_ACTATTACK,	"attack"},
	    { CHAR_ACTDAMAGE,	"damage"},
		{ CHAR_ACTDOWN,		"down"},
		{ CHAR_ACTSIT,		"sit"},
		{ CHAR_ACTHAND,		"hand"},
		{ CHAR_ACTPLEASURE,	"pleasure"},
		{ CHAR_ACTANGRY,	"angry"},
		{ CHAR_ACTSAD,		"sad"},
		{ CHAR_ACTGUARD,	"guard"},
		{ CHAR_ACTNOD,		"nod"},
		{ CHAR_ACTTHROW,	"throw"},
    };
	
	if( OBJECT_getType( objindex) != OBJTYPE_CHARA) return;
	index = OBJECT_getIndex( objindex);
	/* 皿伊奶乩□卞及心  殺允月 */
	if( CHAR_getInt( index, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER) return;
    
    meindex = OBJECT_getIndex( meobjindex);
    
    /* 輊五寧勻化ㄠ弘伉永玉匹卅中午  殺仄卅中 */
    if( NPC_Util_isFaceToFace( meindex, index, 1 ) != TRUE ) return;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	for( i = 0; i < arraysizeof( searchtbl); i ++ ) {
		if( searchtbl[i].act == act) {
			if( NPC_Util_GetStrFromStrWithDelim( argstr, 
												searchtbl[i].string,
												buf, sizeof( buf))
		    	!= NULL )
			{
		    	CHAR_talkToCli( index, meindex ,buf , 
		    					CHAR_getWorkInt( meindex, CHAR_WORK_MSGCOLOR ));
				break;
			}
		}
	}
}
