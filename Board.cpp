#include "Board.h"
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"


Board::Board()
{
	Reset();
	CalcCheckedLevels();
}

void Board::Reset()
{
	//create all the pawns
	for (int file = 0; file < 8; ++file)
	{
		this->chess_board[1][file] = make_shared<Pawn>(*this, Coordinate(1, file), TeamType::BLACK);
		this->chess_board[6][file] = make_shared<Pawn>(*this, Coordinate(6, file), TeamType::WHITE);
	}


	//black pieces
	this->chess_board[0][0] = make_shared<Rook>(*this, Coordinate(0, 0), TeamType::BLACK);
	this->chess_board[0][1] = make_shared<Knight>(*this, Coordinate(0, 1), TeamType::BLACK);
	this->chess_board[0][2] = make_shared<Bishop>(*this, Coordinate(0, 2), TeamType::BLACK);
	this->chess_board[0][3] = make_shared<Queen>(*this, Coordinate(0, 3), TeamType::BLACK);
	this->black_king = make_shared<King>(*this, Coordinate(0, 4), TeamType::BLACK);
	this->chess_board[0][4] = this->black_king;
	this->chess_board[0][5] = make_shared<Bishop>(*this, Coordinate(0, 5), TeamType::BLACK);
	this->chess_board[0][6] = make_shared<Knight>(*this, Coordinate(0, 6), TeamType::BLACK);
	this->chess_board[0][7] = make_shared<Rook>(*this, Coordinate(0, 7), TeamType::BLACK);


	//white pieces
	this->chess_board[7][0] = make_shared<Rook>(*this, Coordinate(7, 0), TeamType::WHITE);
	this->chess_board[7][1] = make_shared<Knight>(*this, Coordinate(7, 1), TeamType::WHITE);
	this->chess_board[7][2] = make_shared<Bishop>(*this, Coordinate(7, 2), TeamType::WHITE);
	this->chess_board[7][3] = make_shared<Queen>(*this, Coordinate(7, 3), TeamType::WHITE);
	this->white_king = make_shared<King>(*this, Coordinate(7, 4), TeamType::WHITE);
	this->chess_board[7][4] = this->white_king;
	this->chess_board[7][5] = make_shared<Bishop>(*this, Coordinate(7, 5), TeamType::WHITE);
	this->chess_board[7][6] = make_shared<Knight>(*this, Coordinate(7, 6), TeamType::WHITE);
	this->chess_board[7][7] = make_shared<Rook>(*this, Coordinate(7, 7), TeamType::WHITE);


	//fill the empty part
	for (int rank = 2; rank < 6; ++rank)
	{
		for (int file = 0; file < 8; ++file)
		{
			this->chess_board[rank][file] = make_shared<Pawn>(*this, Coordinate(rank, file), TeamType::SPECTATOR);
		}
	}

	this->piece_moving_history.push_back(this->white_king);
	this->turn = TeamType::WHITE;
}

void Board::CalcCheckedLevels()
{
	//reset
	for (int rank = 0; rank < 8; ++rank)
	{
		for (int file = 0; file < 8; ++file)
		{
			this->checked_level[0][rank][file] = 0;
			this->checked_level[1][rank][file] = 0;
		}
	}

	//get checked squares from individual pieces
	shared_ptr<Piece> piece;
	shared_ptr<set<Coordinate>> checked_squares;

	for (int rank = 0; rank < 8; ++rank)
	{
		for (int file = 0; file < 8; ++file)
		{
			piece = this->chess_board[rank][file];
			if (piece->team == TeamType::SPECTATOR) continue;

			checked_squares = piece->GetCheckedSquares();
			int team_num = (piece->team == TeamType::WHITE ? 0 : 1);

			for (const Coordinate& checked_square : *checked_squares)
			{
				++this->checked_level[team_num][checked_square.rank][checked_square.file];
			}
		}
	}
}

int Board::EvaluateBoard()
{
	shared_ptr<Piece> piece;
	int white_centipawn_value = 0, black_centipawn_value = 0;

	for (int rank = 0; rank < 8; ++rank)
	{
		for (int file = 0; file < 8; ++file)
		{
			piece = this->chess_board[rank][file];
			if (piece->team == TeamType::SPECTATOR) continue;
			else if (piece->team == TeamType::WHITE) white_centipawn_value += piece->GetCentipawnValue();
			else black_centipawn_value += piece->GetCentipawnValue();
		}
	}

	return white_centipawn_value - black_centipawn_value;
}


void Board::MovePiece(const Coordinate& curr, const Coordinate& next)
{
	//move to the graveyard, for future undo reference
	this->dead_pieces.push_back(this->chess_board[next.rank][next.file]);

	this->chess_board[next.rank][next.file] = this->chess_board[curr.rank][curr.file];
	this->chess_board[next.rank][next.file]->UpdateCoordinate(next);
	this->chess_board[curr.rank][curr.file] = make_shared<Pawn>(*this, curr, TeamType::SPECTATOR);
}

