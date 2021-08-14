#include "Piece.h"


Piece::Piece(Board& new_board, Coordinate new_coordinate, TeamType new_team, int new_centipawn_value) :
    board(new_board), coordinate(new_coordinate), prev_coordinates(make_shared<vector<Coordinate>>()), team(new_team), centipawn_value(new_centipawn_value), move_times(0)
{

}



void Piece::UpdateCoordinate(const Coordinate& next_coordinate)
{
    this->prev_coordinates->push_back(this->coordinate);
    this->coordinate = next_coordinate;

    ++this->move_times;
}


void Piece::RestoreCoordinate()
{
    this->coordinate = this->prev_coordinates->back();
    this->prev_coordinates->pop_back();

    --this->move_times;
}