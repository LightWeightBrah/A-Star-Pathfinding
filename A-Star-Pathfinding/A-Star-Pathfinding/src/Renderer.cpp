#include <iostream>
#include "Renderer.h"
#include "Animator.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OPEN_GL ERROR (" << error << "): " << function << " "
			<< file << ":" << line << std::endl;
		return false;
	}

	return true;
}

void Renderer::Clear(float r, float g, float b, float a) const
{
	GLCall(glClearColor(r, g,b, a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& VAO, const ElementBuffer& EBO, const Shader& shader) const
{
	shader.Bind();
	VAO.Bind();
	EBO.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, EBO.GetCount(), GL_UNSIGNED_INT, 0));
}

void Renderer::DrawModel(const Model& model, Shader& shader, const Animator* animator) const
{
	shader.Bind();

	bool hasAnimations = (animator && model.HasAnimations());
	shader.SetUniform1i("hasAnimations", hasAnimations);
	if (hasAnimations)
		shader.SetBoneMatrices(animator->GetFinalBoneMatrices());

	for (const auto& mesh : model.GetMeshes())
		DrawMesh(mesh, shader);
}

void Renderer::DrawMesh(const Mesh& mesh, Shader& shader) const
{
	mesh.BindTextures(shader);
	Draw(mesh.GetVAO(), mesh.GetEBO(), shader);
}
