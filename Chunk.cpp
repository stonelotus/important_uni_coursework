#include "Chunk.h"


Chunk::Chunk() :sizes(glm::vec3(16,16,16)) {
    setup();
};

Chunk::Chunk(glm::vec3 sizes)
    : sizes(sizes) {
    setup();
}

Chunk::~Chunk() {
	std::cout << "Chunk destructor called" << std::endl;
}

const Block& Chunk::getBlock(glm::vec3 block_position) const {
 
    return blocks[block_position.x][block_position.y][block_position.z];
}

void Chunk::setup() {
    std::cout << "Setting up chunk..." << std::endl;
    blocks = std::vector<std::vector<std::vector<Block>>>(sizes.x, std::vector<std::vector<Block>>(sizes.y, std::vector<Block>(sizes.z,Block())));

    std::cout << "Set up blocks" << std::endl;
    for (int x = 0; x < sizes.x; ++x) {
        for (int y = 0; y < sizes.y; ++y) {
            for (int z = 0; z < sizes.z; ++z) {
                glm::vec3 position(x, y, z);
                BlockType block_type = BlockType::DIRT;
                if(x%2 == 0 ) block_type = BlockType::DIRT;
				else block_type = BlockType::STONE;
             
                setBlock(glm::vec3(x, y, z), Block(position, block_type));  //TODO check if Block still exists after this function
            }
        }
    }
}

glm::vec3 Chunk::getPosition() const
{
    return origin_position;
}
glm::vec3 Chunk::getSize() {
	return sizes;
}
void Chunk::setBlock(glm::vec3 block_position, const Block& block) {
    blocks[block_position.x][block_position.y][block_position.z] = block;
}

void Chunk::generateVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices) const {
    vertices.clear();
    indices.clear();

    for (int x = 0; x < sizes.x; ++x) {
        for (int y = 0; y < sizes.y; ++y) {
            for (int z = 0; z < sizes.z; ++z) {
                const Block& block = getBlock(glm::vec3(x,y,z));
                block.generateVertices(vertices, indices);
            }
        }
    }
}


void Chunk::removeBlock(int position) {
    blocks.erase(blocks.begin() + position);
    std::cout << "Num of active blocks in this chunk: " << blocks.size() << std::endl;
}



BlockLighting Chunk::calculateAverageMaterialProperties() const {
    BlockLighting average = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f };
    int count = 0;

    for (const auto& layer : blocks) {
        for (const auto& row : layer) {
            for (const auto& block : row) {
                average.ambient += block.getLighting().ambient;
                average.diffuse += block.getLighting().diffuse;
                average.specular += block.getLighting().specular;
                average.shininess += block.getLighting().shininess;
                ++count;
            }
        }
    }
    if (count > 0) {
        average.ambient /= static_cast<float>(count);
        average.diffuse /= static_cast<float>(count);
        average.specular /= static_cast<float>(count);
        average.shininess /= static_cast<float>(count);
    }

    return average;
}