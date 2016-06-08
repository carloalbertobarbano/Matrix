#include "SceneGraph.h"
#include "SceneEntity.h"

using namespace MatrixEngine::Scene;

Scene::SceneGraph::SceneGraph()
{
	Scene::_pCurrentSceneGraph = this;
}

Scene::SceneGraph::~SceneGraph()
{

}

void Scene::SceneGraph::AddEntityNode(Scene::SceneEntity * node)
{
	nodes.push_back(node);
}

void Scene::SceneGraph::RemoveEntityNode(Scene::SceneEntity * node)
{
	nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
}

int Scene::SceneGraph::NodesCount()
{
	return nodes.size();
}

std::vector<Scene::SceneEntity*>* Scene::SceneGraph::GetNodes()
{
	return &nodes;
}

Scene::SceneEntity * Scene::SceneGraph::GetNode(int index)
{
	if (index < 0 || index >= nodes.size()) {
		Log::WriteOnStream("SceneGraph::GetNode(): Invalid node index " + std::to_string(index), Log::error_stream);
		return nullptr;
	}

	return nodes[index];
}

void Scene::SceneGraph::RenderScene()
{
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->RenderThisAndSub();
	}
}








