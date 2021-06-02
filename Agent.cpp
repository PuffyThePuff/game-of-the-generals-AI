#include "Agent.h"

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

			switch (possibleMoves[j]->moveType) {
			case Piece::Up:
				if (row > 0) {
					if (!next->boardState[row - 1][col].isOccupied) {
						next->boardState[row][col].isOccupied = false;
						next->boardState[row][col].piece = NULL;
						next->boardState[row - 1][col].isOccupied = true;
						next->boardState[row - 1][col].piece = toMove;
						G->add(current, next, possibleMoves[j]);
					}

					else if (
						next->boardState[row - 1][col].piece != NULL &&
						toMove->team != next->boardState[row - 1][col].piece->team
					) {
						next->boardState[row][col].isOccupied = false;
						next->boardState[row][col].piece = NULL;
						next->boardState[row - 1][col].challengers[0] = toMove;
						next->boardState[row - 1][col].challengers[1] = next->boardState[row - 1][col].piece;
						next->boardState[row - 1][col].piece = NULL;
						G->add(current, next, possibleMoves[j]);
					}

					else G->add(current, NULL, possibleMoves[j]);
				}
				
				else G->add(current, NULL, possibleMoves[j]);
				break;
			case Piece::Right:
				if (col < 8) {
					if (!next->boardState[row][col + 1].isOccupied) {
						next->boardState[row][col].isOccupied = false;
						next->boardState[row][col].piece = NULL;
						next->boardState[row][col + 1].isOccupied = true;
						next->boardState[row][col + 1].piece = toMove;
						G->add(current, next, possibleMoves[j]);
					}

					else if (
						next->boardState[row][col + 1].piece != NULL &&
						toMove->team != next->boardState[row][col + 1].piece->team
					) {
						next->boardState[row][col].isOccupied = false;
						next->boardState[row][col].piece = NULL;
						next->boardState[row][col + 1].challengers[0] = toMove;
						next->boardState[row][col + 1].challengers[1] = next->boardState[row][col + 1].piece;
						next->boardState[row][col + 1].piece = NULL;
						G->add(current, next, possibleMoves[j]);
					}

					else G->add(current, NULL, possibleMoves[j]);
				}
				
				G->add(current, NULL, possibleMoves[j]);
				break;
			case Piece::Down:
				if (row < 7) {
					if (!next->boardState[row + 1][col].isOccupied) {
						next->boardState[row][col].isOccupied = false;
						next->boardState[row][col].piece = NULL;
						next->boardState[row + 1][col].isOccupied = true;
						next->boardState[row + 1][col].piece = toMove;
						G->add(current, next, possibleMoves[j]);
					}

					else if (
						next->boardState[row + 1][col].piece != NULL &&
						toMove->team != next->boardState[row + 1][col].piece->team
					) {
						next->boardState[row][col].isOccupied = false;
						next->boardState[row][col].piece = NULL;
						next->boardState[row + 1][col].challengers[0] = toMove;
						next->boardState[row + 1][col].challengers[1] = next->boardState[row + 1][col].piece;
						next->boardState[row + 1][col].piece = NULL;
						G->add(current, next, possibleMoves[j]);
					}

					else G->add(current, NULL, possibleMoves[j]);
				}
				
				else G->add(current, NULL, possibleMoves[j]);
				break;
			case Piece::Left:
				if (col > 0) {
					if (!next->boardState[row][col - 1].isOccupied) {
						next->boardState[row][col].isOccupied = false;
						next->boardState[row][col].piece = NULL;
						next->boardState[row][col - 1].isOccupied = true;
						next->boardState[row][col - 1].piece = toMove;
						G->add(current, next, possibleMoves[j]);
					}

					else if (
						next->boardState[row][col - 1].piece != NULL &&
						toMove->team != next->boardState[row][col - 1].piece->team
					) {
						next->boardState[row][col].isOccupied = false;
						next->boardState[row][col].piece = NULL;
						next->boardState[row][col - 1].challengers[0] = toMove;
						next->boardState[row][col - 1].challengers[1] = next->boardState[row][col - 1].piece;
						next->boardState[row][col - 1].piece = NULL;
						G->add(current, next, possibleMoves[j]);
					}

					else G->add(current, NULL, possibleMoves[j]);
				}
				
				else G->add(current, NULL, possibleMoves[j]);
				break;
			}
		}

		else G->add(current, NULL, possibleMoves[j]);
	}
}

