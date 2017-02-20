#include "Sphere.h"

Sphere::Sphere(GLuint rings, GLuint sectors)
{
	GLfloat const R = 1. / (GLfloat)(rings - 1);
	GLfloat const S = 1. / (GLfloat)(sectors - 1);
	GLfloat radius = 5.0f;

	for (GLuint r = 0; r < rings; ++r) {
		for (GLuint s = 0; s < sectors; ++s) {
			GLfloat const y = sin(-M_PI_2 + M_PI * r * R);
			GLfloat const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			GLfloat const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			//texcoords.push_back(vec2(s*S, r*R));
			vertices.push_back(glm::vec3(x, y, z) * radius);

			if (r < rings - 1){
				int curRow = r * sectors;
				int nextRow = (r + 1) * sectors;
				int nextS = (s + 1) % sectors;

				indices.push_back(curRow + s);
				indices.push_back(nextRow + s);
				indices.push_back(nextRow + nextS);

				indices.push_back(curRow + s);
				indices.push_back(nextRow + nextS);
				indices.push_back(curRow + nextS);
			}
		}
	}
}