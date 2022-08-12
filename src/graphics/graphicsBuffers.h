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
	// TODO should vertex data be incorporated to this?
	/// <summary>
	///  BufferObject is a container for the memory and
	///  the buffer handle of a drawable graphics object
	/// </summary>
	struct BufferObject {
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
	};

	/// <summary>
	/// Creates buffer for a given usage flag and size
	/// </summary>
	/// <param name="device">handle</param>
	/// <param name="size"> of the buffer</param>
	/// <param name="usageFlags"> implies the usage of the buffer</param>
	/// <returns>VkBuffer handle</returns>
	VkBuffer createBuffer(VkDevice device, size_t size, VkBufferUsageFlags usageFlags);


	// TODO nuke this function later or at least change the allocation to use a proper allocation handler.
	/// <summary>
	/// Creates, allocates and binds a buffer
	/// </summary>
	/// <param name="device">handle</param>
	/// <param name="size">of the alloc</param>
	/// <returns>VkBuffer handle</returns>
	void createAllocateBindBuffer(VkDevice device, VkPhysicalDevice gpu, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProps, VkBuffer& buffer, VkDeviceMemory& mem, size_t size);

	/// <summary>
	/// Function for allocating gpu memory
	/// </summary>
	/// <param name="allocator">device handle</param>
	/// <param name="gpu">handle</param>
	/// <param name="buffer">handle</param>
	/// <param name="memoryFlags">indicates required memory flag bits.</param>
	/// <returns>VkDeviceMemory handle</returns>
	VkDeviceMemory allocateBuffer(VkDevice allocator, VkPhysicalDevice gpu, VkBuffer buffer, VkMemoryPropertyFlags memoryFlags);

	/// <summary>
	///  Copies memory from host to the gpu
	/// </summary>
	/// <param name="allocator">logical device handle</param>
	/// <param name="gpuMem">gpu memory section handle</param>
	/// <param name="memSize">copyable memory size</param>
	/// <param name="copuSrc">is the to be copied data</param>
	void copyMemoryToGpu(VkDevice allocator, VkDeviceMemory gpuMem, size_t memSize, const void*);

	/// <summary>
	/// Copies source buffer to dst
	/// </summary>
	/// <param name="device"></param>
	/// <param name="src"></param>
	/// <param name="dst"></param>
	/// <param name="cmdPool"></param>
	/// <param name="queue"> is the submit queue</param>
	/// <param name="copyRegion"> is the region copy will happen</param>
	void copyBuffer(VkDevice device, VkBuffer src, VkBuffer dst, VkCommandPool cmdPool, VkQueue queue, const VkBufferCopy& copyRegion);

	void freeAndDestroyBuffer(VkDevice allocator, VkBuffer buffer, VkDeviceMemory bufferMemory);

	uint32_t findMemoryType(VkPhysicalDevice gpu, uint32_t typeFilter, VkMemoryPropertyFlags properties);


	// void createIndexBuffer(); Should be created when creating a buffer? maybe we should combine the two as they are used together anyway.

	// Dummy data tetrahedron
	const std::vector<Vertex> tetrahedronVertices = {
		// Position				Color
		{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{0.0f, 0.0f, 1.0f}, {0.8f, 0.7f, 0.0f}}
	};

	const std::vector<Vertex> triangleVertices = {
		// Position				Color
		{{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}}
	};

	const std::vector<uint16_t> triangleIndices = { 
	0, 1, 2
	};

	const std::vector<uint16_t> tetrahedronIndices = {
	0, 2, 1,
	0, 1, 3,
	1, 3, 2,
	0, 3, 2
	};

}