void Board::UndoMovePiece(const Coordinate& srce)
{
	shared_ptr<Piece> undo_piece = this->chess_board[srce.rank][srce.file];
	undo_piece->RestoreCoordinate();
	this->chess_board[undo_piece->coordinate.rank][undo_piece->coordinate.file] = undo_piece;

	this->chess_board[srce.rank][srce.file] = this->dead_pieces.back();
	this->dead_pieces.pop_back();
}

void Board::RemovePiece(const Coordinate& srce)
{
	this->dead_pieces.push_back(this->chess_board[srce.rank][srce.file]);
	this->chess_board[srce.rank][srce.file] = make_shared<Pawn>(*this, srce, TeamType::SPECTATOR);
}

void Board::UndoRemovePiece(const Coordinate& srce)
{
	this->chess_board[srce.rank][srce.file] = this->dead_pieces.back();
	this->dead_pieces.pop_back();
}

void Board::Promote(const Coordinate& srce, shared_ptr<Piece> promoted_piece)
{
	shared_ptr<Piece> pawn = this->chess_board[srce.rank][srce.file];
	promoted_piece->board = pawn->board;
	promoted_piece->coordinate = pawn->coordinate;
	promoted_piece->prev_coordinates = pawn->prev_coordinates;
	promoted_piece->move_times = pawn->move_times;
	promoted_piece->team = pawn->team;

	this->dead_pieces.push_back(pawn);
	this->chess_board[srce.rank][srce.file] = promoted_piece;
}

void Board::UndoPromote(const Coordinate& srce)
{
	this->chess_board[srce.rank][srce.file] = this->dead_pieces.back();
	this->dead_pieces.pop_back();
}


void Board::Move(const Coordinate& srce, const Coordinate& dest)
{
	MovePiece(srce, dest);
	//king castling
	if (dest.is_castling)
	{
		if (dest.file == 2) MovePiece(Coordinate(dest.rank, 0), Coordinate(dest.rank, 3));
		else if (dest.file == 6) MovePiece(Coordinate(dest.rank, 7), Coordinate(dest.rank, 5));
	}
	//pawn enpassant
	else if (dest.is_enpassant)
	{
		RemovePiece(this->piece_moving_history.back()->coordinate);
	}

	this->piece_moving_history.push_back(this->chess_board[dest.rank][dest.file]);
}

void Board::UndoMove(const Coordinate& srce)
{
	shared_ptr<Piece> piece = this->chess_board[srce.rank][srce.file];
	const Coordinate& curr = piece->coordinate;

	this->piece_moving_history.pop_back();

	//king castling
	if (curr.is_castling)
	{
		if (curr.file == 2) UndoMovePiece(Coordinate(curr.rank, 3));
		else if (curr.file == 6) UndoMovePiece(Coordinate(curr.rank, 5));
	}
	//pawn enpassant
	else if (curr.is_enpassant)
	{
		UndoRemovePiece(this->piece_moving_history.back()->coordinate);
	}

	UndoMovePiece(srce);
}

bool Board::PseudoMove(const Coordinate& srce, const Coordinate& dest, const map<Coordinate, shared_ptr<set<Coordinate>>>& map_piece_to_moves)
{
	auto legal_moves_iter = map_piece_to_moves.find(srce);
	if (legal_moves_iter != map_piece_to_moves.end())
	{
		shared_ptr<set<Coordinate>> legal_moves = legal_moves_iter->second;

		//dest and move_iter are different
		//dest is proposed by the player
		//move_iter is generated by the GetLegalMoves() function
		//only move_iter contains the information about castling and en passant
		auto move_iter = legal_moves->find(dest);
		if (move_iter != legal_moves->end())
		{
			Move(srce, *move_iter);

			if (move_iter->is_promoting)
			{
				string promoted_piece_name;
				while (true)
				{
					cout << "Which piece do you want to promote to?\n";
					cin >> promoted_piece_name;
					for (char& letter : promoted_piece_name)
					{
						letter = toupper(letter);
					}

					if (promoted_piece_name == "ROOK")
					{
						Promote(*move_iter, make_shared<Rook>(*this, *move_iter, this->turn));
						break;
					}
					else if (promoted_piece_name == "KNIGHT")
					{
						Promote(*move_iter, make_shared<Knight>(*this, *move_iter, this->turn));
						break;
					}
					else if (promoted_piece_name == "BISHOP")
					{
						Promote(*move_iter, make_shared<Bishop>(*this, *move_iter, this->turn));
						break;
					}
					else if (promoted_piece_name == "QUEEN")
					{
						Promote(*move_iter, make_shared<Queen>(*this, *move_iter, this->turn));
						break;
					}
					else cout << "You can only promote the pawn to either Rook, Knight, Bishop, or Queen!\n";					
				}
			}
			return true;
		}
	}

	return false;
}


void Board::Render()
{
	for (int rank = 0; rank < 8; ++rank)
	{
		cout << 8 - rank << ' ';
		for (int file = 0; file < 8; ++file)
		{
			this->chess_board[rank][file]->Draw();
		}
		cout << '\n';
	}
	cout << "abcdefgh\n";
}

