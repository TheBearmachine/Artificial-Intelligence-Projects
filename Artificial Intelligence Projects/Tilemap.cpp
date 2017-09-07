#define _USE_MATH_DEFINES
#include "Tilemap.h"
#include "Tile.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <queue>
#include <math.h>
#include <functional>


static const int ARROW_LINE_THICKNESS = 5;

Tilemap::Tilemap() :
	mNrTilesX(0), mNrTilesY(0), mLastCheckedIndex(100000)
{
	mArrowShape.setPrimitiveType(sf::PrimitiveType::Quads);
}

Tilemap::~Tilemap()
{

}

void Tilemap::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &mTileset;
	target.draw(mVertices, states);
	states.texture = &mArrowTexture;
	target.draw(mArrowShape, states);
}

void Tilemap::update(sf::Time & deltaTime)
{
	if (!mAvaliableMoves.empty())
	{
		mMovesTimer += deltaTime.asSeconds();
		float offset = std::sinf(mMovesTimer*5.f)*15.f;
		for (int i = 0; i < mAvaliableMoves.size(); i++)
		{
			for (int j = 0; j < 4; j++)
				mVertices[mAvaliableMoves[i]->getAssociatedVertex() + j].color
				= sf::Color(128 + (int)offset, 128 + (int)offset, 128 + (int)offset);

		}
	}
}

bool Tilemap::load(const std::string & tileset, sf::Vector2u tileSize, const int * tiles, unsigned int width, unsigned int height)
{
	if (!mTileset.loadFromFile(tileset))
		return false;


	for (unsigned int i = 0; i < mNrTilesX; i++)
	{
		for (unsigned int j = 0; j < mNrTilesY; j++)
		{
			unsigned int index = i + j * mNrTilesX;
			if (mTiles[index] != nullptr)
				delete mTiles[index];
		}
	}
	delete[mNrTilesX * mNrTilesY] mTiles;

	mTiles = new Tile*[width*height];

	mVertices.setPrimitiveType(sf::Quads);
	mVertices.resize(width * height * 4);
	mNrTilesX = width;
	mNrTilesY = height;
	mTileWidth = tileSize.x;
	mTileHeight = tileSize.y;

	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			unsigned int index = i + j * width;
			Tile* newTile = new Tile;
			mTiles[index] = newTile;

			// get the current tile number
			int tileNumber = tiles[index];
			newTile->setTileType((Tile::TileTypes)tileNumber);
			newTile->setTileIndex(sf::Vector2u(i, j));
			newTile->setAssociatedVertex(index * 4);

			// find its position in the tileset texture
			int tu = tileNumber % (mTileset.getSize().x / tileSize.x);
			int tv = tileNumber / (mTileset.getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &mVertices[index * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
	}

	// Setup a graph
	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			unsigned int index = i + j * width;
			Tile* tileTest = mTiles[index];

			// Assign neighbors to each tile, with exceptions for tiles on the edge of the screen
			mTiles[index]->setNeighbor(j == 0 ? nullptr : mTiles[index - width], 0);
			mTiles[index]->setNeighbor(i == (width - 1) ? nullptr : mTiles[index + 1], 1);
			mTiles[index]->setNeighbor(j == (height - 1) ? nullptr : mTiles[index + width], 2);
			mTiles[index]->setNeighbor(i == 0 ? nullptr : mTiles[index - 1], 3);
			mTiles[index]->setTreeParent(nullptr);
		}
	}
	return true;
}

bool containsElement(std::priority_queue<Tile*, std::vector<Tile*>, CompareTileCosts> queue, Tile* element)
{
	while (!queue.empty())
	{
		Tile* temp = queue.top();
		if (temp == element) return true;
		queue.pop();
	}
	return false;
}

bool containsElement(const std::vector<Tile*> &vector, Tile* element)
{
	for (size_t i = 0; i < vector.size(); i++)
	{
		if (vector[i] == element)
			return true;
	}
	return false;
}

void Tilemap::calculateAvailableMoves(const sf::Vector2f &startPos, int travelLength, int* tileCosts)
{
	Tile* startTile;
	std::priority_queue<Tile*, std::vector<Tile*>, CompareTileCosts> openList;
	std::vector<Tile*> closedList;
	//std::vector<Tile*> openList;
	unsigned int index = getIndexFromVector(startPos);
	startTile = mTiles[index];
	startTile->setAccumulatedCost(0);
	Tile* tileNeighbors[4];
	openList.push(startTile);
	while (!openList.empty())
	{
		auto currentTile = openList.top();
		openList.pop();
		closedList.push_back(currentTile);

		// Expand all neighboring tiles
		for (int i = 0; i < 4; i++)
		{
			tileNeighbors[i] = currentTile->getNeighbors()[i];
			if (tileNeighbors[i] != nullptr &&
				!containsElement(closedList, tileNeighbors[i]) &&
				!containsElement(openList, tileNeighbors[i]))
			{
				unsigned int newCost = tileCosts[tileNeighbors[i]->getTileType()] + currentTile->getAccumulatedCost();
				Tile* newTile = tileNeighbors[i];
				if (newCost <= travelLength)
				{
					newTile->setAccumulatedCost(newCost);
					newTile->setTreeParent(currentTile);
					openList.push(newTile);
				}
			}
		}
		printf("Open list current size: %i\n", openList.size());
	}
	printf("Closed list size: %i\n", closedList.size());
	mAvaliableMoves = closedList;
	mMovesTimer = 0.f;
}

