#include "Tile.h"


Tile::Tile() :
	m_type(TileType::none)
{
	
}

Tile::~Tile()
{

}

TileType Tile::getType()
{
	return m_type;
}
