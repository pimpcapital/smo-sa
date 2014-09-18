#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "readmap.h"
// Arminius 8.4 manor
#include "family.h"

static void NPC_SignBoard_Window( int meindex, int toindex, int num,int select);


/*********************************
* 賡渝質  
*********************************/
BOOL NPC_SignBoardInit( int meindex )
{

	char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *npcarg;
	/*--正奶皿澀爛--*/
   	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEMSG );

	/*--白央奶伙互卅仃木壬准憎  毛澀  今六卅中--*/
	if((npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof(npcarg1)))==NULL){
		print("GetArgStrErr");
		return FALSE;
	}

	return TRUE;

}




/*********************************
* 葦日木凶凜及質  
*********************************/
void NPC_SignBoardLooked( int meindex , int lookedindex)
{

    /* 皿伊奶乩□卞覆仄化分仃  殺允月 */
    if( CHAR_getInt( lookedindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	/* ㄠ弘伉永玉動  及心 */
	if( NPC_Util_CharDistance( lookedindex, meindex ) > 1) return;


	NPC_SignBoard_Window( meindex, lookedindex,0,-1);

}


static void NPC_SignBoard_Window( int meindex, int toindex, int num,int select)
{

	char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *npcarg;
	char token[1024];
	int fd = getfdFromCharaIndex( toindex);


	if((npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof(npcarg1)))==NULL){
		print("GetArgStrErr");
		return;
	}
	
	// Arminius: manor
	{
	  char *p,*q;
	  extern struct FM_POINTLIST  fmpointlist;
	  p=strstr(npcarg, "%manorid:");
	  if (p!=NULL) {
	    char tmp[1024];
	    int a;
	    
	    q=strstr(p+1,"%");
	    if (q!=NULL) {
  	      q[0]='\0';
 	      p[0]='\0';
	      a=atoi(p+9);
#ifdef _FAMILY_MANORNUM_CHANGE	// CoolFish 用來修改裝園數量
	      if ((a>=1) && (a<=MANORNUM)) {		  
#else
	      if ((a>=1) && (a<=4)) {
#endif
                getStringFromIndexWithDelim(fmpointlist.pointlistarray[a-1], "|", 5, token, sizeof(token));
                if (atoi(token)>=0) {
                  getStringFromIndexWithDelim(fmpointlist.pointlistarray[a-1], "|", 6, token, sizeof(token));
  	          sprintf(tmp,"%s%s%s",npcarg,token,q+1);
	          //strcpy(npcarg, tmp);
	          sprintf(token, "　　　　　　＜　看板　＞\n%s",tmp);
	        } else {
	          sprintf(tmp,"%s沒有任何家族%s",npcarg,q+1);
	          //strcpy(npcarg, tmp);
	          sprintf(token, "　　　　　　＜　看板　＞\n%s",tmp);
	        }
	      }
	    }
	    
	  }else{
	  	sprintf(token, "　　　　　　＜　看板　＞\n%s",npcarg);
	  }
	}

	
	
	/*--霜耨--*/
	lssproto_WN_send( fd,WINDOW_MESSAGETYPE_MESSAGE,
					WINDOW_BUTTONTYPE_OK,
					CHAR_WINDOWTYPE_WINDOWPETSHOP_START,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);
}



/*-----------------------------------------
 * 弁仿奶失件玄井日忒勻化五凶凜卞裟太請今木月〔
 *
-------------------------------------------*/
void NPC_SignBoardWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 1) {
		return;
	}
}

