#include "version.h"
#include "common.h"
#include "readmap.h"
#include "char.h"
#include "map_util.h"
#include "lssproto_serv.h"
#include "net.h"
#include "anim_tbl.h"

/*#define MAPEDITORTROUBLE*/
#define SPR_kmydamY CG_HIT_MARK_10 // anim_tbl.h ¤ËÅÐ  ¤µ¤ì¤ë¤Þ¤Ç  ¤Î¥¨¥Õ¥§¥¯¥È

/*------------------------------------------------------------
 * ¬¤¤±¤ë¤«¤É¤¦¤«  ÃÇ¤¹¤ë¡e¥­¥ã¥é¤¬  ¤ó¤Ç¤¤¤ë¤«¤É¤¦¤«°ú¿ô¤¬¤¢¤ë¡e
 * °ú¿ô
 *  ff          int     floor
 *  fx          int     xºÂ  
 *  fy          int     yºÂ  
 *  isfly       int       ¤ó¤Ç¤¤¤ë¤«¤É¤¦¤«
 * ÊÖ¤êÃÍ
 *  ¬¤¤±¤ë      TRUE(1)
 *  ¬¤¤±¤Ê¤¤    FALSE(0)
 ------------------------------------------------------------*/
BOOL MAP_walkAbleFromPoint( int ff, int fx, int fy, BOOL isfly )
{
    int map[2];

    if( !MAP_getTileAndObjData( ff,fx,fy, &map[0], &map[1] ) ){
        return FALSE;
	}

    if( isfly ){
        int i;
        for( i = 0 ; i < 2 ; i ++ )
            if( MAP_getImageInt( map[i], MAP_HAVEHEIGHT ) == TRUE  ){
                return FALSE;
			}
        return TRUE;
    }else{
        switch( MAP_getImageInt( map[1], MAP_WALKABLE ) ){
        case 0:
            return FALSE;
            break;
        case 1:
            if( MAP_getImageInt( map[0], MAP_WALKABLE ) == 1 ){
                return TRUE;
			}else{
                return FALSE;
			}
            break;
        case 2:
            return TRUE;
            break;
        default:
            return FALSE;
            break;
        }
    }

    return FALSE;
}



/*------------------------------------------------------------
 * ¬¤¤±¤ë¤«¤É¤¦¤«  ÃÇ¤¹¤ë¡e
 * °ú¿ô
 *  index       int     Char    ¤Ç¤Î Index
 *  ff          int     floor id
 *  fx          int     xºÂ  
 *  fy          int     yºÂ  
 * ÊÖ¤êÃÍ
 *  ¬¤¤¤¤Æ¤è¤¤      TRUE(1)
 *  ¬¤¤¤¤Æ¤Ï¥À¥á    FALSE(0)
 ------------------------------------------------------------*/
