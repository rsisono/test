#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 64
//設問2の連続タイムアウト回数
#define N 1
int main(void){
    FILE *fp; // FILE型構造体
	char fname[] = "test.txt",str[NUM],str1[4],str2[2];
    char *ymd,*server,*ping,*timeout;
    //int Y,M,D,h,m,s;
    int miss = 0;
    
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
        /*
        sprintf(str1, "%c%c%c%c", str[0], str[1],str[2],str[3]);
        Y = atoi(str1);
        
        sprintf(str1, "%c%c", str[4], str[5]);
        M = atoi(str1);

        sprintf(str1, "%c%c", str[6], str[7]);
        D = atoi(str1);

        sprintf(str1, "%c%c", str[8], str[9]);
        h = atoi(str1);

        sprintf(str1, "%c%c", str[10], str[11]);
        m = atoi(str1);

        sprintf(str1, "%c%c", str[12], str[13]);
        s = atoi(str1);
    printf("miss%d\n",miss);
    */
    }
    //N回以上タイムアウトしたのちにpingの応答があった場合
    else if (miss >= N){
        printf("time out\n%s-%s",timeout,ymd);
    }
    //N回以下でpingが返ってきた場合
    else miss = 0;
    
    }
    //fileを閉じる
	fclose(fp); 
    return 0;
}
