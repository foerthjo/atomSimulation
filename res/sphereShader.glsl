#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform vec3 cameraPosition;
uniform vec3 cameraForward;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

uniform float near, far;

out vec4 vertexColor;

void main() {
    vec3 pos = position - cameraPosition;
    pos = vec3(
        dot(pos, cameraRight),
        dot(pos, cameraUp),
        dot(pos, cameraForward)
    );
    float depth = pos.z;
    pos.z = ((pos.z - near) / (far - near)) * (far + near) - near * 2;
    gl_Position = vec4(pos, depth);

    float brightness = position.y * 0.5 + 0.5;
    vertexColor = vec4(vec3(brightness), 0.5);
};

#shader fragment
#version 330 core

in vec4 vertexColor;

layout(location = 0) out vec4 fragmentColor;

void main() {
    fragmentColor = vertexColor;
};