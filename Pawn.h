#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(Board& new_board, Coordinate new_coordinate, TeamType new_team);

private:
	shared_ptr<set<Coordinate>> GetPseudoMoves() override;

public:
	shared_ptr<set<Coordinate>> GetCheckedSquares() override;
	shared_ptr<set<Coordinate>> GetLegalMoves() override;
	int GetCentipawnValue() override;
	void Draw() override;

private:
	static constexpr int PAWN_VALUE = 100;
	static constexpr int PAWN_TABLE[2][8][8] =
	{
		{ 
			{ 0,  0,   0,  0,  0,  0,  0, 0},
		    {50, 50,  50, 50, 50, 50, 50, 5},
		    {10, 10,  20, 30, 30, 20, 10, 1},
		    { 5,  5,  10, 25, 25, 10,  5, 5},
		    { 0,  0,   0, 20, 20,  0,  0, 0},
		    { 5, -5, -10,  0,  0,-10, -5, 5},
		    { 5, 10,  10,-20,-20, 10, 10, 5},
		    { 0,  0,   0,  0,  0,  0,  0, 0}
		},
		{
			{ 0,  0,   0,  0,  0,  0,  0, 0},
			{ 5, 10,  10,-20,-20, 10, 10, 5},
			{ 5, -5, -10,  0,  0,-10, -5, 5},
			{ 0,  0,   0, 20, 20,  0,  0, 0},
			{ 5,  5,  10, 25, 25, 10,  5, 5},
			{10, 10,  20, 30, 30, 20, 10, 1},
			{50, 50,  50, 50, 50, 50, 50, 5},
			{ 0,  0,   0,  0,  0,  0,  0, 0}
		}
	};
};

