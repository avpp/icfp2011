#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

/*CONSTANTS*/

const short int CARDS_COUNT = 15;
char* CARDS_NAME[CARDS_COUNT] = {"I", "zero", "succ", "dbl", "get", "put", "S", "K", "inc", "dec", "attack", "help", "copy", "revive", "zombie"};
/*END CONSTANTS*/

/*TYPES*/
enum cards {I, zero, succ, dbl, get, put, S, K, inc, dec, attack, help, copy, revive, zombie};

enum choice {ctos = 1, stoc = 2}; //ctos == Card to slot. stoc == Slot to card

struct turn
{
	choice m_choice;
	cards m_card;
	short int m_slot;
};

struct queueItem
{
	turn m_value;
	queueItem* m_next;
};

struct turnQueue
{
	int m_size;
	queueItem* m_Begin;
	queueItem* m_End;
};
/*END TYPES*/

/*VARIABLES*/

turnQueue mainQueue;
int TurnNumber;

/*END VARIABLES*/

//----------------------------------------------------------------------------------
//FUNCTION'S INTERFACES

/*QUEUE FUNCTIONS*/
void InitQueue(turnQueue* q);
void Push(turnQueue* q, turn t);
turn Pop(turnQueue* q);
void ClearQueue(turnQueue* q);
void AddQueueToQueue(turnQueue* source, turnQueue* destination);  //while(source.m_size > 0) push(destination, pop(source));
/*END QUEUE FUNCTIONS*/

/*FUNCTIONS*/
turn CreateTurn(choice ch, cards card, short int slot);
turn CreateTurn(choice ch, short int slot, cards card);
cards StringToCards(char* s);
char* CardsToString(cards card);
turn ReadStep();
void WriteStep(turn t);
void Init();
turn Logic();
/*END FUNCTIONS*/

//---------------------------------------------------------------------------------
//FUNCTION'S IMPLEMENTATIONS

/*QUEUE FUNCTIONS*/

void InitQueue(turnQueue &q)
{
	q.m_size = 0;
	q.m_Begin = NULL;
	q.m_End = NULL;
}

void Push(turnQueue &q, turn t)
{
	queueItem *qi = new queueItem;
	qi -> m_value = t;
	qi -> m_next = NULL;
	if (!q.m_size)
	{
		q.m_Begin = qi;
	}
	else
	{
		q.m_End -> m_next = qi;
	}
	q.m_End = qi;
	q.m_size++;
}

turn Pop(turnQueue &q)
{
	turn ans;
	if (!q.m_size)
	{
		ans.m_choice = ctos;
		ans.m_card = I;
		ans.m_slot = 0;
	}
	else
	{
		ans = q.m_Begin -> m_value;
		queueItem *qi = q.m_Begin;
		q.m_Begin = qi -> m_next;
		q.m_size--;
		delete qi;
	}
	return ans;
}

void ClearQueue(turnQueue &q)
{
	while (q.m_size)
	{
		Pop(q);
	}
}

void AddQueueToQueue(turnQueue &source, turnQueue &destination)
{
	while (source.m_size)
	{
		Push(destination, Pop(source));
	}
}

/*FUNCTIONS*/

turn CreateTurn(choice ch, cards card, short int slot)
{
	turn ans;
	ans.m_choice = ch;
	ans.m_card = card;
	ans.m_slot = slot;
	return ans;
}

turn CreateTurn(choice ch, short int slot, cards card)
{
	return CreateTurn(ch, card, slot);
}

cards StringToCards(char* s)
{
	char i = -1;
	while (++i < CARDS_COUNT && strcmp(CARDS_NAME[i], s) != 0);
	if (i >= CARDS_COUNT)
		return I;
	return (cards)i;
}

char* CardsToString(cards card)
{
	return CARDS_NAME[card];
}

turn ReadStep()
{
	turn ans;
	int ch;
	cin>>ch;
	char s[10];
	ans.m_choice = (choice) ch;
	if (ans.m_choice == ctos)
	{
		cin>>s;
		cin>>ans.m_slot;
	}
	else
	{
		cin>>ans.m_slot;
		cin>>s;
	}
	ans.m_card = StringToCards(s);
	return ans;
}

