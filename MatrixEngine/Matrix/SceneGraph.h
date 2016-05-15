#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "matrix_engine_internal.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Core::IO;

class Scene::SceneGraph {
	public :
		SceneGraph();
		~SceneGraph();

		void AddEntityNode(Scene::SceneEntity *node);
		void RemoveEntityNode(Scene::SceneEntity *node);

		int NodesCount();

		std::vector<Scene::SceneEntity*> *GetNodes();
		Scene::SceneEntity *GetNode(int index);

		void RenderScene();

	protected :
		std::vector<Scene::SceneEntity*> nodes;
};

#endif