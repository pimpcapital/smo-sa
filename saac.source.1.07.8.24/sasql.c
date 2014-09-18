#define _SASQL_C_

#include "version.h"

#ifdef _SASQL1//新添加

#include "main.h"
#include "util.h"
#include "mail.h"
#include "db.h"
#include "saacproto_util.h"
#include "saacproto_serv.h"
#ifdef _UNIVERSE_CHATROOM
#include "chatroom.h"
#endif
// CoolFish: Family 2001/5/9
#include "acfamily.h"

#ifdef _DEATH_CONTEND
#include "deathcontend.h"
#endif

#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/wait.h>
#include <getopt.h>
#include <stdio.h>
#include <malloc.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#include "saacproto_work.h"
#ifdef _OACSTRUCT_TCP
#include "saacproto_oac.h"
#endif
#ifdef _PAUCTION_MAN
#include "auction.h"
#endif
#include "lock.h"
#define BACKLOGNUM 5


#ifdef _FIX_WORKS
#include "saacproto_work.h"
int worksockfd;
#endif

#ifdef _LOCK_SERVER
#include "saacproto_lserver.h"
#endif

#ifdef _SEND_EFFECT                     // WON ADD 送下雪、下雨等特效
#include "recv.h"
#endif

#include "defend.h"

#include "char.h"

#include <mysql/mysql.h>

#define BOOL int
#define FALSE 0
#define TRUE 1
MYSQL mysql;
MYSQL_RES *mysql_result;
MYSQL_ROW mysql_row;

char SASQL_MYSQLSERVER[64];
char SASQL_SQLUSER[64];
char SASQL_SQLPASS[64];
char SASQL_DBNAME[64];
char SASQL_MYSQLPORT[64];
char SASQL_TABLENAME[64];
char SASQL_USER[64];
char SASQL_PASSWORD[64];

static int readConfig( char *path )
{
    char buf[2048];
    FILE *fp;

    fp = fopen( path , "r" );
    if( fp == NULL ){ return -2; }

    while( fgets( buf , sizeof( buf ) , fp )){
        char command[1024];
        char param[2048];
        chop(buf);
        
        easyGetTokenFromString( buf , 1 , command , sizeof( command ));
        easyGetTokenFromString( buf , 2 , param , sizeof( param ));

			if (strcmp(command,"mysqlserver") == 0){
				snprintf( SASQL_MYSQLSERVER, sizeof( SASQL_MYSQLSERVER ), param );
				log( "mysqlserver:%s\n" ,SASQL_MYSQLSERVER);
			}else if (strcmp(command,"sqluser") == 0){
				snprintf( SASQL_SQLUSER, sizeof( SASQL_SQLUSER ), param );
				log( "sqluser:%s\n" ,SASQL_SQLUSER);
			}else if (strcmp(command,"sqlpass") == 0){
				snprintf( SASQL_SQLPASS, sizeof( SASQL_SQLPASS ), param );
				log( "sqlpass:%s\n" ,SASQL_SQLPASS);
			}else if (strcmp(command,"databasename") == 0){
				snprintf( SASQL_DBNAME, sizeof( SASQL_DBNAME ), param );
				log( "databasename:%s\n" ,SASQL_DBNAME);
			}else if (strcmp(command,"mysqlport") == 0){
				snprintf( SASQL_MYSQLPORT, sizeof( SASQL_MYSQLPORT ), param );
				log( "mysqlport:%s\n" ,SASQL_MYSQLPORT);
			}else if (strcmp(command,"tablename") == 0){
				snprintf( SASQL_TABLENAME, sizeof( SASQL_TABLENAME ), param );
				log( "tablename:%s\n" ,SASQL_TABLENAME);
			}else if (strcmp(command,"account") == 0){
				snprintf( SASQL_USER, sizeof( SASQL_USER ), param );
				log( "account:%s\n" ,SASQL_USER);
			}else if (strcmp(command,"password") == 0){
				snprintf( SASQL_PASSWORD, sizeof( SASQL_PASSWORD ), param );
				log( "password:%s\n" ,SASQL_PASSWORD);
      }
		}

    fclose(fp);
    return 0;
}

int sasql_init( void )
{
  if( mysql_init(&mysql) == NULL )
  {
    log("\nmysql init err");
        return FALSE;
  }
  	if(readConfig( "saac.cf" )<0){
        log( "cannot read saac.cf in current directory.\n" );
        exit(1);
    }
    if( !mysql_real_connect( &mysql,
                  SASQL_MYSQLSERVER,
                  SASQL_SQLUSER,//帳號
                  SASQL_SQLPASS,//密碼
                  SASQL_DBNAME,//選擇的資料庫
                  atoi(SASQL_MYSQLPORT),
                  NULL,
                  0 ) )
    {
        log("\nmysql connect err\n");
        printf("\nmysql connect err\n");
        return FALSE;
  }
    printf("\nmysql connect ok\n");
    log("\nmysql connect ok\n");
    return TRUE;
}

void sasql_close( void )
{
    mysql_close( &mysql );
}

int sasql_query(char *nm, char *pas){
char sqlstr[256];
sprintf(sqlstr,"select * from %s where %s='%s' and %s='%s'",SASQL_TABLENAME,SASQL_USER,nm,SASQL_PASSWORD,pas);
if(!mysql_query(&mysql,sqlstr)){
    int num_row=0;
    mysql_result=mysql_store_result(&mysql);
    num_row=mysql_num_rows(mysql_result);
    mysql_free_result(mysql_result);
    if(num_row==1){
    	log("\nsqlquery num_row=%d\n",num_row);
    	return 0;
    }
    else{
    	#ifdef _ID_PASSW_AVOID
			return 0;
			#else
    	return 1;
    	#endif
    }
   }
else{
log("\n query error\n");
#ifdef _ID_PASSW_AVOID
return 0;
#else
return 1;
#endif
}
}
#endif
