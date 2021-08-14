#include "Pawn.h"
#include "King.h"


Pawn::Pawn(Board& new_board, Coordinate new_coordinate, TeamType new_team) : Piece(new_board, new_coordinate, new_team, PAWN_VALUE)
{

}

shared_ptr<set<Coordinate>> Pawn::GetCheckedSquares()
{
	shared_ptr<set<Coordinate>> checked_squares = make_shared<set<Coordinate>>();
	int pawn_direction = (this->team == TeamType::WHITE ? -1 : 1);

	Coordinate dest(this->coordinate.rank + pawn_direction, this->coordinate.file);
	if (dest.IsGoodRank())
	{
		//capture forward left
		dest.file -= 1;
		if (dest.IsGoodFile()) checked_squares->insert(dest);

		//capture forward right
		dest.file += 2;
		if (dest.IsGoodFile()) checked_squares->insert(dest);
	}

	return checked_squares;
}

shared_ptr<set<Coordinate>> Pawn::GetPseudoMoves()
{
	shared_ptr<set<Coordinate>> pseudo_moves = make_shared<set<Coordinate>>();
	int pawn_direction = (this->team == TeamType::WHITE ? -1 : 1);

	Coordinate dest = { this->coordinate.rank + pawn_direction, this->coordinate.file };
	if (dest.IsGoodRank())
	{
		//move forward one square
		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);

		//capture forward left
		dest.file -= 1;
		if (dest.IsGoodFile() && this->board[dest]->team != this->team && this->board[dest]->team != TeamType::SPECTATOR)
		{
			//promoting
			if(this->team == TeamType::WHITE && dest.rank == 0 || this->team == TeamType::BLACK && dest.rank == 7) pseudo_moves->insert(Coordinate(dest.rank, dest.file, false, false, true));
			else pseudo_moves->insert(dest);
		}

		//capture forward right
		dest.file += 2;
		if (dest.IsGoodFile() && this->board[dest]->team != this->team && this->board[dest]->team != TeamType::SPECTATOR)
		{
			if (this->team == TeamType::WHITE && dest.rank == 0 || this->team == TeamType::BLACK && dest.rank == 7) pseudo_moves->insert(Coordinate(dest.rank, dest.file, false, false, true));
			else pseudo_moves->insert(dest);
		}
	}


	//move two squares forward in the opening
	if (this->move_times == 0)
	{
		dest = { this->coordinate.rank + pawn_direction, this->coordinate.file };
		if (this->board[dest]->team == TeamType::SPECTATOR)
		{
			dest.rank += pawn_direction;
			if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		}
	}


	//en passant
	//if the current pawn is on the en passant-able square
	if (this->team == TeamType::WHITE && this->coordinate.rank == 3 || this->team == TeamType::BLACK && this->coordinate.rank == 4)
	{
		shared_ptr<Pawn> moved_pawn = dynamic_pointer_cast<Pawn>(this->board.last_moving_piece);
		//the opponent's last moved piece is a pawn
		if (moved_pawn)
		{
			//moved by two squares
			if (abs(moved_pawn->coordinate.rank - moved_pawn->old_coordinate.rank) == 2)
			{
				//adjacent to the current pawn
				if (abs(moved_pawn->coordinate.file - this->coordinate.file) == 1)
				{
					pseudo_moves->insert({ this->coordinate.rank + pawn_direction, moved_pawn->coordinate.file, false, true });
				}
			}
		}
	}

	return pseudo_moves;
}

shared_ptr<set<Coordinate>> Pawn::GetLegalMoves()
{
	shared_ptr<set<Coordinate>> moves = this->GetPseudoMoves();
	shared_ptr<set<Coordinate>> legal_moves = make_shared<set<Coordinate>>();

	shared_ptr<King> king = (this->team == TeamType::WHITE ? this->board.white_king : this->board.black_king);
	shared_ptr<Piece> removed_backup, enpassanted_pawn;
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
		this->board[coordinate_backup] = make_shared<Pawn>(this->board, coordinate_backup, TeamType::SPECTATOR);

		//remove the enpassant piece
		if (move.is_enpassant)
		{
			enpassanted_pawn = this->board.chess_board[coordinate_backup.rank][move.file];
			this->board.chess_board[coordinate_backup.rank][move.file] = make_shared<Pawn>(this->board, coordinate_backup, TeamType::SPECTATOR);
		}

		//recalculate the checking level, validate the moves 
		this->board.CalcCheckedLevels();

		//king must not be in the check after the move
		if (!king->IsInCheck()) legal_moves->insert(move);

		//restore the premoving piece and position
		this->board[coordinate_backup] = this->board[move];
		this->board[coordinate_backup]->coordinate = coordinate_backup;

		//restore the removed piece
		this->board[move] = removed_backup;

		//restore the enpassanted pawn
		if (move.is_enpassant) this->board.chess_board[coordinate_backup.rank][move.file] = enpassanted_pawn;
	}

	return legal_moves;
}

int Pawn::GetCentipawnValue()
{
	int team_num = (this->team == TeamType::WHITE ? 0 : 1);
	return PAWN_VALUE + PAWN_TABLE[team_num][this->coordinate.rank][this->coordinate.file];
}

void Pawn::Draw()
{
	if (this->team == TeamType::WHITE) cout << 'P';
	else if (this->team == TeamType::BLACK) cout << 'p';
	else cout << ' ';
}