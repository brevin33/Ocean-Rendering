#include <ocean.h>
#include "vertex.h"
#include <Mesmerize/Renderer.h>
#include <Mesmerize/Defaults.h>
using namespace MZ;


void makePlane(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
	int res = 60;
	int width = 600;
	int height = 600;
	std::array<std::array<int, 2>, 4> quad;

	for (int i = -height; i < height; i += (height * 2) / res)
	{
		for (int j = -width; j < width; j += (width * 2) / res)
		{
			Vertex v;
			v.pos = glm::vec3(i, 0, j);
			v.uv = glm::vec2((i + height) / (float)(height * 2), (j + width) / (float)(width * 2));
			vertices.push_back(v);
		}
	}

	for (int j = 0; j < res - 1; j++)
	{
		for (int i = 0; i < res - 1; i++)
		{
			indices.push_back(i + j * res);
			indices.push_back((i + 1) + j * res);
			indices.push_back(i + (j + 1) * res);
			indices.push_back((i + 1) + (j + 1) * res);
		}
	}
}




void addOcean() {
	std::vector<Vertex> plane;
	std::vector<uint32_t> indices;
	TextureID hightMap = createTexture("../../../include/dep/MesmerizeRenderer/textures/iceland_heightmap.png");
	makePlane(plane, indices);
	VertexBufferID planeVertexBuffer = createConstVertexBuffer(plane.data(), plane.size(), plane.size() * sizeof(Vertex));
	IndexBufferID planeIndexBuffer = createConstIndexBuffer(indices.data(), indices.size() * sizeof(uint32_t));
	ShaderStages stages[2] = { SSTessCon | SSTessEval, SSTessCon | SSTessEval };
	ShaderStages stagesTexture[1] = { SSTessEval };
	ShaderID tessShader = createShader("../../../shaders/oceanVert.spv", "../../../shaders/oceanFrag.spv", "../../../shaders/oceanTesc.spv",
		"../../../shaders/oceanTese.spv", 1, stagesTexture, 1, stages, 2, Vertex::getVertexValues().data(), Vertex::getVertexValues().size(), nullptr, 0, FrontCull);
	glm::mat4 model = glm::mat4(1);
	UniformBufferID modelBuffer = createCPUMutUniformBuffer(&model, sizeof(glm::mat4), sizeof(glm::mat4));
	std::array<UniformBufferID, 2> tessUniformBuffers = { mainCameraBuffer , modelBuffer };
	MaterialID tessMaterial = createMaterial(tessShader, &hightMap, 1, tessUniformBuffers.data(), tessUniformBuffers.size());
	RenderObjectID terrain = addRenderObject(tessMaterial, planeVertexBuffer, planeIndexBuffer);
}
