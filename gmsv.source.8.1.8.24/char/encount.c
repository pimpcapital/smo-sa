#include "version.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef _REDHAT_V9
#include <errno.h>
#endif

#include "common.h"
#include "util.h"
#include "buf.h"
#include "char_base.h"
#include "char.h"
#include "configfile.h"
#include "encount.h"
#include "enemy.h"

#ifdef _ADD_ENCOUNT           // WON ADD 增加敵遭遇觸發修件
#include "encount.h"
#endif

/* 巨件市它件玄楮  及末□旦 */

#ifndef _ADD_ENCOUNT           // WON ADD 增加敵遭遇觸發修件
typedef struct tagENCOUNT_Table
{
    int                 index;
    int                 floor;
    int                 encountprob_min;                /* 巨件市它件玄割   */
    int                 encountprob_max;                /* 巨件市它件玄割   */
    int                 enemymaxnum;        /* 升木分仃襯毛綜月井 */
    int                 zorder;
    int                 groupid[ENCOUNT_GROUPMAXNUM];       /* 弘伙□皿No */
    int                 createprob[ENCOUNT_GROUPMAXNUM];    /* 公及弘伙□皿及請蜇   */
    RECT                rect;
}ENCOUNT_Table;
ENCOUNT_Table           *ENCOUNT_table;
#endif

int                     ENCOUNT_encountnum;
#define     ENCOUNT_ENEMYMAXCREATENUM   10

static INLINE BOOL ENCOUNT_CHECKENCOUNTTABLEARRAY( int array)
{
    if( array < 0 || array > ENCOUNT_encountnum-1) return FALSE;
    return TRUE;
}

/*------------------------------------------------------------
 * 巨件市它件玄澀爛及賡渝祭毛允月〔
 * 婁醒
 *  filename        char*       澀爛白央奶伙  
 * 忒曰襖
 *  岳      TRUE(1)
 *  撩      FALSE(0)
 *------------------------------------------------------------*/
