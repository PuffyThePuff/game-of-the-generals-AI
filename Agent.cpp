#include "Agent.h"
#include<iostream>

Agent::Agent() {
	G = new Graph();

	for (unsigned int i = 0; i < 21; i++) {
		Piece::MoveType move;
		
		move = Piece::Up;
		Move* moveUp = new Move();
		moveUp->pieceIndex = i;
		moveUp->moveType = move;
		possibleMoves.push_back(moveUp);

		move = Piece::Right;
		Move* moveRight = new Move();
		moveRight->pieceIndex = i;
		moveRight->moveType = move;
		possibleMoves.push_back(moveRight);

		move = Piece::Down;
		Move* moveDown = new Move();
		moveDown->pieceIndex = i;
		moveDown->moveType = move;
		possibleMoves.push_back(moveDown);

		move = Piece::Left;
		Move* moveLeft = new Move();
		moveLeft->pieceIndex = i;
		moveLeft->moveType = move;
		possibleMoves.push_back(moveLeft);
	}
}

Agent::~Agent() {
	possibleMoves.clear();
	G->adjList.clear();
}

void Agent::generateNext(State* current, vector<Piece*> pieceList, bool isWhite) {
	for (unsigned int j = 0; j < possibleMoves.size(); j++) {
		State* next = new State();
		copy(&current->boardState[0][0], &current->boardState[0][0] + 8 * 9, &next->boardState[0][0]);

		int iPiece = possibleMoves[j]->pieceIndex;
		Piece* toMove = pieceList[iPiece];

		if (toMove->isAlive) {
			int row = toMove->currentRow;
			int col = toMove->currentCol;
			next->boardState[row][col].isOccupied = false;
			next->boardState[row][col].piece = NULL;

			switch (possibleMoves[j]->moveType) {
				// (to set up tomorrow; add a check validity function)
			}

			G->add(current, next, possibleMoves[j]);
		}
	}
}

int Agent::getScore(State* V) {
	int score = 0;

	return score;
}

Move* Agent::getNextMove(State* current, vector<Piece*> whitePieces, vector<Piece*> blackPieces) {
	generateNext(current, blackPieces, false);
	ChildList* list = G->getConnections(current);
	Move* move = NULL;
	int bestScore = INFINITY;
	
	G->adjList[current]->clear();
	G->adjList.clear();
	return move;
}

void Agent::reset() {
}