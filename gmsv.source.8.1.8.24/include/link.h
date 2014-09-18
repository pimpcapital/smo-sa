#ifndef __LINK_H__
#define __LINK_H__

#include "common.h"

/*
 * 伉旦玄厭瞻毛爛聒允月  〔
 * 仇及伉旦玄反val反    讀卞反窒手仄卅中〔勾引曰禾奶件正□及戊疋□及心
 * 墊丹〔
 */
typedef struct tagNode
{
    struct tagNode* next;           /*戚及用□玉尺及禾奶件正□*/
    char* val;                      /*忡  允月  儂  */
    int size;                       /*val及扔奶術*/
}Node;

BOOL Nodeappendhead( Node** top  , Node* add );
BOOL Nodeappendtail( Node** top  , Node* add );
BOOL Noderemovehead( Node** top , Node* ret);
BOOL Noderemovetail( Node** top , Node* ret);
#endif