float Agent::getBlackScore(State* state, Piece* flag, Move* move) {
	float score = 0.f;
	score += getOffensiveScore(state);
	score += getDefensiveScore(state, false);
	score += getOpennessScore(state, false);
	
	if (move->pieceIndex == 0) {
		if(!flagIsSafe(state, flag, move->moveType)) score += FLAG_PENALTY;
	}
	else {
		if (!flagIsSafe(state, flag)) score += FLAG_PENALTY;
	}
	return score;
}

float Agent::getWhiteScore(State* state) {
	float score = 0.f;
	score += getDefensiveScore(state, true);
	score += getOpennessScore(state, true);
	return score;
}

float Agent::getOffensiveScore(State* state) {
	float score = 0.f;
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			// Check if this piece is one of the agent's remaining pieces.
			if (
				state->boardState[i][j].isOccupied &&
				state->boardState[i][j].piece != NULL &&
				state->boardState[i][j].piece->isAlive &&
				!state->boardState[i][j].piece->team
			) {
				// Add points per piece left based on their probability of winning a matchup.
				score += OFFENSE_POINTS_PER_PIECE * getWinProbability(state->boardState[i][j].piece->rank);

				// Add points based on how close the flag is to the end.
				if (state->boardState[i][j].piece->rank == 0) {
					score += FLAG_BUFF * i;
				}

				// Check adjacent spaces for enemies, and add points based on probability of winning
				// a potential challenge.

				// Check space above.
				if (
					i > 0 &&
					state->boardState[i - 1][j].isOccupied &&
					state->boardState[i - 1][j].piece != NULL &&
					(state->boardState[i - 1][j].piece->team != state->boardState[i][j].piece->team)
				) {
					score += OFFENSE_POINTS_PER_PIECE * getWinProbability(state->boardState[i][j].piece->rank);
				}

				// Check space below.
				if (
					i < 7 &&
					state->boardState[i + 1][j].isOccupied &&
					state->boardState[i + 1][j].piece != NULL &&
					(state->boardState[i + 1][j].piece->team != state->boardState[i][j].piece->team)
					) {
					score += OFFENSE_POINTS_PER_PIECE * getWinProbability(state->boardState[i][j].piece->rank);
				}

				// Check space to the left.
				if (
					j > 0 &&
					state->boardState[i][j - 1].isOccupied &&
					state->boardState[i][j - 1].piece != NULL &&
					(state->boardState[i][j - 1].piece->team != state->boardState[i][j].piece->team)
					) {
					score += OFFENSE_POINTS_PER_PIECE * getWinProbability(state->boardState[i][j].piece->rank);
				}

				// Check space to the right.
				if (
					j < 8 &&
					state->boardState[i][j + 1].isOccupied &&
					state->boardState[i][j + 1].piece != NULL &&
					(state->boardState[i][j + 1].piece->team != state->boardState[i][j].piece->team)
					) {
					score += OFFENSE_POINTS_PER_PIECE * getWinProbability(state->boardState[i][j].piece->rank);
				}
			}

			// Check if this move involves challenging an enemy piece, and add points
			// based on probability of winning.
			else if (
				state->boardState[i][j].isOccupied &&
				state->boardState[i][j].piece == NULL &&
				state->boardState[i][j].challengers[0] != NULL &&
				state->boardState[i][j].challengers[1] != NULL
			) {
				score += OFFENSE_POINTS_PER_PIECE * getWinProbability(state->boardState[i][j].challengers[0]->rank);
			}
		}
	}

	return score;
}

