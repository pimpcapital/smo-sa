#include "version.h"
#include "char.h"

/*


  井凶曰屯〔

  伉伉□旦  匹反2潘  及仇午分引〔

 NPCARGUMENT:  0分勻凶日仇午分引0｝1分勻凶日仇午分引1及賄曰  〔  

 橇謫反3勾丐曰｝賜卞褡墊允月〔

 橇謫0「 失奶玉伉件弘〔仇及橇謫匹talk今木月午｝
 ＞方仁五凶｝幻僕幻僕方〔中井卞手｝歹仄互鰻  及井凶曰屯元扎〔＝匹
 橇謫1卞啖墊〔
 橇謫1「talk今木凶日｝
 ＞公卅凶互及冗戈卅日壬｝熔午夫五井尹卞｝雄  卅蛻暱毛夫午勾｝ 今內仃方丹〔
        公及蛻暱毛及冗戈井? yes/no＝yes匹橇謫2尺〔no匹
        ＞匹反｝今日壬元扎〔＝匹橇謫0尺
        
 橇謫2「 talk今木凶日｝
 ＞公卅凶卞鰻  及蛻暱＞幻僕幻僕＝毛鰻噩仄凶〔  及勾亢卞  濠卅手及午仄化
        域戲  濠卞允月屯仄〔今日壬元扎〔＝
        午中勻化熔毛蛹日仄｝蛻暱白仿弘毛馨笛允月〔
        匹橇謫0卞手升月〔



 
 Talked:

 if(     卞反卅仄化井日1坌凶勻化中月井〞){
   橇謫0卞手升月〔
 }
   
 switch(橇謫){
 case 0:  質  〔橇謫1尺〔break;
 case 1:  質  〔橇謫2尺〔break;
 case 2:  質  〔橇謫0尺〔break;
 } 

 }

 
 
 

 



 */
BOOL NPC_StoryTellerInit( int meindex )
{
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPESTORYTELLER );
    CHAR_setFlg( meindex , CHAR_ISATTACKED , 0 );
    CHAR_setFlg( meindex , CHAR_ISOVERED , 0 );

    return TRUE;
}


void NPC_StoryTellerTalked( int meindex , int talker , char *msg , int col )
{
    
}
