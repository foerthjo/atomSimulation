#shader compute
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer position_layout {
	vec4 positions[];
};

layout(std430, binding = 1) buffer mass_layout {
	float masses[];
};

layout(std430, binding = 2) buffer charge_layout {
	float charges[];
};

layout(std430, binding = 3) buffer velocity_layout {
	vec4 velocities[];
};

layout(std430, binding = 4) buffer output_layout {
	vec4 newVelocities[];
};

void main() {
	const uint index = gl_GlobalInvocationID.x;

	float dt = 0.0016;

	bool coreElement = charges[index] >= 0;

	vec3 velocity = velocities[index].xyz;
	velocity *= .98;

	for (int i = 0; i < gl_NumWorkGroups.x; i++) {
		if (i != index) {
			bool otherCoreElement = charges[i] >= 0;
			vec3 delta = positions[index].xyz - positions[i].xyz;
			float distance = length(delta);
			float sqrDistance = distance * distance;
			vec3 direction = delta / distance;

			if (distance > 0.1) {

				velocity += charges[index] * charges[i] * direction / sqrDistance * dt / masses[index];

				velocity -= direction / sqrDistance * dt / masses[index];

				if (coreElement && otherCoreElement) {
					velocity -= direction * dt / masses[index];
				}
			}
			
			if (distance < 0.8) {
				velocity += direction / sqrDistance * dt / masses[index];
			}
		}
	}

	newVelocities[index] = vec4(velocity, 0);
}