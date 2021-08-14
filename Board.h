#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "TeamType.h"
#include "Coordinate.h"

using namespace std;

class Piece;
class King;

class Board
{
public:

	shared_ptr<Piece> chess_board[8][8];
	int checked_level[2][8][8];
	TeamType turn;

	shared_ptr<King> white_king;
	shared_ptr<King> black_king;
	vector<shared_ptr<Piece>> piece_moving_history;
	vector<shared_ptr<Piece>> dead_pieces;

	Board();

	void Reset();
	void CalcCheckedLevels();
	int EvaluateBoard();

	void MovePiece(const Coordinate& curr, const Coordinate& next);
	void UndoMovePiece(const Coordinate& srce);
	void RemovePiece(const Coordinate& srce);
	void UndoRemovePiece(const Coordinate& srce);
	void Promote(const Coordinate& srce, shared_ptr<Piece> promoted_piece);
	void UndoPromote(const Coordinate& srce);

	void Move(const Coordinate& srce, const Coordinate& dest);
	void UndoMove(const Coordinate& srce);
	bool PseudoMove(const Coordinate& srce, const Coordinate& dest, const map<Coordinate, shared_ptr<set<Coordinate>>>& map_piece_to_moves);
	
	void Render();
	int SimulateMove(int depth);
	void Start();
	void Debug();

	shared_ptr<Piece>& operator[](const Coordinate& coordinate);
};

