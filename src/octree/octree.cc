#include "octree.hh"

namespace oLife {
	bool OctreeEntity::operator==(const OctreeEntity &other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	OctreeNode::OctreeNode(
		const std::uint32_t w,
		const std::uint32_t h,
		const std::uint32_t l,
		const std::uint32_t x,
		const std::uint32_t y,
		const std::uint32_t z,
		const std::uint32_t level
	) :
		w(w), h(h), l(l), x(x), y(y), z(z), level(level) {}

	void OctreeNode::insert(OctreeEntity *entity) {
		if (children.size() != 0) {
			const bool left = entity->x - 1 < x,
				right = entity->x + 1 > x,
				bottom = entity->y - 1 < y,
				top = entity->y + 1 > y,
				back = entity->z - 1 < z,
				front = entity->z + 1 > z;

			if (left && bottom && front) children[0]->insert(entity);
			if (left && bottom && back) children[1]->insert(entity);
			if (left && top && front) children[2]->insert(entity);
			if (left && top && back) children[3]->insert(entity);
			if (right && bottom && front) children[4]->insert(entity);
			if (right && bottom && back) children[5]->insert(entity);
			if (right && top && front) children[6]->insert(entity);
			if (right && top && back) children[7]->insert(entity);

			return;
		}

		entities.push_back(entity);

		if (entities.size() == 9 && level <= 8) {
			children[0] = new OctreeNode(w, h, l, x - w / 2, y - h / 2, z + l / 2, level + 1);
			children[1] = new OctreeNode(w, h, l, x - w / 2, y - h / 2, z - l / 2, level + 1);
			children[2] = new OctreeNode(w, h, l, x - w / 2, y + h / 2, z + l / 2, level + 1);
			children[3] = new OctreeNode(w, h, l, x - w / 2, y + h / 2, z - l / 2, level + 1);
			children[4] = new OctreeNode(w, h, l, x + w / 2, y - h / 2, z + l / 2, level + 1);
			children[5] = new OctreeNode(w, h, l, x + w / 2, y - h / 2, z - l / 2, level + 1);
			children[6] = new OctreeNode(w, h, l, x + w / 2, y + h / 2, z + l / 2, level + 1);
			children[7] = new OctreeNode(w, h, l, x + w / 2, y + h / 2, z - l / 2, level + 1);

			for (OctreeEntity *entity : entities) {
				const bool left = entity->x - 1 < x,
					right = entity->x + 1 > x,
					bottom = entity->y - 1 < y,
					top = entity->y + 1 > y,
					back = entity->z - 1 < z,
					front = entity->z + 1 > z;

				if (left && bottom && front) children[0]->insert(entity);
				if (left && bottom && back) children[1]->insert(entity);
				if (left && top && front) children[2]->insert(entity);
				if (left && top && back) children[3]->insert(entity);
				if (right && bottom && front) children[4]->insert(entity);
				if (right && bottom && back) children[5]->insert(entity);
				if (right && top && front) children[6]->insert(entity);
				if (right && top && back) children[7]->insert(entity);
			}

			entities.clear();
		}
	}

	void OctreeNode::update() {
		//
	}

	OctreeEntity *OctreeNode::retrieve(
		const std::uint32_t w,
		const std::uint32_t h,
		const std::uint32_t l,
		const std::uint32_t x,
		const std::uint32_t y,
		const std::uint32_t z
	) {
		//
	}

	void OctreeNode::clear() {
		entities.clear();
		children.clear();
	}
}