int Board::SimulateMove(int depth)
{
	shared_ptr<Piece> piece;
	shared_ptr<set<Coordinate>> legal_moves;
	shared_ptr<map<Coordinate, shared_ptr<set<Coordinate>>>> map_piece_to_legal_moves = make_shared<map<Coordinate, shared_ptr<set<Coordinate>>>>();
	

	//get all the possible moves
	for (int rank = 0; rank < 8; ++rank)
	{
		for (int file = 0; file < 8; ++file)
		{
			piece = this->chess_board[rank][file];
			if (piece->team == this->turn)
			{
				legal_moves = piece->GetLegalMoves();
				if (!legal_moves->empty()) map_piece_to_legal_moves->insert({ Coordinate(rank, file), legal_moves });
			}
		}
	}

	shared_ptr<King> king = (this->turn == TeamType::WHITE ? this->white_king : this->black_king);
	if (map_piece_to_legal_moves->empty())
	{
		//checkmate
		if (king->IsInCheck())
		{
			if (this->turn == TeamType::WHITE) return -10000;
			else return 10000;
		}
		//stalemate
		else return 0;
	}

	if (depth == 15) return EvaluateBoard();

	//simulate every move
}

void Board::Start()
{
	shared_ptr<Piece> piece;
	shared_ptr<King> king;
	shared_ptr<set<Coordinate>> legal_moves;
	map<Coordinate, shared_ptr<set<Coordinate>>> map_piece_to_legal_moves;

	string str;
	Coordinate coords[2];

	TeamType human_team;
	cout << "Do you want to play as white or black: ";
	cin >> str;
	if (str == "white") human_team = TeamType::WHITE;
	else human_team = TeamType::BLACK;


	while (true)
	{
		Render();

		map_piece_to_legal_moves.clear();
		for (int rank = 0; rank < 8; ++rank)
		{
			for (int file = 0; file < 8; ++file)
			{
				piece = this->chess_board[rank][file];
				if (piece->team == this->turn)
				{
					legal_moves = piece->GetLegalMoves();
					if (!legal_moves->empty()) map_piece_to_legal_moves.insert({Coordinate(rank, file), legal_moves });
				}
			}
		}

		//BUGGGGGG, clear the fake checked levels from the legal moves evaluation part
		CalcCheckedLevels();

		king = (this->turn == TeamType::WHITE ? this->white_king : this->black_king);

		if (map_piece_to_legal_moves.empty())
		{
			if (king->IsInCheck()) cout << "CHECK MATE!\n";
			else cout << "STALEMATE!\n";
			break;
		}

		if (king->IsInCheck()) cout << "CHECK!\n";
		cout << (this->turn == TeamType::WHITE ? "White's " : "Black's ") << "turn\n";

		while (true)
		{
			cout << "Move:\n";
			cin >> str;
			for (auto& letter : str)
			{
				letter = toupper(letter);
			}

			if (str == "DEBUG") Debug();
			else
			{
				coords[0].file = str[0] - 'A';
				coords[0].rank = 8 - (str[1] - '0');
				coords[1].file = str[2] - 'A';
				coords[1].rank = 8 - (str[3] - '0');
			}
			

			if (PseudoMove(coords[0], coords[1], map_piece_to_legal_moves))
			{
				cout << string(10, '\n');
				break;
			}

			cout << "Illegal move!\a\n";
		}

		if (this->turn == TeamType::WHITE) this->turn = TeamType::BLACK;
		else this->turn = TeamType::WHITE;
	}
}


void Board::Debug()
{
	for (int rank = 0; rank < 8; ++rank)
	{
		for (int file = 0; file < 8; ++file)
		{
			cout << this->checked_level[0][rank][file];
		}
		cout << "   ";

		for (int file = 0; file < 8; ++file)
		{
			cout << this->checked_level[1][rank][file];
		}

		cout << '\n';
	}

	
	cout << "\n\n";

	if (dynamic_pointer_cast<Pawn>(this->last_moving_piece)) cout << "last moving piece is a Pawn\n";
	else if (dynamic_pointer_cast<Rook>(this->last_moving_piece)) cout << "last moving piece is a Rook\n";
	else if (dynamic_pointer_cast<Knight>(this->last_moving_piece)) cout << "last moving piece is a Knight\n";
	else if (dynamic_pointer_cast<Bishop>(this->last_moving_piece)) cout << "last moving piece is a Bishop\n";
	else if (dynamic_pointer_cast<Queen>(this->last_moving_piece)) cout << "last moving piece is a Queen\n";
	else if (dynamic_pointer_cast<King>(this->last_moving_piece)) cout << "last moving piece is a King\n";
	else cout << "invalid pieces\n";
}


shared_ptr<Piece>& Board::operator[](const Coordinate& coordinate)
{
	return this->chess_board[coordinate.rank][coordinate.file];
}
