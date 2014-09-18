#include "version.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "util.h"
#include "configfile.h"

#define IS_2BYTEWORD( _a_ ) ( (char)(0x80) <= (_a_) && (_a_) <= (char)(0xFF) )
//¾ã­Ó¼Ò¶ô»Ý­n¤ºÁp·J½sÀu¤Æ
/*-----------------------------------------
  ¤±¤Ä¤Î²þ¹Ô¤ò¤³¤í¤¹
  °ú¿ô¡u ¤â¤È¤Î  »ú  ¡e  ÀÜ  ¹¹¤µ¤ì¤ë¤¾

  windows , macintosh , unix¤Î¤É¤Î¹Ô¤âÂÐ±þ¤Ç¤­¤ë¡e

  windows : \r\n
  macintosh : \n\r
  unix : \n

  ¤À¤Ã¤¿¤«¤Ê
  macintosh : \r
  ¤Ç¤¹¡e
  ¤½¤ì¤È¤³¤Î¥×¤Ò¥°¥é¥à¤À¤È¤±¤Ä¤Î  »ú¤¸¤ã¤Ê¤¯¤Æ¤â
  ²þ¹Ô¾Ã¤¹¤±¤É¤¤¤¤¤ó¤Ç¤¹¤«
  -----------------------------------------*/
void chompex( char *buf )
{
    while( *buf ){
        if( *buf == '\r' || *buf == '\n' ){
            *buf='\0';
        }
        buf++;
    }
}



/*----------------------------------------
 * ¤±¤Ä¤Î1  »ú¤ò¼è¤ê½ü¤¯
 * °ú¿ô
 *  src           ¹¹¤¹¤ë  »ú  
 ----------------------------------------*/
void chop( char* src )
{
    /*
     * ¤¿¤À¤¿¤ó¤Ë    ¤Î  »ú¤ò¼è¤ê½ü¤¯
     */
    int length = strlen( src );
    if( length == 0 ) return;
    src[length-1] = '\0';
}


/*----------------------------------------
 * ¤±¤Ä¤Î  »ú¤ò¼è¤ë¡e¤½¤Î  »ú¤ò  ¿ô»ØÄê¤Ç¤­¤ë¡e
 * °ú¿ô
 *  src     ¸µ¤Î  »ú  
 *  delim   ºï½ü¤¹¤ë  »ú¤Î    ¡e    ¤ÏNULL  »ú¤Ç½ª¤Ã¤Æ¤¤¤ë»ö
 ----------------------------------------*/
void dchop( char* src , char* del)
{
    int dellen, srclen;
    int i;
    BOOL delete=FALSE;

    /*
     * src¤Î  ¤í¤«¤éÄ´¤Ù¤Æ¤¤¤¯¡e1  »ú¤Ë¤Ä¤­ del ¤¹¤Ù¤Æ¤ò¥ë¡¼¥×¤·¤Æ
     * °ìÃ×¤¹¤ì¤Ð¾Ã¤¹¡e¤½¤·¤Æ±­¤Î  »ú¤Ë¤¤¤¯¡e
     */

    srclen = strlen( src );
    dellen = strlen( del );
    if( srclen == 0 || dellen == 0 ) return;

    for( i = 0 ; i < dellen ; i ++ ){
        if( src[srclen-1] == del[i] ){
            delete = TRUE;
            break;
        }
    }

    /*  ÀÚ¤ê¼è¤ê    */
    if( delete )src[srclen - 1] = '\0';
}


/*----------------------------------------
 * dchop¤ÎµÕ¡e  ½é¤Î  »ú¤ò¼è¤ë¡e¤½¤Î  »ú¤ò  ¿ô»ØÄê¤Ç¤­¤ë¡e
 * °ú¿ô
 *  src     ¸µ¤Î  »ú  
 *  delim   ºï½ü¤¹¤ë  »ú¤Î    ¡e    ¤ÏNULL  »ú¤Ç½ª¤Ã¤Æ¤¤¤ë»ö
 ----------------------------------------*/
void pohcd( char* src , char* del)
{
    int dellen, srclen;
    int i;
    BOOL delete=FALSE;

    /*
     * src¤ÎÁ°¤«¤éÄ´¤Ù¤Æ¤¤¤¯¡e1  »ú¤Ë¤Ä¤­ del ¤¹¤Ù¤Æ¤ò¥ë¡¼¥×¤·¤Æ
     * °ìÃ×¤¹¤ì¤Ð¾Ã¤¹¡e¤½¤·¤Æ±­¤Î  »ú¤Ë¤¤¤¯¡e
     */

    srclen = strlen( src );
    dellen = strlen( del );
    if( srclen == 0 || dellen == 0 ) return;

    for( i = 0 ; i < dellen ; i ++ ){
        if( src[0] == del[i] ){
            delete = TRUE;
            break;
        }
    }

    /*  ºï½ü    */
    if( delete )
        /*  ¤³¤Î¥³¥Ô¡¼¤Ç NULL  »ú¤â¥³¥Ô¡¼¤µ¤ì¤ë */
        for( i = 0 ; i < srclen ; i ++ )
            src[i] = src[i+1];
}


/*----------------------------------------
 *   »ú  ¤Ë  »ú¤¬´Þ¤Þ¤ì¤Æ¤¤¤ë¤«¤É¤¦¤«Ä´¤Ù¤ë¡e
 * °ú¿ô
 *  src     char*   Ä´¤Ù¤ë  »ú  
 *  include char*   ´Þ¤Þ¤ì¤Æ¤¤¤ë¤«¤É¤¦¤«¤Î  »ú  ¡e
 * ÊÖ¤êÃÍ
 *  ©¨        ½é¤Ë´Þ¤Þ¤ì¤Æ¤¤¤¿src¤Î¾ì½ê
 *  ¼º      -1
 ----------------------------------------*/
int charInclude( char* src, char* include )
{
    int     i,j;
    int     srclen,includelen;

    srclen = strlen( src );
    includelen = strlen( include );

    for( i = 0 ; i < srclen ; i ++ ){
        for( j = 0 ; j < includelen ; j ++ )
            if( src[i]  == include[j] )
                return i;
    }
    return -1;
}


/*------------------------------------------------------------
 * EUC¤Î2¥Ð¥¤¥È  ¤À¤±¤ò¥³¥Ô¡¼¤·¤Ê¤¤strncpy
 * freeBSD ¤Î¥é¥¤¥Ö¥é¥ê¤«¤é strncpy ¤ò¼è¤Ã¤Æ¤­¤Æ¡b
 * ¤½¤ì¤ò¤â¤È¤Ë²þÂ¤¡e*dest++ ¤È¤«»È¤¤¤¿¤¤¤±¤É¡b·ë¹ç½ç½ø¤Ë¼«¿È¤¬
 * ¤Ê¤¤¤Î¤Ç¡b»È¤Ã¤Æ¤¤¤Ê¤¤¡e¤«¤ï¤ê¤Ë    ¤À¤±¤É¡b¥Ý¥¤¥ó¥¿¤â»È¤Ã¤Æ¤Æ
 * ¤«¤Ê¤ê  ¤Ê¥×¤Ò¥°¥é¥à
 * °ú¿ô
 *  dest        char*   ¥³¥Ô¡¼Àè
 *  src         char*   ¥½¡¼¥¹
 *  n           size_t  Ä¹¤µ
 * ÊÖ¤êÃÍ
 *  dest
 ------------------------------------------------------------*/
char* strncpy2( char* dest, const char* src, size_t n )
{
    if( n > 0 ){
        char*   d = dest;
        const char*   s = src;
        int i;
        for( i=0; i<n ; i++ ){
            if( *(s+i) == 0 ){
                /*  ¥³¥Ô¡¼¤·¶³¤ï¤Ã¤¿¤é NULL   »ú¤ò  ¤ì¤ë   */
                *(d+i) = '\0';
                return dest;
            }
            if( *(s+i) & 0x80 ){
                *(d+i)  = *(s+i);
                i++;
                if( i>=n ){
                    *(d+i-1)='\0';
                    break;
                }
                *(d+i)  = *(s+i);
            }else
                *(d+i) = *(s+i);
        }
    }
    return dest;
}

/*----------------------------------------
 * src ¤«¤é dest ¤Ë¥³¥Ô¡¼¤¹¤ë¡e ¥³¥Ô¡¼Àè¤Î¥µ¥¤³N¤òÄ´¤Ù¤ë¡e
 * °ú¿ô
 *      dest        ¥³¥Ô¡¼Àè
 *      n           ¥µ¥¤³N
 *      src         ¸µ¤Î  »ú  
 ----------------------------------------*/