BOOL MAP_walkAble( int index,int ff, int fx, int fy)
{
    /*      ¥­¥ã¥é¤Ê¤éOK    */
    if( CHAR_getFlg( index, CHAR_ISTRANSPARENT ) )      return TRUE;

    // Arminius 7.9 Airplane ok
    if( CHAR_getInt( index, CHAR_WHICHTYPE ) == CHAR_TYPEBUS) return TRUE;
    if( CHAR_getWorkInt( index, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT ) {
      int oyaindex = CHAR_getWorkInt( index, CHAR_WORKPARTYINDEX1);
      if( CHAR_CHECKINDEX( oyaindex)) {
        if (CHAR_getInt( oyaindex, CHAR_WHICHTYPE ) == CHAR_TYPEBUS) return TRUE;
      }
    }

    return MAP_walkAbleFromPoint( ff,fx,fy,CHAR_getFlg(index,CHAR_ISFLYING)|CHAR_getWorkInt(index,CHAR_WORKSKYWALKER) );
}

/*------------------------------------------------------------
 * ºÂ  ¤ò»ØÄê¤·¤Æ¡b¤½¤³¤Î¥ª¥Ö¥¸¥§¥¯¥È¤Ë¹â¤µ¤¬¤¢¤ë¤«¤É¤¦¤«
 * °ú¿ô
 *  fl              int     ¥Õ¤Ò¥¢
 *  x               int     x ºÂ  
 *  y               int     y ºÂ  
 * ÊÖ¤êÃÍ
 *  ¹â¤µ¤¬¤¢¤ë  TRUE
 *  ¹â¤µ¤¬¤Ê¤¤  FALSE
 ------------------------------------------------------------*/
BOOL MAP_haveHeight( int fl, int x, int y )
{
    int     map[2];

    /*  ¥Þ¥Ã¥×¥Ç¡¼¥¿¤ò  ¤Æ¤¯¤ë  */
    if( !MAP_getTileAndObjData( fl,x,y, &map[0], &map[1] ) )
        return FALSE;

    return MAP_getImageInt( map[1], MAP_HAVEHEIGHT );
}



/*----------------------------------------
 * ¥¹  ¡¼¥¿¥¹  ²½·Ï¤Î  ¹¹¤ò¤¹¤ë
 * °ú¿ô
 *  index   int     ¥­¥ã¥é¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
 *  map     int     ¥Þ¥Ã¥×¥Ç¡¼¥¿
 *  outof   BOOL    ¿Ê¤à»þ¡bÂà¤¯»þ¤« TRUE ¤Ê¤é¤Ð ¿Ê¤à»þ¤Ç¤¢¤ë¡e
 * ÊÖ¤êÃÍ
 *  ¥Ñ¥é¥á¡¼¥¿  ²½¤ò¤·¤¿    TRUE
 *  ¥Ñ¥é¥á¡¼¥¿  ²½¤ò¤·¤Ê¤«¤Ã¤¿  FALSE
  ----------------------------------------*/
static BOOL MAP_changeCharStatusFromMapDataAndTime( int index,
                                                    int map, BOOL outof)
{
#if 0
// ¥¹¥È¡¼¥ó¥¨¥¤¥¸¤Ç¤Ï»È¤ï¤Ê¤¤
    int i;
    int offset;
    BOOL    change=FALSE;
    static struct tagStatusInteractionOfBitAndDefine{
        int     mapdataindex;
        int     charadataindex;
    }statusInteraction[]={
        {MAP_INTOPOISON,        CHAR_POISON},
        {MAP_INTOPARALYSIS,     CHAR_PARALYSIS},
        {MAP_INTOSILENCE,       CHAR_SLEEP},
        {MAP_INTOSTONE,         CHAR_STONE},
        {MAP_INTODARKNESS,      CHAR_DRUNK},
        {MAP_INTOCONFUSION,     CHAR_CONFUSION},

        {MAP_OUTOFPOISON,       CHAR_POISON},
        {MAP_OUTOFPARALYSIS,    CHAR_PARALYSIS},
        {MAP_OUTOFSILENCE,      CHAR_SLEEP},
        {MAP_OUTOFSTONE,        CHAR_STONE},
        {MAP_OUTOFDARKNESS,     CHAR_DRUNK},
        {MAP_OUTOFCONFUSION,    CHAR_CONFUSION},
    };

    if( outof == FALSE )offset = 6;
    else                offset= 0;
    int     newdata;
    for( i = 0 ; i < 6 ; i ++ ){
        newdata = MAP_getImageInt(map,statusInteraction[i+offset].
                                  mapdataindex);
        if( newdata > 0 ){
            change=TRUE;
            /*  ¥¹  ¡¼¥¿¥¹¤ÎÀßÄê    */
            CHAR_setInt( index,
                         statusInteraction[i+offset].charadataindex,
                         CHAR_getInt(index,
                                     statusInteraction[i+offset].
                                     charadataindex ) + newdata );
        }
    }
    return change;
#else
	return FALSE;
#endif
}


/*------------------------------------------------------------
 * Map ¥¤¥Ù¥ó¥È¤Î pre post¤ò½è  ¤¹¤ë
 * °ú¿ô
 *  index       int     ¾è¤í¤¦¤È¤·¤Æ¤¤¤ë¥­¥ã¥é¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
 *  mode        BOOL    TRUE¤Î»þ¤Ï¡bin  FALSE¤Î»þ¤Ï out ¤ËÂÐ±þ
 * ÊÖ¤êÃÍ¤Ê¤·
 ------------------------------------------------------------*/
static void MAP_dealprepostevent( int index, BOOL mode )
{
    int     map[2];
    int     i;
    int     damaged=FALSE,statuschange=FALSE;

    if( CHAR_getFlg(index,CHAR_ISFLYING) )
        /*    ¤ó¤Ç¤ë¤ä¤Ä¤Ë¤Ï²¿¤â¤·¤Ê¤¤  */
        return;

    if( !MAP_getMapDataFromCharIndex( index  ,  map ) ) return;
    /*  damage */
    int damage;
    for( i = 0 ; i < 2 ; i ++ ){
        damage = MAP_getImageInt(map[i],
                                 mode ? MAP_INTODAMAGE : MAP_OUTOFDAMAGE);
        if( damage != 0 ){
            /*  ¥À¥á¡¼¥¸¬¤¤òÀµ¤Ç  ¸½¤¹¤ë¤Î¤Ç  ¹æ  ¤ìÂØ¤¨    */
            int opt[2]={SPR_kmydamY,-damage};
            damaged=TRUE;
            CHAR_setInt(index, CHAR_HP,
                        CHAR_getInt(index,CHAR_HP) + damage );
            CHAR_complianceParameter(index);
            /*  ¥À¥á¡¼¥¸¼õ¤±¤¿¥¨¥Õ¥§¥¯¥È½Ð¤¹    */
            CHAR_sendWatchEvent(CHAR_getWorkInt(index,CHAR_WORKOBJINDEX),
                                CHAR_ACTDAMAGE,opt,2,TRUE);
        }
        if( MAP_changeCharStatusFromMapDataAndTime(
            index,map[i], mode ? TRUE : FALSE ) )
            statuschange=TRUE;
    }
    if( damaged )       CHAR_sendStatusString(index,"M");
    if( statuschange ){
        CHAR_sendCToArroundCharacter(index);
        CHAR_sendStatusString(index,"P");
    }

}


/*----------------------------------------
 * ¤½¤Î¥¿¥¤¥ë¤Ë¾è¤ëÁ°¤Ë¸Æ¤Ð¤ì¤ë¡e¤³¤³¤Ç¤½¤³¤«¤éÂà¤¯¤È¡bxxx·Ï¤Î
 * »ö¤ò  ¸½¤¹¤ë¡e
 * °ú¿ô
 *  index       int     ¾è¤í¤¦¤È¤·¤Æ¤¤¤ë¥­¥ã¥é¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
 *  flooor      int     ¥Õ¤Ò¥¢ID
 *  fx          int     xºÂ  
 *  fy          int     yºÂ  
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 ----------------------------------------*/
void MAP_preovered( int index )
{
    MAP_dealprepostevent( index, FALSE );
}

/*----------------------------------------
 * ¤½¤Î¥¿¥¤¥ë¤Ë¾è¤Ã¤¿  ¤Ë¸Æ¤Ð¤ì¤ë¡e
 * °ú¿ô
 *  index       int     ¾è¤í¤¦¤È¤·¤Æ¤¤¤ë¥­¥ã¥é¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 ----------------------------------------*/
void MAP_postovered( int index )
{
    MAP_dealprepostevent( index, TRUE );
}


/*----------------------------------------
 * ¥­¥ã¥é¤Î¼þ¤ê¤Î¥Þ¥Ã¥×¤ò¤¹¤Ù¤ÆÁ÷¤ë
 * °ú¿ô
 *  fd          int
 *  charaindex  int     ¥­¥ã¥é¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
 * ÊÖ¤êÃÍ
 *  ©¨      TRUE(1)
 *  ¼º      FALSE(0)
 ----------------------------------------*/
BOOL MAP_sendArroundCharNeedFD( int fd,int charaindex )
{
    char*   stringdata;
    int     x=CHAR_getInt(charaindex,CHAR_X);
    int     y=CHAR_getInt(charaindex,CHAR_Y);
    int     fl=CHAR_getInt(charaindex,CHAR_FLOOR);
    int     size=MAP_CHAR_DEFAULTSEESIZ;
    RECT    seekr,retr;
    seekr.x = x - (int)(size/2);
    seekr.y = y - (int)(size/2);
    seekr.width  = size;
    seekr.height = size;
#if 1
{
	int		tilesum, objsum, eventsum;
    stringdata = MAP_getChecksumFromRECT(fl,&seekr,&retr, &tilesum,&objsum,&eventsum);
    if( stringdata == NULL )
        return FALSE;

    lssproto_MC_send(fd,fl,
                    retr.x,              retr.y,
                    retr.x + retr.width, retr.y + retr.height,
                    tilesum,
                    objsum,
                    eventsum,
                    stringdata );
}
#else
    stringdata = MAP_getdataFromRECT(fl,&seekr,&retr);
    if( stringdata == NULL )
        return FALSE;

    lssproto_M_send(fd,fl,
                    retr.x,              retr.y,
                    retr.x + retr.width, retr.y + retr.height,
                    stringdata );
#endif
    return TRUE;
}


/*----------------------------------------
 * ¥Þ¥Ã¥×¥Ç¡¼¥¿¤ò  ¤ë¡e
 * °ú¿ô
 *  charaindex      int     ¥­¥ã¥é¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
 * ÊÖ¤êÃÍ
 *  ©¨      TRUE(1)
 *  ¼º      FALSE(0)
 ----------------------------------------*/
BOOL MAP_sendArroundChar(int charaindex)
{
    int fd;
    fd = getfdFromCharaIndex( charaindex );
    if( fd == -1 )return FALSE;

    return MAP_sendArroundCharNeedFD(fd, charaindex);
}