void WriteStep(turn t)
{
	cout<<t.m_choice<<"\n";
	if (t.m_choice == ctos)
	{
		cout<<CardsToString(t.m_card)<<"\n";
		cout<<t.m_slot<<"\n";
	}
	else
	{
		cout<<t.m_slot<<"\n";
		cout<<CardsToString(t.m_card)<<"\n";
	}
}
void Init()
{
	InitQueue(mainQueue);
	TurnNumber = 0;
}

void func0(turnQueue &a, short int slot)  //S(K(a))
{
	Push(a, CreateTurn(ctos, K, slot));
	Push(a, CreateTurn(ctos, S, slot));
}

turnQueue func1(turnQueue &a, turnQueue &b, short int slot) //S(K(a))(K(b))
{
	turnQueue ans;
	InitQueue(ans);
	AddQueueToQueue(a, ans);
	while (b.m_size)
	{
		func0(ans, slot);
    Push(ans, Pop(b));
	}
	Push(ans, CreateTurn(stoc, slot, zero));
  return ans;
}

turn Logic()
{
	//Here we change our main queue
	if (TurnNumber == 0)
	{
    int i;
    Push(mainQueue, CreateTurn(stoc, 3, zero));
    Push(mainQueue, CreateTurn(ctos, 3, succ));
    for(i = 0; i < 13; i++)
        Push(mainQueue, CreateTurn(ctos, 3, dbl));

    Push(mainQueue, CreateTurn(stoc, 2, zero));
    Push(mainQueue, CreateTurn(ctos, succ, 2));
    Push(mainQueue, CreateTurn(ctos, succ, 2));
    Push(mainQueue, CreateTurn(ctos, succ, 2));
    Push(mainQueue, CreateTurn(ctos, get, 2));
    Push(mainQueue, CreateTurn(ctos, K, 2));

    Push(mainQueue, CreateTurn(stoc, 1, help));
    Push(mainQueue, CreateTurn(stoc, 1, zero));
    Push(mainQueue, CreateTurn(stoc, 1, zero));

		Push(mainQueue, CreateTurn(ctos, K, 1));
    Push(mainQueue, CreateTurn(ctos, S, 1));
    Push(mainQueue, CreateTurn(ctos, K, 1));
    Push(mainQueue, CreateTurn(ctos, S, 1));

    Push(mainQueue, CreateTurn(stoc, 1, get));
    Push(mainQueue, CreateTurn(ctos, K, 1));
    Push(mainQueue, CreateTurn(ctos, S, 1));
    Push(mainQueue, CreateTurn(stoc, 1, succ));
    Push(mainQueue, CreateTurn(ctos, K, 1));
    Push(mainQueue, CreateTurn(ctos, S, 1));
    Push(mainQueue, CreateTurn(stoc, 1, succ));
    Push(mainQueue, CreateTurn(stoc, 1, zero));

		Push(mainQueue, CreateTurn(ctos, S, 1));
		Push(mainQueue, CreateTurn(stoc, 1, I));

		Push(mainQueue, CreateTurn(stoc, 0, get));
		Push(mainQueue, CreateTurn(ctos, S, 0));
		Push(mainQueue, CreateTurn(ctos, K, 0));
		Push(mainQueue, CreateTurn(ctos, S, 0));
		Push(mainQueue, CreateTurn(stoc, 0, get));
		Push(mainQueue, CreateTurn(ctos, K, 0));
		Push(mainQueue, CreateTurn(ctos, S, 0));
		Push(mainQueue, CreateTurn(stoc, 0, succ));
		Push(mainQueue, CreateTurn(stoc, 0, zero));
		
		Push(mainQueue, CreateTurn(stoc, 0, zero));

    Push(mainQueue, CreateTurn(ctos, zero, 0));
    Push(mainQueue, CreateTurn(ctos, zero, 1));
    Push(mainQueue, CreateTurn(ctos, zero, 2));


/*Init Fields*/
// 8 - count for help
// 9 - count for attack
// 10 - #recursive slot
// 11 - #start slot

Push(mainQueue, CreateTurn(stoc, 8, zero));
Push(mainQueue, CreateTurn(ctos, 8, succ));
for (i = 0; i < 13; i++)
    Push(mainQueue, CreateTurn(ctos, 8, dbl));

Push(mainQueue, CreateTurn(stoc, 9, zero));
Push(mainQueue, CreateTurn(ctos, 9, succ));
for (i = 0; i < 12; i++)
    Push(mainQueue, CreateTurn(ctos, 9, dbl));

Push(mainQueue, CreateTurn(stoc, 10, zero));
Push(mainQueue, CreateTurn(ctos, succ, 10));

Push(mainQueue, CreateTurn(stoc, 11, zero));
/*end Init Fields*/

/*F, H = 1; C, alpha = 2; betta = 3*/

/*Create S(K(get))(K(Num in zero))*/
Push(mainQueue, CreateTurn(stoc, 0, get));
Push(mainQueue, CreateTurn(ctos, K, 0));
Push(mainQueue, CreateTurn(ctos, S, 0));
Push(mainQueue, CreateTurn(ctos, K, 0));
Push(mainQueue, CreateTurn(ctos, S, 0));
Push(mainQueue, CreateTurn(stoc, 0, K));
Push(mainQueue, CreateTurn(ctos, K, 0));
Push(mainQueue, CreateTurn(ctos, S, 0));
Push(mainQueue, CreateTurn(stoc, 0, get));

/*Copy this functions*/
Push(mainQueue, CreateTurn(stoc, 4, zero));
Push(mainQueue, CreateTurn(ctos, get, 4));
Push(mainQueue, CreateTurn(stoc, 5, zero));
Push(mainQueue, CreateTurn(ctos, get, 5));
Push(mainQueue, CreateTurn(stoc, 6, zero));
Push(mainQueue, CreateTurn(ctos, get, 6));
Push(mainQueue, CreateTurn(stoc, 7, zero));
Push(mainQueue, CreateTurn(ctos, get, 7));

/*execute*/
Push(mainQueue, CreateTurn(ctos, zero, 0));
Push(mainQueue, CreateTurn(stoc, 0, zero));
Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(ctos, dbl, 0));
Push(mainQueue, CreateTurn(ctos, dbl, 0));
Push(mainQueue, CreateTurn(ctos, dbl, 0));
Push(mainQueue, CreateTurn(stoc, 4, zero));
Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(stoc, 5, zero));
Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(stoc, 6, zero));
Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(stoc, 7, zero));

