#pragma once

#include "renderer.h"

class VertexBuffer {
public:
	VertexBuffer(unsigned int max_size) : max_size_(max_size) {};

    ~VertexBuffer() {
		GL_CALL(glDeleteBuffers(1, &id_));
	}

	void init (const void* data, unsigned int size) {
		ASSERT(size <= max_size_, "vb init size error");

		GL_CALL(glGenBuffers(1, &id_));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id_));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
	};

	void bind() const {
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id_));
	}

	void unbind() const {
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void subData(const void* data, unsigned int size) const {
		ASSERT(size <= max_size_, "vb subData size error");

		GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}
private:
	unsigned int id_{};
	const unsigned int max_size_;
};