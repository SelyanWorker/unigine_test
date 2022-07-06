#include "UnitsManager.h"

#include <vector>
#include <thread>
#include <iostream>

void UpdateAllVisibleUnitCount(ut::UnitsManager& manager, float viewDistance, float fov, size_t threadsCount)
{
	const size_t unitsCount = manager.Count();

	if (!unitsCount)
		return;

	size_t step = unitsCount / threadsCount;
	size_t remainder = unitsCount % threadsCount;

	std::vector<std::thread> threads;
	threads.reserve(threadsCount + remainder ? 1 : 0);

	auto threadedFun = [&manager, viewDistance, fov](size_t start, size_t end)
	{
		for (; start < end; ++start)
		{
			manager.UpdateVisibleUnitCount(start, viewDistance, fov);
		}
	};

	if (step != 0)
	{
		for (size_t i = 0; i < unitsCount - remainder; i += step)
		{
			threads.emplace_back(std::thread{ threadedFun, i, i + step });
		}
	}

	if (remainder != 0)
	{
		threads.emplace_back(std::thread{ threadedFun, unitsCount - remainder, unitsCount });
	}

	for (auto& t : threads)
	{
		t.join();
	}
}

int main()
{
	std::cout << "Enter fov and viewing distance: ";
	
	float fov = 0;
	float viewDistance = 0;
	
	std::cin >> fov >> viewDistance;

	if (fov <= 0)
	{
		std::cout << "\nIncorrect fov!";
		return 0;
	}

	if (viewDistance <= 0)
	{
		std::cout << "\nIncorrect viewing distance!";
		return 0;
	}

	std::cout << "Enter number of units: ";

	size_t unitCount = 0;
	std::cin >> unitCount;
	
	if (unitCount == 0)
	{
		std::cout << "\nIncorrect number of units!";
		return 0;
	}

	std::cout << "Number of units: " << unitCount <<
		". Fov: " << fov << " degree, viewing distance: " << viewDistance << " units." << std::endl;

	ut::UnitsManager manager(unitCount);

	for (size_t i = 0; i < manager.Count(); ++i)
	{
		const auto& unit = manager[i];
		std::cout << "Unit" << i << ", position " << unit.Position << ", direction " << unit.Direction << std::endl;
	}
	
	UpdateAllVisibleUnitCount(manager, viewDistance, fov, 16);

	std::cout << "Result: " << std::endl;
	for (size_t i = 0; i < manager.Count(); ++i)
	{
		const auto& unit = manager[i];
		std::cout << "Unit" << i << ", sees: " << unit.VisibleUnitCount << std::endl;
	}

	return 0;
}

