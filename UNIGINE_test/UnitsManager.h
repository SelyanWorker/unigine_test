#pragma once

#include "Vector.h"

#include <vector>
#include <random>

namespace ut
{
	struct Unit
	{
		Vector Position;
		Vector Direction;

		size_t VisibleUnitCount = 0;

		bool IsVisiblePoint(const Vector& point, float viewDistance, float fov) const;
	};

	class UnitsManager
	{
	public:
		explicit UnitsManager(size_t count);

		void UpdateVisibleUnitCount(size_t unitIndex, float viewDistance, float fov);

		size_t Count() const
		{
			return units.size();
		}

		Unit& operator[](size_t index)
		{
			return units[index];
		}

		const Unit& operator[](size_t index) const
		{
			return units[index];
		}

	private:
		void GenerateRandomUnits(const std::uniform_real_distribution<float>& dist, size_t count);
		void GenerateSectors(const std::vector<Unit>& units);

		std::pair<size_t, size_t> GetSectorCoords(const Vector& point) const;

	private:
		Vector min;
		Vector max;

		float sectorWidth = 0;
		float sectorHeight = 0;

		std::vector<Unit> units;
		std::vector<std::vector<size_t>> sectors;
	};
}