void strcpysafe( char* dest ,size_t n ,const char* src )
{
    /*
     * src ¤«¤é dest ¤Ø¥³¥Ô¡¼¤¹¤ë.
     * strcpy, strncpy ¤Ç¤Ï dest ¤è¤ê ¥³¥Ô¡¼¤¹¤ë¬¤¤¬
     *   ¤­¤¤»þ¤Ë¸Æ¤Ö¤È,¥á¥â¥ê³°¥¢¥¯¥»¥¹¤¬µ¯¤ë.
     * ¤³¤ì¤ò  ¤°°Ù¤Ë, strncpy ¤¬¤¢¤ë¤¬ strlen( src ) ¤¬ n ¤è¤ê
     *   ¤­¤¤»þ¤Ë¤Ï, dest ¤Î    ¤¬ NULL   »ú¤È¤Ï¤Ê¤é¤Ê¤¤.
     *
     * ¤·¤¿¤¬¤Ã¤Æ dest ¤Î  ¤­¤µ¤è¤ê src ¤Î¤Û¤¦¤¬Ä¹¤¤»þ¤Ë¤Ï
     * n-1 ¤Ç strncpy ¤ò¤¹¤ë. ¾®¤µ¤¤»þ¤Ï¤½¤Î¤Þ¤Þ¥³¥Ô¡¼¤¹¤ë
     *
     * n ¤¬  ¤Î»þ¤Ï¶³¤«¤·¤¯¤Ê¤ë¤Î¤Ç  ¤Î»þ¤Ï ²¿¤â¤·¤Ê¤¤¡e
     *
     */
    // Nuke +1 (08/25): Danger if src=0
    if (!src) {
        *dest = '\0';
        return;
    }
    if( n <= 0 )        /* ²¿¤â¤·¤Ê¤¤   */
        return;

    /*  ¤³¤Î»þÅÀ¤Ç¡b n >= 1 °Ê¾å¤¬·èÄê  */
    /*  NULL  »ú¤ò¹Í  ¤·¤Æ  ³Ó¤¹¤ë  */
    else if( n < strlen( src ) + 1 ){
        /*
         * ¥Ð¥Ã¥Õ¥¡¤¬Â­¤ê¤Ê¤¤¤Î¤Ç n - 1(NULL  »ú)
         * ¤Ç strncpy ¤ò¸Æ¤Ö
         */
        strncpy2( dest , src , n-1 );
        dest[n-1]='\0';
    }else
        strcpy( dest , src );

}

/*----------------------------------------
 * src ¤«¤é length ±Ç¤¹¡e   ¥³¥Ô¡¼Àè¤Î  ¤­¤µ¤òÄ´¤Ù¤ë¡e
 * °ú¿ô
 *          dest        ¥³¥Ô¡¼Àè
 *          n           ¥³¥Ô¡¼Àè¤Î¥µ¥¤³N
 *          src         ¥³¥Ô¡¼¸µ
 *          length      ²¿  »ú¥³¥Ô¡¼¤¹¤ë¤«
 ----------------------------------------*/
void strncpysafe( char* dest , const size_t n ,
                  const char* src ,const int length )
{
    /*
     * src ¤«¤é dest ¤Ë length ¥³¥Ô¡¼¤¹¤ë
     * strcpy, strncpy ¤Ç¤Ï dest ¤è¤ê ¥³¥Ô¡¼¤¹¤ë¬¤¤¬
     *   ¤­¤¤»þ¤Ë¸Æ¤Ö¤È,¥á¥â¥ê³°¥¢¥¯¥»¥¹¤¬µ¯¤ë.
     * ¤³¤Î´Ø¿ô¤Ç¤Ï¡bstrlen( src ) ¤È length ¤Î¾®¤µ¤¤¤Û¤¦
     * (  ºÝ¤Ë¥³¥Ô¡¼¤¹¤ë¬¤) ¤È dest ¤Î¥µ¥¤³N¤ò  ¤Ù¤Æ¡b
     * strcpysafe ¤È  ¤¸»ö¤ò¤¹¤ë¡e
     */

    int Short;
    Short = min( strlen( src ) , length );

    /* NULL  »ú ¤ò¹Í  ¤·¤¿  ³Ó */
    if( n < Short + 1 ){
        /*
         * ¥Ð¥Ã¥Õ¥¡¤¬Â­¤ê¤Ê¤¤¤Î¤Ç n - 1(NULL  »ú)
         * ¤Ç strncpy ¤ò¸Æ¤Ö
         */
        strncpy2( dest , src , n-1 );
        dest[n-1]='\0';

    }else if( n <= 0 ){
        return;
    }else{
        /*
         * ¥Ð¥Ã¥Õ¥¡¤Ï½½Ê¬¤Ë¤¢¤ë¤Î¤Ç Short ¤Çstrncpy¤ò¸Æ¤Ö
         * ¤Ê¶³ src ¤Ë¤Ï Short ¤ÎÄ¹¤µ  ¤Ë NULL ¤¬¤Ê¤¤¤Î¤Ç¡b
         * dest ¤Ë¤Ï ÄÉ²Ã¤·¤Æ¶³¤¯¡e
         */

        strncpy2( dest , src , Short );
        dest[Short]= '\0';

    }
}

/*------------------------------------------------------------
 * strcat¤ò°ÂÁ´¤Ë  ¸½¤¹¤ë¡eÃÙ¤¤¤Î¤Ç¡b    ¤Ê¾ì¹ç¤ò½ü¤¤¤Æ»È¤ï¤Ê¤¤»ö¡e
 * °ú¿ô
 *  src     char*       ¸µ¤Ë¤Ê¤ë  »ú  
 *  size    int         src ¤Î¥µ¥¤³N(   ¤í¤Ë¤É¤ì¤À¤±»Ä¤Ã¤Æ¤¤¤ë¤«¤Ç¤Ï
 *                          ¤Ê¤¯¤Æ¥Ð¥Ã¥Õ¥¡Á´   )
 *  ap      char*       ÉÕ¤±²Ã¤¨¤ë  
 * ÊÖ¤êÃÍ
 *  src     (  ¤ì»ÒÍÑ)
 ------------------------------------------------------------*/
char* strcatsafe( char* src, int size, char* ap )
{
    strcpysafe( src + strlen( src ) , size - strlen(src), ap );
    return src;
}

/*----------------------------------------
 *   »ú  ¤Î    ¤ò  ³Ó¤¹¤ë¡e  ¤¸¤«¤É¤¦¤«¤·¤«  Äê¤·¤Ê¤¤ by ringo
 * char *s1 , *s2 :   ³Ó¤¹¤ë  »ú  
 * ÊÖ¤êÃÍ
 * 0 ¡u ¶³¤Ê¤¸
 * 1 : ¤³¤È¤Ê¤ë
 * ¤³¤ì¤ÏNPC¤Î  ¥­¥¹¥È²ò  ¤Ë¤ä¤¯¤À¤Ä¡e
 ----------------------------------------*/
//«ÝÀu¤Æ¨ç¼Æ¦N¥JADD
int strcmptail( char *s1 , char *s2 )
{
    int i;
    int len1 = strlen( s1 );
    int len2 = strlen( s2 );
		int ind1,ind2;
    for(i=0;;i++){
        ind1 = len1 - 1 - i;
        ind2 = len2 - 1 - i;
        if( ind1 < 0 || ind2 < 0 ) return 0;
        if( s1[ind1] != s2[ind2] ) return 1;
    }
}



/*----------------------------------------
 *   »ú  ¤Î    ¤Ë½ñ¤«¤ì¤Æ¤¤¤ëÀ°¿ô¤ò¥²¥Ã¥È¤¹¤ë¡e
 * NPC¤Î  ¥­¥¹¥È²ò  ¤Ë¤ä¤¯¤À¤Ä¡e
 * ¤¿¤À¤·    ¤Ë¤¤¤é¤ó  »ú¤¬¤¢¤ë¾ì¹ç  »ë¤¹¤ë¡e
 * ¿®²¬¤¬  ¤Á¤Ê¤¤¥¢¥ë¥´¥ê³N¥à¤Ë  ¹¹¡e
 * °ú¿ô
 *  hoge    char*   ²ò  ¤¹¤ë  »ú  
 * ÊÖ¤êÃÍ
 *  ²ò  ¤·¤¿À°¿ô¡e
 *
 *   ¡u
 * "ringo ( §a¼P§a¼P¬P¤H ) ¡G 100"
 *      ¤Î¾ì¹ç¤Ï100¤¬¤«¤¨¤ë¡e
 * "ringo ( §a¼P§a¼P¬P¤H ) ¡G ¬O100"
 *      ¤â100¤Ë¤Ê¤ë¡e¤Û¤ó¤Þ¤ËNPCÍÑ¤Ê¤ê¤Í¡e
 ----------------------------------------*/
#define DECIMALCHARNUM 10
#define FIGURECHARNUM 3
#define SPECIALCHARNUM 2
char *UTIL_decimalchar[DECIMALCHARNUM] = {"0","1","2",
                                     "3",  "4","5",
                                     "6","7","8","9"};
char *UTIL_figurechar[FIGURECHARNUM] = {"","",""};
char *UTIL_specialchar[SPECIALCHARNUM] = {"",""};

/*
 *   »ú  ¤«¤é¡b¿ôÃÍ¤ò  ¤¹¤È  ¤ï¤ì¤ë  Ê¬¤ò  ¤­½Ð¤¹¡eÀè  ¤Î¤Û¤¦¤«¤é
 * Ãµ¤·¤Æ  ¤­½Ð¤¹¡eeucÍÑ¡e
 *
 *    : in "³o¥»®Ñ¤C¸U¥U300¬üª÷¶Ü¡H"
 *      out "7¸U"
 * °ú¿ô
 * char *in:      »ú  
 * char *out:½Ð    »ú  
 * int outsiz:½Ð    »ú  ¤Î¥Ð¥Ã¥Õ¥¡¥µ¥¤³N
 * ÊÖ¤êÃÍ
 * ¿ôÃÍ¤ò  ¤¹  Ê¬¤¬¤¢¤Ã¤¿¤é¤½¤Î  »ú  ¤ÎÄ¹¤µ¡b¤Ê¤«¤Ã¤¿¤é£°
 */
