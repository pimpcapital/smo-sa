#include "version.h"
#include <stdio.h>

#include "common.h"
#include "char_base.h"
#include "char.h"
#include "magic_base.h"
#include "magic.h"
#include "magic_field.h"

/*
 * ¼M¾å¤Ç¤Î  Ë¡¤Î½è  
 */
/*----------------------------------------------------------------------
 * ¼«Ê¬¤Î¤ß²óÉü
 * °ú¿ô
 * charaindex		int		¼«Ê¬¤Îindex
 * magicindex		int		  Ë¡¤Îindex
 *---------------------------------------------------------------------*/
int MAGIC_Recovery_Field( int charaindex, int magicindex)
{
	char *magicarg;
	float power;
	int workhp;
	int	prevhp;
	char	msgbuf[64];

#ifdef _TYPE_TOXICATION
	if( CHAR_CanCureFlg( charaindex, "HP") == FALSE ) return TRUE;
#endif
	magicarg = MAGIC_getChar( magicindex, MAGIC_OPTION );
	power =	atoi( magicarg );
	power = RAND( (power*0.9), (power*1.1) );
#ifndef _MAGIC_REHPAI
	power *= GetRecoveryRate( charaindex );
#endif
	prevhp = CHAR_getInt( charaindex, CHAR_HP );
	workhp = prevhp + (int)power;
	CHAR_setInt( charaindex, CHAR_HP,
		min( workhp, CHAR_getWorkInt( charaindex, CHAR_WORKMAXHP ) ) );
	CHAR_send_P_StatusString( charaindex , CHAR_P_STRING_HP|CHAR_P_STRING_MP);
	if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE ) {
		int		mypartyarray = -1;
		int		oyaindex = CHAR_getWorkInt( charaindex, CHAR_WORKPARTYINDEX1);
		if( CHAR_CHECKINDEX( oyaindex )) {
			int		i,workindex;
			for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
				workindex = CHAR_getWorkInt( oyaindex, CHAR_WORKPARTYINDEX1 +i);
				if( CHAR_CHECKINDEX( workindex) ) {
					if( workindex == charaindex ) {
						mypartyarray = i;
						break;
					}
				}
			}
			if( mypartyarray != -1 ) {
				int index;
				for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
					index = CHAR_getPartyIndex( charaindex, i);
					if( CHAR_CHECKINDEX( index) ) {
						if( index != charaindex) {
							CHAR_send_N_StatusString( index, mypartyarray,
									CHAR_N_STRING_HP|CHAR_N_STRING_MP);
						}
					}
				}
			}
		}
	}
	snprintf( msgbuf, sizeof( msgbuf), "­@¤[¤O¦^´_%d",
			min( workhp, CHAR_getWorkInt( charaindex, CHAR_WORKMAXHP)) - prevhp);
	CHAR_talkToCli( charaindex, -1, msgbuf, CHAR_COLORWHITE);
	return TRUE;

}
/*----------------------------------------------------------------------
 * Â¾¿Í¤ò²óÉü
 * °ú¿ô
 * charaindex		int		¼«Ê¬¤Îindex
 * toindex			int		²óÉü¤¹¤ë¿Í¤Îindex
 * magicindex		int		  Ë¡¤Îindex
 *---------------------------------------------------------------------*/
int MAGIC_OtherRecovery_Field( int charaindex, int toindex, int magicindex)
{
	char *magicarg;
	float power;
	int workhp;
	int	prevhp;
	char	msgbuf[64];

#ifdef _TYPE_TOXICATION
	if( CHAR_CanCureFlg( charaindex, "HP") == FALSE )return FALSE;
#endif
	magicarg = MAGIC_getChar( magicindex, MAGIC_OPTION );
	power =	atoi( magicarg );
	power = RAND( (power*0.9), (power*1.1) );
#ifndef _MAGIC_REHPAI
	power *= GetRecoveryRate( toindex );
#endif
	prevhp = CHAR_getInt( toindex, CHAR_HP );
	workhp = prevhp + (int)power;
	CHAR_setInt( toindex, CHAR_HP,
		min( workhp, CHAR_getWorkInt( toindex, CHAR_WORKMAXHP ) ) );
	if( CHAR_getInt( toindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER) {
		if( charaindex != toindex ) {
			CHAR_send_P_StatusString( toindex, CHAR_P_STRING_HP);
		}
	}
	CHAR_send_P_StatusString( charaindex, CHAR_P_STRING_HP|CHAR_P_STRING_MP);
	if( charaindex != toindex) {
		CHAR_PartyUpdate( charaindex, CHAR_N_STRING_MP);
		CHAR_PartyUpdate( toindex, CHAR_N_STRING_HP);
	}
	else {
		CHAR_PartyUpdate( charaindex, CHAR_N_STRING_MP|CHAR_N_STRING_HP);
	}
	if( CHAR_getInt( toindex, CHAR_WHICHTYPE) == CHAR_TYPEPET ){
		int		i,workindex;
		for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
			workindex = CHAR_getCharPet( charaindex, i );
			if( workindex == toindex ){
				CHAR_send_K_StatusString( charaindex, i, CHAR_K_STRING_HP);
			}
		}
	}

	if( charaindex != toindex) {
		snprintf( msgbuf, sizeof( msgbuf), "%sªº­@¤[¤O¦^´_%d",
				CHAR_getUseName( toindex ),
				min( workhp, CHAR_getWorkInt( toindex, CHAR_WORKMAXHP)) - prevhp);
		CHAR_talkToCli( charaindex, -1, msgbuf, CHAR_COLORWHITE);

		if( CHAR_getInt( toindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER) {
			snprintf( msgbuf, sizeof( msgbuf), "ÂÇ¥Ñ%s­@¤[¤O¦^´_%d",
					CHAR_getUseName( charaindex ),
					min( workhp,CHAR_getWorkInt( toindex, CHAR_WORKMAXHP)) - prevhp);
			CHAR_talkToCli( toindex, -1, msgbuf, CHAR_COLORWHITE);
		}
	}
	else {
		snprintf( msgbuf, sizeof( msgbuf), "­@¤[¤O¦^´_%d",
				min( workhp,CHAR_getWorkInt( charaindex, CHAR_WORKMAXHP)) - prevhp);
		CHAR_talkToCli( charaindex, -1, msgbuf, CHAR_COLORWHITE);

	}

	return TRUE;

}