/*betta*/
Push(mainQueue, CreateTurn(ctos, put, 3));
Push(mainQueue, CreateTurn(stoc, 3, help));
Push(mainQueue, CreateTurn(stoc, 3, zero));
Push(mainQueue, CreateTurn(stoc, 3, zero));
Push(mainQueue, CreateTurn(ctos, K, 3));
Push(mainQueue, CreateTurn(ctos, S, 3));

Push(mainQueue, CreateTurn(ctos, K, 3));
Push(mainQueue, CreateTurn(ctos, S, 3));
Push(mainQueue, CreateTurn(stoc, 3, get));
Push(mainQueue, CreateTurn(ctos, K, 3));
Push(mainQueue, CreateTurn(ctos, S, 3));
Push(mainQueue, CreateTurn(stoc, 3, get));

Push(mainQueue, CreateTurn(ctos, zero, 0));
Push(mainQueue, CreateTurn(stoc, 0, zero));
Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(ctos, dbl, 0));
Push(mainQueue, CreateTurn(ctos, dbl, 0));

Push(mainQueue, CreateTurn(stoc, 3, zero));
/*end betta*/

/*alpha*/

Push(mainQueue, CreateTurn(stoc, 2, zero));
Push(mainQueue, CreateTurn(ctos, attack, 2));
Push(mainQueue, CreateTurn(ctos, K, 2));
Push(mainQueue, CreateTurn(ctos, S, 2));
Push(mainQueue, CreateTurn(stoc, 2, I));
Push(mainQueue, CreateTurn(ctos, S, 2));

Push(mainQueue, CreateTurn(ctos, K, 2));
Push(mainQueue, CreateTurn(ctos, S, 2));
Push(mainQueue, CreateTurn(stoc, 2, get));
Push(mainQueue, CreateTurn(ctos, K, 2));
Push(mainQueue, CreateTurn(ctos, S, 2));
Push(mainQueue, CreateTurn(stoc, 2, get));

Push(mainQueue, CreateTurn(ctos, succ, 0));

Push(mainQueue, CreateTurn(stoc, 2, zero));
/*end alpha*/

/*C*/

