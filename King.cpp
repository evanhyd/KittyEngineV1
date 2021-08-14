#include "King.h"


King::King(Board& new_board, Coordinate new_coordinate, TeamType new_team) : Piece(new_board, new_coordinate, new_team, KING_VALUE)
{

}

shared_ptr<set<Coordinate>> King::GetCheckedSquares()
{
	shared_ptr<set<Coordinate>> checked_squares = make_shared<set<Coordinate>>();
	Coordinate dest;

	for (int rank_offset = -1; rank_offset <= 1; ++rank_offset)
	{
		for (int file_offset = -1; file_offset <= 1; ++file_offset)
		{
			if (rank_offset == 0 && file_offset == 0) continue;

			dest = { this->coordinate.rank + rank_offset, this->coordinate.file + file_offset };
			if (dest.IsGood()) checked_squares->insert(dest);
		}
	}

	return checked_squares;
}

shared_ptr<set<Coordinate>> King::GetPseudoMoves()
{
	shared_ptr<set<Coordinate>> pseudo_moves = make_shared<set<Coordinate>>();
	Coordinate dest;

	int enemy_team_num = (this->team == TeamType::WHITE ? 1 : 0);

	for (int rank_offset = -1; rank_offset <= 1; ++rank_offset)
	{
		for (int file_offset = -1; file_offset <= 1; ++file_offset)
		{
			if (rank_offset == 0 && file_offset == 0) continue;

			dest = { this->coordinate.rank + rank_offset, this->coordinate.file + file_offset };

			//empty or enemy, and it is not checked by the enemy
			if (dest.IsGood() && this->board[dest]->team != this->team && this->board.checked_level[enemy_team_num][dest.rank][dest.file] == 0) pseudo_moves->insert(dest);
		}
	}

	//castle
	int rook_rank = (this->team == TeamType::WHITE ? 7 : 0);

	//king has never move moved
	if (this->move_times == 0)
	{
		//queen side rook has never moved
		if (this->board[{rook_rank, 0}]->move_times == 0)
		{
			//no pieces between the rook and the king
			if (this->board[{rook_rank, 1}]->team == TeamType::SPECTATOR &&
				this->board[{rook_rank, 2}]->team == TeamType::SPECTATOR &&
				this->board[{rook_rank, 3}]->team == TeamType::SPECTATOR)
			{
				//king and the left two squares are not in check
				if (this->board.checked_level[enemy_team_num][rook_rank][2] == 0 &&
					this->board.checked_level[enemy_team_num][rook_rank][3] == 0 &&
					this->board.checked_level[enemy_team_num][rook_rank][4] == 0)

				{
					pseudo_moves->insert(Coordinate(rook_rank, 2, true, false));
				}
			}
		}

		//king side rook has never moved
		if (this->board[{rook_rank, 7}]->move_times == 0)
		{
			//no pieces between the rook and the king
			if (this->board[{rook_rank, 5}]->team == TeamType::SPECTATOR && this->board[{rook_rank, 6}]->team == TeamType::SPECTATOR)
			{
				//king and the right two squares are not in check
				if(this->board.checked_level[enemy_team_num][rook_rank][4] == 0 &&
					this->board.checked_level[enemy_team_num][rook_rank][5] == 0 &&
					this->board.checked_level[enemy_team_num][rook_rank][6] == 0)
				
				{
					pseudo_moves->insert({ rook_rank, 6, true, false});
				}
			}
		}
	}

	return pseudo_moves;
}

shared_ptr<set<Coordinate>> King::GetLegalMoves()
{
	return this->GetPseudoMoves();
}

int King::GetCentipawnValue()
{
	int team_num = (this->team == TeamType::WHITE ? 0 : 1);
	return KING_VALUE + KING_TABLE_MIDDLEGAME[team_num][this->coordinate.rank][this->coordinate.file];
}

bool King::IsInCheck()
{
	int enemy_team_num = (this->team == TeamType::WHITE ? 1 : 0);
	return this->board.checked_level[enemy_team_num][this->coordinate.rank][this->coordinate.file] > 0;
}


void King::Draw()
{
	if (this->team == TeamType::WHITE) cout << 'K';
	else if (this->team == TeamType::BLACK) cout << 'k';
	else cout << ' ';
}