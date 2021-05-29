#include"State.h"
#include"Move.h"
#include"Graph.h"
#include"Piece.h"
#include<algorithm>
#include<vector>
#pragma once

class Agent {
public:
	Agent();
	~Agent();

	Move* getNextMove(State* current, vector<Piece*> whitePieces, vector<Piece*> blackPieces);
	void reset();
private:
	Graph* G;
	vector<Move*> possibleMoves;

	void generateNext(State*, vector<Piece*>, bool);
	int getScore(State* V);
};