void Tilemap::clearMoves()
{
	if (!mAvaliableMoves.empty())
	{
		for (int i = 0; i < mAvaliableMoves.size(); i++)
		{
			for (int j = 0; j < 4; j++)
				mVertices[mAvaliableMoves[i]->getAssociatedVertex() + j].color
				= sf::Color(255, 255, 255);
			mAvaliableMoves[i]->setTreeParent(nullptr);
			mAvaliableMoves[i]->setAccumulatedCost(0);
		}
		mAvaliableMoves.clear();
		mPathPoints.clear();
		mArrowShape.resize(0);
	}
}

template <typename T>
void reverseVectorOrder(std::vector<T> &vector)
{
	auto tempVector = vector;
	vector.clear();
	while (!tempVector.empty())
	{
		vector.push_back(tempVector.back());
		tempVector.pop_back();
	}
}

void rotationMatrix(sf::Vector2f &vector, float degrees)
{
	degrees = (degrees * M_PI) / 180.f;
	sf::Vector2f tempVec = sf::Vector2f(vector);
	vector.x = tempVec.x * cosf(degrees) - tempVec.y * sinf(degrees);
	vector.y = tempVec.x * sinf(degrees) + tempVec.y * cosf(degrees);
}

void Tilemap::calculatePath(const sf::Vector2f &point)
{
	unsigned int index = getIndexFromVector(point);
	if (mAvaliableMoves.empty() || index == mLastCheckedIndex) return;
	mLastCheckedIndex = index;
	if (containsElement(mAvaliableMoves, mTiles[index]))
	{
		mPathPoints.clear();
		Tile* currentTile = mTiles[index];
		while (currentTile->getTreeParent() != nullptr)
		{
			mPathPoints.push_back(sf::Vector2f((float)currentTile->getTileIndex().x * (float)mTileWidth, (float)currentTile->getTileIndex().y * (float)mTileHeight));
			currentTile = currentTile->getTreeParent();
		}
		mPathPoints.push_back(sf::Vector2f((float)currentTile->getTileIndex().x * (float)mTileWidth, (float)currentTile->getTileIndex().y * (float)mTileHeight));
		reverseVectorOrder(mPathPoints);

		// Draw a pretty arrow covering the optimal path to the requested tile
		if (mPathPoints.size() <= 1)
		{
			mArrowShape.resize(0);
			return;
		}
		mArrowShape.resize(mPathPoints.size() * 4);
		for (size_t i = 0; i < mPathPoints.size(); i++)
		{
			sf::Vector2f dir;
			sf::Vector2f pos(mPathPoints[i].x + 0.5f * (float)mTileWidth, mPathPoints[i].y + 0.5f * (float)mTileHeight);
			if (i != mPathPoints.size() - 1)
			{
				dir = sf::Vector2f(mPathPoints[i + 1]) - sf::Vector2f(mPathPoints[i]);
				dir.x /= mTileWidth;
				dir.y /= mTileHeight;
				rotationMatrix(dir, 135.f);
				mArrowShape[i * 4].position = pos + dir * (float)ARROW_LINE_THICKNESS;
				rotationMatrix(dir, 90);
				mArrowShape[i * 4 + 1].position = pos + dir * (float)ARROW_LINE_THICKNESS;
				pos = sf::Vector2f(mPathPoints[i + 1].x + 0.5f * (float)mTileWidth, mPathPoints[i + 1].y + 0.5f * (float)mTileHeight);
				rotationMatrix(dir, 90);
				mArrowShape[i * 4 + 2].position = pos + dir * (float)ARROW_LINE_THICKNESS;
				rotationMatrix(dir, 90);
				mArrowShape[i * 4 + 3].position = pos + dir * (float)ARROW_LINE_THICKNESS;
			}
			else
			{
				dir = sf::Vector2f(mPathPoints[i]) - sf::Vector2f(mPathPoints[i - 1]);
				dir.x /= mTileWidth;
				dir.y /= mTileHeight;
				rotationMatrix(dir, 45.f);
				mArrowShape[i * 4].position = pos + dir * (float)ARROW_LINE_THICKNESS;
				rotationMatrix(dir, 90.f);
				mArrowShape[i * 4 + 1].position = pos + dir * (float)ARROW_LINE_THICKNESS * 5.f;
				rotationMatrix(dir, 90.f);
				mArrowShape[i * 4 + 2].position = pos + dir * (float)ARROW_LINE_THICKNESS * 5.f;
				rotationMatrix(dir, 90.f);
				mArrowShape[i * 4 + 3].position = pos + dir * (float)ARROW_LINE_THICKNESS;
			}
		}
	}
}

std::vector<sf::Vector2f> Tilemap::getCurrentPath(const sf::Vector2f &mousePos)
{
	unsigned int index = getIndexFromVector(mousePos);
	if (containsElement(mAvaliableMoves, mTiles[index]))
		return mPathPoints;
	else
		return std::vector<sf::Vector2f>();
}

unsigned int Tilemap::getIndexFromVector(const sf::Vector2f & pos)
{
	sf::Vector2f newPos = pos;
	newPos.x /= mTileWidth;
	newPos.y /= mTileHeight;
	unsigned int index = (int)newPos.x + (int)newPos.y * mNrTilesX;
	return index;
}
