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

float Agent::getScore(Move* move, State* state, vector<Piece*> ownPieces, vector<Piece*> enemyPieces) {
	float score = 0.f;
	score += getOffensiveScore(move, state, ownPieces);
	score += getDefensiveScore(move, state, ownPieces, enemyPieces);
	score += getOpennessScore(state, ownPieces);
	if (!flagIsSafe(state, ownPieces[0])) score += FLAG_PENALTY;
	return score;
}

float Agent::getOffensiveScore(Move* move, State* state, vector<Piece*> ownPieces) {
	float score = 0.f;
	
	for (int i = 0; i < ownPieces.size(); i++) {
		if (ownPieces[i]->isAlive && i != move->pieceIndex) {
			score += POINTS_PER_PIECE * getWinProbability(ownPieces[i]->rank);

			int row = ownPieces[i]->currentRow;
			int col = ownPieces[i]->currentCol;

			// Check adjacent spaces for enemies.
			if (state->boardState[row][col].isOccupied && state->boardState[row][col].piece != NULL) {
				// Check space above.
				if (
					state->boardState[row - 1][col].isOccupied &&
					(state->boardState[row - 1][col].piece->team != ownPieces[i]->team)
				) {
					score += POINTS_PER_PIECE * getWinProbability(ownPieces[i]->rank);
				}

				// Check space below.
				if (
					state->boardState[row + 1][col].isOccupied &&
					(state->boardState[row + 1][col].piece->team != ownPieces[i]->team)
				) {
					score += POINTS_PER_PIECE * getWinProbability(ownPieces[i]->rank);
				}

				// Check space to the left.
				if (
					state->boardState[row][col - 1].isOccupied &&
					(state->boardState[row][col - 1].piece->team != ownPieces[i]->team)
				) {
					score += POINTS_PER_PIECE * getWinProbability(ownPieces[i]->rank);
				}

				// Check space to the right.
				if (
					state->boardState[row][col + 1].isOccupied &&
					(state->boardState[row][col + 1].piece->team != ownPieces[i]->team)
				) {
					score += POINTS_PER_PIECE * getWinProbability(ownPieces[i]->rank);
				}
			}
		}
	}

	Piece* last = ownPieces[move->pieceIndex];
	int row = last->currentRow;
	int col = last->currentCol;
	switch (move->moveType) {
	case Piece::Up:
		row -= 1;
		break;
	case Piece::Right:
		col += 1;
		break;
	case Piece::Down:
		row += 1;
		break;
	case Piece::Left:
		col -= 1;
		break;
	}

	if (state->boardState[row][col].isOccupied && state->boardState[row][col].piece == NULL) {
		if (
			state->boardState[row][col].challengers[0] != NULL &&
			state->boardState[row][col].challengers[1] != NULL
		) {
			score += (POINTS_PER_PIECE * getWinProbability(last->rank));
		}
	}

	return score;
}

float Agent::getDefensiveScore(Move* move, State* state, vector<Piece*> ownPieces, vector<Piece*> enemyPieces) {
	float score = 0.f;

	for (int i = 0; i < enemyPieces.size(); i++) {
		if (enemyPieces[i]->isAlive) {
			score -= POINTS_PER_PIECE * 0.424;
				// Average win probability.
		}
	}

	for (int i = 0; i < ownPieces.size(); i++) {
		if (ownPieces[i]->isAlive && i != move->pieceIndex) {
			int row = ownPieces[i]->currentRow;
			int col = ownPieces[i]->currentCol;

			// Check adjacent spaces for enemies.
			if (state->boardState[row][col].isOccupied && state->boardState[row][col].piece != NULL) {
				// Check space above.
				if (
					state->boardState[row - 1][col].isOccupied &&
					(state->boardState[row - 1][col].piece->team != ownPieces[i]->team)
				) {
					score -= POINTS_PER_PIECE * getElimProbability(ownPieces[i]->rank);
				}

				// Check space below.
				if (
					state->boardState[row + 1][col].isOccupied &&
					(state->boardState[row + 1][col].piece->team != ownPieces[i]->team)
				) {
					score -= POINTS_PER_PIECE * getElimProbability(ownPieces[i]->rank);
				}

				// Check space to the left.
				if (
					state->boardState[row][col - 1].isOccupied &&
					(state->boardState[row][col - 1].piece->team != ownPieces[i]->team)
				) {
					score -= POINTS_PER_PIECE * getElimProbability(ownPieces[i]->rank);
				}

				// Check space to the right.
				if (
					state->boardState[row][col + 1].isOccupied &&
					(state->boardState[row][col + 1].piece->team != ownPieces[i]->team)
				) {
					score -= POINTS_PER_PIECE * getElimProbability(ownPieces[i]->rank);
				}
			}
		}
	}

	Piece* last = ownPieces[move->pieceIndex];
	int row = last->currentRow;
	int col = last->currentCol;
	switch (move->moveType) {
	case Piece::Up:
		row -= 1;
		break;
	case Piece::Right:
		col += 1;
		break;
	case Piece::Down:
		row += 1;
		break;
	case Piece::Left:
		col -= 1;
		break;
	}

	if (state->boardState[row][col].isOccupied && state->boardState[row][col].piece == NULL) {
		if (
			state->boardState[row][col].challengers[0] != NULL &&
			state->boardState[row][col].challengers[1] != NULL
		) {
			score -= (POINTS_PER_PIECE * getElimProbability(last->rank));
		}
	}

	return score;
}

