/******
       shooting.c
       時止める系シューティング
       953300 岡山 紘大
       2019.7.11
 *****/


#include<stdio.h>
#include<handy.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>


#define WINDOWX 800  //縦サイズ
#define WINDOWY 800  //横サイズ
#define PI 3.14159265 //円周率

typedef struct{  //弾丸のステータス
    double x;//弾丸のX座標
    double y; //弾丸のY座標
    double r; //弾丸の半径
    double xspeed; //弾丸のXベクトル
    double yspeed; //弾丸のYベクトル
    int status; //敵の弾丸か自分の弾丸か
    int stoped;  //時が止まっている時に発射されたかを判定
    hgcolor color; //弾丸の色
   
}bullet;

typedef struct{  //自分のステータス
    double x; //自分のX座標
    double y; // 自分のY座標
    double r; //自分の半径
    double xspeed; //自分のXスピード
    double yspeed; //自分のYスピード
    int firerate; //自分が弾丸を発射する時のレート
    int HP; //残機
    int damage; //自分が敵の弾丸に当たったか 1なら当たった
    int dtimecount; //敵の弾丸に当たった時の無敵時間のカウント
    long score; //スコア
}myself;

typedef struct{  //ボスのステータス
    double x; //ボスのX座標
    double y; //ボスのY座標
    double r; //ボスの半径
    double HP; //ボスの体力
    double plusR; //ボスの攻撃角度のための変数
    int phase; //ボスの状態を表すための変数
    int timecount; //ボスの攻撃の間隔のための変数
    int plusRswitch; //ボスの攻撃角度のための変数
    
}boss;

//////
myself my;
bullet bl[100000];
boss boss1;
/////



int bulletstatus[100000]={0}; //弾丸の番号を入れる配列
int bulletcheck(); //プロトタイプ宣言
int boss1action(double *Br); //プロトタイプ宣言
int loopvalue=1000; //画面上に表示できる弾丸の最大数
int stoptime=0; //時間が止まっているかの判定用 1なら止まっている



