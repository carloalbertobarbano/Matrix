#include "SceneEntity.h"
#include "SceneGraph.h"

Scene::SceneEntity::SceneEntity(std::string name)
{
	if (name == "" && Scene::_pCurrentSceneGraph)
		node_name = std::string("node") + std::to_string(Scene::_pCurrentSceneGraph->NodesCount());
	else
		node_name = name;

	scale = glm::vec3(1.0, 1.0, 1.0);
	rot = glm::vec3(1.0, 1.0, 1.0);
	angle = 0;
	parent = NULL;
}

Scene::SceneEntity::~SceneEntity()
{

}

void Scene::SceneEntity::AddChild(SceneEntity * child)
{
	children.push_back(child);
	children.back()->SetParent(this);
}

void Scene::SceneEntity::RemoveChild(SceneEntity * child)
{
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
	child->SetParent(NULL);
}

Scene::SceneEntity * Scene::SceneEntity::GetParent()
{
	return parent;
}

Scene::SceneEntity * Scene::SceneEntity::GetChild(int index)
{
	if (index < 0 || index >= children.size()) {
		Log::WriteOnStream("SceneEntity::GetChild(): Invalid child index " + std::to_string(index), Log::error_stream);
		return nullptr;
	}

	return children[index];
}

int Scene::SceneEntity::GetChildrenCount()
{
	return children.size();
}

void Scene::SceneEntity::SetNodeName(std::string name)
{
	node_name = name;
}

std::string Scene::SceneEntity::GetNodeName()
{
	return node_name;
}

void Scene::SceneEntity::Translate(glm::vec3 p)
{
	pos = p;
}

void Scene::SceneEntity::Rotate(float angle, float x, float y, float z)
{
	if (angle >= 360)angle -= 360;

	rot = glm::vec3(x, y, z);
	this->angle = angle;
}


void Scene::SceneEntity::Scale(glm::vec3 s)
{
	scale = s;
}

void Scene::SceneEntity::BuildModelMatrix()
{
	glm::mat4 base = glm::mat4(1.0);
	if(parent)base = parent->GetModelMatrix();

	modMatrix = glm::translate(base, pos);
	modMatrix = glm::rotate(modMatrix, glm::radians(angle), rot);
	//SDL_Log("Angle: %f Rot: %f %f %f\n", angle, rot.x, rot.y, rot.z);

	modMatrix = glm::scale(modMatrix, scale);
}

void Scene::SceneEntity::RenderThis()
{
	Render();
}

void Scene::SceneEntity::RenderThisAndSub()
{
	Render();
	for (int i = 0; i < children.size(); i++)
		children[i]->Render();
}

void Scene::SceneEntity::Render()
{
	
}

void Scene::SceneEntity::SetParent(SceneEntity * parent)
{
	this->parent = parent;
}
