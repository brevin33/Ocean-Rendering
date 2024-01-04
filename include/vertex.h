#pragma once
#include <glm/glm.hpp>
#include <Mesmerize/Renderer.h>

using namespace MZ;
struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;

	static std::array<VertexValueType,2> getVertexValues() {
		std::array<VertexValueType, 2> vertValues;
		vertValues[0] = VTfloat3;
		vertValues[1] = VTfloat3;
		return vertValues;
	}
};