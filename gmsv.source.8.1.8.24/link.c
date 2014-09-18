#include <stdlib.h>
#include "version.h"
#include "link.h"
#include "buf.h"

/*
 *  伉件弁及  卞勾仃月
 * 婁醒
 *      top         玄永皿用□玉
 *      add         尥仃笛尹月用□玉及    毛忡  允月用□玉尺及禾奶件正□
 * 忒曰襖
 *      TRUE(1)     岳  
 *      FALSE(0)    撩  
 */
BOOL Nodeappendtail( Node** top  , Node* add )
{
    Node* c;        /*伙□皿迕*/
    Node* next;     /*蕙仄仁綜月用□玉迕*/

    /*玄永皿互NULL井升丹井及民尼永弁*/
    if( *top == NULL ){
        *top = allocateMemory( sizeof( Node  ) );
        if( *top== NULL ) return FALSE;
        (*top)->next    = NULL;            /*糧五反  中*/
        (*top)->size    = add->size;   /*贏今及忡棟*/
        (*top)->val     = add->val;         /*禾奶件正□及戊疋□*/
        return TRUE;
    }

    for( c = *top ; c->next ; c = c->next ); /* c及匏  毛內日允 */
    next = allocateMemory( sizeof(Node) );
    if( next == NULL )return FALSE;
    c->next = next;                         /* next卞襖毛澀爛允月 */
    next->next  = NULL;                      /*糧五反  中*/
    next->val   = add->val;                   /*禾奶件正□及戊疋□*/
    next->size  = add->size;             /*贏今及忡棟*/
    return TRUE;
}




/*
 *  伉件弁及蟆卞勾仃月
 * 婁醒
 *      top         玄永皿用□玉
 *      add         尥仃笛尹月用□玉及    毛忡  允月用□玉尺及禾奶件正□
 * 忒曰襖
 *      TRUE(1)     岳  
 *      FALSE(0)    撩  
 */
BOOL Nodeappendhead( Node** nowtop  , Node* add )
{
    Node* newtop;       /*蕙仄中燮  迕*/

    /*玄永皿互NULL井升丹井及民尼永弁*/
    if( *nowtop == NULL ){
        *nowtop = allocateMemory( sizeof( Node  ) );
        if( *nowtop == NULL ) return FALSE;
        (*nowtop)->next = NULL;             /*糧五反  中*/
        (*nowtop)->size = add->size;        /*贏今及忡棟*/
        (*nowtop)->val  = add->val;         /*禾奶件正□及戊疋□*/
        return TRUE;
    }

    /*
     * 蕙仄中用□玉毛域勾綜月
     * next 互犒允手及毛 nowtop 卞允月〔
     * nowtop 卞反 割忡仄凶 newtop 毛醫  允月〔
     */
    newtop = allocateMemory( sizeof(Node) );
    newtop->next    = *nowtop;
    newtop->val     = add->val;
    newtop->size    = add->size;
    *nowtop = newtop;
    return TRUE;
}




/*
 * 域    賡及用□玉毛婁醒卞戊疋□仄化潸曰輪仁
 * 婁醒
 *          top         用□玉及燮  
 *          ret         潸曰輪中凶用□玉及戊疋□
 * 忒曰襖
 *      TRUE(1)         岳  
 *      FALSE(0)        撩      用□玉及燮  互NULL
 */
BOOL  Noderemovehead( Node** top , Node* ret)
{
    Node* newtop;         /*蕙仄仁燮  卞卅月用□玉*/

    if( *top == NULL )return FALSE;

    ret->val    = (*top)->val;
    ret->size   = (*top)->size;
    newtop = (*top)->next;
    freeMemory( *top );
    *top = newtop;

    return TRUE;
}





/*
 * 域      及用□玉毛婁醒卞戊疋□仄化潸曰輪仁
 * 婁醒
 *          top         用□玉及燮  
 *          ret         潸曰輪中凶用□玉及戊疋□
 * 忒曰襖
 *      TRUE(1)         岳  
 *      FALSE(0)        撩      用□玉及燮  互NULL
 */
BOOL  Noderemovetail( Node** top , Node* ret)
{
    Node* c;             /*伙□皿迕*/
    Node* c1;            /*伙□皿迕 中勾匹手 c->next毛隙允*/

    if( *top == NULL )return FALSE;

    c = *top;           /*賡渝襖澀爛*/
    c1 = c->next;       /*賡渝襖澀爛*/
    while(1){
        if( c1->next == NULL )
            /*
             * 仇及凜鰾匹          c1
             *                      |
             *  c ---> +------+       
             *         | next |---->+------+
             *         |------|     | next |---->NULL
             *         |      |     +------+
             *         +------+     |      |
             *                      +------+
             *午卅勻化中月
             */
            break;
        c=c->next;
        c1=c->next;
    }
    c->next = NULL;     /*c1卞丐凶月手及毛綽輪允月及匹next反NULL午允月*/
    /*戊疋□*/
    ret->val    = c1->val;
    ret->size   = c1->size;

    freeMemory( c1 );

    return TRUE;
}
