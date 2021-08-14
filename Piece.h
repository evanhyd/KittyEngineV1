#pragma once
#include <string>
#include <vector>
#include <set>
#include "TeamType.h"
#include "Coordinate.h"
#include "Board.h"

using namespace std;

class Piece
{

public:
	Board& board;
	Coordinate coordinate;
	shared_ptr<vector<Coordinate>> prev_coordinates;
	TeamType team;
	const int centipawn_value;
	int move_times;

protected:
	Piece(Board& new_board, Coordinate new_coordinate, TeamType new_team, int new_centipawn_value);

public:
	void UpdateCoordinate(const Coordinate& new_coordinate);
	void RestoreCoordinate();

	virtual shared_ptr<set<Coordinate>> GetCheckedSquares() = 0;
	virtual shared_ptr<set<Coordinate>> GetPseudoMoves() = 0;
	virtual shared_ptr<set<Coordinate>> GetLegalMoves() = 0;
	virtual void Draw() = 0;
	virtual int GetCentipawnValue() = 0;
};

