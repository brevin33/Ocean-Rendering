#include <ocean.h>
#include "vertex.h"
#include <Mesmerize/Renderer.h>
#include <Mesmerize/Defaults.h>
using namespace MZ;
std::vector<Vertex> plane = {
	{glm::vec3(10,0,10), glm::vec3(1,0,0)}, {glm::vec3(10,0,-10), glm::vec3(0,1,0)},
	{glm::vec3(-10,0,10), glm::vec3(0,0,1)}, {glm::vec3(-10,0,-10), glm::vec3(1,1,1)},
};

std::vector<uint32_t> indices = {
	0,1,2,
	2,1,3,
};


void addOcean() {
	VertexBufferID planeVertexBuffer = createConstVertexBuffer(plane.data(), plane.size(), plane.size() * sizeof(Vertex));
	IndexBufferID planeIndexBuffer = createConstIndexBuffer(indices.data(), indices.size() * sizeof(uint32_t));
	ShaderID oceanShader = createShader("../../../shaders/oceanVert.spv", "../../../shaders/oceanFrag.spv", 1, 0, 2, Vertex::getVertexValues().data(), Vertex::getVertexValues().size(), nullptr, 0);
	glm::mat4 model = glm::mat4(1);
	UniformBufferID modelBuffer = createCPUMutUniformBuffer(&model,sizeof(glm::mat4), sizeof(glm::mat4));
	std::array<UniformBufferID,2> oceanUniformBuffers = { mainCameraBuffer , modelBuffer };
	MaterialID oceanMaterial = createMaterial(oceanShader, nullptr, 0, oceanUniformBuffers.data(), oceanUniformBuffers.size());
	RenderObjectID ocean = addRenderObject(oceanMaterial, planeVertexBuffer, planeIndexBuffer);
}