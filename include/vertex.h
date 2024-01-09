#pragma once
#include <glm/glm.hpp>
#include <Mesmerize/Renderer.h>

using namespace MZ;
struct Vertex {
	glm::vec3 pos;
	glm::vec2 uv;

	static std::array<VertexValueType,2> getVertexValues() {
		std::array<VertexValueType, 2> vertValues;
		vertValues[0] = VTfloat3;
		vertValues[1] = VTfloat2;
		return vertValues;
	}
};