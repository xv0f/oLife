#ifndef OLIFE_OCTREE_NODE_HH
#define OLIFE_OCTREE_NODE_HH

#include <memory>
#include <cstdint>
#include <vector>

template <typename T>
void fast_erase(std::vector<T> &vector, std::size_t index);

namespace oLife {
	class Octree;
	class OctreeNode;

	struct OctreeEntity {
		std::int32_t x, y, z;
		std::uint8_t cell_state;

		bool operator==(const OctreeEntity &other) const;
	};

	struct OctreeNodeData {
		Octree *octree;
		OctreeNode *parent;

		std::int32_t x, y, z;
		std::uint32_t w, h, l;

		std::uint32_t level, index;
	};

	class OctreeNode {
		private:
			OctreeNodeData *data;

			std::vector<std::unique_ptr<OctreeNode>> children;
			std::vector<std::unique_ptr<OctreeEntity>> entities;

			OctreeNode(OctreeNodeData *data);

		public:
			static std::unique_ptr<OctreeNode> create_node(OctreeNodeData *data);

			~OctreeNode();

			void insert(OctreeEntity *entity);

			std::vector<std::unique_ptr<OctreeEntity>> query(
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