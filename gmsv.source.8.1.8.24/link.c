#include <stdlib.h>
#include "version.h"
#include "link.h"
#include "buf.h"

/*
 *  ¥ê¥ó¥¯¤Î  ¤Ë¤Ä¤±¤ë
 * °ú¿ô
 *      top         ¥È¥Ã¥×¥Î¡¼¥É
 *      add         ÉÕ¤±²Ã¤¨¤ë¥Î¡¼¥É¤Î    ¤òÊÝ  ¤¹¤ë¥Î¡¼¥É¤Ø¤Î¥Ý¥¤¥ó¥¿¡¼
 * ÊÖ¤êÃÍ
 *      TRUE(1)     ©¨  
 *      FALSE(0)    ¼º  
 */
BOOL Nodeappendtail( Node** top  , Node* add )
{
    Node* c;        /*¥ë¡¼¥×ÍÑ*/
    Node* next;     /*¿·¤·¤¯ºî¤ë¥Î¡¼¥ÉÍÑ*/

    /*¥È¥Ã¥×¤¬NULL¤«¤É¤¦¤«¤Î¥Á¥§¥Ã¥¯*/
    if( *top == NULL ){
        *top = allocateMemory( sizeof( Node  ) );
        if( *top== NULL ) return FALSE;
        (*top)->next    = NULL;            /*Â³¤­¤Ï  ¤¤*/
        (*top)->size    = add->size;   /*Ä¹¤µ¤ÎÊÝ´É*/
        (*top)->val     = add->val;         /*¥Ý¥¤¥ó¥¿¡¼¤Î¥³¥Ô¡¼*/
        return TRUE;
    }

    for( c = *top ; c->next ; c = c->next ); /* c¤Î°Ì  ¤ò¤º¤é¤¹ */
    next = allocateMemory( sizeof(Node) );
    if( next == NULL )return FALSE;
    c->next = next;                         /* next¤ËÃÍ¤òÀßÄê¤¹¤ë */
    next->next  = NULL;                      /*Â³¤­¤Ï  ¤¤*/
    next->val   = add->val;                   /*¥Ý¥¤¥ó¥¿¡¼¤Î¥³¥Ô¡¼*/
    next->size  = add->size;             /*Ä¹¤µ¤ÎÊÝ´É*/
    return TRUE;
}




/*
 *  ¥ê¥ó¥¯¤ÎÁ°¤Ë¤Ä¤±¤ë
 * °ú¿ô
 *      top         ¥È¥Ã¥×¥Î¡¼¥É
 *      add         ÉÕ¤±²Ã¤¨¤ë¥Î¡¼¥É¤Î    ¤òÊÝ  ¤¹¤ë¥Î¡¼¥É¤Ø¤Î¥Ý¥¤¥ó¥¿¡¼
 * ÊÖ¤êÃÍ
 *      TRUE(1)     ©¨  
 *      FALSE(0)    ¼º  
 */
BOOL Nodeappendhead( Node** nowtop  , Node* add )
{
    Node* newtop;       /*¿·¤·¤¤Àè  ÍÑ*/

    /*¥È¥Ã¥×¤¬NULL¤«¤É¤¦¤«¤Î¥Á¥§¥Ã¥¯*/
    if( *nowtop == NULL ){
        *nowtop = allocateMemory( sizeof( Node  ) );
        if( *nowtop == NULL ) return FALSE;
        (*nowtop)->next = NULL;             /*Â³¤­¤Ï  ¤¤*/
        (*nowtop)->size = add->size;        /*Ä¹¤µ¤ÎÊÝ´É*/
        (*nowtop)->val  = add->val;         /*¥Ý¥¤¥ó¥¿¡¼¤Î¥³¥Ô¡¼*/
        return TRUE;
    }

    /*
     * ¿·¤·¤¤¥Î¡¼¥É¤ò°ì¤Äºî¤ë
     * next ¤¬º¹¤¹¤â¤Î¤ò nowtop ¤Ë¤¹¤ë¡e
     * nowtop ¤Ë¤Ï ³ÎÊÝ¤·¤¿ newtop ¤òÂå  ¤¹¤ë¡e
     */
    newtop = allocateMemory( sizeof(Node) );
    newtop->next    = *nowtop;
    newtop->val     = add->val;
    newtop->size    = add->size;
    *nowtop = newtop;
    return TRUE;
}




/*
 * °ì    ½é¤Î¥Î¡¼¥É¤ò°ú¿ô¤Ë¥³¥Ô¡¼¤·¤Æ¼è¤ê½ü¤¯
 * °ú¿ô
 *          top         ¥Î¡¼¥É¤ÎÀè  
 *          ret         ¼è¤ê½ü¤¤¤¿¥Î¡¼¥É¤Î¥³¥Ô¡¼
 * ÊÖ¤êÃÍ
 *      TRUE(1)         ©¨  
 *      FALSE(0)        ¼º      ¥Î¡¼¥É¤ÎÀè  ¤¬NULL
 */
BOOL  Noderemovehead( Node** top , Node* ret)
{
    Node* newtop;         /*¿·¤·¤¯Àè  ¤Ë¤Ê¤ë¥Î¡¼¥É*/

    if( *top == NULL )return FALSE;

    ret->val    = (*top)->val;
    ret->size   = (*top)->size;
    newtop = (*top)->next;
    freeMemory( *top );
    *top = newtop;

    return TRUE;
}





/*
 * °ì      ¤Î¥Î¡¼¥É¤ò°ú¿ô¤Ë¥³¥Ô¡¼¤·¤Æ¼è¤ê½ü¤¯
 * °ú¿ô
 *          top         ¥Î¡¼¥É¤ÎÀè  
 *          ret         ¼è¤ê½ü¤¤¤¿¥Î¡¼¥É¤Î¥³¥Ô¡¼
 * ÊÖ¤êÃÍ
 *      TRUE(1)         ©¨  
 *      FALSE(0)        ¼º      ¥Î¡¼¥É¤ÎÀè  ¤¬NULL
 */
BOOL  Noderemovetail( Node** top , Node* ret)
{
    Node* c;             /*¥ë¡¼¥×ÍÑ*/
    Node* c1;            /*¥ë¡¼¥×ÍÑ ¤¤¤Ä¤Ç¤â c->next¤ò»Ø¤¹*/

    if( *top == NULL )return FALSE;

    c = *top;           /*½é´üÃÍÀßÄê*/
    c1 = c->next;       /*½é´üÃÍÀßÄê*/
    while(1){
        if( c1->next == NULL )
            /*
             * ¤³¤Î»þÅÀ¤Ç          c1
             *                      |
             *  c ---> +------+       
             *         | next |---->+------+
             *         |------|     | next |---->NULL
             *         |      |     +------+
             *         +------+     |      |
             *                      +------+
             *¤È¤Ê¤Ã¤Æ¤¤¤ë
             */
            break;
        c=c->next;
        c1=c->next;
    }
    c->next = NULL;     /*c1¤Ë¤¢¤¿¤ë¤â¤Î¤òºï½ü¤¹¤ë¤Î¤Çnext¤ÏNULL¤È¤¹¤ë*/
    /*¥³¥Ô¡¼*/
    ret->val    = c1->val;
    ret->size   = c1->size;

    freeMemory( c1 );

    return TRUE;
}
