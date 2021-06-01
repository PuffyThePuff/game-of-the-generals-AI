#include"State.h"
#include"Move.h"
#include"Graph.h"
#include"Piece.h"
#include<algorithm>
#include<random>
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
	const int SAMPLE_SIZE = 20;

	void generateNext(State*, vector<Piece*>);
	float getBlackScore(State* state, Piece* flag, Move* move);
	float getWhiteScore(State* state);
	float getOffensiveScore(State* state);
	float getDefensiveScore(State* state, bool isWhite);
	float getOpennessScore(State* state, bool isWhite);
	bool flagIsSafe(State* state, Piece* flag);
	bool flagIsSafe(State* state, Piece* flag, Piece::MoveType direction);
	float getWinProbability(int rank);
	float getElimProbability(int rank);
};