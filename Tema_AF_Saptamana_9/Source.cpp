/*
* Student: Spaczai Carla Noemi
* Grupa: 30229
* Am implementat operatiile de baza pe multimi disjuncte si algoritmul lui Kruskal folosind multimi disjuncte.
*
* makeSet:
* Aceasta operatie creeaza o multime noua ce contine un singur element primit ca parametru. Acesta va fi radacina si nodul reprezentativ al setului creat. Parintele
* acestui nod este el insusi.
*
* findSet:
* Aceasta operatie cauta prin intermediul relatiei de parinte multimea in care se afla elementul primit ca parametru si returneaza radacina multimii gasite. Atunci
* cand gasim un element al carui parinte este el insusi, cautarea ia sfarsit deoarece acesta este nodul reprezentativ al multimii (radacina).
*
* unionSet:
* Aceasta operatie realizeaza reuniunea dintre multimea care contine primul element primit ca parametru si multimea care contine al doilea element primit ca
* parametru. Reuniunea are loc doar daca cele doua elemente nu se afla deja in aceeasi multime.
* 
* Kruskal:
* Acest algoritm presupune crearea multimilor de cate un element si ordonarea multimii muchiilor in functie de "Weight" pentru a obtine arborele de acoperire minima.
* Luam pe rand fiecare muchie din multimea ordonata si verificam daca prin adaugarea ei arborelui de acoperire minima se creeaza cicluri. Daca nu obtinem cicluri
* prin adaugarea ei, o adaugam arborelui de acoperire minima, dar daca obtinem cicluri, nu o adaugam arborelui de acoperire minima.
* Complexitate: O(E*log(E)). (E = numarul de muchii)
*/

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "Profiler.h"

Profiler P("DS&K");

#define MAX_SIZE 10000
#define STEP_SIZE 100

typedef struct DS
{
	int Data;
	int Rank;
	struct DS* Parent;
} DS;

DS* makeSet(int Data)
{
	DS* Root = (DS*)malloc(sizeof(DS));
	Root->Data = Data;
	Root->Rank = 0;
	Root->Parent = Root;
	return Root;
}

DS* makeSet2(int Data, int n)
{
	Operation OP = P.createOperation("Kruskal", n);

	DS* Root = (DS*)malloc(sizeof(DS));
	Root->Data = Data;
	Root->Rank = 0;
	Root->Parent = Root;
	OP.count(3);
	return Root;
}

DS* findSet(DS** Forest, int Data)
{
	if (Forest[Data] != Forest[Data]->Parent)
	{
		Forest[Data]->Parent = findSet(Forest, Forest[Data]->Parent->Data);
	}
	return Forest[Data]->Parent;
}

DS* findSet2(DS** Forest, int Data, int n)
{
	Operation OP = P.createOperation("Kruskal", n);

	OP.count();
	if (Forest[Data] != Forest[Data]->Parent)
	{
		OP.count();
		Forest[Data]->Parent = findSet2(Forest, Forest[Data]->Parent->Data, n);
	}
	return Forest[Data]->Parent;
}

void unionSet(DS** Forest, int X, int Y)
{
	DS* XR = findSet(Forest, X);
	DS* YR = findSet(Forest, Y);
	if (XR->Rank > YR->Rank)
	{
		YR->Parent = XR;
	}
	else
	{
		XR->Parent = YR;
	}
	if (XR->Rank == YR->Rank)
	{
		YR->Rank = YR->Rank + 1;
	}
}

void unionSet2(DS** Forest, int X, int Y, int n)
{
	Operation OP = P.createOperation("Kruskal", n);

	DS* XR = findSet(Forest, X);
	DS* YR = findSet(Forest, Y);
	OP.count(2);
	if (XR->Rank > YR->Rank)
	{
		YR->Parent = XR;
		OP.count(2);
	}
	else
	{
		XR->Parent = YR;
		OP.count(2);
	}
	if (XR->Rank == YR->Rank)
	{
		OP.count();
		YR->Rank = YR->Rank + 1;
	}
}

