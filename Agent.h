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
	const float POINTS_PER_PIECE = 10.f;
	const float FLAG_PENALTY = -10000.f;

	void generateNext(State*, vector<Piece*>);
	float getScore(Move* move, State* state, vector<Piece*> ownPieces, vector<Piece*> enemyPieces);
	float getOffensiveScore(Move* move, State* state, vector<Piece*> ownPieces);
	float getDefensiveScore(Move* move, State* state, vector<Piece*> ownPieces, vector<Piece*> enemyPieces);
	float getOpennessScore(State* state, vector<Piece*> ownPieces);
	bool flagIsSafe(State* state, Piece* flag);
	float getWinProbability(int rank);
	float getElimProbability(int rank);
};