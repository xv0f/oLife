#include <cstdlib>
#include <chrono>
#include <iostream>

#include <windows.h>
#include <psapi.h>

#include "octree/octree.hh"

int main() {
	oLife::Octree octree(16384, 16384, 16384);

	auto start = std::chrono::high_resolution_clock::now();

	for (std::size_t i = 0; i < 500000; i++) {
		oLife::OctreeEntity entity = oLife::OctreeEntity {
			std::rand() % 8192,
			std::rand() % 8192,
			std::rand() % 8192,
			
			static_cast<uint8_t>(std::rand() % 2)
		};

		octree.root->insert(&entity);
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T mem = pmc.PrivateUsage;

	std::cout << mem << std::endl;
}