#include<iostream>
using namespace std;

struct node
{
	int p,e;
	node* next;
};

class list {
public:
	node* head;
	int length;

	list();
	~list();
	int	insert(int i, int x);
	int remove(int i);
	int check(int x);
	int review();
	int	decommon();
	int	convert(int beg, int end);
};

list::list()
{
	length = 0;
	head = new(nothrow)	node;
	if (head == NULL)
	{
		cout << "内存不足" << endl;
		return;
	}
	head->p = 0;
	head->e = 0x80000000;
	head->next = new(nothrow)	node;
	if (head->next == NULL)
	{
		cout << "内存不足" << endl;
		return;
	}
	head->next->p = 0;
	head->next->e = 0x7fffffff;
	head->next->next = NULL;
	length = 0;
}

list::~list()
{
	node* p = head, * q;
	while (p->next != NULL)
	{
		q = p->next;
		delete	p;
		p = q;
	}
}

int	list::insert(int p1, int e)
{
	node* p = head, * q;
	while (p->next != NULL) {
		
		if (p->next->e == e)
		{
			p->next->p += p1;
			if (p->next->p == 0)
			{
				q = p->next->next;
				delete	p->next;
				p->next = q;
			}
		}
		else
		{
			if (p->e<e && p->next->e>e)
			{
				q = new(nothrow) node;
				if (q == NULL)
				{
					cout << "内存不足" << endl;
					return -2;
				}
				q->next = p->next;
				p->next = q;
				q->p = p1;
				q->e = e;
				length++;
				break;
			}
		}
		p = p->next;
	}
	return 0;
}

int list::review() {
	node* p = head;
	while (p->next != NULL) {
		p = p->next;
		if(p->p)
			cout << p->p << " " << p->e << " ";

	}
	cout << endl;
	return 0;
}

int	list_add(list& L1, list& L2, list& L3)
{
	node* p;
	p = L1.head;
	while (p->next != NULL)
	{
		p = p->next;
		if (p->p != 0)
			L3.insert(p->p, p->e);
	}
	p = L2.head;
	while (p->next != NULL)
	{
		p = p->next;
		if (p->p != 0)
			L3.insert(p->p, p->e);
	}
	return 0;
}

int	list_multi(list& L1, list& L2, list& L3)
{
	node* p, * q;
	p = L1.head;
	while (p->next != NULL)
	{
		p = p->next;
		q = L2.head;
		while (q->next != NULL)
		{
			q = q->next;
			if (p->p != 0&&q->p!=0)
				L3.insert(p->p*q->p, p->e+q->e);
		}
	}
	
	
	return 0;
}

int main()
{
	int n, p, e;
	list	L1,L2,L3,L4;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> p >> e;
		L1.insert(p,e);
	}
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> p >> e;
		L2.insert(p, e);
	}
	
	int com;
	cin >> com;
	switch(com) {
	case 0:
		list_add(L1, L2, L3);
		L3.review();
		break;
	case 1:
		list_multi(L1, L2, L4);
		L4.review();
		break;
	case 2:
		list_add(L1, L2, L3);
		L3.review();
		list_multi(L1, L2, L4);
		L4.review();
		break;
	}
	return 0;
}