static int findNumberString( char *in, char *out, int outsiz )
{
    int len,i,j,k, findflag, numstrflag;
    char tmp[3];

    len = strlen(in);
    if( len == 0 ) return 0;

    numstrflag=0;
    findflag = 0;
    for( i=0,j=0;in[i]!='\0'&& j<outsiz-2;i++ ){
        findflag = 0;
        if( in[i] & 0x80 ){
            /* EUC¤Î¾ì¹ç */
            tmp[0] = in[i];
            tmp[1] = in[i+1];
            tmp[2] = '\0';
        }else{
            /* ASCII¤Î¾ì¹ç */
            tmp[0] = in[i];
            tmp[1] = '\0';
        }
        for( k=0;k<DECIMALCHARNUM;k++ ){
            if( strstr( UTIL_decimalchar[k], tmp ) ){
                numstrflag = 1;
                findflag = 1;
            }
        }
        for( k=0;k<FIGURECHARNUM;k++ ){
            if( strstr( UTIL_figurechar[k], tmp ) ){
                numstrflag = 1;
                findflag = 1;
            }
        }
        for( k=0;k<SPECIALCHARNUM;k++ ){
            if( strstr( UTIL_specialchar[k], tmp ) ){
                numstrflag = 1;
                findflag = 1;
            }
        }
        if( findflag ){
            if( in[i] & 0x80 ){
                out[j] = tmp[0];
                out[j+1] = tmp[1];
                j+=2;
                i++;
            }else{
                out[j] = tmp[0];
                j+=1;
            }
            continue;
        }

        if( numstrflag ){
            out[j] = '\0';
            return j;
        }

    }

    if( findflag ){
        out[j] = '\0';
        return j;
    }else{
        return 0;
    }

}


/*
 *   »ú  ¤òint¤Ë  ´¹¡e
 *
 */
int numstrIsKanji( char *str );
int decimalstrToInt( char *str );
int kanjistrToInt( char *str );

int textToInt( char *str )
{
    char numstr[256];
    if( !findNumberString(str,numstr,sizeof(numstr)) ) return 0;

    if( numstrIsKanji(numstr) ) return kanjistrToInt(numstr);
    else return decimalstrToInt(numstr);

}

/*
 * ¿ôÃÍ¤ò  ¤¹  »ú  ¤¬¡b    ¸ì¼°¤Ê¤Î¤«À¤³¦¼°¤Ê¤Î¤«¤ò    
 * °ú¿ô     ¤¹¤ë  »ú  ¡e¿ô»ú¤ò  ¤¹  »ú  °Ê³°¤Ï´Þ¤ó¤Ç¤Ï¤¤¤±¤Ê¤¤¡e
 * ÊÖ¤êÃÍ     ¸ì¼°¤Ê¤é£±¡bÀ¤³¦¼°¤Ê¤é£°
 *    str ¤¬"¤@»õ¥|¤d¸U¡C"¤Î¤È¤­¡aÊÖ¤êÃÍ1
 *    str ¤¬"123"¤Î¤È¤­¡aÊÖ¤êÃÍ£°
 *
 */
int numstrIsKanji( char *str )
{
    int i,j;
    char tmp[3];

    for( i=0;str[i]!='\0';i++ ){
        if( str[i] & 0x80 ){
            tmp[0] = str[i];
            tmp[1] = str[i+1];
            tmp[2] = '\0';
            i++;
        }else{
            tmp[0] = str[i];
            tmp[1] = '\0';
        }
        for( j=0;j<FIGURECHARNUM;j++ ){
            if( strstr(UTIL_figurechar[j],tmp) ){
                return 1;
            }
        }
        for( j=0;j<SPECIALCHARNUM;j++ ){
            if( strstr(UTIL_specialchar[j],tmp) ){
                return 1;
            }
        }
    }
    return 0;

}

/*
 * ½½¿ÊË¡¤Ç¿ôÃÍ¤ò  ¤·¤¿  »ú  ¤òint¤Ë  ´¹
 * char *str   »ú  
 * ÊÖ¤êÃÍ   »ú  ¤¬  ¤¹¿ôÃÍ
 */
int decimalstrToInt( char *str )
{

    double val;
    char tmp[3];
    int i,j;

    val = 0;
    for( i=0;str[i]!='\0';i++ ){
        if( str[i] & 0x80 ){
            tmp[0] = str[i];
            tmp[1] = str[i+1];
            tmp[2] = '\0';
            i++;
        }else{
            tmp[0] = str[i];
            tmp[1] = '\0';
        }
        for( j=0;j<DECIMALCHARNUM;j++ ){
            if( strstr(UTIL_decimalchar[j],tmp) ){
                val = val*10+j;
            }
        }
    }

    if( val > 0x7fffffff ) return 0x7fffffff;
    else     return (int)val;

}


/*
 *     ¼°¤Ç  ¤·¤¿¿ôÃÍ¤òÀ°¿ô¤Ë  ´¹
 * char *str ¿ôÃÍ¤ò  ¤¹  »ú  
 * ÊÖ¤êÃÍ ¿ôÃÍ
 *   
 * str¤¬"¤@»õ4¤d¸U¡H"¤Î¤È¤­ÊÖ¤êÃÍ 140000000
 */
int kanjistrToInt( char *str )
{
    double val,tmpval;
    char tmp[3];
    int num,i,j,numflag;

    numflag = 0;
    num = 1;
    tmpval = 0;
    val = 0;
    for( i=0;str[i]!='\0';i++ ){
        if( str[i] & 0x80 ){
            tmp[0] = str[i];
            tmp[1] = str[i+1];
            tmp[2] = '\0';
            i++;
        }else{
            tmp[0] = str[i];
            tmp[1] = '\0';
        }
        for( j=0;j<SPECIALCHARNUM;j++ ){
            if( strstr(UTIL_specialchar[j],tmp ) ){
                if( numflag == 1 ) tmpval += num;
                if( j == 0 ) val += tmpval*10000;
                else if( j == 1 ) val += tmpval*100000000;
                num = 1;
                numflag = 0;
                tmpval = 0;
                goto nextchar;
            }
        }
        for( j=0;j<FIGURECHARNUM;j++ ){
            if( strstr(UTIL_figurechar[j],tmp) ){
                if( j == 0 ) tmpval += num*10;
                else if( j == 1 ) tmpval += num*100;
                else if( j == 2 ) tmpval += num*1000;
                num = 1;
                numflag = 0;
                goto nextchar;
            }
        }
        for( j=0;j<DECIMALCHARNUM;j++ ){
            if( strstr(UTIL_decimalchar[j],tmp) ){
                num = j;
                numflag = 1;
                goto nextchar;
            }
        }
nextchar:
		continue;
    }
    if( numflag == 1 ) tmpval += num;
    val += tmpval;

    if( val > 0x7fffffff ) return 0x7fffffff;
    else     return (int)val;

}



/*----------------------------------------
 *   »ú  ¤Î    ¤Ë½ñ¤«¤ì¤Æ¤¤¤ëÀ°¿ô¤ò¥²¥Ã¥È¤¹¤ë¡e
 * NPC¤Î  ¥­¥¹¥È²ò  ¤Ë¤ä¤¯¤À¤Ä¡e
 * ¤¿¤À¤·    ¤Ë¤¤¤é¤ó  »ú¤¬¤¢¤ë¾ì¹ç  »ë¤¹¤ë¡e
 * ¿®²¬¤¬  ¤Á¤Ê¤¤¥¢¥ë¥´¥ê³N¥à¤Ë  ¹¹¡e
 * °ú¿ô
 *  hoge    char*   ²ò  ¤¹¤ë  »ú  
 * ÊÖ¤êÃÍ
 *  ²ò  ¤·¤¿À°¿ô¡e
 *
 *   ¡u
 * "ringo ( §a¼P§a¼P¬P¤H ) ¡G 100"
 *      ¤Î¾ì¹ç¤Ï100¤¬¤«¤¨¤ë¡e
 * "ringo ( §a¼P§a¼P¬P¤H ) ¡G ¬O100"
 *      ¤â100¤Ë¤Ê¤ë¡e¤Û¤ó¤Þ¤ËNPCÍÑ¤Ê¤ê¤Í¡e
 ----------------------------------------*/
int texttoiTail( char *hoge)
{
    return 0; /* ¤Ð¤°¤Ã¤Æ¤¿¤Î¤Ç¤³¤¦¤·¤¿¡e¤½¤Î¤¦¤Á´Ø¿ô¤´¤È¾Ã¤¹¡e */


#if 0
    int len = strlen(hoge);

    if( len == 0 )return atoi(hoge);
    /*    ¤«¤éÃµ¤·¤Æ  ½é¤Ë¿ô»ú¤ò¸«¤Ä¤±¤ë    */
    while(--len && len >= 0){
        if( isdigit(hoge[len]) )
            break;
    }
    while( --len && len >= 0 ){
        if( !isdigit(hoge[len]) )
            break;
        if( hoge[len]=='+' || hoge[len]=='-')
            break;
    }
    return atoi(hoge+len+1);
#endif
#if 0
    int f = 0;
    for(i=len-1;i>=0;i--){
        if( isdigit( hoge[i] ) ) f = 1;
        if( (f == 1)
            && !( isdigit(hoge[i]) || hoge[i]=='+' || hoge[i]=='-') ){
            return atoi( hoge + i + 1 );
        }
    }
    return atoi(hoge);
#endif
}

/*----------------------------------------
 *   »ú  ¤ò¿ô»ú¤«¤É¤¦¤«¥Á¥§¥Ã¥¯¤·¤Æ¡b¿ô»ú¤À¤Ã¤¿¤é
 *  °ú¿ô¤Ë  ¤ì¤ÆÊÖ¤¹
 * °ú¿ô
 *      arg               ´¹¸µ
 *      number            ´¹  ¤òÂå  ¤¹¤ë½ê
 *      base            ´ð¿ô
 *      type            °ú¿ô¤Î·¿( CTYPE ¤Ï common.h ¤ËÀë¸À¤µ¤ì¤Æ¤¤¤ë )
 * ÊÖ¤êÃÍ
 *      TRUE(1)    ©¨  
 *      FALSE(0)   strtol¤¬´°àú¤Ë¤Ï©¨  ¤·¤Ê¤«¤Ã¤¿
 *                  ¤³¤Î»þ¤Înumber¤Ïstrtol¤ÎÊÖ¤êÃÍ¤Ç¤¢¤ë
 ----------------------------------------*/
