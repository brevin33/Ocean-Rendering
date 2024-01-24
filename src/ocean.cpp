#include <ocean.h>
#include "vertex.h"
#include <Mesmerize/Renderer.h>
#include <Mesmerize/Defaults.h>
using namespace MZ;


void makePlane(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
	int res = 10;
	int width = 100;
	int height = 100;
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

float phillips(int n_prime, int m_prime, glm::vec2 w) {
	float length = 1;
	glm::vec2 k(3.1415926 * (2 * n_prime - 256) / length,
		3.1415926 * (2 * m_prime - 256) / length);
	float k_length = k.length();
	if (k_length < 0.000001) return 0.0;

	float k_length2 = k_length * k_length;
	float k_length4 = k_length2 * k_length2;

	float k_dot_w = glm::dot(glm::normalize(k), glm::normalize(w));
	float k_dot_w2 = k_dot_w * k_dot_w;

	float w_length = w.length();
	float L = w_length * w_length / 9.8;
	float L2 = L * L;

	float damping = 0.001;
	float l2 = L2 * damping * damping;

	return 2 * exp(-1.0f / (k_length2 * L2)) / k_length4 * k_dot_w2 * exp(-k_length2 * l2);
}

TextureID createOceanFrequencyTexture(float windSpeed, float gravity) {
	int res = 256;
	std::vector<glm::vec2> imageData(res * res);
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(5, 10);
	for (int y = 0; y < res; y++) {
		for (int x = 0; x < res; x++) {
			float randR = distribution(generator);
			float randI = distribution(generator);
			float phillipsSpectrum = phillips(x, y, glm::vec2(windSpeed, 0));
			float pixValR = (phillipsSpectrum * randR) / sqrt(2);
			float pixValI = (phillipsSpectrum * randI) / sqrt(2);
			imageData[x + y * res] = glm::vec2(pixValR, pixValI);
		}
	}

	return createConstTexture(imageData.data(), res, res, IFFloat2);
}

TextureID getOceanHeightmap(TextureID freq) {
	TextureID i = createGPUMutTexture(256, 256, IFFloat);
	ComputeShaderID c = createComputeShader("../../../shaders/oceanHeightmapComp.spv", 1, 0, 1, 0, 1);
	addCompute(c, 256 / 8, 256 / 8, 1, nullptr, 0, &freq, 1, nullptr, 0, nullptr, 0, nullptr, 0, &i, 1, false);
	return i;
}

uint32_t reverseBits(uint32_t num, uint32_t bits) {
	uint32_t ans = 0;
	for (int i = 0; i < bits; i++) {
		if(num & (1 << (i))) ans = ans | (1 << (bits - 1 - i));
	}
	return ans;
}

typedef glm::vec2 complex;

UniformBufferID makeButterfly() {
	std::vector<uint32_t> reversedBitsArr(256);
	for (int i = 0; i < 256; i++) {
		reversedBitsArr[i] = reverseBits(i, 8);
	}
	std::vector<std::array<glm::vec4, 256>> butterfly(8);
	int N = 256;
	float PI = 3.1415926535897932384626433832795;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < N; y++) {
			float k = fmod(y * ((float)N / pow(2, x + 1)), N);
			complex twiddle = complex(cos(2.0 * PI * k / (float)N), sin(2.0 * PI * k / (float)N));

			int butterflyspan = (int)pow(2, x);
			int butterflywing;
			if ((fmod(y , pow(2, x + 1))) < pow(2, x)) {
				butterflywing = 1;
			}
			else butterflywing = 0;

			if (x == 0) {
				if (butterflywing == 1) {
					butterfly[x][y] = glm::vec4(twiddle.x,twiddle.y, reversedBitsArr[y], reversedBitsArr[y+1]);
				}
				else {
					butterfly[x][y] = glm::vec4(twiddle.x, twiddle.y, reversedBitsArr[y - 1], reversedBitsArr[y]);
				}
			}
			else {
				if (butterflywing == 1) {
					butterfly[x][y] = glm::vec4(twiddle.x, twiddle.y, y, y + butterflyspan);
				}
				else {
					butterfly[x][y] = glm::vec4(twiddle.x, twiddle.y, y - butterflyspan, y);
				}
			}
		}
	}
	return createConstUniformBuffer(butterfly.data(), sizeof(glm::vec4) * 8 * 256);
}

void addOcean() {
	std::vector<Vertex> plane;
	std::vector<uint32_t> indices;
	UniformBufferID a = makeButterfly();
	//TextureID hightMap = createConstTexture("../../../include/dep/MesmerizeRenderer/textures/iceland_heightmap.png");
	TextureID freq = createOceanFrequencyTexture(10, 9.8);
	makePlane(plane, indices);
	TextureID hightMap = getOceanHeightmap(freq);
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
