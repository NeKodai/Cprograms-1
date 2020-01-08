#include <stdio.h>
#include <handy.h>
#include <stdlib.h>
#include<time.h>

#define SIZE 21
#define WINDOWSIZE 600


typedef struct{
    int x;
    int y;
    
}enemy_struct;

typedef struct{
    int x;
    int y;
    int before;
    
}que_struct;

enemy_struct enemy[20];
que_struct que[999999];
int array[SIZE][SIZE] = {};


int maze(void)
{
    int item_count=0;


    //壁を生成
    for (int i = 0; i < SIZE; i++)
    {
        array[i][0]=1;
        array[0][i]=1;
        array[i][SIZE-1]=1;
        array[SIZE-1][i]=1;

    }
    
    for(int i=0;i<SIZE;i++)
    {
        if(i%2==0)
        {
            for(int j=0;j<SIZE;j++)
            {
                if(j%2==0)
                {
                    // array[i][j]=1;
                    if(array[i][j]==0)
                    {
                        array[i][j]=1;
                        int k=rand()%4;
                        switch(k)
                        {
                        case 0:
                            array[i-1][j]=1;
                            break;
                        case 1:
                            array[i+1][j]=1;
                            break;
                        case 2:
                            array[i][j-1]=1;
                            break;
                        case 3:
                            array[i][j+1]=1;
                            break;
                        }
                    }
                }
            }
        }
    }

    for(int i=0;i<SIZE;i++)
    {
        if(i%2!=0&&i!=0&&i!=SIZE-1)
        {
           
            for(int j=0;j<SIZE;j++)
            {
                if(j%2!=0&&j!=0&&j!=SIZE-1)
                {
                    if(array[i][j+1]==1&&array[i][j-1]==1)
                    {
                        if(array[i+1][j]==1&&array[i-1][j]==1)
                        {
                           
                            int rnd=rand()%4;
                            switch(rnd)
                            {
                            case 0:
                                array[i-1][j]=0;
                                break;
                            case 1:
                                array[i+1][j]=0;
                                break;
                            case 2:
                                array[i][j-1]=0;
                                break;
                            case 3:
                                array[i][j+1]=0;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    //アイテム生成
    for(int i=0;i<SIZE;i++)
    {
        if(i%2!=0&&i!=0&&i!=SIZE-1)
        {
           
            for(int j=0;j<SIZE;j++)
            {
                if(j%2!=0&&j!=0&&j!=SIZE-1)
                {
                    if(array[j][i]==0)
                    {
                        if(rand()%5==1)
                        {
                            array[j][i]=2;
                            item_count++;
                        }
                    }
                }
            }
        }
    }
    return item_count;
}


//マスを描写
void draw(void)
{
    float size=WINDOWSIZE/SIZE;
    for(int i=0;i<SIZE;i++)
    {
        for(int j=0;j<SIZE;j++)
        {
            if(array[j][i]==1)
            {
                HgSetFillColor(HG_BLACK);
                HgBoxFill(j*size,(WINDOWSIZE-size)-i*size,size,size,1);
            }
            if(array[j][i]==2)
            {
                HgSetFillColor(HG_YELLOW);
                HgCircleFill(j*size+size/2,(WINDOWSIZE-size)-i*size+size/2,size/4,1);
            }
        }
    }
}




//キャラクターを描写
void move_draw(int lay,int x,int y,int ecount)
{
    float size=WINDOWSIZE/SIZE;
    HgLClear(lay);
    HgWSetFillColor(lay,HG_YELLOW);
    HgWCircleFill(lay,x*size+size/2,(WINDOWSIZE-size)-y*size+size/2,size/2,1);
    HgWSetFillColor(lay,HG_BLUE);
    for(int i=0;i<ecount;i++)
    {
        if(i==1)HgWSetFillColor(lay,HG_RED);
        HgWBoxFill(lay,enemy[i].x*size,(WINDOWSIZE-size)-enemy[i].y*size,size,size,1);
    }
}


//幅優先探索
int  BFS(int my_x,int my_y,int e_num)
{
    int add_count=0;
    int s_count=0;
    
    que[add_count].x=enemy[e_num].x;
    que[add_count].y=enemy[e_num].y;
    if(que[add_count].x==my_x&&que[add_count].y==my_y)
    {
        return 1;
    }
    add_count++;
    
    while(1){
        
        if(array[que[s_count].x-1][que[s_count].y]!=1&&que[s_count].x-1>0){
            int flag=0;
            for(int i=0;i<add_count;i++)
            {
                if(que[i].x==que[s_count].x-1&&que[i].y==que[s_count].y){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                // printf("a%d %d\n",que[s_count].x-1,que[s_count].y);
                que[add_count].x=que[s_count].x-1;
                que[add_count].y=que[s_count].y;
                que[add_count].before=s_count;
                if(que[add_count].x==my_x&&que[add_count].y==my_y)
                {
                    break;
                }
                add_count++;
            }
        }
        if(array[que[s_count].x+1][que[s_count].y]!=1&&que[s_count].x+1<SIZE){
            int flag=0;
            for(int i=0;i<add_count;i++)
            {
                if(que[i].x==que[s_count].x+1&&que[i].y==que[s_count].y){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                // printf("b%d %d\n",que[s_count].x+1,que[s_count].y);
                que[add_count].x=que[s_count].x+1;
                que[add_count].y=que[s_count].y;
                que[add_count].before=s_count;
                if(que[add_count].x==my_x&&que[add_count].y==my_y)
                {
                    break;
                }
                add_count++;
            }
           
        }
        
        if(array[que[s_count].x][que[s_count].y-1]!=1&&que[s_count].y-1>0){
            int flag=0;
            for(int i=0;i<add_count;i++)
            {
                if(que[i].x==que[s_count].x&&que[i].y==que[s_count].y-1){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                // printf("c%d %d\n",que[s_count].x,que[s_count].y-1);
                que[add_count].x=que[s_count].x;
                que[add_count].y=que[s_count].y-1;
                que[add_count].before=s_count;
                if(que[add_count].x==my_x&&que[add_count].y==my_y)
                {
                    break;
                }
                add_count++;
            }
        }
        
        if(array[que[s_count].x][que[s_count].y+1]!=1&&que[s_count].y+1<SIZE){
            int flag=0;
            for(int i=0;i<add_count;i++)
            {
                if(que[i].x==que[s_count].x&&que[i].y==que[s_count].y+1){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                // printf("d%d %d\n",que[s_count].x,que[s_count].y+1);
                que[add_count].x=que[s_count].x;
                que[add_count].y=que[s_count].y+1;
                que[add_count].before=s_count;
                if(que[add_count].x==my_x&&que[add_count].y==my_y)
                {
                    break;
                }
                add_count++;
            }
        }
        s_count++;
        if(add_count==s_count)return -1;
        
    }
    //int Badd_count=0;
    while(1){
        // printf("x:%d y:%d\n",que[add_count].x,que[add_count].y);
        enemy[e_num].x=que[add_count].x;
        enemy[e_num].y=que[add_count].y;
        //Badd_count=add_count;
        //printf("badd %d : add %d ",Badd_count,add_count);
        add_count=que[add_count].before;
        if(add_count==0)break;
    }
    
    return 0; 
}


int  BFS_saki(int my_x,int my_y,int e_num,int key)
{
    int add_count=0;
    int s_count=0;

     switch(key)
            {
            case 97:
                while(array[my_x-1][my_y]!=1)
                {
                    my_x-=1;
                }
                break;
            case 100:
                while(array[my_x+1][my_y]!=1)
                {
                    my_x+=1;
                }
                break;
            case 115:
                while(array[my_x][my_y+1]!=1)
                {
                    my_y+=1;
                }
                break;
            case 119:
                while(array[my_x][my_y-1]!=1)
                {
                    my_y-=1;
                }
                break;
            }

    
    que[add_count].x=enemy[e_num].x;
    que[add_count].y=enemy[e_num].y;
    if(que[add_count].x==my_x&&que[add_count].y==my_y)
    {
        return 1;
    }
    add_count++;
    
    while(1){
        
        if(array[que[s_count].x-1][que[s_count].y]!=1&&que[s_count].x-1>0){
            int flag=0;
            for(int i=0;i<add_count;i++)
            {
                if(que[i].x==que[s_count].x-1&&que[i].y==que[s_count].y){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                // printf("a%d %d\n",que[s_count].x-1,que[s_count].y);
                que[add_count].x=que[s_count].x-1;
                que[add_count].y=que[s_count].y;
                que[add_count].before=s_count;
                if(que[add_count].x==my_x&&que[add_count].y==my_y)
                {
                    break;
                }
                add_count++;
            }
        }
        if(array[que[s_count].x+1][que[s_count].y]!=1&&que[s_count].x+1<SIZE){
            int flag=0;
            for(int i=0;i<add_count;i++)
            {
                if(que[i].x==que[s_count].x+1&&que[i].y==que[s_count].y){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                // printf("b%d %d\n",que[s_count].x+1,que[s_count].y);
                que[add_count].x=que[s_count].x+1;
                que[add_count].y=que[s_count].y;
                que[add_count].before=s_count;
                if(que[add_count].x==my_x&&que[add_count].y==my_y)
                {
                    break;
                }
                add_count++;
            }
           
        }
        
        if(array[que[s_count].x][que[s_count].y-1]!=1&&que[s_count].y-1>0){
            int flag=0;
            for(int i=0;i<add_count;i++)
            {
                if(que[i].x==que[s_count].x&&que[i].y==que[s_count].y-1){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                // printf("c%d %d\n",que[s_count].x,que[s_count].y-1);
                que[add_count].x=que[s_count].x;
                que[add_count].y=que[s_count].y-1;
                que[add_count].before=s_count;
                if(que[add_count].x==my_x&&que[add_count].y==my_y)
                {
                    break;
                }
                add_count++;
            }
        }
        
        if(array[que[s_count].x][que[s_count].y+1]!=1&&que[s_count].y+1<SIZE){
            int flag=0;
            for(int i=0;i<add_count;i++)
            {
                if(que[i].x==que[s_count].x&&que[i].y==que[s_count].y+1){
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                // printf("d%d %d\n",que[s_count].x,que[s_count].y+1);
                que[add_count].x=que[s_count].x;
                que[add_count].y=que[s_count].y+1;
                que[add_count].before=s_count;
                if(que[add_count].x==my_x&&que[add_count].y==my_y)
                {
                    break;
                }
                add_count++;
            }
        }
        s_count++;
        if(add_count==s_count)return -1;
        
    }
    //int Badd_count=0;
    while(1){
        // printf("x:%d y:%d\n",que[add_count].x,que[add_count].y);
        enemy[e_num].x=que[add_count].x;
        enemy[e_num].y=que[add_count].y;
        //Badd_count=add_count;
        //printf("badd %d : add %d ",Badd_count,add_count);
        add_count=que[add_count].before;
        if(add_count==0)break;
    }
    
    
    
    return 0; 
}



int main(void)
{
    int my_x=1;
    int my_y=1;
    int key=0;
    int wait=0;
    int item_total=0;
    int item_count=0;
    int enemy_count=2;
    hgevent *event;

    enemy[0].x=SIZE-2;
    enemy[0].y=SIZE-2;
    enemy[1].x=1;
    enemy[1].y=SIZE-2;
    
    srand(time(NULL));
    HgOpen(WINDOWSIZE+200, WINDOWSIZE);
    HgGetChar();
    HgSetEventMask(HG_KEY_DOWN);
    int lay=HgWAddLayer(0);
    item_total=maze();
   
    
    for(int i=0;i<SIZE;i++)
    {
        for(int j=0;j<SIZE;j++)
        {
            printf("%d ",array[j][i]);
        }
        printf("\n");
    }
    
    draw();

    //printf("x:%d y:%d", enemy[0].x, enemy[0].y);
    while(1)
    {
        event=HgEventNonBlocking();
        if(event!=NULL)
        {
            int temp_key= event->ch;
            switch(temp_key)
            {
            case 97:
                if(array[my_x-1][my_y]!=1)
                {
                    key=97;
                }
                break;
            case 100:
                if(array[my_x+1][my_y]!=1)
                {
                    key=100;
                }
                break;
            case 115:
                if(array[my_x][my_y+1]!=1)
                {
                    key=115;
                }
                break;
            case 119:
                if(array[my_x][my_y-1]!=1)
                {
                    key=119;
                }
                break;
            }
            
        }

        if(wait%100==0){
            for(int i=0;i<enemy_count;i++)
            {
                if(my_x==enemy[i].x&&my_y==enemy[i].y)return 0;
            }
            
            switch(key)
            {
            case 97:
                if(array[my_x-1][my_y]!=1)
                {
                    my_x-=1;
                }
                break;
            case 100:
                if(array[my_x+1][my_y]!=1)
                {
                    my_x+=1;
                }
                break;
            case 115:
                if(array[my_x][my_y+1]!=1)
                {
                    my_y+=1;
                }
                break;
            case 119:
                if(array[my_x][my_y-1]!=1)
                {
                    my_y-=1;
                }
                break;
            }
            if(array[my_x][my_y]==2)
            {
                
                array[my_x][my_y]=0;
                HgSetFillColor(HG_WHITE);
                int size=WINDOWSIZE/SIZE;
                HgBoxFill(my_x*size,(WINDOWSIZE-size)-my_y*size,size-1,size-1,0);
                item_count++;
                if(item_total==item_count)
                {
                    return 0;
                }
                
            }
            
            move_draw(lay, my_x, my_y,enemy_count);
           
        }
        if(wait%200==0)
        {
            if(BFS(my_x,my_y,0)==-1)
            {
                printf("error\n");
                return -1;
            }
            if(BFS_saki(my_x,my_y,1,key)==-1)
            {
                printf("error\n");
                return -1;
            }
            wait=0;
        }
        wait++;
        HgSleep(0.001);
        
    }
   
    return 0;
}