BOOL strtolchecknum( char* arg , void* number,int base ,CTYPE type)
{
    char* buf;
    int     num;

    num = strtol( arg, &buf, base);

    switch( type ){
    case CHAR:
        *(char*)number = (char)num;
        break;
    case SHORT:
        *(short*)number = (short)num;
        break;
    case INT:
        *(int*)number = num;
        break;
    case DOUBLE:
        *(double*)number = (double)num;
        break;
    default:
        break;
    }


    if( strlen( buf ) >= 1 )
        /*
         * »Ä¤ê  »ú¤¬¤¢¤ë¤È¤¤¤¦»ö¤Ï  ´¹¤·¤­¤ì¤Æ¤Ê¤¤¤È¤¤¤¦»ö¤Ê¤Î¤À¡e
         */
        return FALSE;

    return TRUE;

}




/*----------------------------------------
 * src ¤«¤é dels ¤Ç»ØÄê¤·¤¿  »ú¤ò¼è¤ê½ü¤¯
 * ¥Ð¥Ã¥¯¥¹¥é¥Ã¼³¥å¤Ï¥¨¥¹¥±¡¼¥×¤Ç¤¢¤ë»ö¤ËÃí°Õ¡e
 * ¤Ä¤Þ¤ê¥Ð¥Ã¥¯¥¹¥é¥Ã¼³¥å¤Ïºï½ü¤Ç¤­¤Ê¤¤¡e
 * °ú¿ô
 *  src      ¹¹¤µ¤ì¤ë  »ú  
 *  char*  ºï½ü¤¹¤ë  »ú(  »ú  ¤Ç»ØÄê²Ä)
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 ----------------------------------------*/
void deleteCharFromString( char* src , char* dels )
{
    int index=0;    /* ¤Ç¤­¤¢¤¬¤ê¤Î  »ú  ¤Ç¤Î index */
    int delength;   /* dels ¤ÎÄ¹¤µ¤òÀßÄê¤¹¤ë(¾¯¤·¤ÏÂ®ÅÙ¥¢¥Ã¥×¤Î°Ù */
    int i=0,j;/* i ¤Ï src ¤ò¥ë¡¼¥×¤¹¤ë  ¿ô j ¤Ï dels ¤ò¥ë¡¼¥×¤¹¤ë  ¿ô */

    delength= strlen( dels );

    while( src[i] != '\0' ){
        if( src[i] == BACKSLASH  )
            /*
             * ¥Ð¥Ã¥¯¥¹¥é¥Ã¼³¥å¤Ê¤Î¤Ç¥¨¥¹¥±¡¼¥×¤¹¤ë
             * ¤Ä¤Þ¤ê±­¤Î  »ú¤Ï  ¾ò·ï¤Ë°Ü  ¤¹¤ë
             */
            src[index++] = src[++i];         /* substitute next char */

        else{
            for( j = 0 ; j < delength ; j ++ ){
                if( src[i] == dels[j] )
                    /*
                     * ºï½ü¤¹¤ë  »ú¤À¡e¤è¤Ã¤Æ i ¤Ë 1 ¤ò¤¿¤¹
                     */
                    goto incrementi;
            }
            /*
             * ¾Ã¤¹  ¤Ë»ØÄê¤µ¤ì¤Æ¤¤¤Ê¤¤¤Î¤Ç¥³¥Ô¡¼¤¹¤ë¡e
             */
            src[index++] =  src[i];
        }
    incrementi:
        i++;
    }

    src[index] = '\0';
}



/*----------------------------------------
 * src ¤«¤é dels ¤Ç»ØÄê¤·¤¿  »ú¤ò¼è¤ê½ü¤¯
 * ¥¨¥¹¥±¡¼¥×¤Ï¤Ê¤¤
 * °ú¿ô
 *  src      ¹¹¤µ¤ì¤ë  »ú  
 *  char*  ºï½ü¤¹¤ë  »ú(  »ú  ¤Ç»ØÄê²Ä)
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 ----------------------------------------*/
void deleteCharFromStringNoEscape( char* src , char* dels )
{
    int index=0;    /* ¤Ç¤­¤¢¤¬¤ê¤Î  »ú  ¤Ç¤Î index */
    int delength;   /* dels ¤ÎÄ¹¤µ¤òÀßÄê¤¹¤ë(¾¯¤·¤ÏÂ®ÅÙ¥¢¥Ã¥×¤Î°Ù */
    int i=0,j;/* i ¤Ï src ¤ò¥ë¡¼¥×¤¹¤ë  ¿ô j ¤Ï dels ¤ò¥ë¡¼¥×¤¹¤ë  ¿ô */

    delength= strlen( dels );

    while( src[i] != '\0' ){
        for( j = 0 ; j < delength ; j ++ ){
            if( src[i] == dels[j] )
                /*
                 * ºï½ü¤¹¤ë  »ú¤À¡e¤è¤Ã¤Æ i ¤Ë 1 ¤ò¤¿¤¹
                 */
                goto incrementi;
        }
        /*
         * ¾Ã¤¹  ¤Ë»ØÄê¤µ¤ì¤Æ¤¤¤Ê¤¤¤Î¤Ç¥³¥Ô¡¼¤¹¤ë¡e
         */
        src[index++] =  src[i];
    incrementi:
        i++;
    }

    src[index] = '\0';
}

/*------------------------------------------------------------
 * »ØÄê¤µ¤ì¤¿  »ú¤ò»ØÄê¤µ¤ì¤¿  »ú¤Ç  ¤­¤«¤¨¤ë
 * °ú¿ô
 *  src     char*         ¹¹¤µ¤ì¤ë  »ú  
 *  oldc    char          ¹¹¤µ¤ì¤ë  »ú
 *  newc    char          ¹¹¤¹¤ë  »ú
 * ÊÖ¤êÃÍ
 *  src
 ------------------------------------------------------------*/
char*   replaceString( char* src, char oldc ,char newc )
{
    char*   cp=src;

    do{
        if( *cp == oldc ) *cp=newc;
    }while( *cp++ );
    return src;
}

typedef struct tagEscapeChar
{
    char     escapechar;
    char     escapedchar;
}EscapeChar;
static EscapeChar   escapeChar[]=
{
    { '\n',   'n' },
    { ',',    'c' },
    { '|',    'z' },
    { '\\',   'y' },
};


char makeCharFromEscaped( char c )//add this function,because the second had it
{
        int i;
        
        for( i = 0; i < sizeof( escapeChar )/sizeof( escapeChar[0] ); i++ )
        {
            if( escapeChar[i].escapedchar == c )
            {
               c = escapeChar[i].escapechar;
               break;
            }
        }
        return c;
}
/*----------------------------------------
 * makeEscapeString¤Çºî©¨¤µ¤ì¤¿  »ú  ¤ò¸µ¤Ë  ¤¹
 * °ú¿ô
 *  src             char*       ¸µ¤Ë¤Ê¤ë  »ú  ¡e¤³¤ì¤¬½ñ¤­ÂØ¤ë¡e
 * ÊÖ¤êÃÍ
 *  src    ¤òÊÖ¤¹¡e(´Ø¿ô¤ò  ¤ì¤³¤Ë¤·¤ä¤¹¤¤¤è¤¦¤Ë)
 ----------------------------------------*/
char   *makeStringFromEscaped( char* src )
{//ttom this function all change,beside copy from the second

    int     i;
    // CoolFish: Fix bug 2001/10/13
    // int     srclen = strlen( src );
    int	    srclen = 0;
    int     searchindex=0;
    
    // CoolFish: Fix bug 2001/10/13
    if (!src)	return	NULL;
    srclen = strlen(src);
    
    for( i = 0; i < srclen; i ++ )
    {
    // for 2Byte Word
    if( IS_2BYTEWORD( src[i] ) ){
        src[searchindex++] = src[i++];
        src[searchindex++] = src[i];
    }else
            if( src[i] == '\\' )
            {	
                // ±­¤Î  »ú¤Ë¹Ô¤¯
                i++;
                src[searchindex++] = makeCharFromEscaped( src[i] );
            }
            else
            {
                src[searchindex++] = src[i];
            }
    }        
            src[searchindex] = '\0';
        
            return src;
}

char*  makeEscapeString( char* src , char* dest, int sizeofdest)
{ //ttom this function all change, copy from the second
    int i;
    int	    srclen = 0;
    int     destindex=0;
    
    // CoolFish: Fix bug 2001/10/13
    if (!src)	return	NULL;
    srclen = strlen(src);
  	BOOL dirty;
    int  j;
    char escapechar='\0';
    for( i = 0 ; i < srclen ; i ++ ){
    	dirty=FALSE;
		if( destindex + 1 >= sizeofdest )break;
         if( IS_2BYTEWORD( src[i] ) ){
               if( destindex + 2 >= sizeofdest )break;
               
            dest[destindex] = src[i];
            dest[destindex+1] = src[i+1];
                 destindex += 2;
                 i ++;
                 continue;
          }
          for( j = 0; j<sizeof(escapeChar)/sizeof(escapeChar[0]); j++){
               if( src[i] == escapeChar[j].escapechar ){
                   dirty=TRUE;
                   escapechar= escapeChar[j].escapedchar;
                   break;
               }
          }
          if( dirty == TRUE ){
              if( destindex + 2 < sizeofdest ){
                  dest[destindex] = '\\';
                  dest[destindex+1] = escapechar;
                  destindex+=2;
                  dirty=TRUE;
                  continue;
              }else{
                  dest[destindex] = '\0';
                  return dest;
              }
          }else{
                dest[destindex] = src[i];
                destindex++;
          }
     }
     dest[destindex] = '\0';
     return dest;
}

