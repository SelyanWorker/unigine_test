#include "UnitsManager.h"

#include <vector>
#include <random>
#include <set>
#include <cmath>

using namespace ut;

namespace
{
	constexpr float MinRandom = -10.f;
	constexpr float MaxRandom = 10.f;

	constexpr size_t SectionInRow = 10;
	constexpr size_t SectionInColumns = 10;
}

bool Unit::IsVisiblePoint(const Vector& point, float viewDistance, float fov) const
{
	Vector subVector = point - Position;
	float lengthQuad = subVector.LengthQuad();
	if (lengthQuad > std::powf(viewDistance, 2))
		return false;

	subVector.Normalize();

	float dotProduct = Direction.Dot(subVector);
	float angle = (180.f / 3.14159265358f) * std::fabsf(std::acosf(dotProduct));
	if (angle > (fov / 2))
		return false;

	return true;
}

UnitsManager::UnitsManager(size_t count)
{
	GenerateRandomUnits(std::uniform_real_distribution<float>{ MinRandom, MaxRandom }, count);
	GenerateSectors(units);
}

void UnitsManager::UpdateVisibleUnitCount(size_t unitIndex, float viewDistance, float fov)
{
	if (unitIndex >= units.size())
		return;

	auto& unit = units[unitIndex];
	unit.VisibleUnitCount = 0;

	const Vector distanceVector{ viewDistance };

	auto start = GetSectorCoords(unit.Position - distanceVector);
	auto end = GetSectorCoords(unit.Position + distanceVector);

	for (size_t y = start.second; y <= end.second; ++y)
	{
		for (size_t x = start.first; x <= end.first; ++x)
		{
			const auto& sector = sectors[y * SectionInRow + x];
			for (auto i : sector)
			{
				const Vector& point = units[i].Position;
				if (i == unitIndex)
					continue;

				if (unit.IsVisiblePoint(point, viewDistance, fov))
					++unit.VisibleUnitCount;
			}
		}
	}
}

void UnitsManager::GenerateSectors(const std::vector<Unit>& units)
{
	sectors.clear();
	sectors.resize(SectionInColumns * SectionInRow);

	for (const auto& unit : units)
	{
		min.x = std::fminf(min.x, unit.Position.x);
		min.y = std::fminf(min.y, unit.Position.y);

		max.x = std::fmaxf(max.x, unit.Position.x);
		max.y = std::fmaxf(max.y, unit.Position.y);
	}

	sectorWidth = (max.x - min.x) / SectionInRow;
	sectorHeight = (max.y - min.y) / SectionInColumns;

	for (size_t i = 0; i < units.size(); ++i)
	{
		Vector shiftedPosition = units[i].Position - min;

		size_t sectionY = std::min(size_t(shiftedPosition.y / sectorHeight), SectionInColumns - 1);
		size_t sectionX = std::min(size_t(shiftedPosition.x / sectorWidth), SectionInRow - 1);

		sectors[sectionY * SectionInRow + sectionX].push_back(i);
	}
}

void UnitsManager::GenerateRandomUnits(const std::uniform_real_distribution<float>& dist,
	size_t count)
{
	units.clear();
	units.reserve(count);

	std::random_device rd;
	std::mt19937_64 generator(rd());

	std::set<Vector, Vector::Comparator> uniqPositions;

	while (uniqPositions.size() != count)
	{
		uniqPositions.emplace(dist(generator), dist(generator));
	}

	for (const auto& position : uniqPositions)
	{
		Vector direction{ dist(generator), dist(generator) };
		direction.Normalize();

		units.push_back({ position, direction });
	}
}

std::pair<size_t, size_t> UnitsManager::GetSectorCoords(const Vector& point) const
{
	Vector shiftedPoint = point - min;
	if (shiftedPoint.x < 0)
		shiftedPoint.x = 0;
	if (shiftedPoint.y < 0)
		shiftedPoint.y = 0;

	Vector shiftedMax = max - min;
	if (shiftedPoint.x > shiftedMax.x)
		shiftedPoint.x = shiftedMax.x;
	if (shiftedPoint.y > shiftedMax.y)
		shiftedPoint.y = shiftedMax.y;

	return { std::min(size_t(shiftedPoint.x / sectorWidth), SectionInRow - 1),
			 std::min(size_t(shiftedPoint.y / sectorHeight), SectionInColumns - 1) };
}