#include <stdio.h>
#include <handy.h>
#include <stdlib.h>

typedef struct TEMP
{
    int x;
    int y;
    int id;
    struct TEMP *next;
} Temp;

void kakuho(Temp *tail)
{
    printf("cccccc\n");
    Temp *p = tail;
    while (p->next != NULL)
    {
        p = p->next;
    }
    Temp *new = (Temp *)malloc(sizeof(Temp));
    p->next = new;
    new->id = p->id + 1;
    new->next = NULL;
    new->x = p->x;
    new->y = p->y;
    printf("endcccc\n");
}

void kaihou(Temp *tail)
{
    Temp *p = tail->next;
    free(tail);
    while (p != NULL)
    {
        Temp *p1 = p->next;
        free(p);
        p = p1;
    }
}

void Move(Temp *tail)
{
    HgClear();
    printf("aaaaaa\n");
    Temp *pb = tail;
    Temp *p = tail->next;
    printf("%d\n", pb->id);

    while (p != NULL)
    {
        printf("%d\n", p->id);
        int tempx = p->x;
        int tempy = p->y;
        pb->x = tempx;
        pb->y = tempy;
        pb = p;
        p = pb->next;
    }
    pb->x += 20;
    pb->y += 20;
}

void Draw(Temp *tail)
{
    Temp *p = tail;
    while (1)
    {
        printf("%d\n", p->id);
        printf("%d %d\n", p->x, p->y);
        HgCircle(p->x, p->y, 20);
        if (p->next == NULL)
        {
            break;
        }
        else
        {
            p = p->next;
        }
    }
}

int main(void)
{
    Temp *head = (Temp *)malloc(sizeof(Temp));
    Temp *tail = head;
    head->x = 100;
    head->y = 100;
    head->id = 1;
    head->next = NULL;
    //kakuho(tail);
    //kaihou(tail);
    HgOpen(400, 400);
    hgevent *event;
    HgSetEventMask(HG_KEY_DOWN);

    while (1)
    {
        event = HgEventNonBlocking();
        if (event != NULL)
        {
            int key = event->ch;
            switch (key)
            {
            case 122:
                printf("eeeeee\n");
                kakuho(tail);
                break;
            case 97:
                break;
            case 100:
                break;
            case 115:
                break;
            case 119:
                break;
            }
        }
        Move(tail);
        Draw(tail);
        HgSleep(1);
    }
    return 0;
}