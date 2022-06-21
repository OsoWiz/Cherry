#pragma once
#include "glm/vec3.hpp"
#include <vulkan/vulkan.h>
// std
#include <array>
#include <vector>
#include <stdexcept>

struct Vertex {
	// members
	glm::vec3 pos;
	glm::vec3 color;

	// methods

	///<summary>
	/// Returns the format in which the vertex shader input is.
	///</summary>
	VkVertexInputBindingDescription getBindingDescription();

	///<summary>
	/// Returns how to interpret the members of vertexdata
	///</summary>
	std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

///<summary>
/// Returns a Vertex buffer object for given device with the specified size.
/// <remark> 
/// DOES NOT ALLOCATE/ASSIGN MEMORY FOR THE BUFFER. 
/// However the size specified can be used in the allocation phase in order to make sure the memory size match
/// </remark>
///</summary>
VkBuffer createVertexBuffer(VkDevice device, size_t size);

void createIndexBuffer();
