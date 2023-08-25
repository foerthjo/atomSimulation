#shader compute
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer position_layout {
	vec4 position[];
};

layout(std430, binding = 1) buffer velocity_layout {
	vec4 velocity[];
};

layout(std430, binding = 2) buffer output_layout {
	vec4 newPosition[];
};

void main() {
	const uint index = gl_GlobalInvocationID.x;
	newPosition[index] = vec4(position[index].xyz + velocity[index].xyz * 0.0016, 0);
}