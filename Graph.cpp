#include "Graph.h"

void Graph::add(State* V, State* neighbor, Move action) {
	if (adjList[V] == NULL) adjList[V] = new ChildList();
	pair<Move, State*> toInsert(action, neighbor);
	adjList[V]->insert(toInsert);
}

ChildList* Graph::getConnections(State* V) {
	if (adjList[V] != NULL) return adjList[V];
	else return NULL;
}