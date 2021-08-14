#include "Knight.h"
#include "King.h"


Knight::Knight(Board& new_board, Coordinate new_coordinate, TeamType new_team) : Piece(new_board, new_coordinate, new_team, KNIGHT_VALUE)
{

}

shared_ptr<set<Coordinate>> Knight::GetCheckedSquares()
{
	shared_ptr<set<Coordinate>> checked_squares = make_shared<set<Coordinate>>();
	Coordinate dest;

	for (int offset_0 : {-1, 1})
	{
		for (int offset_1 : {-2, 2})
		{
			dest = { this->coordinate.rank + offset_0, this->coordinate.file + offset_1 };
			if (dest.IsGood()) checked_squares->insert(dest);
			dest = { this->coordinate.rank + offset_1, this->coordinate.file + offset_0 };
			if (dest.IsGood()) checked_squares->insert(dest);
		}
	}
	
	return checked_squares;
}

shared_ptr<set<Coordinate>> Knight::GetPseudoMoves()
{
	shared_ptr<set<Coordinate>> pseudo_moves = make_shared<set<Coordinate>>();
	Coordinate dest;

	for (int offset_0 : {-1, 1})
	{
		for (int offset_1 : {-2, 2})
		{
			dest = { this->coordinate.rank + offset_0, this->coordinate.file + offset_1 };
			if (dest.IsGood() && this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			dest = { this->coordinate.rank + offset_1, this->coordinate.file + offset_0 };
			if (dest.IsGood() && this->board[dest]->team != this->team) pseudo_moves->insert(dest);
		}
	}
	return pseudo_moves;
}

shared_ptr<set<Coordinate>> Knight::GetLegalMoves()
{
	shared_ptr<set<Coordinate>> moves = this->GetPseudoMoves();
	shared_ptr<set<Coordinate>> legal_moves = make_shared<set<Coordinate>>();

	shared_ptr<King> king = (this->team == TeamType::WHITE ? this->board.white_king : this->board.black_king);
	shared_ptr<Piece> removed_backup;
	Coordinate coordinate_backup;

	for (const Coordinate& move : *moves)
	{
		//make a copy of the removed piece, and pawn's premoving position
		removed_backup = this->board[move];
		coordinate_backup = this->coordinate;

		//move the pawn to the destination
		this->board[move] = this->board[this->coordinate];
		this->coordinate = move;

		//replace the premoving position with a neutral piece
		this->board[coordinate_backup] = make_shared<Knight>(this->board, coordinate_backup, TeamType::SPECTATOR);

		//recalculate the checking level, validate the moves 
		this->board.CalcCheckedLevels();

		//king must not be in the check after the move
		if (!king->IsInCheck()) legal_moves->insert(move);

		//restore the premoving piece and position
		this->board[coordinate_backup] = this->board[move];
		this->board[coordinate_backup]->coordinate = coordinate_backup;

		//restore the removed piece
		this->board[move] = removed_backup;
	}

	return legal_moves;
}

int Knight::GetCentipawnValue()
{
	int team_num = (this->team == TeamType::WHITE ? 0 : 1);
	return KNIGHT_VALUE + KNIGHT_TABLE[team_num][this->coordinate.rank][this->coordinate.file];
}


void Knight::Draw()
{
	if (this->team == TeamType::WHITE) cout << 'N';
	else if (this->team == TeamType::BLACK) cout << 'n';
	else cout << ' ';
}