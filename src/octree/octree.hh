#ifndef OLIFE_OCTREE_HH
#define OLIFE_OCTREE_HH

#include <cstdint>
#include <vector>

namespace oLife {
	struct OctreeEntity {
		std::uint32_t x, y, z;
		std::uint8_t cell_state;

		bool operator==(const OctreeEntity &other) const;
	};

	class OctreeNode {
		public:
			const std::uint32_t w, h, l, x, y, z, level;

			std::vector<OctreeNode *> children;
			std::vector<OctreeEntity *> entities;

			OctreeNode(
				const std::uint32_t w,
				const std::uint32_t h,
				const std::uint32_t l,
				const std::uint32_t x = 0,
				const std::uint32_t y = 0,
				const std::uint32_t z = 0,
				const std::uint32_t level = 0
			);

			void insert(OctreeEntity *entity);

			OctreeEntity *retrieve(
				const std::uint32_t w,
				const std::uint32_t h,
				const std::uint32_t l,
				const std::uint32_t x,
				const std::uint32_t y,
				const std::uint32_t z
			);

			void update();

			void clear();
	};
}

#endif
