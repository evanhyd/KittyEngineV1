#include "Coordinate.h"

Coordinate::Coordinate() : 
	rank(0), file(0), is_castling(false), is_enpassant(false), is_promoting(false)
{

}

Coordinate::Coordinate(int new_rank, int new_file, bool new_is_castling, bool new_is_enpassant, bool new_is_promoting) :
	rank(new_rank), file(new_file), is_castling(new_is_castling), is_enpassant(new_is_enpassant), is_promoting(new_is_promoting)
{

}

bool Coordinate::IsGoodRank() const
{
	return this->rank >= 0 && this->rank < 8;
}

bool Coordinate::IsGoodFile() const
{
	return this->file >= 0 && this->file < 8;
}

bool Coordinate::IsGood() const
{
	return IsGoodRank() && IsGoodFile();
}

bool Coordinate::operator<(const Coordinate& c) const
{
	return this->rank < c.rank || this->rank == c.rank && this->file < c.file;
}

bool Coordinate::operator==(const Coordinate& c) const
{
	return this->rank == c.rank && this->file == c.file;
}