#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"


//¸½ºß¤Î»ÈÍÑ¤Ç¤­¤ë¥Õ¥é¥°¤Î¿ô
#define MAXEVENTFLG 96

/*
 * ¥¤¥Ù¥ó¥È¤Î¥Õ¥é¥°¤ò¥Á¥§¥Ã¥¯¤¹¤ë      
 *
 */
 
static void NPC_CheckMan_selectWindow( int meindex, int toindex, int num);
int NPC_NowFlgCheck(int meindex,int talker,int now[MAXEVENTFLG]);
int NPC_EndFlgCheck(int meindex,int talker ,int nowflg[MAXEVENTFLG]);
BOOL NPC_FlgCheckMain( int meindex,int talker,int nowindex,int now[MAXEVENTFLG],char *work2);

/*********************************
* ½é´ü½è  
*********************************/
BOOL NPC_CheckManInit( int meindex )
{
	/*--¥­¥ã¥é¤Î¥¿¥¤¥×¤òÀßÄê--*/
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPECHECKMAN);

	return TRUE;
}


/*********************************
*   ¤·¤«¤±¤é¤ì¤¿»þ¤Î½è  
*********************************/
void NPC_CheckManTalked( int meindex , int talkerindex , char *szMes ,int color )
{

    /* ¥×¥ì¥¤¥ä¡¼¤ËÂÐ¤·¤Æ¤À¤±  ±þ¤¹¤ë */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER )
    {
    	return;
    }

	/*--  ¤ÎÁ°¤Ë¤¤¤ë¤«¤É¤¦¤«¡ª--*/
	if(NPC_Util_isFaceToFace( meindex ,talkerindex , 2) == FALSE) {
		/* £±¥°¥ê¥Ã¥É°Ê  ¤Î¤ß */
		if(NPC_Util_isFaceToChara( talkerindex, meindex, 1) == FALSE) return;
	}

	//  ½é¤Î¥¦¥¤¥ó¥É¥¦¤Ë
	NPC_CheckMan_selectWindow( meindex, talkerindex, 0);

}


/*
 * ³Æ½è  ¤ËÊ¬¤±¤ë
 */
