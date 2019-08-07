/*
  gl.c
  ターミナル上でブラウザを起動して検索するくそプログラム
  コマンドを実行しているだけです。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
    if (argc == 1) //引数がないときの処理
    {       
        printf("usage: gl  <option> <search words>\n");
        printf("option:\n -c use Google Chrome(default)\n");
        printf(" -s use Safari\n");
        
    }
    else
    {
        int sum=0; //検索する文字数をカウントする変数
        int i=0; //カウンタ変数
        char url[1500]={}; //最終的に実行するコマンド
        char appname[50]={}; //アプリケーションの名前を入れる配列
        char filepath[] = "cd /Applications/"; //ファイルパス。人によって変えてください
        char ourl[] = "https://www.google.co.jp/search?q="; //サーチするためのURL
        char sword[1000]={}; //検索する文字をいれる配列
        
        //オプションが入力されたときの処理
        if(argv[1][0]=='-')
        {
            if(argv[1][1]=='s')
            {
                strcpy(appname, "-a Safari.app");
                i=2;
            }
            else  if(argv[1][1]=='c')
            {
                strcpy(appname, "-a Google' 'Chrome.app");
                i=2;
            }
            else
            {
                printf("Invalid argument\n");
                printf("option:\n -c use Google Chrome(default)\n");
                printf(" -s use Safari\n");
                return -1;
            }
        }
        else //オプションがなかったら
        {
            strcpy(appname, "-a Google' 'Chrome.app");
            i=1;
        }
        
       
        //引数が2個以上の場合はスペースとして処理する
        for(;i<=argc-1;i++)
        {
            sum+=strlen(argv[i]);
            if (sum > 1000)
            {
                printf("error: too many words\n");
                printf("Please input less than 1000 words");
                return -1;
            }
            
            if(i==argc-1)
            {
                strcat(sword,argv[argc-1]);
            }
            else
            {
                strcat(sword,argv[i]);
                strcat(sword," ");
            }
        }
        
        //全ての文字列を組み合わせる
        sprintf(url, "%s\nopen  \"%s%s\" %s", filepath,ourl,sword, appname);
        system(url);
        
    }
    return 0;
}