void demo1()
{
	DS** Forest = (DS**)malloc(10 * sizeof(DS*));
	printf("Verificare makeSet:\n");
	for (int i = 0; i < 10; i++)
	{
		Forest[i] = makeSet(i);
		printf("Setul %d: Data = %d, PData = %d, Rank = %d\n", i, Forest[i]->Data, Forest[i]->Parent->Data, Forest[i]->Rank);
	}
	printf("\n");
	printf("Verificare findSet si unionSet:\n");
	unionSet(Forest, 2, 1);
	DS* FS = findSet(Forest, 1);
	printf("Data = %d, PData = %d, Rank = %d\n", FS->Data, FS->Parent->Data, FS->Rank);
	unionSet(Forest, 0, 1);
	FS = findSet(Forest, 1);
	printf("Data = %d, PData = %d, Rank = %d\n", FS->Data, FS->Parent->Data, FS->Rank);
	unionSet(Forest, 3, 4);
	FS = findSet(Forest, 3);
	printf("Data = %d, PData = %d, Rank = %d\n", FS->Data, FS->Parent->Data, FS->Rank);
	unionSet(Forest, 7, 9);
	FS = findSet(Forest, 7);
	printf("Data = %d, PData = %d, Rank = %d\n", FS->Data, FS->Parent->Data, FS->Rank);
	unionSet(Forest, 9, 3);
	FS = findSet(Forest, 9);
	printf("Data = %d, PData = %d, Rank = %d", FS->Data, FS->Parent->Data, FS->Rank);
}

typedef struct Edge
{
	int Node1;
	int Node2;
	int Weight;
} Edge;

bool COMPARE(Edge E1, Edge E2)
{
	return(E1.Weight < E2.Weight);
}

void Kruskal(int V, int E, Edge* EDGE)
{
	DS** Forest = (DS**)malloc(V * sizeof(DS*));
	for (int i = 0; i < V; i++)
	{
		Forest[i] = makeSet(i);
	}
	std::sort(EDGE, EDGE + E, COMPARE);
	printf("EDGE:\n");
	for (int i = 0; i < E; i++)
	{
		printf("Node1: %d, Node2: %d, Weight: %d\n", EDGE[i].Node1, EDGE[i].Node2, EDGE[i].Weight);
	}
	printf("\nKEEP:\n");
	for (int i = 0; i < E; i++)
	{
		if (findSet(Forest, EDGE[i].Node1) != findSet(Forest, EDGE[i].Node2))
		{
			unionSet(Forest, EDGE[i].Node1, EDGE[i].Node2);
			printf("Node1: %d, Node2: %d, Weight: %d\n", EDGE[i].Node1, EDGE[i].Node2, EDGE[i].Weight);
		}
	}
}

void Kruskal2(int V, int E, Edge* EDGE, int n)
{
	DS** Forest = (DS**)malloc(V * sizeof(DS*));
	for (int i = 0; i < V; i++)
	{
		Forest[i] = makeSet2(i, n);
	}
	std::sort(EDGE, EDGE + E, COMPARE);
	for (int i = 0; i < E; i++)
	{
		if (findSet2(Forest, EDGE[i].Node1, n) != findSet2(Forest, EDGE[i].Node2, n))
		{
			unionSet2(Forest, EDGE[i].Node1, EDGE[i].Node2, n);
		}
	}
}

void demo2()
{
	Edge EDGE[15];
	int q = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = i + 1; j < 5; j++)
		{
			EDGE[q].Node1 = i;
			EDGE[q].Node2 = j;
			EDGE[q].Weight = rand() % 5;
			q++;
		}
	}
	Kruskal(5, q, EDGE);
}

void perf()
{
	Edge EDGE[5 * MAX_SIZE];
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		EDGE[0].Node1 = 0;
		EDGE[0].Weight = rand() % STEP_SIZE;
		for (int i = 1; i < n - 1; i++)
		{
			EDGE[i].Node1 = i;
			EDGE[i - 1].Node2 = i;
			EDGE[i].Weight = rand() % STEP_SIZE;
		}
		EDGE[n - 2].Node2 = n - 1;
		for (int i = n - 1; i < 4 * n - 1; i++)
		{
			EDGE[i].Node1 = rand() % STEP_SIZE;
			EDGE[i].Node2 = rand() % STEP_SIZE;
			EDGE[i].Weight = rand() % STEP_SIZE;
		}
		Kruskal2(n, 4 * n, EDGE, n);
	}
	P.showReport();
}
 
int main()
{
	//demo1();
	demo2();
	//perf();
	return 0;
}