//this function copy all from the second
char * ScanOneByte( char *src, char delim ){
	// Nuke
	if (!src) return NULL;

        //   »ú  ¤¬¤Ê¤¯¤Ê¤ë¤Þ¤Ç¸¡º÷
        for( ;src[0] != '\0'; src ++ ){
             // Á´³Ñ¤Î£±¥Ð¥¤¥È  ¤«¤É¤¦¤«¤ò¥Á¥§¥Ã¥¯
          if( IS_2BYTEWORD( src[0] ) ){
              // Á´³Ñ¤À¡e¤½¤Î¾ì¹ç¤Ï£±¥Ð¥¤¥ÈÍ¾Ê¬¤Ë¿Ê¤Þ¤»¤ë¡e
              // ¤¿¤À¤·£±¥Ð¥¤¥È¤·¤«¤Ê¤¤¾ì¹ç¤Ï¤½¤¦¤·¤Ê¤¤
              if( src[1] != 0 ){
                  src ++;
              }
              continue;
          }
          //   ³Ñ¤À¤Ã¤¿¡e¤³¤³¤ÇÂÐ¾Ý¤Î  »ú¤È  ³Ó
          if( src[0] == delim ){
              return src;
          }
        }
        // ¥ë¡¼¥×  ¤±¤¿¤é¸«¤Ä¤«¤é¤Ê¤«¤Ã¤¿¡e
        return NULL;
}
                                                                                                                                                                                                                                                                                                                


/*----------------------------------------
 * delim ¤Ç»ØÄê¤µ¤ì¤¿  »ú  ¤ò¶èÀÚ¤ê  »ú¤È¤·¤Æ
 * index     ¤Î  ¤ò  ¤ë¡eindex¤Ï1¤Ç¤Ï¤¸¤Þ¤ë¡e
 *   »ú  »Å  ¡e
 * °ú¿ô
 *  src     ¸µ¤Î  »ú  
 *  delim   ¥Ç¥ê¥ß¥¿¤È¤Ê¤ë  »ú  ¡e    ¤ÏNULL  »ú¤Ç½ª¤Ã¤Æ¤¤¤ë»ö
 *  index   ²¿    ¤Î  ¤ò¼è¤ë¤«
 *  buf     ·ë²Ì  »ú  ¤ò³Ê  ¤¹¤ë¾ì½ê¤Ø¤Î¥Ý¥¤¥ó¥¿¡¼
 *  buflen  ·ë²Ì  »ú  ¤ò³Ê  ¤¹¤ë¾ì½ê¤Î¥µ¥¤³N
 * ÊÖ¤êÃÍ
 *  ¤¢¤Ã¤¿¤é TRUE(1);
 *  ¤Ê¤«¤Ã¤¿¤é FALSE(0);
 *  ex
 *      getStringFromIndexWithDelim( "aho=hoge","=",1,buf,sizeof(buf) );
 *      buf ... aho
 *
 *      getStringFromIndexWithDelim( "aho=hoge","=",2,buf,sizeof(buf) );
 *      buf ... hoge
 *      ¤³¤ì¤â  ¤á¤é¤ì¤ë¡e
 *
 *      getStringFromIndexWithDelim( "aho=hoge","=",3,buf,sizeof(buf) );
 *      ÊÖ¤êÃÍ FALSE
 ----------------------------------------*/
BOOL getStringFromIndexWithDelim_body( char* src ,char* delim ,int index,
                                 char* buf , int buflen ,
                                       char *file, int line )
{//ttom this function all change,copy from the second
    int i;          /* ¥ë¡¼¥×  ¿ô */
    int length =0;  /* ¼è¤ê½Ð¤·¤¿  »ú  ¤ÎÄ¹¤µ */
    int addlen=0;   /* Â­¤µ¤ì¤ëÄ¹¤µ */
    int oneByteMode = 0; /* £±¥Ð¥¤¥È¥â¡¼¥É¤«¡ª */
		char* last;
    if( strlen( delim ) == 1 ){ // ¸¡º÷¤¬£±¥Ð¥¤¥È¤Ê¤é£±¥Ð¥¤¥È¥â¡¼¥É¤Ë¤¹¤ë
        oneByteMode = 1;// ¤½¤ÎºÝ£²¥Ð¥¤¥È  »ú¤Ï¥Á¥§¥Ã¥¯¤·¤Ê¤¤
    }
    for( i =  0 ; i < index ; i ++ ){
    	
         src += addlen;/* ¤ß¤Ä¤«¤Ã¤¿Ä¹¤µ¤òÂ­¤¹ */
      
         if( oneByteMode ){
             // £±¥Ð¥¤¥È¥â¡¼¥É¤À¤Ã¤¿¤é¤³¤Á¤é¤Ç¸¡º÷
             last = ScanOneByte( src, delim[0] );
         }else{
                 last  = strstr( src , delim );  /* ¸«ÉÕ¤±¤ë */
         }
         if( last == NULL ){
            /*
             * ¤ß¤Ä¤«¤é¤Ê¤«¤Ã¤¿¤Î¤Ç¤¹¤Ù¤Æ¥³¥Ô¡¼¤·¤Æ return¡e
            */
            strcpysafe( buf , buflen, src );

            if( i == index - 1 )
                /*¤Á¤ç¤¦¤É¤ß¤Ä¤«¤Ã¤¿*/
                return TRUE;
                                                                                                           
                /*¤ß¤Ä¤«¤é¤Ê¤«¤Ã¤¿*/
             return FALSE;
          }
          
          /*
           * ¤ß¤Ä¤«¤Ã¤¿½ê¤È  ½é¤Î°Ì  ¤Îº¹¤òµá¤á¤ë
           * ¤Ä¤Þ¤ê¶èÀÚ¤é¤ì¤Æ¤¤¤ë  »ú  ¤ÎÄ¹¤µ
          */
          length = last - src;
                                           
          /*
           * ±­¤Î¥ë¡¼¥×¤Î°Ù¤Ë¤ß¤Ä¤«¤Ã¤¿Ä¹¤µ¤È delim ¤ÎÄ¹¤µ¤òÂ­¤·¤Æ¶³¤¯
          */
          addlen= length + strlen( delim );
       }
       strncpysafe( buf, buflen , src,length );

       return TRUE;
}


/*------------------------------------------------------------
 *   »ú   "a,b,c,d" ¤ò°ú¿ô¤ËÂå  ¤¹¤ë¡e¥Ç¥Õ¥©¥ë¥È¤Ï 0 ¤Ë¤¹¤ë¡e
 * ¤«¤Ê¤ê¤Î  ¼ì´Ø¿ô¡e¸½ºß¤Ï¤Á¤ç¤Ã¤ÈÃÙ¤¤¡e
 * °ú¿ô
 *  src         char*   ¸µ¤Î  »ú  
 *  int1        int*    int¤Î¥Ý¥¤¥ó¥¿¡e(a¤òÂå  ¤¹¤ë)
 *  int2        int*    int¤Î¥Ý¥¤¥ó¥¿¡e(b¤òÂå  ¤¹¤ë)
 *  int3        int*    int¤Î¥Ý¥¤¥ó¥¿¡e(c¤òÂå  ¤¹¤ë)
 *  int4        int*    int¤Î¥Ý¥¤¥ó¥¿¡e(d¤òÂå  ¤¹¤ë)
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 ------------------------------------------------------------*/
void getFourIntsFromString(char* src,int* int1,int* int2,int* int3,
                           int* int4)
{
    int     ret;
    char    string[128];

    ret = getStringFromIndexWithDelim( src,"," ,1,string,sizeof(string));
    if( ret == FALSE )*int1=0;
    else    *int1 = atoi(string);

    ret = getStringFromIndexWithDelim( src, ",",2,string,sizeof(string) );
    if( ret == FALSE )*int2=0;
    else    *int2 = atoi(string);

    ret = getStringFromIndexWithDelim( src, ",",3,string,sizeof(string) );
    if( ret == FALSE )*int3=0;
    else    *int3 = atoi(string);

    ret = getStringFromIndexWithDelim( src, ",",4,string,sizeof(string) );
    if( ret == FALSE )*int4=0;
    else    *int4 = atoi(string);

}