BOOL ENCOUNT_initEncount( char* filename )
{
    FILE*   f;
    char    line[256];
    int     linenum=0;
    int     encount_readlen=0;

    f = fopen(filename,"r");
    if( f == NULL ){
        errorprint;
        return FALSE;
    }

    ENCOUNT_encountnum=0;

    /*  引內  躲卅墊互窒墊丐月井升丹井譬屯月    */
    while( fgets( line, sizeof( line ), f ) ){
        linenum ++;
        if( line[0] == '#' )continue;        /* comment */
        if( line[0] == '\n' )continue;       /* none    */
        chomp( line );

        ENCOUNT_encountnum++;
    }

    if( fseek( f, 0, SEEK_SET ) == -1 ){
        fprint( "Seek Error\n" );
        fclose(f);
        return FALSE;
    }

    ENCOUNT_table = allocateMemory( sizeof(struct tagENCOUNT_Table)
                                   * ENCOUNT_encountnum );
    if( ENCOUNT_table == NULL ){
        fprint( "Can't allocate Memory %d\n" ,
                sizeof(ENCOUNT_table)*ENCOUNT_encountnum);
        fclose( f );
        return FALSE;
    }

    /* 賡渝祭 */
{
    int     i,j;
    for( i = 0; i < ENCOUNT_encountnum; i ++ ) {
        ENCOUNT_table[i].index = -1;
        ENCOUNT_table[i].floor = 0;
        ENCOUNT_table[i].encountprob_min = 1;
        ENCOUNT_table[i].encountprob_min = 50;
        ENCOUNT_table[i].enemymaxnum = 4;
        ENCOUNT_table[i].rect.x = 0;
        ENCOUNT_table[i].rect.y = 0;
        ENCOUNT_table[i].rect.height = 0;
        ENCOUNT_table[i].rect.width = 0;
        ENCOUNT_table[i].zorder = 0;
        for( j = 0; j < ENCOUNT_GROUPMAXNUM; j ++ ) {
            ENCOUNT_table[i].groupid[j] = -1;
            ENCOUNT_table[i].createprob[j] = -1;
        }
#ifdef _ADD_ENCOUNT           // WON ADD 增加敵遭遇觸發修件
		ENCOUNT_table[i].event_now = -1;
		ENCOUNT_table[i].event_end = -1;
		ENCOUNT_table[i].enemy_group = -1;
#endif
    }
}

    /*  引凶  心  允    */
    linenum = 0;
    while( fgets( line, sizeof( line ), f ) ){
        linenum ++;
        if( line[0] == '#' )continue;        /* comment */
        if( line[0] == '\n' )continue;       /* none    */
        chomp( line );

        /*  墊毛幫溥允月    */
        /*  引內 tab 毛 " " 卞  五晶尹月    */
        replaceString( line, '\t' , ' ' );
        /* 燮  及旦矢□旦毛潸月〔*/
{
        int     i;
        char    buf[256];
        for( i = 0; i < strlen( line); i ++) {
            if( line[i] != ' ' ) {
                break;
            }
            strcpy( buf, &line[i]);
        }
        if( i != 0 ) {
            strcpy( line, buf);
        }
}
{
        char    token[256];
        int     ret;
        int     x1,x2,y1,y2;
		int		j;
        
        /*   蘸戶及伙□皿卞  勻凶凜及啃及賡渝祭 */
        ENCOUNT_table[encount_readlen].index = -1;
        ENCOUNT_table[encount_readlen].floor = 0;
        ENCOUNT_table[encount_readlen].encountprob_min = 1;
        ENCOUNT_table[encount_readlen].encountprob_min = 50;
        ENCOUNT_table[encount_readlen].enemymaxnum = 4;
        ENCOUNT_table[encount_readlen].rect.x = 0;
        ENCOUNT_table[encount_readlen].rect.y = 0;
        ENCOUNT_table[encount_readlen].rect.height = 0;
        ENCOUNT_table[encount_readlen].rect.width = 0;
        ENCOUNT_table[encount_readlen].zorder = 0;
        for( j = 0; j < ENCOUNT_GROUPMAXNUM; j ++ ) {
            ENCOUNT_table[encount_readlen].groupid[j] = -1;
            ENCOUNT_table[encount_readlen].createprob[j] = -1;
        }
#ifdef _ADD_ENCOUNT           // WON ADD 增加敵遭遇觸發修件
		ENCOUNT_table[encount_readlen].event_now = -1;
		ENCOUNT_table[encount_readlen].event_end = -1;
		ENCOUNT_table[encount_readlen].enemy_group = -1;
#endif


        /*  夫午勾戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",1,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].index = atoi(token);
        
        /*  2勾戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",2,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].floor = atoi(token);

        /*  3勾戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",3,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        x1 = atoi(token);

        /*  4勾戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",4,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        y1= atoi(token);
        
        /*  5勾戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",5,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        
        x2 = atoi(token);
        
        /*  6勾戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",6,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        y2= atoi(token);

        ENCOUNT_table[encount_readlen].rect.x      = min(x1,x2);
        ENCOUNT_table[encount_readlen].rect.width  = max(x1,x2) - min(x1,x2);
        ENCOUNT_table[encount_readlen].rect.y      = min(y1,y2);
        ENCOUNT_table[encount_readlen].rect.height = max(y1,y2) - min(y1,y2);

        /*  7戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",7,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].encountprob_min = atoi(token);

        /*  8戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",8,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].encountprob_max = atoi(token);

{
		int		a,b;
		a = ENCOUNT_table[encount_readlen].encountprob_min;
		b = ENCOUNT_table[encount_readlen].encountprob_max;
		/*   凝及譬幫 */
        ENCOUNT_table[encount_readlen].encountprob_min 
        	= min( a,b);
        ENCOUNT_table[encount_readlen].encountprob_max 
        	= max( a,b);
}
        /*  9勾戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",9,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        {
            int maxnum = atoi( token);
            /* 醒及懇癲嶺及民尼永弁 */
            if( maxnum < 1 || maxnum > ENCOUNT_ENEMYMAXCREATENUM ) {
                fprint("createnum error file:%s line:%d\n",filename,linenum);
                continue;
            }
            ENCOUNT_table[encount_readlen].enemymaxnum = maxnum;
        }
        /*  10戶及玄□弁件毛葦月    */
        ret = getStringFromIndexWithDelim( line,",",10,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].zorder = atoi(token);
        #define		CREATEPROB_TOKEN	11
        
        /*  11  31戶及玄□弁件毛葦月    */
        {
            int     i;
            
            for( i = CREATEPROB_TOKEN; i < CREATEPROB_TOKEN +ENCOUNT_GROUPMAXNUM*2; i ++) {
                ret = getStringFromIndexWithDelim( line,",",i,token,
                                                   sizeof(token));
                if( ret==FALSE ){
                    fprint("Syntax Error file:%s line:%d\n",filename,linenum);
                    continue;
                }
                if( strlen( token) != 0 ) {
                    if( i < CREATEPROB_TOKEN + ENCOUNT_GROUPMAXNUM ) {
                        ENCOUNT_table[encount_readlen].groupid[i-CREATEPROB_TOKEN] 
                            = atoi(token);
                    }
                    else {
                        ENCOUNT_table[encount_readlen].createprob[i-(CREATEPROB_TOKEN + ENCOUNT_GROUPMAXNUM)] 
                            = atoi(token);
                    }
                }
            }

            /* 褐  民尼永弁 */
            if( checkRedundancy( ENCOUNT_table[encount_readlen].groupid, 
            			arraysizeof( ENCOUNT_table[encount_readlen].groupid)))
            {
            	fprint( "error:團隊重複file:%s line:%d\n", 
            				filename,linenum);
            	continue;
            }
        }


#ifdef _ADD_ENCOUNT           // WON ADD 增加敵遭遇觸發修件
        ret = getStringFromIndexWithDelim( line,",",31,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].event_now = atoi(token);

        ret = getStringFromIndexWithDelim( line,",",32,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].event_end = atoi(token);

        ret = getStringFromIndexWithDelim( line,",",33,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].enemy_group = atoi(token);
#endif

        encount_readlen ++;
}
    }
    fclose(f);

    ENCOUNT_encountnum = encount_readlen;

    print( "Valid encount field Num is %d..", ENCOUNT_encountnum );

