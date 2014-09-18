#ifndef __NPC_QUIZ_H__
#define __NPC_QUIZ_H__

typedef struct NPC_Quiz{

	int 	no;				//  玛の  规
	int 	type;			//  玛のタイプ  ゲ〖ム    スポ〖ツ)	
	int		level;  		//  玛のレベル
	int 	answertype;		//批えの联びかた(２  ３  帽胳マッチ  
	int 	answerNo;		//  玛の批え
	char	question[512]; 	//  玛
	char	select1[128]; 	//联  晦1
	char	select2[128]; 	//联  晦2
	char	select3[128]; 	//联  晦3

}NPC_QUIZ;


void NPC_QuizTalked( int meindex , int talkerindex , char *msg ,
                     int color );
BOOL NPC_QuizInit( int meindex );
void NPC_QuizWindowTalked( int meindex, int talkerindex, int seqno, int select, char *data);

BOOL QUIZ_initQuiz( char *filename);


#endif 

/*__NPC_QUIZ_H__*/
