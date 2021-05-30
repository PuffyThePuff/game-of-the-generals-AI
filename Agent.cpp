#include "Agent.h"
#include<iostream>

Agent::Agent() {
	G = new Graph();

	for (unsigned int i = 0; i < 21; i++) {
		Move* moveUp = new Move();
		moveUp->pieceIndex = i;
		moveUp->moveType = Piece::Up;
		possibleMoves.push_back(moveUp);

		Move* moveRight = new Move();
		moveRight->pieceIndex = i;
		moveRight->moveType = Piece::Right;
		possibleMoves.push_back(moveRight);

		Move* moveDown = new Move();
		moveDown->pieceIndex = i;
		moveDown->moveType = Piece::Down;
		possibleMoves.push_back(moveDown);

		Move* moveLeft = new Move();
		moveLeft->pieceIndex = i;
		moveLeft->moveType = Piece::Left;
		possibleMoves.push_back(moveLeft);
	}
}

Agent::~Agent() {
	possibleMoves.clear();
	G->adjList.clear();
}

void Agent::generateNext(State* current, vector<Piece*> pieceList) {
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
			case Piece::Up:
				if (!next->boardState[row - 1][col].isOccupied) {
					next->boardState[row - 1][col].isOccupied = true;
					next->boardState[row - 1][col].piece = toMove;
					G->add(current, next, possibleMoves[j]);
				}

				else if (toMove->team != next->boardState[row-1][col].piece->team){
					next->boardState[row - 1][col].challengers[0] = toMove;
					next->boardState[row - 1][col].challengers[1] = next->boardState[row - 1][col].piece;
					next->boardState[row - 1][col].piece = NULL;
					G->add(current, next, possibleMoves[j]);
				}

				else G->add(current, NULL, possibleMoves[j]);

				break;
			case Piece::Right:
				if (!next->boardState[row][col + 1].isOccupied) {
					next->boardState[row][col + 1].isOccupied = true;
					next->boardState[row][col + 1].piece = toMove;
					G->add(current, next, possibleMoves[j]);
				}

				else if (toMove->team != next->boardState[row][col + 1].piece->team) {
					next->boardState[row][col + 1].challengers[0] = toMove;
					next->boardState[row][col + 1].challengers[1] = next->boardState[row][col + 1].piece;
					next->boardState[row][col + 1].piece = NULL;
					G->add(current, next, possibleMoves[j]);
				}

				else G->add(current, NULL, possibleMoves[j]);

				break;
			case Piece::Down:
				if (!next->boardState[row + 1][col].isOccupied) {
					next->boardState[row + 1][col].isOccupied = true;
					next->boardState[row + 1][col].piece = toMove;
					G->add(current, next, possibleMoves[j]);
				}

				else if (toMove->team != next->boardState[row + 1][col].piece->team) {
					next->boardState[row + 1][col].challengers[0] = toMove;
					next->boardState[row + 1][col].challengers[1] = next->boardState[row + 1][col].piece;
					next->boardState[row + 1][col].piece = NULL;
					G->add(current, next, possibleMoves[j]);
				}

				else G->add(current, NULL, possibleMoves[j]);

				break;
			case Piece::Left:
				if (!next->boardState[row][col - 1].isOccupied) {
					next->boardState[row][col - 1].isOccupied = true;
					next->boardState[row][col - 1].piece = toMove;
					G->add(current, next, possibleMoves[j]);
				}

				else if (toMove->team != next->boardState[row][col - 1].piece->team) {
					next->boardState[row][col - 1].challengers[0] = toMove;
					next->boardState[row][col - 1].challengers[1] = next->boardState[row][col - 1].piece;
					next->boardState[row][col - 1].piece = NULL;
					G->add(current, next, possibleMoves[j]);
				}

				else G->add(current, NULL, possibleMoves[j]);

				break;
			}
		}

		else G->add(current, NULL, possibleMoves[j]);
	}
}

int Agent::getScore(State* V) {
	int score = 0;

	return score;
}

Move* Agent::getNextMove(State* current, vector<Piece*> whitePieces, vector<Piece*> blackPieces) {
	generateNext(current, blackPieces);
	ChildList* list = G->getConnections(current);
	Move* move = NULL;
	int bestScore = INFINITY;

	for (unsigned int i = 0; i < possibleMoves.size(); i++) {
		State* check = list->at(possibleMoves[i]);
		if (check != NULL) {
			int score = getScore(check);
			if (score < bestScore) {
				bestScore = score;
				move = possibleMoves[i];
			}
		}
	}
	
	G->adjList[current]->clear();
	G->adjList.clear();
	return move;
}

void Agent::reset() {
}