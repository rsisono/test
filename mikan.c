#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 64
//設問2の連続タイムアウト回数
#define N 2
//設問3の直近m回の平均応答時間tミリ秒
#define t 500
#define m 2
int main(void){
    FILE *fp; // FILE型構造体
    //fnameはログファイルの読み込み変数、str[]はファイルの一行を読み込む変数、
    //server_S[]は出てきたサーバーアドレスを記憶する変数
	char fname[] = "test.txt",str[NUM];
    char *ymd,*server,*ping,timeout[14],*server_S[NUM],*subnet[5];
    int miss[NUM],T = m * t,M[m][NUM],i,j,flag[NUM],os = 0,s = 0;
    int subnet_N[5],subnet_C[NUM],sub[NUM];
    //配列の初期化
    for(i = 0;i < NUM;i ++){
        miss[i] = 0;
        flag[i] = 0;
        for(j = 0;j < m;j ++)M[j][i] = 0;
    }
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
        server = strtok(NULL, ",");
        ping = strtok(NULL, ",");
        
        //サーバーの数だけ繰り返し、sは過去にログに出てきたサーバーの数
        for(i = 0;i <= s;i ++){
            //過去にサーバーのログが出てきていた場合
            if(strcmp(server_S[i],server) == 0){
                //タイムアウトした場合
                if('-' == ping[0]){
                    if(miss[i] == 0){
                        strcpy(timeout,ymd);
                    }
                    miss[i] += 1;
                    if(subnet_C[i] >= N){
                        printf("server error\n");
                        subnet[0] = strtok(server, ".");
                        subnet[1] = strtok(NULL, ".");
                        subnet[2] = strtok(NULL, ".");
                        subnet[3] = strtok(NULL, "/");
                        subnet[4] = strtok(NULL, ".");
                        subnet_N[0] = atoi(subnet[0]);
                        subnet_N[1] = atoi(subnet[1]);
                        subnet_N[2] = atoi(subnet[2]);
                        subnet_N[3] = atoi(subnet[3]);
                        subnet_N[4] = atoi(subnet[4]);
                        if(subnet_N[4] == 24){
                            printf("%d.%d.%d\n",subnet_N[0],subnet_N[1],subnet_N[2]);
                        }else if(subnet_N[4] == 16){
                            printf("%d.%d\n",subnet_N[0],subnet_N[1]);
                        }else if(subnet_N[4] == 8){
                            printf("%d\n",subnet_N[0]);
                        }
                    }
                    break;
                }
                //前にタイムアウトしており、今回はpingを返した場合
                else if(miss[i] >= N){
                    printf("time out\n");
                    printf("server adress %s\n",server);
                    for(j = 0;j < 14;j ++){
                        printf("%c",timeout[j]);
                    }
                    printf("-%s\n",ymd);
                }
                miss[i] = 0;
                //過負荷状態の計算
                //m回の平均値がtを超えていないかの計算
                T = atoi(ping);
                if(flag[i] == m){
                    flag[i] = 0;
                    M[flag[i]][i] = T;
                }
                else{
                    M[flag[i]][i] = T;
                }
                flag[i] += 1;
                T = m*t;
                for(j = 0;j < m;j ++){
                    T = T - M[j][i];
                }
                //過負荷状態のはじまり
                if(T < 0){
                    if(os == 0){
                        os += 1;
                        printf("overload start\n");
                        printf("server adress %s\n",server);
                        printf("%s\n",ymd);
                    }
                }
                //過負荷状態が終わったとき
                else{
                    if(os != 0){
                        printf("overload end\n");
                        printf("server adress %s\n",server);
                        printf("%s\n",ymd);
                        os = 0;
                    }
                }
                break;
            }
            //サーバーが初めて出てきたとき
            else if(i == s){

                //pingがタイムアウトした場合
                if('-' == ping[0]){
                //最初にタイムアウトした時間を変数timeoutに代入
                    if(miss[i] == 0){
                        strcpy_s(server_S[i],15,server);
                        strcpy(timeout,ymd);
                        if( s != 0 )s++;
                    }
                    miss[i] += 1;
                }
                else{
                //過負荷状態の計算,初めてサーバーがログに出てきたとき
                T = atoi(ping);
                M[0][i] = T;
                flag[i] = 1;
                strcpy(server_S[i],server);
                strcpy(timeout,ymd);
                if( s != 0 )s++;
                }
                                //サブネットマスクの分類
                subnet[0] = strtok(server, ".");
                subnet[1] = strtok(NULL, ".");
                subnet[2] = strtok(NULL, ".");
                subnet[3] = strtok(NULL, "/");
                subnet[4] = strtok(NULL, ".");
                subnet_N[0] = atoi(subnet[0]);
                subnet_N[1] = atoi(subnet[1]);
                subnet_N[2] = atoi(subnet[2]);
                subnet_N[3] = atoi(subnet[3]);
                subnet_N[4] = atoi(subnet[4]);
                if(subnet_N[4] == 24){
                    subnet_C[i] = subnet_N[3];
                }else if(subnet_N[4] == 16){
                    subnet_C[i] = subnet_N[3] + subnet_N[2] * 256;
                }else if(subnet_N[4] == 8){
                    subnet_C[i] = subnet_N[3] + subnet_N[2] * 256 + subnet_N[1] * 256 * 256;
                }
                
                break;
            }  
        }


        
    }
    //fileを閉じる
	fclose(fp); 
    return 0;
}