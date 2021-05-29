#include"State.h"
#include"Move.h"
#include<unordered_map>
#pragma once
using namespace std;

typedef unordered_map<Move, State*> ChildList;
typedef unordered_map<State*, ChildList*> AdjList;

class Graph {
public:
	AdjList adjList;

	Graph() {}
	void add(State* V, State* neighbor, Move action);
	ChildList* getConnections(State* V);
};