Push(mainQueue, CreateTurn(ctos, S, 2));

Push(mainQueue, CreateTurn(ctos, K, 2));
Push(mainQueue, CreateTurn(ctos, S, 2));
Push(mainQueue, CreateTurn(stoc, 2, get));
Push(mainQueue, CreateTurn(ctos, K, 2));
Push(mainQueue, CreateTurn(ctos, S, 2));
Push(mainQueue, CreateTurn(stoc, 2, get));

Push(mainQueue, CreateTurn(ctos, zero, 0));
Push(mainQueue, CreateTurn(stoc, 0, zero));
Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(ctos, dbl, 0));
Push(mainQueue, CreateTurn(ctos, succ, 0));

Push(mainQueue, CreateTurn(stoc, 2, zero));

//S(C)(succ)

Push(mainQueue, CreateTurn(ctos, S, 2));
Push(mainQueue, CreateTurn(stoc, 2, succ));

/*end C*/

/*H*/

Push(mainQueue, CreateTurn(ctos, dbl, 0));
Push(mainQueue, CreateTurn(stoc, 1, zero));
Push(mainQueue, CreateTurn(ctos, get, 1));
Push(mainQueue, CreateTurn(ctos, get, 1));
Push(mainQueue, CreateTurn(ctos, S, 1));

Push(mainQueue, CreateTurn(ctos, K, 1));
Push(mainQueue, CreateTurn(ctos, S, 1));
Push(mainQueue, CreateTurn(stoc, 1, get));
Push(mainQueue, CreateTurn(ctos, K, 1));
Push(mainQueue, CreateTurn(ctos, S, 1));
Push(mainQueue, CreateTurn(stoc, 1, get));

Push(mainQueue, CreateTurn(ctos, zero, 0));
Push(mainQueue, CreateTurn(stoc, 0, zero));
Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(ctos, dbl, 0));

Push(mainQueue, CreateTurn(stoc, 1, zero));
/*end H*/

/*F*/

Push(mainQueue, CreateTurn(ctos, K, 1));
Push(mainQueue, CreateTurn(ctos, S, 1));
Push(mainQueue, CreateTurn(ctos, K, 1));
Push(mainQueue, CreateTurn(ctos, S, 1));
Push(mainQueue, CreateTurn(stoc, 1, get));
Push(mainQueue, CreateTurn(ctos, K, 1));
Push(mainQueue, CreateTurn(ctos, S, 1));
Push(mainQueue, CreateTurn(stoc, 1, get));

Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(ctos, dbl, 0));
Push(mainQueue, CreateTurn(ctos, succ, 0));

Push(mainQueue, CreateTurn(stoc, 1, zero));

/*end F*/

//Change Health add

Push(mainQueue, CreateTurn(ctos, zero, 8));
Push(mainQueue, CreateTurn(stoc, 8, zero));
Push(mainQueue, CreateTurn(ctos, succ, 8));
Push(mainQueue, CreateTurn(ctos, succ, 8));
Push(mainQueue, CreateTurn(ctos, succ, 8));
for (i = 0; i < 14; i++)
    Push(mainQueue, CreateTurn(ctos, dbl, 8));
//Copy function
Push(mainQueue, CreateTurn(ctos, zero, 0));
Push(mainQueue, CreateTurn(stoc, 0, zero));
Push(mainQueue, CreateTurn(ctos, succ, 0));
Push(mainQueue, CreateTurn(ctos, get, 0));

/*Recursive*/
Push(mainQueue, CreateTurn(ctos, zero, 1));
for (int j =0; j <=255; j++)
{
for (i=0; i<3; i++)
{
    Push(mainQueue, CreateTurn(stoc, 1, zero));
    Push(mainQueue, CreateTurn(ctos, get, 1));
    Push(mainQueue, CreateTurn(stoc, 1, zero));
}
    Push(mainQueue, CreateTurn(ctos, succ, 11));
}

	}
  /*if (!mainQueue.m_size)
    exit(0);*/
	return Pop(mainQueue);
}

int main(int argc, char* argv[])
{
	if (argc!=2)
		return 1;
	Init();
  
  if (atoi(argv[1])==1)
	{
		ReadStep();
	}
	while (true)
	{
		WriteStep(Logic());
		TurnNumber++;
		ReadStep();
	}
	return 0;
}