float Agent::getDefensiveScore(State* state, bool isWhite) {
	float score = 0.f;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			// Check if this is one of the opponent's remaining pieces.
			if (
				state->boardState[i][j].isOccupied &&
				state->boardState[i][j].piece != NULL &&
				state->boardState[i][j].piece->isAlive &&
				state->boardState[i][j].piece->team != isWhite
			) {
				// Deduct points per piece left based on average probability of winning a matchup.
				score -= DEFENSE_POINTS_PER_PIECE * 0.424;
			}

			// Only perform if evaluating the agent's score.
			if (!isWhite) {
				// Check if this piece is one of the agent's remaining pieces.
				if (
					state->boardState[i][j].isOccupied &&
					state->boardState[i][j].piece != NULL &&
					state->boardState[i][j].piece->isAlive &&
					!state->boardState[i][j].piece->team
				) {
					// Deduct points per piece left based on their probability of
					// getting eliminated in a matchup.
					score -= DEFENSE_POINTS_PER_PIECE * getElimProbability(state->boardState[i][j].piece->rank);

					// Check adjacent spaces for enemies, and deduct points based on probability of
					// getting eliminated in a potential challenge.

					// Check space above.
					if (
						i > 0 &&
						state->boardState[i - 1][j].isOccupied &&
						state->boardState[i - 1][j].piece != NULL &&
						(state->boardState[i - 1][j].piece->team != state->boardState[i][j].piece->team)
					) {
						score -= DEFENSE_POINTS_PER_PIECE * getElimProbability(state->boardState[i][j].piece->rank);
					}

					// Check space below.
					if (
						i < 7 &&
						state->boardState[i + 1][j].isOccupied &&
						state->boardState[i + 1][j].piece != NULL &&
						(state->boardState[i + 1][j].piece->team != state->boardState[i][j].piece->team)
					) {
						score -= DEFENSE_POINTS_PER_PIECE * getElimProbability(state->boardState[i][j].piece->rank);
					}

					// Check space to the left.
					if (
						j > 0 &&
						state->boardState[i][j - 1].isOccupied &&
						state->boardState[i][j - 1].piece != NULL &&
						(state->boardState[i][j - 1].piece->team != state->boardState[i][j].piece->team)
					) {
						score -= DEFENSE_POINTS_PER_PIECE * getElimProbability(state->boardState[i][j].piece->rank);
					}

					// Check space to the right.
					if (
						j < 8 &&
						state->boardState[i][j + 1].isOccupied &&
						state->boardState[i][j + 1].piece != NULL &&
						(state->boardState[i][j + 1].piece->team != state->boardState[i][j].piece->team)
					) {
						score -= DEFENSE_POINTS_PER_PIECE * getElimProbability(state->boardState[i][j].piece->rank);
					}
				}

				// Check if this move involves challenging an enemy piece, and deduct points
				// based on probability of getting eliminated.
				else if (
					state->boardState[i][j].isOccupied &&
					state->boardState[i][j].piece == NULL &&
					state->boardState[i][j].challengers[0] != NULL &&
					state->boardState[i][j].challengers[1] != NULL
					) {
					score -= DEFENSE_POINTS_PER_PIECE * getElimProbability(state->boardState[i][j].challengers[0]->rank);
				}
			}
		}
	}

	return score;
}

