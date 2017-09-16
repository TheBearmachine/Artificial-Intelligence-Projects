#include "TilemapGenerator.h"
#include <fstream>
#include <SFML/Window/Event.hpp>

static const char* TILEMAP_TEXTUREFILE = "FancyTiles.png";
static const char* WRITE_FILE = "TileData.txt";
static const char* WRITE_FILE_RAW = "TileDataRaw.txt";

TilemapGenerator::TilemapGenerator() :
	mPaintColor(0), mMouseButtonDown(false)
{
}

TilemapGenerator::~TilemapGenerator()
{
}

void intToCharArray(unsigned int inUnsignedInt, char* outCharArray)
{
	unsigned int val = inUnsignedInt;
	outCharArray[0] = (unsigned char)val;
	val >>= 8;
	outCharArray[1] = (unsigned char)val;
	val >>= 8;
	outCharArray[2] = (unsigned char)val;
	val >>= 8;
	outCharArray[3] = (unsigned char)val;
}

void writeToFile(const sf::Vector2u &mapSize, const unsigned int *tileIndices)
{
	std::ofstream ofs(WRITE_FILE);
	if (!ofs.is_open()) return; // Error creating and/or opening file.
	std::string x = std::to_string(mapSize.x);
	std::string y = std::to_string(mapSize.y);
	size_t size = mapSize.x * mapSize.y;
	std::string toWrite = "X: " + x + ", Y: " + y + "\n";

	ofs.write(toWrite.c_str(), toWrite.size());

	toWrite = "{";
	for (size_t i = 0; i < size; i++)
	{
		if (i % mapSize.x == 0) toWrite += "\n";
		toWrite += std::to_string(tileIndices[i]);
		if (i != size - 1) toWrite += ",";
	}
	toWrite += "}";
	ofs.write(toWrite.c_str(), toWrite.size());
}

void writeToFileRawData(const sf::Vector2u &mapSize, const unsigned int *tileIndices)
{
	std::ofstream ofs(WRITE_FILE_RAW, std::ios::binary);
	if (!ofs.is_open()) return; // Error creating and/or opening file.
	// Size is 32 bytes
	std::streampos size = 4;
	char* memblock = new char[size];

	// Start writing map size
	intToCharArray(mapSize.x, memblock);
	ofs.write(memblock, size);
	intToCharArray(mapSize.y, memblock);
	ofs.write(memblock, size);

	// Then write each tiletyp
	size_t top = mapSize.x * mapSize.y;
	for (size_t i = 0; i < top; i++)
	{
		intToCharArray(tileIndices[i], memblock);
		ofs.write(memblock, size);
	}
	ofs.close();
	delete[size] memblock;
}

void TilemapGenerator::run(size_t nrTilesX, size_t nrTilesY, size_t defaultTile)
{
	mTileIndexArray = new int[nrTilesX * nrTilesY];
	for (int i = 0; i < nrTilesX * nrTilesY; i++)
	{
		mTileIndexArray[i] = defaultTile;
	}

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pathfinding AI");
	sf::Clock clock;

	mTilemap.load(TILEMAP_TEXTUREFILE, sf::Vector2u(64, 64), mTileIndexArray, nrTilesX, nrTilesY);
	delete[nrTilesX * nrTilesY] mTileIndexArray;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			else if (event.type == sf::Event::MouseButtonPressed)
			{
				// Start painting
				sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
				sf::Vector2f worldPos(window.mapPixelToCoords(mousePos));

				mTilemap.setTileTexture(worldPos, mPaintColor);
				mMouseButtonDown = true;
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				// Stop painting
				mMouseButtonDown = false;
			}
			else if (event.type == sf::Event::MouseMoved && mMouseButtonDown)
			{
				sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
				sf::Vector2f worldPos(window.mapPixelToCoords(mousePos));

				mTilemap.setTileTexture(worldPos, mPaintColor);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if ((int)event.key.code >= 26 && (int)event.key.code <= 31)
				{
					// Handle color swaps
					mPaintColor = (int)event.key.code - 26;
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					// Save to file
					sf::Vector2u mapSize = mTilemap.getMapSizeInTiles();
					size_t size = mapSize.x * mapSize.y;
					unsigned int *indices = new unsigned int[size];
					for (size_t i = 0; i < size; i++)
					{
						indices[i] = mTilemap.getTileTypeFromIndex(i);
					}
					writeToFile(mapSize, indices);
					writeToFileRawData(mapSize, indices);
					delete[size] indices;
				}
			}
		}
		update(clock.restart());
		draw(window);
	}
}

void TilemapGenerator::update(sf::Time & deltaTime)
{

}

void TilemapGenerator::draw(sf::RenderWindow & window)
{
	window.clear();
	window.draw(mTilemap);
	window.display();
}
