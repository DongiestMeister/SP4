#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "Vertex.h"

class Mesh;

class RenderHelper
{
public:
	static void RenderMesh(Mesh* _mesh);
	static void RenderMeshWithLight(Mesh* _mesh);
	static void RenderText(Mesh* _mesh, const std::string& _text, Color _color);
	static void RenderTextOnScreen(Mesh* _mesh, const std::string& _text, Vector3 translate, float scale, Color _color);
	static void Render2DMesh(Mesh* mesh,Vector3 translate, Vector3 scale, Vector3 rotate);
};

#endif // RENDER_HELPER_H