int main(void){
    srand(time(NULL)); //シードを変更
    
////HandyGraphics用変数
    int lay=0,lay2=0; //レイヤ
    int key=0; //取得したキーコード
    hgevent *event; //イベント
    doubleLayer layer; //ダブルレイヤー
    
////爆発描写用変数
    int Ecount=0; //爆発のアニメーションの時間をカウントする変数
    int size=10; //爆発のサイズ
    int randomx=0; //爆発の中心X
    int randomy=0; //爆発の中心Y
    
////自機用変数
    double xspeed=0; //自分のXベクトル
    double yspeed=0; //自分のYベクトル
    double stoptimecount=2.5; //右下の円弧のラジアン
    int firemode=0; //スペースを押しているかの判定
    int firetimecount=0; //発射間隔を計測する変数
    int stoptimekeydown=0; //Mキーが押されているか

////ボス用変数
    double Bkakudo=0; //ボスの攻撃角度のための変数
    
/////ステータス初期化
    my.x=300;
    my.y=300;
    my.r=5;
    my.xspeed=8;
    my.yspeed=8;
    my.firerate=2;
    my.HP=4;
    my.damage=0;
    my.dtimecount=0;
    my.score=0;

    boss1.x=250;
    boss1.y=600;
    boss1.r=20;
    boss1.HP=50000;
    boss1.timecount=0;
    boss1.phase=1;
    boss1.plusR=0;
    boss1.plusRswitch=1;
////
    
    HgOpen(WINDOWX,WINDOWY);
    
////タイトルと操作説明
    HgSetFont(HG_G,60);
    HgText(40,500,"時止める系シューティング\n");
    HgText(100,100,"何かキーを押してね\n");
    HgSleep(0.5);
    HgGetChar();
    HgClear();
    
    HgSetFont(HG_G,100);
    HgText(200,550,"操作方法\n");
    HgSetFont(HG_G,50);
    HgText(10,300," Wキー：上移動　Sキー：下移動\n Aキー：左移動　 Dキー：右移動\n\n           スペース：攻撃\n");
    HgSetFont(HG_G,40);
    HgText(50,150,"右下のゲージが赤色まで溜まった時に\n");
    HgSetFont(HG_G,70);
    HgText(50,10,"Mキー：\n");
    HgSetColor(HG_RED);
    HgText(350,10,"時止め\n");
    HgSetColor(HG_BLACK);
    HgSetFont(HG_G,30);
    HgSleep(0.5);
    HgText(500,10,"何かキーを押してね");
    key=HgGetChar();
    
    //Zキーで体力を4000にする
    if(key==122){
        my.HP=4000;
    }
    
    HgSleep(0.5);
    HgClear();
    
////戦闘開始
    HgSetFont(HG_G,23);
    HgText(510, 720, "時止める系シューティング\n");
    layer=HgWAddDoubleLayer(0);
    lay2=HgWAddLayer(0);
    HgSetFillColor(HG_BLACK);
    HgBoxFill(0,0,500,WINDOWY,1);
    HgSetEventMask(HG_KEY_DOWN|HG_KEY_UP);
    while(1){
        
////キー入力
        event=HgEventNonBlocking();
        if(event!=NULL&&event->type==HG_KEY_DOWN&&my.HP>=0){
            key=event->ch;
            switch(key){
            case 97:
               
                xspeed=-my.xspeed;
                break;
            case 100:
             
                xspeed=my.xspeed;
                break;
                
            case 115:
              
                yspeed=-my.yspeed;
                break;
                
            case 119:
               
                yspeed=my.yspeed;
                break;
            case 32:
                if(firemode==0){
                    firetimecount=my.firerate;
                }
                firemode=1;
                break;
            case 109:
                stoptimekeydown=1;
                break;
            }
        }
        else if(event!=NULL&&event->type==HG_KEY_UP){
            key=event->ch;
            switch(key){
            case 97:         
                xspeed=0;
                break;
            case 100:
               
                xspeed=0;
                break;
            case 115:
             
                yspeed=0;
                break;
            case 119:
               
                yspeed=0;
                break;
            case 32:
                firemode=0;
                firetimecount=0;
                break;
            case 109:
                stoptimekeydown=0;
                break;
           
            }
        }
        
////自分が壁を貫通しないようにする処理
        if(my.x-my.r<=0){
            if(xspeed<0) xspeed=0;
        }
        else if(my.x+my.r>=500){
            if(xspeed>0) xspeed=0;
        }
        if(my.y-my.r<=0){
            if(yspeed<0) yspeed=0;
        }
        else if(my.y+my.r>=WINDOWY){
            if(yspeed>0) yspeed=0;
        }
        
////自分の体力が０未満の時の処理(ゲームオーバー)
        if(my.HP<0){
            firemode=0;
            if(size<20) size=20;
            if(size<=100){
                HgWSetFillColor(lay, HG_RED);
                HgWCircleFill(lay, my.x,my.y,size,1);
                HgWSetFillColor(lay, HG_BLACK);
                HgWCircleFill(lay, my.x,my.y,size-20,1);
                size+=10;
            }
            if(size==100){
                HgWSetFillColor(lay2,HgRGBA(0,0,0,0.7));
                HgWBoxFill(lay2,0,0,500,WINDOWY,1);
                HgWSetColor(lay2, HG_WHITE);
                HgWSetFont(lay2, HG_G, 70);
                HgWText(lay2, 40, 400, "GAMEOVER");
                size+=1;
            }
            else if(size==101){
                HgSetEventMask(HG_KEY_DOWN);
                event=HgEventNonBlocking();
                if(event!=NULL) {
                    HgClose();
                    return 0;
                }
            }
        }
        
        
////時間停止時は速度を1/2にする処理
        if(stoptime==1){
            my.x+=xspeed/2;
            my.y+=yspeed/2;
        }
        else {
            my.x+=xspeed;
            my.y+=yspeed;
        }

////自分の弾丸発射処理
        if(firemode==1){
            if(firetimecount==my.firerate){
                firemode=2;
                firetimecount=0;
            }
            else firetimecount+=1;
        }
        
        if(firemode==2){
            int i=bulletcheck();
            if(i!=-1){
                bulletstatus[i]=1;
                bl[i].x=my.x;
                bl[i].y=my.y;
                bl[i].r=3;
                bl[i].xspeed=0;
                bl[i].yspeed=20;
                bl[i].status=1;
                bl[i].color=HG_SKYBLUE;
                if(stoptime==1) bl[i].stoped=1;
                firemode=1;
            }
            i=bulletcheck();
            if(i!=-1){
                bulletstatus[i]=1;
                bl[i].x=my.x;
                bl[i].y=my.y;
                bl[i].r=3;
                bl[i].xspeed=-1;
                bl[i].yspeed=20;
                bl[i].status=1;
                bl[i].color=HG_SKYBLUE;
                if(stoptime==1) bl[i].stoped=1;
                firemode=1;
            }
            i=bulletcheck();
            if(i!=-1){
                bulletstatus[i]=1;
                bl[i].x=my.x;
                bl[i].y=my.y;
                bl[i].r=3;
                bl[i].xspeed=1;
                bl[i].yspeed=20;
                bl[i].status=1;
                bl[i].color=HG_SKYBLUE;
                if(stoptime==1) bl[i].stoped=1;
                firemode=1;
            }
        }
        boss1action(&Bkakudo);
        
        lay=HgLSwitch(&layer);
        HgLClear(lay);
        
////時止め処理
        if(stoptime==0){
            if(stoptimecount<0.5) stoptimecount=0.5;
            if(stoptimekeydown==1){
                if(stoptimecount==0.5){
                    HgSetFillColor(HG_WHITE);
                    HgBoxFill(0,0,500,WINDOWY,1);
                    stoptime=1;
                }
            }
        }
        else {
            stoptimecount+=0.05;
            if(stoptimecount>=2.5){
                HgSetFillColor(HG_BLACK);
                HgBoxFill(0,0,500,WINDOWY,1);
                stoptime=0;
                stoptimecount=2.5;
            }
        }
        
        if(stoptimecount==0.5||stoptime==1) HgWSetFillColor(lay,HG_RED);
        else  HgWSetFillColor(lay,HG_BLUE);
        HgWCircleFill(lay,650, 140, 100,1);
        HgWSetFillColor(lay,HG_WHITE);
        HgWFanFill(lay,650,140,100,PI/2,stoptimecount*PI,1);
        
////自分が敵の弾丸にあったった時の無敵時間の円
        if(my.damage==1&&my.dtimecount<=30&&my.HP>=0){
            if(stoptime==1) HgWSetColor(lay,HG_BLACK);
            else HgWSetColor(lay,HG_WHITE);
            HgWCircle(lay,my.x, my.y,40-my.dtimecount);
            HgWSetColor(lay,HG_BLACK);
            my.dtimecount++;
        }
        else {
            my.dtimecount=0;
            my.damage=0;
        }
        
////キャラクターの円
        HgWSetFillColor(lay, HG_GREEN);
        if(my.HP>=0) HgWCircleFill(lay, my.x, my.y, my.r,1);
        HgWSetFillColor(lay, HG_RED);
        if(boss1.phase!=-1) HgWCircleFill(lay, boss1.x, boss1.y, boss1.r,1);
        
////右側の情報
        HgWSetFont(lay, HG_G, 30);
        if(my.HP<0) HgWText(lay, 650, 600,"0\n");
        else HgWText(lay, 550, 600,"残機：%d\n",my.HP);
        HgWSetFont(lay, HG_G, 20);
        HgWText(lay, 550, 300,"SCORE: %d\n",my.score);
        
////ボス死亡時の処理（ゲームクリア）
        if(boss1.HP<0){
            boss1.phase=0;
            if(Ecount==0){
                for(int i=0;i<loopvalue;i++){
                    bulletstatus[i]=0;
                }
            }
            if(Ecount<=80){
                if(size>50){
                    size=10;
                    randomx=rand()%100;
                    if(rand()%2==1) randomx*=-1;
                    randomy=rand()%100;
                    if(rand()%2==1) randomy*=-1;
                }
                
                HgWSetFillColor(lay, HG_RED);
                HgWCircleFill(lay, boss1.x+randomx,boss1.y+randomy,size,1);
                if(stoptime<1){
                    size+=10;
                }
               
                if(Ecount==80) size=20;
            }
            else if(Ecount<=90){
                HgWSetFillColor(lay, HG_RED);
                HgWCircleFill(lay, boss1.x,boss1.y,size,1);
                HgWSetFillColor(lay, HG_BLACK);
                HgWCircleFill(lay, boss1.x,boss1.y,size-20,1);
                if(stoptime<1){
                    size+=20;
                }
            }
            if(Ecount>=90){
                boss1.phase=-1;
                HgWSetFillColor(lay2,HgRGBA(0,0,0,0.7));
                HgWBoxFill(lay2,0,0,500,WINDOWY,1);
                HgWSetColor(lay2, HG_WHITE);
                HgWSetFont(lay2, HG_G, 70);
                HgWText(lay2, 20, 400, "GAMECLEAR");
                HgSleep(1);
                HgClose();
                return 0;
               
            }
            if(stoptime<1){
                Ecount++;
            }
        }
        
////弾処理&弾の当たり判定
        for(int i=0;i<loopvalue;i++){
            if(bulletstatus[i]==1){
                //printf("%d\n",i);
                if(bl[i].x<0+bl[i].r||bl[i].x>500-bl[i].r||bl[i].y<0||bl[i].y>WINDOWY){
                    bulletstatus[i]=0;
                    continue;
                }
                else if(my.damage!=1&&bl[i].status==2&&bl[i].x+bl[i].r>my.x-my.r&&bl[i].x-bl[i].r<my.x+my.r){
                    if(bl[i].y+bl[i].r>my.y-my.r&&bl[i].y-bl[i].r<my.y+my.r){
                        my.damage=1;
                        bulletstatus[i]=0;
                        my.HP-=1;
                        continue;
                    }
                }
                else if(bl[i].status==1&&bl[i].x+bl[i].r>=boss1.x-boss1.r&&bl[i].x-bl[i].r<=boss1.x+boss1.r){
                    if(bl[i].y+bl[i].r>=boss1.y-boss1.r&&bl[i].y-bl[i].r<=boss1.y+boss1.r){
                        boss1.HP-=100;
                        my.score+=100;
                        if(bl[i].stoped!=1) stoptimecount-=0.05;
                        bulletstatus[i]=0;
                        continue;
                    }
                }
                HgWSetFillColor(lay,bl[i].color);
                HgWCircleFill(lay, bl[i].x, bl[i].y, bl[i].r,1);
                if(stoptime==0){
                    bl[i].x+=bl[i].xspeed;
                    bl[i].y+=bl[i].yspeed;
                }
            }
        }
        
////ボスのHPバー(描写順の関係で最後に持ってきています)
        HgWSetFillColor(lay, HG_GREEN);
        if(boss1.HP/50000<0){
        }
        else HgWBoxFill(lay,20,750,460*(boss1.HP/50000),30,1);
        

    }
////ここまでループ

    
    return 0;
}



