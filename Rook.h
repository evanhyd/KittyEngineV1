#pragma once
#include "Piece.h"

class Rook : public Piece
{
public:
	Rook(Board& new_board, Coordinate new_coordinate, TeamType new_team);

private:
	shared_ptr<set<Coordinate>> GetPseudoMoves() override;

public:
	shared_ptr<set<Coordinate>> GetCheckedSquares() override;
	shared_ptr<set<Coordinate>> GetLegalMoves() override;
	int GetCentipawnValue() override;
	void Draw() override;

private:
	static constexpr int ROOK_VALUE = 525;
	static constexpr int ROOK_TABLE[2][8][8] =
	{
		{
	        { 0,  0,  0,  0,  0,  0,  0,  0},
            { 5, 10, 10, 10, 10, 10, 10,  5},
            {-5,  0,  0,  0,  0,  0,  0, -5},
            {-5,  0,  0,  0,  0,  0,  0, -5},
            {-5,  0,  0,  0,  0,  0,  0, -5},
            {-5,  0,  0,  0,  0,  0,  0, -5},
            {-5,  0,  0,  0,  0,  0,  0, -5},
            { 0,  0,  0,  5,  5,  0,  0,  0}
		},
		{
			{ 0,  0,  0,  5,  5,  0,  0,  0},
			{-5,  0,  0,  0,  0,  0,  0, -5},
			{-5,  0,  0,  0,  0,  0,  0, -5},
			{-5,  0,  0,  0,  0,  0,  0, -5},
			{-5,  0,  0,  0,  0,  0,  0, -5},
			{-5,  0,  0,  0,  0,  0,  0, -5},
			{ 5, 10, 10, 10, 10, 10, 10,  5},
			{ 0,  0,  0,  0,  0,  0,  0,  0}
		}
	};
};