/*----------------------------------------------
 * src ¤Î  ¤Çdels¤Ç»ØÄê¤·¤¿  »ú¤¬  Â³¤·¤Æ¤¤¤¿¤é
 * 1¤Ä¤Ë¤Þ¤È¤á¤ë¡e
 * ¥Ð¥Ã¥¯¥¹¥é¥Ã¼³¥å¤Ï¥¨¥¹¥±¡¼¥×¼³¡¼¥±¥ó¥¹¤È¤Ê¤ë¡e
 * ¤·¤¿¤¬¤Ã¤Æ  Â³¤·¤¿¥Ð¥Ã¥¯¥¹¥é¥Ã¼³¥å¤ò¾Ã¤¹»ö¤Ï¤Ç¤­¤Ê¤¤¡e
 * °ú¿ô
 *  src      ²½¤¹¤ë  »ú  
 *  dels   ¾Ã¤¹  »ú(  »ú  ¤Ç»ØÄê²Ä)
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 ---------------------------------------------*/
void deleteSequentChar( char* src , char* dels )
{
    int length;         /* src ¤ÎÄ¹¤µ */
    int delength;       /* dels ¤ÎÄ¹¤µ */
    int i,j;            /* ¥ë¡¼¥×  ¿ô */
    int index=0;        /* ³ÎÄê  »ú  ¤Î index */
    char backchar='\0';   /* ºï½ü¤¹¤ë  »ú */

    length = strlen( src );
    delength = strlen( dels );

    /*
     * ¥ë¡¼¥×¤¹¤ë¤´¤È¤Ë¥³¥Ô¡¼¤¹¤ë
     */
    for( i = 0 ; i < length ; i ++ ){
        if( src[i] == BACKSLASH ){
            /*
             * ¥Ð¥Ã¥¯¥¹¥é¥Ã¼³¥å¤À¤Ã¤¿¤é
             * ¶¯À©Åª¤Ë  ¤Ä( BACKSLASH¼«¿È¤È¡b±­¤Î  »ú )
             * ¿Ê¤á¤ë
             */
            src[index++]=src[i++];
            if( i >= length ){
                print( "*\n");
                break;
            }
            src[index++]=src[i];
            /*   Â³¤¬»ß¤Ã¤¿»ö¤Ë¤¹¤ë */
            backchar = '\0';
            continue;
        }
        if( src[i] == backchar )
            /*
             * ºï½ü¤¹¤ë¤â¤¸¤À¤Ã¤¿¤é²¿¤â¤·¤Ê¤¤¡e¤Ä¤Þ¤êºï½ü¤Ë¤Ê¤ë¡e
             */
            continue;

        /*
         *   Â³¤Îºï½ü¤¹¤ë  »ú¤¬¤Ê¤¯¤Ê¤Ã¤¿¤Î¤Ç backchar ¤Ï NULL   »ú¤Ë¤¹¤ë
         * ¤³¤ì¤¬¤«¤ó¤¸¤ó
         */
        backchar = '\0';
        for( j = 0 ; j < delength ; j ++ ){
            if( src[i] == dels[j] ){
                /*
                 * ºï½ü¤¹¤ë  »ú  ¤À¡e ¤³¤³¤Ç continue ¤·¤Ê¤¤»ö¤Ë¤è¤ê
                 * °ì  »ú¤Ïºï½ü¤·¤Ê¤¤»ö¤¬ÊÝ¾Ú¤Ç¤­¤ë¡e
                 */
                backchar=src[i];
                break;
            }
        }
        src[index++]=src[i];
    }
    src[index++]='\0';
}

/*----------------------------------------
 * hashÃÍ¤òÊÖ¤¹¡e  »ú  ¤Ë¤Î¤ß»È¤¦¤³¤È
 * ¤³¤ì¤Ï  ¤½¤Î¤Þ¤Þ¤Ê¤Î¤Ç¥¢¥ë¥´¥ê³N¥à¤Ï¤è¤¦¤ï¤«¤é¤ó¤¬
 * ¤¤¤¤¥¢¥ë¥´¥ê³N¥à¤é¤·¤¤¡e
 * °ú¿ô
 *  s     »ú  
 * ÊÖ¤êÃÍ
 *  int ¥Ï¥Ã¼³¥åÃÍ
 *----------------------------------------*/
#define PRIME 211
int hashpjw ( char* s )
{
    char *p;
    unsigned int h= 0 ,g;
    for( p = s ; *p ; p ++ ){
        h = ( h<< 4 ) + (*p);
        if( (g = h & 0xf0000000) != 0){
            h = h ^ (g>>24);
            h = h ^ g;
        }
    }
    return h % PRIME;
}

/*----------------------------------------
 * ¤Ò¡¼¥«¥ë¤Ë¥Ð¥¤¥ó¥É¤·¤¿¥½¥±¥Ã¥È¤òÊÖ¤¹( TCP )
 * °ú¿ô
 *          port        ¤Ò¡¼¥«¥ë¥Ý¡¼¥È
 * ÊÖ¤êÃÍ
 *          -1      ¥¨¥é¡¼
 *                      1. socket¼³¥¹  ¥à¥³¡¼¥ë¤Î¥¨¥é¡¼
 *                      2. bind¼³¥¹  ¥à¥³¡¼¥ë¤Î¥¨¥é¡¼
 *                      3. listen¼³¥¹  ¥à¥³¡¼¥ë¤Î¥¨¥é¡¼
 ----------------------------------------*/
int bindlocalhost( int port )
{
    struct sockaddr_in sin;         /*¤Ò¡¼¥«¥ë¥¢¥É¥ì¥¹ÍÑ*/
    int sfd;                        /*¥½¥±¥Ã¥È¥Ç¥£¥¹¥¯¥ê¥×¥¿*/
    int rc;                         /*¤Ò¡¼¥«¥ë¥ê¥¿¡¼¥ó¥³¡¼¥É*/

    /*AF_INET¥×¤Ò¥È¥³¥ë ¤Î SOCK_STREAM */
    sfd = socket( AF_INET, SOCK_STREAM, 0 );
    if( sfd == -1 ){
        print( "%s\n" , strerror(errno ) );
        return -1;
    }
  #ifdef _JZ_EPOLL
    if(!setnonblocking(sfd))return -1;			//¦N¥JADD «Dªý¶ë¼Ò¦¡
  #endif
    if( getReuseaddr() ) {
		int sendbuff;
	    /* ¤Ò¡¼¥«¥ë¥¢¥É¥ì¥¹¤ÎºÆÍøÍÑ */
		setsockopt( sfd, SOL_SOCKET, SO_REUSEADDR, 
						(char *)&sendbuff, sizeof( sendbuff));
	}
	
    /*0¥¯¥ê¥¢¤·¤ÆÃÍ¤òÂå  */
    memset( &sin ,0, sizeof( struct sockaddr_in ) );
    sin.sin_family=AF_INET;
    sin.sin_port = htons( port );
    sin.sin_addr.s_addr = INADDR_ANY;

    rc = bind( sfd , (struct sockaddr*)&sin, sizeof(struct sockaddr_in));
    if( rc == -1 ){
        print( "%s\n" , strerror(errno ) );
        return -1;
    }
	
	
    rc = listen( sfd , 5 );
    if( rc == -1 ){
        print( "%s\n" , strerror(errno ) );
        return -1;
    }
    return sfd;
}



/*----------------------------------------
 * ¥³¥Í¥¯¥È¤¹¤ë¡e( TCP )
 * °ú¿ô
 *      hostname    ÀÜÂ³¤¹¤ë¥Û¥¹¥È  
 *                      (xx.xxx.xxx.xxx¤Ç¤â¤¤¤¤¤·xxx.co.jp¤È¤«¤Ç¤â¤¤¤¤)
 *      port        ÀÜÂ³¤¹¤ë¥Û¥¹¥È¤Î¥Ý¡¼¥È  
 * ÊÖ¤êÃÍ
 *      ¥Ç¥£¥¹¥¯¥ê¥×¥¿( -1 ¤Î»þ¤Ï¥¨¥é¡¼ )
 ----------------------------------------*/
