#pragma once
#include "Piece.h"

class Bishop : public Piece
{
public:
	Bishop(Board& new_board, Coordinate new_coordinate, TeamType new_team);

private:
	shared_ptr<set<Coordinate>> GetPseudoMoves() override;

public:
	shared_ptr<set<Coordinate>> GetCheckedSquares() override;
	shared_ptr<set<Coordinate>> GetLegalMoves() override;
	int GetCentipawnValue() override;
	void Draw() override;

private:
	static constexpr int BISHOP_VALUE = 350;
	static constexpr int BISHOP_TABLE[2][8][8] =
	{
		{
			{-20,-10,-10,-10,-10,-10,-10,-20},
		    {-10,  0,  0,  0,  0,  0,  0,-10},
		    {-10,  0,  5, 10, 10,  5,  0,-10},
		    {-10,  5,  5, 10, 10,  5,  5,-10},
		    {-10,  0, 10, 10, 10, 10,  0,-10},
		    {-10, 10, 10, 10, 10, 10, 10,-10},
		    {-10,  5,  0,  0,  0,  0,  5,-10},
		    {-20,-10,-10,-10,-10,-10,-10,-20}
		},
		{
			{-20,-10,-10,-10,-10,-10,-10,-20},
			{-10,  5,  0,  0,  0,  0,  5,-10},
			{-10, 10, 10, 10, 10, 10, 10,-10},
			{-10,  0, 10, 10, 10, 10,  0,-10},
			{-10,  5,  5, 10, 10,  5,  5,-10},
			{-10,  0,  5, 10, 10,  5,  0,-10},
			{-10,  0,  0,  0,  0,  0,  0,-10},
			{-20,-10,-10,-10,-10,-10,-10,-20}
		}
	};
};