#if 0

    {
        int i;
        for( i=0; i <ENCOUNT_encountnum ; i++ )
            print( "encount idx[%d] fl[%d] prob_min[%d] prob_max[%d] e_max[%d] x[%d] wth[%d] y[%d] hgt[%d] \n",
                   ENCOUNT_table[i].index,
                   ENCOUNT_table[i].floor,
                   ENCOUNT_table[i].encountprob_min,
                   ENCOUNT_table[i].encountprob_max,
                   ENCOUNT_table[i].enemymaxnum,
                   ENCOUNT_table[i].rect.x,
                   ENCOUNT_table[i].rect.width,
                   ENCOUNT_table[i].rect.y,
                   ENCOUNT_table[i].rect.height);
    }
#endif
    return TRUE;
}
/*------------------------------------------------------------------------
 * 巨件市它件玄澀爛白央奶伙  心  仄
 *-----------------------------------------------------------------------*/
BOOL ENCOUNT_reinitEncount( void )
{
	freeMemory( ENCOUNT_table);
	return( ENCOUNT_initEncount( getEncountfile()));
}

/*------------------------------------------------------------
 * 隙爛今木凶甄  及ENCOUNT_table及驕儂毛譬屯月〔
 * zorder及醒儂毛葦化穸燮賜匏及嫖中  毛潸  允月〔
 * 婁醒
 *  floor       int     白夫失ID
 *  x           int     x甄  
 *  y           int     y甄  
 * 忒曰襖
 *  懇橘      驕儂
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountAreaArray( int floor, int x, int y)
{
    int     i;
    int     index = -1;
    for( i=0 ; i<ENCOUNT_encountnum ; i++ ) {
        if( ENCOUNT_table[i].floor == floor ) {
            if( CoordinateInRect( &ENCOUNT_table[i].rect, x, y) ) {
                int curZorder = ENCOUNT_getZorderFromArray(i);
                if( curZorder >0) {
                    if( index != -1 ) {
                        /* 穸燮賜匏毛譬屯月 */
                        /*   五中  穸燮 */
                        if(  curZorder > ENCOUNT_getZorderFromArray(index)) {
                            index = i;
                        }
                    }
                    else {
                        index = i;
                    }
                }
            }
        }
    }
    return index;
}