////弾丸の番号を振り分ける関数
int bulletcheck(){
    int i=0;
    for(i=0;i<loopvalue;i++){
        if(bulletstatus[i]==0){
            return i;
        }
    }
    return -1;
}

////ボスのアクション
int boss1action(double *Br){
    double Bkakudo=*Br;
    int bosstimecount=boss1.timecount;
    double random=(double)rand()/RAND_MAX;
    
    if(stoptime==0){
        bosstimecount++;
        if(boss1.phase==1){
            //ボスの体力が半分になったら第二段階にする
            if(boss1.HP<25000) {
                boss1.phase=2;
                bosstimecount=0;
            }
       
            if(Bkakudo>=2.0){
                Bkakudo=0;
                if(boss1.plusR>=0.06||boss1.plusR<=-0.06) boss1.plusRswitch*=-1;
                if(boss1.plusRswitch==1){
                    boss1.plusR+=0.02;
                }
                else boss1.plusR-=0.02;
            }
       
            double bsin=sin(Bkakudo*PI)+boss1.plusR;
            double bcos=cos(Bkakudo*PI)+boss1.plusR;
            bsin*=3;
            bcos*=3;
      
        
            int i=bulletcheck();
            bulletstatus[i]=1;
            bl[i].x=boss1.x;
            bl[i].y=boss1.y;
            bl[i].r=10;
            bl[i].xspeed=bcos;
            bl[i].yspeed=bsin;
            bl[i].status=2;
            bl[i].color=HgColorCode(0x00ffff);
            Bkakudo+=0.03;
        
        
            
            if(bosstimecount==15){
                if(rand()%2==1) random*=-1;
                for(double rad=0;rad<=2;rad+=0.1){
                    double bsin=sin(rad*PI);
                    double bcos=cos(rad*PI);
               
                    bsin*=8;
                    bcos*=8;
                    bsin+=random;
                    bcos+=random;
                    int i=bulletcheck();
                    bulletstatus[i]=1;
                    bl[i].x=boss1.x;
                    bl[i].y=boss1.y;
                    bl[i].r=25;
                    bl[i].xspeed=bcos;
                    bl[i].yspeed=bsin;
                    bl[i].status=2;
                    bosstimecount=0;
                    bl[i].color=HgColorCode(0xff0000);
                }
            }
        }
        else if(boss1.phase==2){
           
            if(Bkakudo>=2.0){
                Bkakudo=0;
                if(boss1.plusR>=0.06||boss1.plusR<=-0.06) boss1.plusRswitch*=-1;
                if(boss1.plusRswitch==1){
                    boss1.plusR+=0.02;
                }
                else boss1.plusR-=0.02;
            }
       
            double bsin=sin(Bkakudo*PI)+boss1.plusR;
            double bcos=cos(Bkakudo*PI)+boss1.plusR;
            bsin*=3;
            bcos*=3;
      
        
            int i=bulletcheck();
            bulletstatus[i]=1;
            bl[i].x=boss1.x;
            bl[i].y=boss1.y;
            bl[i].r=10;
            bl[i].xspeed=bcos;
            bl[i].yspeed=bsin;
            bl[i].status=2;
            bl[i].color=HgColorCode(0x00ffff);
            Bkakudo+=0.03;
            
            if(bosstimecount==6){
                double xsa=my.x-boss1.x;
                double ysa=my.y-boss1.y;
                double syahen=sqrt(xsa*xsa+ysa*ysa);
           
                double bsin=ysa/syahen;
                double bcos=xsa/syahen;
                bsin*=10;
                bcos*=10;
                
                int i=bulletcheck();
                bulletstatus[i]=1;
                bl[i].x=boss1.x;
                bl[i].y=boss1.y;
                bl[i].r=10;
                bl[i].xspeed=bcos;
                bl[i].yspeed=bsin;
                bl[i].status=2;
                bl[i].color=HgColorCode(HG_YELLOW);
                bosstimecount=0;
            }
            
            
        }
        else if(boss1.phase==0){
            
        }
   
    }
    *Br=Bkakudo;
    boss1.timecount=bosstimecount;
    return 0;
}