float Agent::getOpennessScore(State* state, bool isWhite) {
	float score = 0.f;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			// Check adjacent spaces if free.
			if (
				state->boardState[i][j].isOccupied &&
				state->boardState[i][j].piece != NULL &&
				state->boardState[i][j].piece->team == isWhite
			) {
				// Check space above.
				if (i > 0 && !state->boardState[i - 1][j].isOccupied) {
					score += 1.f;
				}

				// Check space below.
				if (i < 7 && !state->boardState[i + 1][j].isOccupied) {
					score += 1.f;
				}

				// Check space to the left.
				if (j > 0 && !state->boardState[i][j - 1].isOccupied) {
					score += 1.f;
				}

				// Check space to the right.
				if (j < 8 && !state->boardState[i][j + 1].isOccupied) {
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
			row > 0 &&
			state->boardState[row - 1][col].isOccupied &&
			state->boardState[row - 1][col].piece != NULL &&
			(state->boardState[row - 1][col].piece->team != flag->team)
		) {
			return false;
		}

		// Check space below.
		if (
			row < 7 &&
			state->boardState[row + 1][col].isOccupied &&
			state->boardState[row + 1][col].piece != NULL &&
			(state->boardState[row + 1][col].piece->team != flag->team)
		) {
			return false;
		}

		// Check space to the left.
		if (
			col > 0 &&
			state->boardState[row][col - 1].isOccupied &&
			state->boardState[row][col - 1].piece != NULL &&
			(state->boardState[row][col - 1].piece->team != flag->team)
		) {
			return false;
		}

		// Check space to the right.
		if (
			col < 8 &&
			state->boardState[row][col + 1].isOccupied &&
			state->boardState[row][col + 1].piece != NULL &&
			(state->boardState[row][col + 1].piece->team != flag->team)
		) {
			return false;
		}
	}

	else if (
		state->boardState[row][col].piece == NULL &&
		state->boardState[row][col].challengers[0] != NULL &&
		state->boardState[row][col].challengers[1] != NULL
	) {
		return false;
	}

	return true;
}

bool Agent::flagIsSafe(State* state, Piece* flag, Piece::MoveType move) {
	int row = flag->currentRow;
	int col = flag->currentCol;

	switch (move) {
	case Piece::Up: row -= 1; break;
	case Piece::Right: col += 1; break;
	case Piece::Down: row += 1; break;
	case Piece::Left: col -= 1; break;
	}

	if (state->boardState[row][col].isOccupied && state->boardState[row][col].piece != NULL) {
		// Check space above.
		if (
			row > 0 &&
			state->boardState[row - 1][col].isOccupied &&
			state->boardState[row - 1][col].piece != NULL &&
			(state->boardState[row - 1][col].piece->team != flag->team)
			) {
			return false;
		}

		// Check space below.
		if (
			row < 7 &&
			state->boardState[row + 1][col].isOccupied &&
			state->boardState[row + 1][col].piece != NULL &&
			(state->boardState[row + 1][col].piece->team != flag->team)
			) {
			return false;
		}

		// Check space to the left.
		if (
			col > 0 &&
			state->boardState[row][col - 1].isOccupied &&
			state->boardState[row][col - 1].piece != NULL &&
			(state->boardState[row][col - 1].piece->team != flag->team)
			) {
			return false;
		}

		// Check space to the right.
		if (
			col < 8 &&
			state->boardState[row][col + 1].isOccupied &&
			state->boardState[row][col + 1].piece != NULL &&
			(state->boardState[row][col + 1].piece->team != flag->team)
			) {
			return false;
		}
	}

	else if (
		state->boardState[row][col].piece == NULL &&
		state->boardState[row][col].challengers[0] != NULL &&
		state->boardState[row][col].challengers[1] != NULL
	) {
		return false;
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
	float bestScore = -INFINITY;
	std::shuffle(possibleMoves.begin(), possibleMoves.end(), std::default_random_engine());
	int stateCount = 0;

	for (unsigned int i = 0; i < possibleMoves.size(); i++) {
		State* check = list->at(possibleMoves[i]);
		if (check != NULL) {
			float score = getBlackScore(check, blackPieces[0], possibleMoves[i]);
			generateNext(check, whitePieces);
			ChildList* list2 = G->getConnections(check);
			float counterScore = 0.f;
			int counterCount = 0;
			for (int j = possibleMoves.size() - 1; j >= 0; j--) {
				State* check2 = list2->at(possibleMoves[j]);
				if (check2 != NULL) {
					counterScore += getWhiteScore(check2);
					counterCount++;
				}

				if (counterCount >= SAMPLE_SIZE) break;
			}

			G->adjList[check]->clear();
			score -= float(counterScore / counterCount);
			if (score > bestScore) {
				bestScore = score;
				move = possibleMoves[i];
			}

			stateCount++;
		}

		if (stateCount >= SAMPLE_SIZE) break;
	}
	
	G->adjList[current]->clear();
	G->adjList.clear();
	return move;
}

void Agent::reset() {
}