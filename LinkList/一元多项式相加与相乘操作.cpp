#include<stdlib.h>
#include<iostream>
using namespace std;


typedef struct PNode
{
    float coef;//系数
    int expn;//指数
    struct PNode* next;
}PNode, * Polynomial;

void CreatePolyn(Polynomial& P, int n)
{
    Polynomial pre, q;
    P = new PNode;
    P->next = NULL;
    for (int i = 1; i <= n; ++i)
    {
        Polynomial s = new PNode;
        cin >> s->coef >> s->expn;
        if (s->coef == 0)
            continue;
        pre = P;
        q = P->next;
        while (q && q->expn < s->expn)//递增排序
        {
            pre = q;
            q = q->next;
        }
        if (q != NULL)
        {
            if (q->expn == s->expn)
            {
                q->coef = q->coef + s->coef;
                if (q->coef == 0)
                {
                    Polynomial t;
                    t = q;
                    pre->next = q->next;
                    delete t;
                }
                //确保多项式中无重复
                continue;
            }
        }
        s->next = q;
        pre->next = s;
    }
}
/*
void AddPolyn(Polynomial& Pa, Polynomial& Pb,Polynomial &Pc)
{
    Polynomial r;
    Polynomial p1 = Pa->next;
    Polynomial p2 = Pb->next;
    Polynomial p3 = Pa;//p3是新的多项式的移动指针
    int sum;
    while (p1 && p2)//两者都不为空
    {
        if (p1->expn == p2->expn)
        {
            sum = p1->coef + p2->coef;
            if (sum != 0)
            {
                p1->coef = sum;
                p3->next = p1;
                p3 = p1;
                p1 = p1->next;
                r = p2; p2 = p2->next; delete r;
            }
            else
            {
                p3->next = p1->next;
                r = p1; p1 = p1->next; delete r;
                r = p2; p2 = p2->next; delete r;
            }
        }
        else if(p1->expn<p2->expn)//此时p1后移
        {
            p3->next = p1;
            p3 = p1;
            p1 = p1->next;
        }
        else//此时直接将p2接到多项式中
        {
            p3->next = p2;
            p3 = p2;
            p2 = p2->next;
        }
    }
    p3->next = p1 ? p1 : p2;
    delete Pb;
}
*/



void AddPolyn(Polynomial& Pa, Polynomial& Pb, Polynomial& Pc)
{
    Polynomial p1 = Pa->next;
    Polynomial p2 = Pb->next;
    Polynomial p3 = Pc;
    int sum;
    while (p1 && p2)//两多项式都没取完
    {
        if (p1->expn > p2->expn)
        {
            Polynomial s = new PNode;
            s->expn = p2->expn;
            s->coef = p2->coef;
            p3->next = s;
            p3 = s;
            p2 = p2->next;
        }
        else if (p1->expn < p2->expn)
        {
            Polynomial s = new PNode;
            s->expn = p1->expn;
            s->coef = p1->coef;
            p3->next = s;
            p3 = s;
            p1 = p1->next;
        }
        else
        {
            sum = p1->coef + p2->coef;
            if (!sum)//两项之和为0
            {
                p1 = p1->next;
                p2 = p2->next;
            }
            else
            {
                Polynomial s = new PNode;
                s->coef = sum;
                s->expn = p1->expn;
                p3->next = s;
                p3 = s;
                p1 = p1->next;
                p2 = p2->next;
            }
        }
    }
    while (p1)
    {
        Polynomial s = new PNode;
        s->coef = p1->coef;
        s->expn = p1->expn;
        p3->next = s;
        p3 = s;
        p1 = p1->next;
    }
    while (p2)
    {
        Polynomial s = new PNode;
        s->coef = p2->coef;
        s->expn = p2->expn;
        p3->next = s;
        p3 = s;
        p2 = p2->next;
        
    }
    p3->next = NULL;
}

void InsertNode(Polynomial Pc, PNode s)
{
    Polynomial p1 = Pc->next, p2 = Pc;
    if (p1 == NULL)
    {
        Polynomial p3 = new PNode;
        *p3 = s;
        Pc->next = p3;
        p3->next = NULL;
    }
    else
    {
        while (p1 != NULL)
        {
            if (p1->expn < s.expn)
            {
                p2 = p1;
                p1 = p1->next;
            }
            else if (p1->expn > s.expn)
            {
                p2->next = &s;
                s.next = p1;
                break;
            }
            else if(p1->expn==s.expn)
            {
                int num = s.coef + p1->coef;
                if (num == 0)
                {
                    Polynomial r = p1;
                    p2->next = p1->next; delete r;
                    break;
                }
                else
                {
                    p1->coef = s.coef + p1->coef;
                    break;
                }
            }
        }
        if (p1 == NULL)
        {
            Polynomial t = new PNode;
            t->coef = s.coef; t->expn = s.expn;
            p2->next = t; t->next = NULL;
        }
    }
}

void Mutipolymial(Polynomial Pa, Polynomial Pb, Polynomial Pc)
{

    Polynomial p1, p2;
    for (p1 = Pa->next; p1 != NULL; p1 = p1->next)
        for (p2 = Pb->next; p2 != NULL; p2 = p2->next)
        {
            PNode n;
            n.coef = p1->coef * p2->coef;
            n.expn = p1->expn + p2->expn;
            
            InsertNode(Pc, n);
        }
}


int main()
{
    int m, n, order;
    cin >> m;
    Polynomial Pa, Pb, p1, Pc, Pd;
    CreatePolyn(Pc, 0);
    CreatePolyn(Pd, 0);
    CreatePolyn(Pa, m);
    cin >> n;
    CreatePolyn(Pb, n);
    cin >> order;
    if (order == 0)
    {
        AddPolyn(Pa, Pb, Pc);
        if (Pc->next == NULL)
            cout << 0 << " " << 0;
        for (p1 = Pc->next; p1 != NULL; p1 = p1->next)
        {
            cout << p1->coef << " " << p1->expn << " ";
        }
		cout << endl;
    }
    else if (order == 1)
    {
        Mutipolymial(Pa, Pb, Pd);
        
        for (p1 = Pd->next; p1 != NULL; p1 = p1->next)
        {
            cout << p1->coef << " " << p1->expn << " ";
        }
		cout << endl;
    }
    else if (order == 2)
    {
        AddPolyn(Pa, Pb, Pc);
       
        for (p1 = Pc->next; p1 != NULL; p1 = p1->next)
        {
            cout << p1->coef << " " << p1->expn << " ";
        }
        cout << endl;
        Mutipolymial(Pa, Pb, Pd);
      
        for (p1 = Pd->next; p1 != NULL; p1 = p1->next)
        {
            cout << p1->coef << " " << p1->expn << " ";
        }
		cout << endl;
    }
}