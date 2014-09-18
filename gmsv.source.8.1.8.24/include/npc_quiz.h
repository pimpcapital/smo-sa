#ifndef __NPC_QUIZ_H__
#define __NPC_QUIZ_H__

typedef struct NPC_Quiz{

	int 	no;				//  鎖及  寞
	int 	type;			//  鎖及正奶皿  必□丞    ｝旦禾□汁)	
	int		level;  		//  鎖及伊矛伙
	int 	answertype;		//蠶尹及薊太井凶(ㄡ  ｝ㄢ  ｝簽賄穴永民  
	int 	answerNo;		//  鎖及蠶尹
	char	question[512]; 	//  鎖
	char	select1[128]; 	//薊  靼1
	char	select2[128]; 	//薊  靼2
	char	select3[128]; 	//薊  靼3

}NPC_QUIZ;


void NPC_QuizTalked( int meindex , int talkerindex , char *msg ,
                     int color );
BOOL NPC_QuizInit( int meindex );
void NPC_QuizWindowTalked( int meindex, int talkerindex, int seqno, int select, char *data);

BOOL QUIZ_initQuiz( char *filename);


#endif 

/*__NPC_QUIZ_H__*/
