#pragma once
#include "Piece.h"
class Knight : public Piece
{
public:
	Knight(Board& new_board, Coordinate new_coordinate, TeamType new_team);

private:
	shared_ptr<set<Coordinate>> GetPseudoMoves() override;

public:
	shared_ptr<set<Coordinate>> GetCheckedSquares() override;
	shared_ptr<set<Coordinate>> GetLegalMoves() override;
	int GetCentipawnValue() override;
	void Draw() override;

private:
	static constexpr int KNIGHT_VALUE = 350;
	static constexpr int KNIGHT_TABLE[2][8][8] = 
	{
		{ 
            {-50,-40,-30,-30,-30,-30,-40,-50},
            {-40,-20,  0,  0,  0,  0,-20,-40},
            {-30,  0, 10, 15, 15, 10,  0,-30},
            {-30,  5, 15, 20, 20, 15,  5,-30},
            {-30,  0, 15, 20, 20, 15,  0,-30},
            {-30,  5, 10, 15, 15, 10,  5,-30},
            {-40,-20,  0,  5,  5,  0,-20,-40},
            {-50,-40,-30,-30,-30,-30,-40,-50}
		},
		{
		    {-50,-40,-30,-30,-30,-30,-40,-50},
		    {-40,-20,  0,  5,  5,  0,-20,-40},
		    {-30,  5, 10, 15, 15, 10,  5,-30},
		    {-30,  0, 15, 20, 20, 15,  0,-30},
		    {-30,  5, 15, 20, 20, 15,  5,-30},
		    {-30,  0, 10, 15, 15, 10,  0,-30},
		    {-40,-20,  0,  0,  0,  0,-20,-40},
		    {-50,-40,-30,-30,-30,-30,-40,-50}
		}
	};
};

