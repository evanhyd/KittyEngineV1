#include "Queen.h"
#include "King.h"


Queen::Queen(Board& new_board, Coordinate new_coordinate, TeamType new_team) : Piece(new_board, new_coordinate, new_team, QUEEN_VALUE)
{

}

shared_ptr<set<Coordinate>> Queen::GetCheckedSquares()
{
	shared_ptr<set<Coordinate>> checked_squares = make_shared<set<Coordinate>>();

	Coordinate dest = this->coordinate;
	while (true)
	{
		++dest.rank;
		if (!dest.IsGoodRank()) break;

		checked_squares->insert(dest);

		if (this->board[dest]->team != TeamType::SPECTATOR) break;
	}

	dest.rank = this->coordinate.rank;
	while (true)
	{
		--dest.rank;
		if (!dest.IsGoodRank()) break;

		checked_squares->insert(dest);
		if (this->board[dest]->team != TeamType::SPECTATOR) break;
	}

	dest.rank = this->coordinate.rank;
	while (true)
	{
		++dest.file;
		if (!dest.IsGoodFile()) break;

		checked_squares->insert(dest);
		if (this->board[dest]->team != TeamType::SPECTATOR) break;
	}

	dest.file = this->coordinate.file;
	while (true)
	{
		--dest.file;
		if (!dest.IsGoodFile()) break;

		checked_squares->insert(dest);
		if (this->board[dest]->team != TeamType::SPECTATOR) break;
	}

	dest.file = this->coordinate.file;
	while (true)
	{
		--dest.rank;
		--dest.file;
		if (!dest.IsGood()) break;

		checked_squares->insert(dest);
		if (this->board[dest]->team != TeamType::SPECTATOR) break;
	}

	dest = this->coordinate;
	while (true)
	{
		++dest.rank;
		--dest.file;
		if (!dest.IsGood()) break;

		checked_squares->insert(dest);
		if (this->board[dest]->team != TeamType::SPECTATOR) break;
	}

	dest = this->coordinate;
	while (true)
	{
		++dest.rank;
		++dest.file;
		if (!dest.IsGood()) break;

		checked_squares->insert(dest);
		if (this->board[dest]->team != TeamType::SPECTATOR) break;
	}

	dest = this->coordinate;
	while (true)
	{
		--dest.rank;
		++dest.file;
		if (!dest.IsGood()) break;

		checked_squares->insert(dest);
		if (this->board[dest]->team != TeamType::SPECTATOR) break;
	}

	return checked_squares;
}

shared_ptr<set<Coordinate>> Queen::GetPseudoMoves()
{
	shared_ptr<set<Coordinate>> pseudo_moves = make_shared<set<Coordinate>>();

	Coordinate dest = this->coordinate;
	while (true)
	{
		++dest.rank;
		if (!dest.IsGoodRank()) break;

		//continue if no obstacles, otherwise has to stop
		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		else
		{
			if (this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			break;
		}
	}

	dest.rank = this->coordinate.rank;
	while (true)
	{
		--dest.rank;
		if (!dest.IsGoodRank()) break;

		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		else
		{
			if (this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			break;
		}
	}

	dest.rank = this->coordinate.rank;
	while (true)
	{
		++dest.file;
		if (!dest.IsGoodFile()) break;

		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		else
		{
			if (this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			break;
		}
	}

	dest.file = this->coordinate.file;
	while (true)
	{
		--dest.file;
		if (!dest.IsGoodFile()) break;

		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		else
		{
			if (this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			break;
		}
	}

	dest.file = this->coordinate.file;
	while (true)
	{
		--dest.rank;
		--dest.file;
		if (!dest.IsGood()) break;

		//continue if no obstacles, otherwise has to stop
		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		else
		{
			if (this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			break;
		}
	}

	dest = this->coordinate;
	while (true)
	{
		++dest.rank;
		--dest.file;
		if (!dest.IsGood()) break;

		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		else
		{
			if (this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			break;
		}
	}

	dest = this->coordinate;
	while (true)
	{
		++dest.rank;
		++dest.file;
		if (!dest.IsGood()) break;

		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		else
		{
			if (this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			break;
		}
	}

	dest = this->coordinate;
	while (true)
	{
		--dest.rank;
		++dest.file;
		if (!dest.IsGood()) break;

		if (this->board[dest]->team == TeamType::SPECTATOR) pseudo_moves->insert(dest);
		else
		{
			if (this->board[dest]->team != this->team) pseudo_moves->insert(dest);
			break;
		}
	}
	return pseudo_moves;
}

shared_ptr<set<Coordinate>> Queen::GetLegalMoves()
{
	shared_ptr<set<Coordinate>> moves = this->GetPseudoMoves();
	shared_ptr<set<Coordinate>> legal_moves = make_shared<set<Coordinate>>();

	shared_ptr<King> king = (this->team == TeamType::WHITE ? this->board.white_king : this->board.black_king);
	shared_ptr<Piece> removed_backup;
	Coordinate coordinate_backup;

	for (const Coordinate& move : *moves)
	{
		removed_backup = this->board[move];
		coordinate_backup = this->coordinate;

		this->board[move] = this->board[this->coordinate];
		this->coordinate = move;

		this->board[coordinate_backup] = make_shared<Queen>(this->board, coordinate_backup, TeamType::SPECTATOR);

		this->board.CalcCheckedLevels();

		if (!king->IsInCheck()) legal_moves->insert(move);

		this->board[coordinate_backup] = this->board[move];
		this->board[coordinate_backup]->coordinate = coordinate_backup;

		//restore the removed piece
		this->board[move] = removed_backup;
	}

	return legal_moves;
}

int Queen::GetCentipawnValue()
{
	int team_num = (this->team == TeamType::WHITE ? 0 : 1);
	return QUEEN_VALUE + QUEEN_TABLE[team_num][this->coordinate.rank][this->coordinate.file];
}

void Queen::Draw()
{
	if (this->team == TeamType::WHITE) cout << 'Q';
	else if (this->team == TeamType::BLACK) cout << 'q';
	else cout << ' ';
}