#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 64
//設問2の連続タイムアウト回数
#define N 1
//設問3の直近m回の平均応答時間tミリ秒
#define t 500
#define m 2
int main(void){
    FILE *fp; // FILE型構造体
	char fname[] = "test.txt",str[NUM];
    char *ymd,*server,*ping,*timeout;
    int miss = 0,T,M;
    
    // ファイルを開く。失敗するとNULLを返す。
	fp = fopen(fname, "r"); 
	if(fp == NULL) {
		printf("%s file not open\n", fname);
		return -1;
	} else {
		printf("%s file opened\n", fname);
	}
    //一行ずつ読み込み
    while(fgets(str, NUM, fp) != NULL) {
	
    //確認日時、サーバーアドレス、応答結果に分割
    ymd = strtok(str, ",");
    //printf("%s\n", ymd);
    server = strtok(NULL, ",");
    //printf("%s\n", server);
    ping = strtok(NULL, ",");

    //pingがタイムアウトした場合
    if(strncmp("-",&ping[0],1) == 0 ){
        //最初にタイムアウトした時間を変数timeoutに代入
        if(miss == 0)strcpy(timeout,ymd);
        miss += 1;
    }
    else{
        //N回以上タイムアウトしたのちにpingの応答があった場合
        if (miss >= N){
        printf("time out\n%s-%s\n",timeout,ymd);
        }
        //N回以下でpingが返ってきた場合
        else miss = 0;
        T = atoi(ping);
        if(t < T){
        printf("%d\n",T);
        }
    }
    }
    //fileを閉じる
	fclose(fp); 
    return 0;
}