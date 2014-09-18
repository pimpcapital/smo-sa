#ifndef __READNPCTEMPLATE_H__
#define __READNPCTEMPLATE_H__

#include "util.h"
#include "char_base.h"

#define NPC_TEMPLATEFILEMAGIC   "NPCTEMPLATE\n"

#undef EXTERN
#ifdef __NPCTEMPLATE__
#define EXTERN
#else
#define EXTERN extern
#endif /*__NPCTEMPLATE__*/



typedef struct tagNPC_haveItem
{
    int     itemnumber;
    int     haverate;
    int     havenum;
}NPC_haveItem;

typedef enum
{
    NPC_TEMPLATENAME,           /*    ンプレートの  前  */
    NPC_TEMPLATECHARNAME,       /*    前    */

    NPC_TEMPLATEINITFUNC,       /*  CHAR_INITFUNCに行く */
    NPC_TEMPLATEWALKPREFUNC,    /*  CHAR_WALKPREFUNC    */
    NPC_TEMPLATEWALKPOSTFUNC,   /*  CHAR_WALKPOSTFUNC   */
    NPC_TEMPLATEPREOVERFUNC,    /*  CHAR_PREOVERFUNC    */
    NPC_TEMPLATEPOSTOVERFUNC,   /*  CHAR_POSTOVERFUNC   */
    NPC_TEMPLATEWATCHFUNC,      /*  CHAR_WATCHFUNC      */
    NPC_TEMPLATELOOPFUNC,       /*  CHAR_LOOPFUNC   */
    NPC_TEMPLATEDYINGFUNC,      /*  CHAR_DYINGFUNC  */
    NPC_TEMPLATETALKEDFUNC,     /*  CHAR_TALKEDFUNC */

    NPC_TEMPLATEPREATTACKEDFUNC,        /*  CHAR_PREATTACKEDFUNC    */
    NPC_TEMPLATEPOSTATTACKEDFUNC,       /*  CHAR_POSTATTACKEDFUNC    */

    NPC_TEMPLATEOFFFUNC,                /*  CHAR_OFFFUNC    */
    NPC_TEMPLATELOOKEDFUNC,            /*  CHAR_LOOKEDFUNC  */
    NPC_TEMPLATEITEMPUTFUNC,            /*  CHAR_ITEMPUTFUNC    */

    NPC_TEMPLATESPECIALTALKEDFUNC,    /*  CHAR_SPECIALTALKEDFUNC   */
    NPC_TEMPLATEWINDOWTALKEDFUNC,    /*  CHAR_WINDOWTALKEDFUNC   */
#ifdef _USER_CHARLOOPS
	NPC_TEMPLATELOOPFUNCTEMP1,		//CHAR_LOOPFUNCTEMP1,
	NPC_TEMPLATELOOPFUNCTEMP2,		//CHAR_LOOPFUNCTEMP2,
	NPC_TEMPLATEBATTLEPROPERTY,		//CHAR_BATTLEPROPERTY,
#endif
    NPC_TEMPLATECHARNUM,
}NPC_TEMPLATECHAR;

typedef enum
{
    NPC_TEMPLATEMAKEATNOBODY,           /* 誰もいない時にも作るかどうか */
    NPC_TEMPLATEMAKEATNOSEE,            /* 見えない所で作るかどうか    */
    NPC_TEMPLATEIMAGENUMBER,            /*   像  号    */
    NPC_TEMPLATETYPE,                   /*     決める時に決める        */

    NPC_TEMPLATEMINHP,                  /* HP   */

    NPC_TEMPLATEMINMP,                  /* MP   */

    NPC_TEMPLATEMINSTR,                 /* STR  */

    NPC_TEMPLATEMINTOUGH,               /* TOUGH    */

    NPC_TEMPLATEISFLYING,               /*  んでるかどうか  */

    NPC_TEMPLATEITEMNUM,                /*   ちうるアイ  ムの    数    */

    NPC_TEMPLATELOOPFUNCTIME,           /*
                                         * 何ミリ  ごとにループ関数
                                         * を呼ぶか
                                         */
    NPC_TEMPLATEFUNCTIONINDEX,         /*
                                        * fucntionSet の何    の
                                        * インデックスか
                                        */

    NPC_TEMPLATEINTNUM,
}NPC_TEMPLATEINT;

typedef struct tagNPC_Template
{
    STRING64    chardata[NPC_TEMPLATECHARNUM];
    int         intdata[NPC_TEMPLATEINTNUM];
    int         randomdata[NPC_TEMPLATEINTNUM]; /*  ランダムの  きさ
                                                    が  っている  */
    int         hash;
    NPC_haveItem*   haveitem;
}NPC_Template;


EXTERN NPC_Template*   NPC_template;
EXTERN int             NPC_templatenum;
EXTERN int             NPC_template_readindex;

INLINE int NPC_CHECKTEMPLATEINDEX(int index);

BOOL NPC_copyFunctionSetToChar( int id, Char* ch );

BOOL NPC_readNPCTemplateFiles( char* topdirectory ,int templatesize);
int NPC_templateGetTemplateIndex( char* templatename );

#endif
 /*__READNPCTEMPLATE_H__*/