float Agent::getOpennessScore(State* state, vector<Piece*> ownPieces) {
	float score = 0.f;

	for (int i = 0; i < ownPieces.size(); i++) {
		if (ownPieces[i]->isAlive) {
			int row = ownPieces[i]->currentRow;
			int col = ownPieces[i]->currentCol;

			// Check adjacent spaces if free.
			if (state->boardState[row][col].isOccupied && state->boardState[row][col].piece != NULL) {
				// Check space above.
				if (!state->boardState[row - 1][col].isOccupied) {
					score += 1.f;
				}

				// Check space below.
				if (!state->boardState[row + 1][col].isOccupied) {
					score += 1.f;
				}

				// Check space to the left.
				if (!state->boardState[row][col - 1].isOccupied) {
					score += 1.f;
				}

				// Check space to the right.
				if (!state->boardState[row][col + 1].isOccupied) {
					score += 1.f;
				}
			}
		}
	}

	return score;
}

bool Agent::flagIsSafe(State* state, Piece* flag) {
	int row = flag->currentRow;
	int col = flag->currentCol;

	if (state->boardState[row][col].isOccupied && state->boardState[row][col].piece != NULL) {
		// Check space above.
		if (
			state->boardState[row - 1][col].isOccupied &&
			(state->boardState[row - 1][col].piece->team != flag->team)
		) {
			return false;
		}

		// Check space below.
		if (
			state->boardState[row + 1][col].isOccupied &&
			(state->boardState[row + 1][col].piece->team != flag->team)
		) {
			return false;
		}

		// Check space to the left.
		if (
			state->boardState[row][col - 1].isOccupied &&
			(state->boardState[row][col - 1].piece->team != flag->team)
		) {
			return false;
		}

		// Check space to the right.
		if (
			state->boardState[row][col + 1].isOccupied &&
			(state->boardState[row][col + 1].piece->team != flag->team)
		) {
			return false;
		}
	}

	return true;
}

float Agent::getWinProbability(int rank) {
	switch (rank) {
	case 0: return 1.f / 21.f;
	case 1: return 9.f / 21.f;
	case 14: return 15.f / 21.f;
	default: return float(6.f + rank) / 21.f;
	}
}

float Agent::getElimProbability(int rank) {
	switch (rank) {
	case 0: return 1.f;
	case 1: return 18.f / 21.f;
	case 14: return 8.f / 21.f;
	default: return float(16.f - rank) / 21.f;
	}
}

Move* Agent::getNextMove(State* current, vector<Piece*> whitePieces, vector<Piece*> blackPieces) {
	generateNext(current, blackPieces);
	ChildList* list = G->getConnections(current);
	Move* move = NULL;
	float bestScore = INFINITY;

	for (unsigned int i = 0; i < possibleMoves.size(); i++) {
		State* check = list->at(possibleMoves[i]);
		if (check != NULL) {
			float score = getScore(possibleMoves[i], check, blackPieces, whitePieces);
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