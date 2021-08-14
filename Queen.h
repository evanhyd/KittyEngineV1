#pragma once
#include "Piece.h"
class Queen : public Piece
{
public:
	Queen(Board& new_board, Coordinate new_coordinate, TeamType new_team);

private:
	shared_ptr<set<Coordinate>> GetMoves() override;

public:
	shared_ptr<set<Coordinate>> GetCheckedSquares() override;
	shared_ptr<set<Coordinate>> GetLegalMoves() override;
	int GetCentipawnValue() override;
	void Draw() override;

private:
	static constexpr int QUEEN_VALUE = 1000;
	static constexpr int QUEEN_TABLE[2][8][8] = 
	{
		{
			{-20,-10,-10, -5, -5,-10,-10,-20},
            {-10,  0,  0,  0,  0,  0,  0,-10},
            {-10,  0,  5,  5,  5,  5,  0,-10},
            { -5,  0,  5,  5,  5,  5,  0, -5},
            {  0,  0,  5,  5,  5,  5,  0, -5},
            {-10,  5,  5,  5,  5,  5,  0,-10},
            {-10,  0,  5,  0,  0,  0,  0,-10},
            {-20,-10,-10, -5, -5,-10,-10,-20}
		},
		{
			{-20,-10,-10, -5, -5,-10,-10,-20},
			{-10,  0,  5,  0,  0,  0,  0,-10},
			{-10,  5,  5,  5,  5,  5,  0,-10},
			{  0,  0,  5,  5,  5,  5,  0, -5},
			{ -5,  0,  5,  5,  5,  5,  0, -5},
			{-10,  0,  5,  5,  5,  5,  0,-10},
			{-10,  0,  0,  0,  0,  0,  0,-10},
			{-20,-10,-10, -5, -5,-10,-10,-20}
		}
	};
};

