#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "VectorRotation.h"

class VectorFunctions
{
public:
	static float vectorMagnitude(const sf::Vector2f &vector)
	{
		return (std::powf(vector.x * vector.x + vector.y * vector.y, .5f));
	}

	static void normalizeVector(sf::Vector2f &vector)
	{
		float magnitude = vectorMagnitude(vector);
		if (magnitude < 0.01) return; // Cannot normalize a zero vector, duh

		vector /= magnitude;
	}

	static sf::Vector2f lerp(const sf::Vector2f &start, const sf::Vector2f &goal, float step, bool &reachedGoal)
	{
		sf::Vector2f dir = goal - start;
		if (vectorMagnitude(dir) <= step)
		{
			reachedGoal = true;
			return goal;
		}
		else
		{
			normalizeVector(dir);
			reachedGoal = false;
			return (start + dir * step);
		}
	}

	static float dotProduct(const sf::Vector2f &v1, const sf::Vector2f &v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	static float angleBetweenVectors(const sf::Vector2f &v1, const sf::Vector2f &v2)
	{
		float l1 = vectorMagnitude(v1), l2 = vectorMagnitude(v2);
		return acosf(dotProduct(v1, v2) / (l1*l2));
	}
};