int connectHost( char* hostname , unsigned short port )
{
    struct sockaddr_in sock;        /*connect¤Î»þ¤Ë»È¤¦*/
    struct hostent* hoste;          /*hostname¤Îhostent*/
    int     fd;                     /*¥³¥Í¥¯¥È¤·¤¿¥½¥±¥Ã¥È¥Ç¥£¥¹¥¯¥ê¥×¥¿*/
    int     lr;                     /*¤Ò¡¼¥«¥ë¥ê¥¿¡¼¥ó¥³¡¼¥É*/

    memset( &sock ,  0 , sizeof( struct sockaddr_in ) );
    sock.sin_family = AF_INET;
    sock.sin_port = htons( port );

    /* dot notation ¤«¤É¤¦¤«¤òÄ´¤Ù¤ë */
    sock.sin_addr.s_addr = inet_addr( hostname );
    if( sock.sin_addr.s_addr == -1 ){
        /*
         *¥¨¥é¡¼ dot notation ¤Ç¤Ï¤Ê¤¤¡e¤·¤¿¤¬¤Ã¤Æ  ÄÌ¤Î  Á°²ò·è¤¬  Í×¤À¡e
         * dns¤ò¤Ò¤«¤Ê¤±¤ì¤Ð¤Ê¤é¤Ê¤¤»þ¤Ï¤Ò¤­¤Ë¤¤¤¯¤Î¤â¤³¤³¤Ç¤ä¤Ã¤Æ¤¯¤ì¤ë¡e
         */
        hoste = gethostbyname( hostname );
        if( hoste == NULL ){
            print( "gethostbyname error hostname: %s\n", hostname);
            return -1;
        }

        memcpy((void*)&sock.sin_addr.s_addr ,
               hoste->h_addr , sizeof(struct in_addr) );
    }

    /*¥½¥±¥Ã¥È¤òºî¤ë*/
    fd = socket( AF_INET, SOCK_STREAM , 0 );
    if( fd == -1 ){
        print("Cannot Create Socket(%s errno:%d)\n"
              ,strerror( errno ) ,errno );
        return -1;
    }
    /*¥³¥Í¥¯¥È¤¹¤ë*/
    lr = connect(fd,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if( lr != 0 ){
        print("Cannot connect. (%s errno:%d)\n"
              ,strerror( errno ) ,errno );
        return -1;
    }

    return fd;
}


/*----------------------------------------
 * ¥Ð¥Ã¥Õ¥¡¤Î  ¤Ë²þ¹Ôµ­¹æ¤¬¤¢¤ë¤«¤É¤¦¤«¡e
 * ¾¯¤·Áá¤¤¥¢¥ë¥´¥ê³N¥à¤Ë  ¹¹¡e
 * °ú¿ô
 *  char*   Ä´¤Ù¤ë  »ú  
 * ÊÖ¤êÃÍ
 *    ½é¤Ë¸«¤Ä¤«¤¿²þ¹Ôµ­¹æ¤Î°Ì       ¥¢¥¯¥»¥¹¤·¤¿»þ¤Ë¤¤¤¤ÃÍ¡e
 *  ¤É¤¦¤¤¤¦»ö¤«¤È¤¤¤¦¤È¡b
 *      existsNewLinwCharacter( "aho\nhoge\n" )
 *  ¤À¤È¡bÊÖ¤êÃÍ¤Ï 3 ¤Ë¤Ê¤ë¡e¤³¤ì¤ËÃí°Õ¤¹¤ë»ö¡e
 *  -1      ¤Ê¤«¤Ã¤¿
 *
 ----------------------------------------*/
int existsNewLineCharacter( char* line )
{
#if 1
    char*   old = line;
    do{
        if( *line == NEWLINE)return line-old;
    }while( *(line++) );
#else
    int i;                          /*¥ë¡¼¥×  ¿ô*/
    int length = strlen( line );    /*Ä´¤Ù¤ë  »ú  ¤ÎÄ¹¤µ*/
    for( i = 0 ; i < length ; i ++ ){
        if( line[i] == NEWLINE )
            return i;
    }
#endif

    return -1;
}



/*----------------------------------------
 *    »ú    ¤Ë  »ú¤¬  ¤ï¤ì¤ë¾ì½ê¤ò¤«¤¨¤¹¡e²¿    ¤«¤ò·è¤á¤ì¤ë¡e
 *  nindex( string , c , 1 ) ¤È index( string , c ) ¤Ï equivalent ¤Ç¤¢¤ë¡e
 *  number¤¬  ¤Þ¤¿¤Ï0¤Î»þ¤ÏÊÖ¤êÃÍ¤Ï¡bstring¤È  ¤¸¤Ç¤¢¤ë¡e
 *
 *  °ú¿ô
 *      string  char*   Ä´¤Ù¤ë  »ú  
 *      c       int     Ä´¤Ù¤ë  »ú
 *      number  int       ¤ï¤ë  ¹æ
 *  ÊÖ¤êÃÍ
 *      °ìÃ×¤·¤¿  »ú¤Î¥Ý¥¤¥ó¥¿¡¼
 *      NULL    ¤ß¤Ä¤«¤é¤Ê¤«¤Ã¤¿¡e
 ----------------------------------------*/
char* nindex( char* string, int c , int number )
{
    int i;                          /*¥ë¡¼¥×  ¿ô*/
    int num=0;
    int length = strlen( string );  /*Ä´¤Ù¤ë  »ú  ¤ÎÄ¹¤µ*/
    if( number <= 0 )return string;
    for( i = 0 ; i < length ; i ++ ){
        if( string[i] == c )num++;
        if( number == num ) return &string[i];
    }
    return NULL;
}

BOOL rrd( char* dirname , STRING64* buf ,int bufsize, int* index)
{
    DIR*    d;
    char    dirn[1024];

    d = opendir( dirname );
    if( d == NULL )return FALSE;

    while( 1 ){
        struct dirent* dent;
        struct  stat    st;
        dent = readdir( d );
        if( dent == NULL ){
            if( errno == EBADF ){
                errorprint;
                closedir(d);
                return FALSE;
            }else
                /*  successful */
                break;
        }

        /*  . ¤Ç»Ï¤Þ¤ë¥Õ¥¡¥¤¥ë¤Ï  ¤Þ¤Ê¤¤    */
        if(  dent->d_name[0] == '.' )continue;

        snprintf(dirn, sizeof(dirn), "%s/%s" , dirname,dent->d_name );
        if( stat( dirn , &st ) == -1 )continue;
        if( S_ISDIR( st.st_mode ) ){
            if( rrd( dirn , buf ,bufsize, index) == FALSE ){
                closedir(d);
                return FALSE;
            }
        }else{
            if( *index >= bufsize )
                break;

            strcpysafe( buf[*index].string , sizeof( buf[*index].string ), dirn );
            (*index)++;
        }
    }
    closedir(d);
    return TRUE;
}

int rgetFileName( char* dirname , STRING64* string, int size)
{
    int     index=0;
    if( rrd( dirname , string ,size ,&index ) == FALSE )return -1;
    else return index;
}

BOOL checkStringsUnique( char** strings, int num ,int verbose)
{
    int     i,j;
    for( i = 0 ; i < num - 1; i ++ ){
        for( j = i+1 ; j < num ; j ++){
            if( !strcmp( strings[i] ,strings[j] )){
                if( verbose )
                    print( "Overlapp string is %s\n", strings[i] );
                return FALSE;
            }
		}
	}
    return TRUE;
}

BOOL PointInRect( RECT* rect, POINT* p )
{
    if( rect->x         <= p->x && p->x <= rect->x + rect->width &&
        rect->y         <= p->y && p->y <= rect->y + rect->height )
        return TRUE;
    return FALSE;
}

BOOL CoordinateInRect( RECT* rect, int x, int y)
{
    POINT   p={x,y};
    return PointInRect(rect,&p);
}

int clipRect( RECT *rect1, RECT *rect2, RECT *ret )
{
    if(   rect1->x > rect2->x + rect2->width -1
       || rect2->x > rect1->x + rect1->width -1
       || rect1->y > rect2->y + rect2->height -1
       || rect2->y > rect1->y + rect1->height -1 )
	   return 0;
    ret->x = max( rect1->x, rect2->x );
    ret->y = max( rect1->y, rect2->y );
    ret->width = min( rect1->x+rect1->width, rect2->x+rect2->width ) - ret->x;
    ret->height = min( rect1->y+rect1->height, rect2->y+rect2->height ) - ret->y;
    return 1;
}

BOOL isstring1or0( char*  string )
{
    if( strcasecmp(string,"TRUE" ) == 0 )return TRUE;
    if( strcasecmp(string,"FALSE") == 0 )return FALSE;
    if( strcasecmp(string,"1"    ) == 0 )return TRUE;
    if( strcasecmp(string,"0"    ) == 0 )return FALSE;
    if( strcasecmp(string,"ON"   ) == 0 )return TRUE;
    if( strcasecmp(string,"OFF"  ) == 0 )return FALSE;
    return FALSE;
}

void easyGetTokenFromString( char *src,int count,char*output,int len )
{
    int i;
    int counter = 0;

    if( len <= 0 )return;


#define ISSPACETAB( c )   ( (c) == ' ' || (c) == '\t' )

    for(i=0;;i++){
        if( src[i]=='\0'){
            output[0] = '\0';
            return;
        }
        if( i > 0 && ! ISSPACETAB( src[i-1] ) &&
            ! ISSPACETAB(  src[i] ) ){
            continue;
        }

        if( ! ISSPACETAB( src[i]) ){
            counter++;
            if( counter == count){
                /* copy it */
                int j;
                for(j=0;j<len-1;j++){
                    if( src[i+j] == '\0' ||
                        ISSPACETAB( src[i+j] ) ){
                        break;
                    }
                    output[j]=src[i+j];
                }
                output[j]='\0';
                return;

            }
        }

    }
}

/*------------------------------------------------------------
 * Àþ·¿¤Ç  Ê¬ÅÀ¤ò¤â¤È¤á¤ë¡e by ringo
 * Á¥¤Î°Ü  ÍÑ
 *
 * double val1 , val2 : ¤³¤ÎÃÍ¤Î¤¢¤¤¤À¤ò¤È¤ë
 * double d :   Ê¬  
 *
 *
 *  -d<0----- val1 ---0<d<1------- val2 ------d>1----
 *
 *
 ------------------------------------------------------------*/
float  linearDiv( float val1 , float val2 , float d )
{
    return val1 + ( val2 - val1 ) * ( d );
}


/*------------------------------------------------------------
 *   »ú  ¤Î¤Ê¤«¤«¤é  °Õ¤Ê  »ú¤òlen  »ú¤È¤ê¤À¤·¤Æ¥é¥ó¥À¥à¤Ê
 *   »ú  ¤ò¤Ä¤¯¤ë¡eNPC¤Î¥é¥ó¥À¥à¥Ñ¥¹¥ï¡¼¥ÉÀ¸©¨¤Ë¤Ä¤«¤Ã¤Æ¤ë
 *
 * char *cand : ¤³¤³¤«¤éÁª¤Ö¡e¸õ  ¤Î  »ú¡e
 * char *out : ½Ð  ¥Ð¥Ã¥Õ¥¡
 * int len : ½Ð  ¤ÎÄ¹¤µ
 ------------------------------------------------------------*/
void makeRandomString( char *cand , char *out , int len )
{
    int i;
    int l = strlen( cand );

    for(i=0;i<len;i++){
        out[i] = cand[ rand() % l];
    }
    out[i] = '\0';
}


/*------------------------------------------------------------
 * °ú¿ô¤Ë»ØÄê¤µ¤ì¤¿¥Õ¥¡¥¤¥ë¤¬¥Õ¥¡¥¤¥ë¤«¤É¤¦¤òÄ´¤Ù¤ë
 * °ú¿ô
 *  filename        char*        ¥Õ¥¡¥¤¥ë  
 * ÊÖ¤êÃÍ
 *  ¥Õ¥¡¥¤¥ë        TRUE(1)
 *  ¥Õ¥¡¥¤¥ë¤Ç¤Ê¤¤  FALSE(0)
 ------------------------------------------------------------*/
BOOL isExistFile( char* filename )
{
    /*  fopen ¤ÇÄ´¤Ù¤ë  */
    FILE*   fp;
    fp = fopen( filename, "w" );
    if( fp ){
        fclose( fp );
        return TRUE;
    }else
        return FALSE;
}
/*------------------------------------------------------------
 * 10¿Ê  62¿Ê  ´¹
 * °ú¿ô
 * a        int     ¸µ¤Î£±£°¿Ê
 * out      char *  ³Ê  ¥Ð¥Ã¥Õ¥¡¡¼
 * outlen   int     ³Ê  ¥Ð¥Ã¥Õ¥¡¤Î  ¤­¤µ
 *   ¤êÃÍ
 *          ©¨      out¤Î¥¢¥É¥ì¥¹
 *          ¼º      NULL
 ------------------------------------------------------------*/
char *cnv10to62( int a, char *out, int outlen )
{
#if 1
	int		i, j;
    char    base[] = { "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    int     tmp[64];
    int     src;
    int minus;
	int baselen = sizeof( base)-1;
    if( a < 0 ){
		minus = 1;
		a *= -1;
	} else {
		minus = 0;
	}
    /* special case */
    if( a < baselen) {
		if( minus ){
			*(out) = '-';
			*(out+1) = base[a];
			*(out+2) = '\0';
			return (out);
		} else {
			*out = base[a];
			*(out+1) = '\0';
			return( out);
		}
    }
    src = a;
    for( i = 0; src >= baselen; i ++ ) {
        tmp[i] = src % baselen;
        src /= baselen;
    }
    i--;
    if( minus ){
		*out = '-';
     	*(out+1) = base[src];
		for( j = 2; i >= 0; i --, j ++ ) {
			if( j > outlen - 2 ) return NULL;
			*(out+j) = base[tmp[i]];
		}
	} else {
     	*out = base[src];
		for( j = 1; i >= 0; i --, j ++ ) {
			if( j > outlen - 2 ) return NULL;
			*(out+j) = base[tmp[i]];
		}
	}
	*(out+j) = '\0';
    return( out);
#else
/* ¸Å¤¤   */
#define     CNV_NUMBER  62
	int		i, j;
    char    base[] = { "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    int     tmp[64];
    int     src;
    if( a < 0 ) return( NULL);
    if( a < CNV_NUMBER) {
	    *out = base[a];
	    *(out+1) = '\0';
        return( out);
    }
    src = a;
    for( i = 0; src >= CNV_NUMBER; i ++ ) {
        tmp[i] = src % CNV_NUMBER;
        src /= CNV_NUMBER;
    }
    i--;
	*out = base[src];
    for( j = 1; i >= 0; i --, j ++ ) {
		if( j > outlen - 2 ) return NULL;
		*(out+j) = base[tmp[i]];
    }
	*(out+j) = '\0';
    return( out);
#undef      CNV_NUMBER
#endif
}
/* 
 * Í¿¤¨¤é¤ì¤¿    ¤Î  ¤Ç¡f¿ôÃÍ¤¬½Å  ¤·¤Æ¤¤¤ë¤«Ä´¤Ù¤ë´Ø¿ô
 * -1 ¤ÏÂÐ¾Ý³°¤È¤¤¤¦  ¤Ê´Ø¿ô
 *
 * °ú¿ô
 * src		*int		Ä´¤Ù¤ë¸µ¤Î    
 * srclen	int			src ¤Î  ¤­¤µ
 *
 *   ¤êÃÍ   ½Å  ¤¢¤ê	TRUE
 *				¤Ê¤·	FALSE
 */
BOOL checkRedundancy( int *src, int srclen)
{
	int		i,j;
	int		ret = FALSE;
	
	for( i = 0; i < srclen; i ++ ) {
		if( *(src + i) != -1 ) {
			for( j = i+1; j < srclen; j ++ ) {
				if( *(src +i) == *( src+j) ) {
					ret = TRUE;
					break;
				}
			}
		}
	}
	return ret;
}
void
shuffle_ints( int num, int *a, int t )
{
    int i;
    int x ,y ,s;
    for(i=0;i<t; i++ )
    {
    	y = random() % num;
			x = random() % num;
      s = a[x];
      a[x] = a[y];
      a[y] = s;
    }
}


static unsigned char BitTable[] =	/*	¥Ó¥Ã¥È¤ÎÊÂ¤Ó½ç¤òµÕ¤Ë¤¹¤ë  ¡¼¥Ö¥ë	*/
{
		0x00 , 0x80 , 0x40 , 0xC0 , 0x20 , 0xA0 , 0x60 , 0xE0 , 
		0x10 , 0x90 , 0x50 , 0xD0 , 0x30 , 0xB0 , 0x70 , 0xF0 , 
		0x08 , 0x88 , 0x48 , 0xC8 , 0x28 , 0xA8 , 0x68 , 0xE8 , 
		0x18 , 0x98 , 0x58 , 0xD8 , 0x38 , 0xB8 , 0x78 , 0xF8 , 
		0x04 , 0x84 , 0x44 , 0xC4 , 0x24 , 0xA4 , 0x64 , 0xE4 , 
		0x14 , 0x94 , 0x54 , 0xD4 , 0x34 , 0xB4 , 0x74 , 0xF4 , 
		0x0C , 0x8C , 0x4C , 0xCC , 0x2C , 0xAC , 0x6C , 0xEC , 
		0x1C , 0x9C , 0x5C , 0xDC , 0x3C , 0xBC , 0x7C , 0xFC , 
		0x02 , 0x82 , 0x42 , 0xC2 , 0x22 , 0xA2 , 0x62 , 0xE2 , 
		0x12 , 0x92 , 0x52 , 0xD2 , 0x32 , 0xB2 , 0x72 , 0xF2 , 
		0x0A , 0x8A , 0x4A , 0xCA , 0x2A , 0xAA , 0x6A , 0xEA , 
		0x1A , 0x9A , 0x5A , 0xDA , 0x3A , 0xBA , 0x7A , 0xFA , 
		0x06 , 0x86 , 0x46 , 0xC6 , 0x26 , 0xA6 , 0x66 , 0xE6 , 
		0x16 , 0x96 , 0x56 , 0xD6 , 0x36 , 0xB6 , 0x76 , 0xF6 , 
		0x0E , 0x8E , 0x4E , 0xCE , 0x2E , 0xAE , 0x6E , 0xEE , 
		0x1E , 0x9E , 0x5E , 0xDE , 0x3E , 0xBE , 0x7E , 0xFE , 
		0x01 , 0x81 , 0x41 , 0xC1 , 0x21 , 0xA1 , 0x61 , 0xE1 , 
		0x11 , 0x91 , 0x51 , 0xD1 , 0x31 , 0xB1 , 0x71 , 0xF1 , 
		0x09 , 0x89 , 0x49 , 0xC9 , 0x29 , 0xA9 , 0x69 , 0xE9 , 
		0x19 , 0x99 , 0x59 , 0xD9 , 0x39 , 0xB9 , 0x79 , 0xF9 , 
		0x05 , 0x85 , 0x45 , 0xC5 , 0x25 , 0xA5 , 0x65 , 0xE5 , 
		0x15 , 0x95 , 0x55 , 0xD5 , 0x35 , 0xB5 , 0x75 , 0xF5 , 
		0x0D , 0x8D , 0x4D , 0xCD , 0x2D , 0xAD , 0x6D , 0xED , 
		0x1D , 0x9D , 0x5D , 0xDD , 0x3D , 0xBD , 0x7D , 0xFD , 
		0x03 , 0x83 , 0x43 , 0xC3 , 0x23 , 0xA3 , 0x63 , 0xE3 , 
		0x13 , 0x93 , 0x53 , 0xD3 , 0x33 , 0xB3 , 0x73 , 0xF3 , 
		0x0B , 0x8B , 0x4B , 0xCB , 0x2B , 0xAB , 0x6B , 0xEB , 
		0x1B , 0x9B , 0x5B , 0xDB , 0x3B , 0xBB , 0x7B , 0xFB , 
		0x07 , 0x87 , 0x47 , 0xC7 , 0x27 , 0xA7 , 0x67 , 0xE7 , 
		0x17 , 0x97 , 0x57 , 0xD7 , 0x37 , 0xB7 , 0x77 , 0xF7 , 
		0x0F , 0x8F , 0x4F , 0xCF , 0x2F , 0xAF , 0x6F , 0xEF , 
		0x1F , 0x9F , 0x5F , 0xDF , 0x3F , 0xBF , 0x7F , 0xFF
};
static unsigned short crctab16[] =	/*	crc¡a¤Î·×»»  ¡¼¥Ö¥ë		*/
{
		0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
		0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
		0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
		0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
		0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
		0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
		0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
		0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
		0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
		0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
		0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
		0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
		0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
		0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
		0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
		0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
		0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
		0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
		0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
		0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
		0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
		0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
		0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
		0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
		0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
		0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
		0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
		0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
		0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
		0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
		0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
		0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0,
};
unsigned short CheckCRC( unsigned char *p , int size )
{
	unsigned short	crc = 0;
	int		i;
	
	for( i = 0 ; i < size ; i ++ ){
		crc = ( crctab16[ ( crc >> 8 ) & 0xFF ] 
					^ ( crc << 8 ) ^ BitTable[ p[ i ] ] );
	}
	return crc;
}

// Add Code By Shan  2001.06.16
INLINE double time_diff(struct timeval subtrahend,
                      struct timeval subtractor)
{
    return( (subtrahend.tv_sec - subtractor.tv_sec)
             + (subtrahend.tv_usec
             - subtractor.tv_usec  ) / (double)1E6 );
}
#ifdef _JZ_EPOLL
BOOL setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL,0);
	if(opts<0)
	{
		perror("fcntl(sock,GETFL)");
		return FALSE;
	}
	opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		perror("fcntl(sock,SETFL,opts)");
		return FALSE;
	}
	return TRUE;
}
#endif
