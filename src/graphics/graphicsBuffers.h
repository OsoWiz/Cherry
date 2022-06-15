#pragma once
#include "glm/vec3.hpp"
#include <vulkan/vulkan.h>

struct Vertex {
	glm::vec3 pos;
};

static VkVertexInputBindingDescription getBindingDescription() {
	VkVertexInputBindingDescription desc;

	desc.binding = 0;
	desc.stride = sizeof(Vertex);
	desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return desc;
}