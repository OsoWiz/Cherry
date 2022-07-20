#pragma once
#include <glm/vec3.hpp>
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
	static VkVertexInputBindingDescription getBindingDescription();

	///<summary>
	/// Returns how to interpret the members of vertexdata
	///</summary>
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

///<summary>
/// Vertex buffer wrapper that deals with allocation and deallocation.
///</summary>
class VertexBuffer 
{

	VertexBuffer(VkDevice device, size_t size, uint32_t memType, std::vector<Vertex> vertices);
	~VertexBuffer();

	void bindForRender(VkCommandBuffer cmdBuffer);

private:
	VkDevice allocatorDevice;

	VkBuffer vertexBuffer;
	VkDeviceMemory gpuMemory;

	std::vector<Vertex> vertexData;
};

namespace GXBuffer
{
	/// <summary>
	/// Creates a vertex buffer
	/// </summary>
	/// <param name="device">device handle</param>
	/// <param name="size">alloc size</param>
	/// <returns>VkBuffer handle</returns>
	VkBuffer createVertexBuffer(VkDevice device, size_t size);

	/// <summary>
	/// Function for allocating gpu memory
	/// </summary>
	/// <param name="allocator">device handle</param>
	/// <param name="buffer">handle</param>
	/// <param name="memType">is a memory type index</param>
	/// <returns>VkDeviceMemory handle</returns>
	VkDeviceMemory allocateBuffer(VkDevice allocator, VkPhysicalDevice gpu, VkBuffer buffer, uint32_t memType);

	/// <summary>
	///  Copies memory from host to the gpu
	/// </summary>
	/// <param name="allocator">logical device handle</param>
	/// <param name="gpuMem">gpu memory handle</param>
	/// <param name="memSize">copyable memory size</param>
	/// <param name="vertices">vertex data</param>
	void copyMemoryToGpu(VkDevice allocator, VkDeviceMemory gpuMem, size_t memSize, std::vector<Vertex> vertices);

	uint32_t findMemoryType(VkPhysicalDevice gpu, uint32_t typeFilter, VkMemoryPropertyFlags properties);


	// void createIndexBuffer(); Should be created when creating a buffer? maybe we should combine the two as they are used together anyway.

	//Dummy data tetrahedron
	const std::vector<Vertex> tetrahedronVertices = {
		// Position				Color
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{1.0f, 1.0f, 0.0f}, {0.8f, 0.7f, 0.0f}}
	};

}