/*------------------------------------------------------------
 * 隙爛今木凶甄  及巨件市它件玄割  毛譬屯月〔
 * 婁醒
 *  floor       int     白夫失ID
 *  x           int     x甄  
 *  y           int     y甄  
 * 忒曰襖
 *  懇橘      ㄟ動曉及割  
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountPercentMin( int charaindex, int floor , int x, int y )
{
    int ret;
    
    ret = ENCOUNT_getEncountAreaArray( floor, x, y);
    if( ret != -1 ) {
        ret = ENCOUNT_table[ret].encountprob_min;
		/* 玄目夫旦躲絆毛勾仃月 */
		if( CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_COUNT) > 0 ) {
			ret = ceil( ret * 
				((100 + CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_CUTRATE)) 
					/ 100.0));
		}
		if( ret < 0 ) ret = 0;
		if( ret > 100 ) ret = 100;
    }
    return ret;
}
/*------------------------------------------------------------
 * 隙爛今木凶甄  及巨件市它件玄割  毛譬屯月〔
 * 婁醒
 *  floor       int     白夫失ID
 *  x           int     x甄  
 *  y           int     y甄  
 * 忒曰襖
 *  懇橘      ㄟ動曉及割  
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountPercentMax( int charaindex, int floor , int x, int y )
{
    int ret;
    
    ret = ENCOUNT_getEncountAreaArray( floor, x, y);
    if( ret != -1 ) {
        ret = ENCOUNT_table[ret].encountprob_max;
		/* 玄目夫旦躲絆毛勾仃月 */
		if( CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_COUNT) > 0 ) {
			ret = ceil( ret * 
				((100 + CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_CUTRATE)) 
					/ 100.0));
		}
		if( ret < 0 ) ret = 0;
		if( ret > 100 ) ret = 100;
    }
    return ret;
}
/*------------------------------------------------------------
 * 隙爛今木凶甄  及襯戲岳MAX醒毛譬屯月〔
 * 婁醒
 *  floor       int     白夫失ID
 *  x           int     x甄  
 *  y           int     y甄  
 * 忒曰襖
 *  懇橘      ㄟ動曉及割  
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getCreateEnemyMaxNum( int floor , int x, int y )
{
    int ret;
    
    ret = ENCOUNT_getEncountAreaArray( floor, x, y);
    if( ret != -1 ) {
        ret = ENCOUNT_table[ret].enemymaxnum;
    }
    return ret;
}
/*------------------------------------------------------------
 * 隙爛今木凶甄  及巨件市它件玄白奴□伙玉及index毛譬屯月〔
 * 婁醒
 *  floor       int     白夫失ID
 *  x           int     x甄  
 *  y           int     y甄  
 * 忒曰襖
 *  懇橘      ㄟ動曉
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountIndex( int floor , int x, int y )
{
    int ret;
    
    ret = ENCOUNT_getEncountAreaArray( floor, x, y);
    if( ret != -1 ) {
        ret = ENCOUNT_table[ret].index;
    }
    return ret;
}
/*------------------------------------------------------------
 * 隙爛今木凶甄  及巨件市它件玄白奴□伙玉及index毛譬屯月〔
 * 婁醒
 *  array           int     ENCOUNTTABLE及驕儂
 * 忒曰襖
 *  懇橘      ㄟ動曉
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountIndexFromArray( int array )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].index;
}
/*------------------------------------------------------------
 * 隙爛今木凶甄  及巨件市它件玄割  毛譬屯月〔
 * 婁醒
 *  array           int     ENCOUNTTABLE及驕儂
 * 忒曰襖
 *  懇橘      ㄟ動曉
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountPercentFromArray( int array )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].encountprob_min;
}
/*------------------------------------------------------------
 * 隙爛今木凶甄  及襯戲岳MAX醒毛譬屯月〔
 * 婁醒
 *  array           int     ENCOUNTTABLE及驕儂
 * 忒曰襖
 *  懇橘      ㄟ動曉
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getCreateEnemyMaxNumFromArray( int array )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].enemymaxnum;
}
/*------------------------------------------------------------
 * 隙爛今木凶驕儂及弘伙□皿  寞毛譬屯月〔
 * 婁醒
 *  array           int     ENCOUNTTABLE及驕儂
 * 忒曰襖
 *  懇橘      ㄟ動曉
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getGroupIdFromArray( int array, int grouparray )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].groupid[grouparray];
}
/*------------------------------------------------------------
 * 隙爛今木凶驕儂及弘伙□皿及請蜇  毛譬屯月〔
 * 婁醒
 *  array           int     ENCOUNTTABLE及驕儂
 * 忒曰襖
 *  懇橘      ㄟ動曉
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getGroupProbFromArray( int array, int grouparray )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].createprob[grouparray];
}
/*------------------------------------------------------------
 * 隙爛今木凶驕儂及穸燮賜匏毛譬屯月〔
 * 婁醒
 *  array           int     ENCOUNTTABLE及驕儂
 * 忒曰襖
 *  懇橘      ㄟ動曉
 *  潸  撩    -1
 ------------------------------------------------------------*/
int ENCOUNT_getZorderFromArray( int array )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].zorder;
}
