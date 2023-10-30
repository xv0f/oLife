#ifndef OLIFE_OCTREE_NODE_HH
#define OLIFE_OCTREE_NODE_HH

#include <cstdint>
#include <vector>

template <typename T>
void fast_erase(std::vector<T> &vector, std::size_t index);

namespace oLife {
	class Octree;

	struct OctreeEntity {
		std::int32_t x, y, z;
		std::uint8_t cell_state;

		bool operator==(const OctreeEntity &other) const;
	};

	class OctreeNode {
		private:
			Octree *octree;

			std::uint32_t level;
			std::uint32_t index;

			std::vector<OctreeNode *> children;
			std::vector<OctreeEntity *> entities;

		public:
			OctreeNode(Octree *octree, std::uint32_t index, std::uint32_t level = 0);

			std::int32_t x();
			std::int32_t y();
			std::int32_t z();

			std::uint32_t w();
			std::uint32_t h();
			std::uint32_t l();

			void insert(OctreeEntity *entity);

			std::vector<OctreeEntity *> query(
				std::uint32_t w,
				std::uint32_t h,
				std::uint32_t l,
				std::int32_t x_pos,
				std::int32_t y_pos,
				std::int32_t z_pos
			);

			void scale(std::uint32_t scalar);

			void update();

			void empty();
			void clear();
	};
}

#endif