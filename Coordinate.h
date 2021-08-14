#pragma once
class Coordinate
{
public:
	int rank;
	int file;
	bool is_castling;
	bool is_enpassant;
	bool is_promoting;

	Coordinate();
	Coordinate(int new_rank, int new_file, bool new_is_castling = false, bool new_is_enpassant = false, bool new_is_promoting = false);

	bool IsGoodRank() const;
	bool IsGoodFile() const;
	bool IsGood() const;

	bool operator<(const Coordinate& c) const;
	bool operator==(const Coordinate& c) const;
};

