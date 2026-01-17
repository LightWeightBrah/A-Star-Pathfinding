#pragma once

#pragma once

class VertexArray
{
private:
	unsigned int id;
public:
	VertexArray();
	~VertexArray();

	void AddAttrib(unsigned int index, unsigned int size,
		unsigned int type, bool normalized, unsigned int stride, const void* offset) const;
	void Bind() const;
	void Unbind() const;
};