static void NPC_CheckMan_selectWindow( int meindex, int talker, int num)
{

	char token[1024];
	char work[256];
	char work2[512];

	char escapedname[1024];
	int fd = getfdFromCharaIndex( talker);
	int buttontype = 0;
	int windowtype = 0;
	int windowno = 0;
	int now[MAXEVENTFLG];
	int nowindex;
	int i;	
	int page;
	
	work[0] = 0;
	work2[0] = 0;
	token[0] = 0;

	now[0] =0;
	
	/*--¥¦¥¤¥ó¥É¥¦¥¿¥¤¥×¥á¥Ã¥»¡¼¥¸¤¬¤ª¤ª¤¤¤Î¤ÇÀè¤ËÀßÄê--*/
  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;

	switch( num) {
	
	  case 0:
		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,0);
		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,0);
		
  		/*--Áª      --*/
		sprintf(token,"3\n ¡a¡a¡a¡a¡Ö¡Ö¡a¥Á¥§¥Ã¥¯¥Þ¥ó¡a¡Ö¡Ö "
				  "\n¡a¡a¡a¸½ºß¤Î¥Õ¥é¥°¥Á¥§¥Ã¥¯¤ò¤·¤Þ¡¼¤¹"
				  "\n"
				  "\n¡a¡a¡a¡a ¢ã¡aNOW¥Õ¥é¥°¥Á¥§¥Ã¥¯¡a¢ä "
				  "\n¡a¡a¡a¡a ¢ã¡aEND¥Õ¥é¥°¥Á¥§¥Ã¥¯¡a¢ä "
				  "\n\n¡a¡a ¢ã¡aNOW¥Õ¥é¥°¥Á¥§¥Ã¥¯  ¾ÜºÙ  ¢ä "
				  "\n¡a¡a ¢ã¡aEND¥Õ¥é¥°¥Á¥§¥Ã¥¯  ¾ÜºÙ  ¢ä "
		);

	  	buttontype = WINDOW_BUTTONTYPE_CANCEL;
	  	windowtype = WINDOW_MESSAGETYPE_SELECT;
	  	windowno = CHAR_WINDOWTYPE_CHECKMAN_START; 
	  	break;

	//NOW¥Õ¥é¥°¤Î´ÊÃ±  ¼¨
	  case 1:
	  	
	  	//NOW¥Õ¥é¥°¤Î¥Á¥§¥Ã¥¯
		nowindex = NPC_NowFlgCheck( meindex, talker, now);

		//²¿¥Ú¡¼¥¸  ¤«
		page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;
		
		if(page == 0 || page == 1){
			i = 0;
		}else{
			i = 83;
		}
		
		//¥Ú¡¼¥¸
		for(; i < nowindex ; i++)
		{
			sprintf(work,"%d,",now[ i]);
			strcat(work2,work);
		}
		
		sprintf(token,"¡a¡a¡a¡a¡a¡Ö¡Ö¡a¥Á¥§¥Ã¥¯¥Þ¥ó¡a¡Ö¡Ö "
					"\n¡a¡a¸½ºß¤Î¿v  ¤Î  ¤Ã¤Æ¤¤¤ë NOW¥¤¥Ù¥ó¥È"
					"\n%s"
			 	,work2);	
	  	
	  	if(page != 2 && nowindex > 83)
	  	{
	  		//¥â¡¼¥É
	  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,1);
	  		//¥Ú¡¼¥¸
	  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,2);

			buttontype = WINDOW_BUTTONTYPE_NEXT;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		  	windowno = CHAR_WINDOWTYPE_CHECKMAN_MAIN; 

		}else{
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		}
		
		break;


	//END¥Õ¥é¥°¤Î´ÊÃ±  ¼¨
	  case 2:
		//½ª  ¥Õ¥é¥°¤Î¥Á¥§¥Ã¥¯
		nowindex = NPC_EndFlgCheck( meindex, talker, now);
		
		//²¿¥Ú¡¼¥¸  ¤«
		page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;
		
		if(page == 0 || page == 1){
			i = 0;
		}else{
			i = 83;
		}
		work2[0]=0;
		
		//¥Ú¡¼¥¸
		for(; i < nowindex ; i++)
		{
			sprintf(work,"%d,",now[ i]);
			strcat(work2,work);
		}
		sprintf(token,"¡a¡a¡a¡a¡a¡Ö¡Ö¡a¥Á¥§¥Ã¥¯¥Þ¥ó¡a¡Ö¡Ö "
					"\n¡a¡a¸½ºß¤Î¿v  ¤Î  ¤Ã¤Æ¤¤¤ë END¥¤¥Ù¥ó¥È"
					"\n%s"
			 	,work2);	
	  	
	  	if(page != 2 && nowindex > 83)
	  	{
	  		//¥â¡¼¥É
	  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,2);
	  		//¥Ú¡¼¥¸
	  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,2);

			buttontype = WINDOW_BUTTONTYPE_NEXT;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		  	windowno = CHAR_WINDOWTYPE_CHECKMAN_MAIN; 

		}else{
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		}
		
		break;
		
	
	// NOW¥Õ¥é¥°¤Î¾ÜºÙ  ¼¨
	  case 4:
		{
			
			//NOW¥Õ¥é¥°¥Á¥§¥Ã¥¯
			nowindex = NPC_NowFlgCheck( meindex, talker, now);

			if(NPC_FlgCheckMain( meindex, talker, nowindex,now,work2)
			 == FALSE)
			 {
			 	return;
			 }
			

			sprintf(token,"¡a¡a¡a¡a¡a¡Ö¡Ö¡a¥Á¥§¥Ã¥¯¥Þ¥ó¡a¡Ö¡Ö "
						"\n¡a¡a¸½ºß¤Î¿v  ¤Î  ¤Ã¤Æ¤¤¤ë NOW¥¤¥Ù¥ó¥È"
						"\n%s"
				 	,work2);	

			page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;
		  	nowindex = (nowindex / ((6*page)+1));

		  	if(page != 16 &&  nowindex != 0)
		  	{
		  		//¥â¡¼¥É
		  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,4);
		  		//¥Ú¡¼¥¸
		  		page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC);
		  		page++;
		  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,page);
			
				buttontype = WINDOW_BUTTONTYPE_NEXT;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			  	windowno = CHAR_WINDOWTYPE_CHECKMAN_MAIN; 
			}else{
				buttontype = WINDOW_BUTTONTYPE_OK;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			}

		}
	break;

	// END¥Õ¥é¥°¤Î¾ÜºÙ  ¼¨
	  case 5:
		{
			//END¥Õ¥é¥°¤Î¥Á¥§¥Ã¥¯
			nowindex = NPC_EndFlgCheck( meindex, talker, now);
			
			
			if(NPC_FlgCheckMain( meindex, talker, nowindex, now, work2)
			 == FALSE)
			 {
			 	return;
			 }

			sprintf(token,"¡a¡a¡a¡a¡a¡Ö¡Ö¡a¥Á¥§¥Ã¥¯¥Þ¥ó¡a¡Ö¡Ö "
						"\n¡a¡a¸½ºß¤Î¿v  ¤Î  ¤Ã¤Æ¤¤¤ë END¥¤¥Ù¥ó¥È"
						"\n%s"
				 	,work2);	

			page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;

		  	nowindex = (nowindex / ((6*page)+1));

		  	if(page != 16 &&  nowindex != 0)
		  	{
		  		//¥â¡¼¥É
		  		CHAR_setWorkInt(talker, CHAR_WORKSHOPRELEVANT, 5);
		  		//¥Ú¡¼¥¸
		  		page = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC);
		  		page++;
		  		CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, page);
			
				buttontype = WINDOW_BUTTONTYPE_NEXT;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			  	windowno = CHAR_WINDOWTYPE_CHECKMAN_MAIN; 
			}else{
				buttontype = WINDOW_BUTTONTYPE_OK;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			}

		}
	}
	
	makeEscapeString( token, escapedname, sizeof( escapedname));
	/*-¤³¤³¤ÇÁ÷¿®¤¹¤ë--*/
	lssproto_WN_send( fd, windowtype, 
					buttontype, 
					windowno,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					escapedname);

}


/*-----------------------------------------
¥¯¥é¥¤¥¢¥ó¥È¤«¤éÊÖ¤Ã¤Æ¤­¤¿»þ¤Ë¸Æ¤Ó½Ð¤µ¤ì¤ë¡e
-------------------------------------------*/
void NPC_CheckManWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	int datano;
	
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) return;

	datano = atoi(data);
	
	if(select == WINDOW_BUTTONTYPE_OK) 
	{
		NPC_CheckMan_selectWindow( meindex, talkerindex, 0 );
	}else if(select == WINDOW_BUTTONTYPE_CANCEL) {
		return;
	}



	
	switch(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANT)){
	  case 1:
	  	if(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTSEC) == 2){
			NPC_CheckMan_selectWindow( meindex, talkerindex, 1 );
	  	}
	  	break;

	  case 2:
	  	if(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTSEC) == 2){
			NPC_CheckMan_selectWindow( meindex, talkerindex, 2 );
	  	}
	  	break;

	  case 4:
	  	if(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTSEC) >= 2){
			NPC_CheckMan_selectWindow( meindex, talkerindex, 4 );
	  	}

	  case 5:
		if(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTSEC) >= 2){
			NPC_CheckMan_selectWindow( meindex, talkerindex, 5 );
		}

		break;
	}

	switch( datano ){
	  case 1:
			NPC_CheckMan_selectWindow( meindex, talkerindex, 1 );
		break;

	  case 2:
			NPC_CheckMan_selectWindow( meindex, talkerindex, 2 );
		break;

	  case 4:
			NPC_CheckMan_selectWindow( meindex, talkerindex, 4 );
		break;
		
	  case 5:
			NPC_CheckMan_selectWindow( meindex, talkerindex, 5 );
		break;
	}

}

/*
 *¡a¥¤¥Ù¥ó¥È  ¥Õ¥é¥°¤ò¥Á¥§¥Ã¥¯¤¹¤ë
 */
int NPC_NowFlgCheck(int meindex,int talker ,int nowflg[MAXEVENTFLG])
{
	int i = 0;
	int j = 0;
	
	for(i= 0; i < MAXEVENTFLG ; i++){
		if(NPC_NowEventCheckFlg( talker, i) == TRUE)
		{
			nowflg[j] = i;
			j++;
		}
 	}
 	
	return j;
}

/*
 *¡a¥¤¥Ù¥ó¥È½ª  ¥Õ¥é¥°¤ò¥Á¥§¥Ã¥¯¤¹¤ë
 */
int NPC_EndFlgCheck(int meindex,int talker ,int nowflg[MAXEVENTFLG])
{
	int i = 0;
	int j = 0;
	
	for(i= 0; i < MAXEVENTFLG ; i++){

		if(NPC_EventCheckFlg( talker, i) == TRUE)
		{
			nowflg[j] = i;
			j++;
		}
 	}
 	
	return j;
}

/*
 * ¾ÜºÙ
 *
 */
BOOL NPC_FlgCheckMain( int meindex,int talker,int nowindex,int now[MAXEVENTFLG],char *work2)
{
	int page;
	int max;
	int i;
	int shou;
	int j=1;
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char work[512];
	char buf[40];
	char buf2[42];
	
	//²¿¥Ú¡¼¥¸  ¤«
	page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;

	if(page == 0) {
		page =1;
		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,1) ;
	}

	if(page == 1){
		i = 0;
		if( nowindex >6 ){
			max =7;
		}else{
			max =nowindex;
		}
	}else{
		max = (page * 6) +1;
		i = max - 7;
		shou = nowindex / max;

		if(shou == 0){
			max = nowindex;
		}else{
			i = max - 7;
		}
	}
			
	//¥¤¥Ù¥ó¥È¤Î¾ÜºÙ¤¬½ñ¤«¤ì¤Æ¤¤¤ë¥Õ¥¡¥¤¥ë¤ò  ¤ß¤³¤à
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
		print("NPC_CheckMan:GetArgStrErr");
		return FALSE;
	}
			
	work[0] = 0;
	work2[0] = 0;

	//¥Ú¡¼¥¸
	for(; i < max ; i++)
	{
		sprintf(work,"#%d:",now[ i]);
		j = 1;
		while( getStringFromIndexWithDelim(argstr, "|", j, buf,sizeof( buf))
		 !=FALSE )
		 {	
			j++;
			if(strstr(buf,work) != NULL){
				sprintf(buf2,"%s\n",buf);
				strcat(work2,buf2);
				break;
			}
		}
	}
	
	return TRUE;
}
