#include "MeshAsset.h"

MeshSection::MeshSection() : vao()
{

}

void MeshSection::ConstructVAO()
{
	int vertex_location = 0;
	int normal_location = 1;
	int texture_location = 2;
	int tangent_location = 3;

	vao = std::make_shared<Memory::VAO>();
	vao->Construct(4);

	vao->BufferData(vertex_location, GL_ARRAY_BUFFER, vert.size()*sizeof(float), &vert.data()[0], GL_STATIC_DRAW);
	vao->VertexAttribute(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	vao->vert_count = vert.size();

	if (norm.size() > 0)
	{
		vao->BufferData(normal_location, GL_ARRAY_BUFFER, norm.size()*sizeof(float), &norm.data()[0], GL_STATIC_DRAW);
		vao->VertexAttribute(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}


	if (tex_coord.size() > 0)
	{
		vao->BufferData(texture_location, GL_ARRAY_BUFFER, tex_coord.size()*sizeof(float), &tex_coord.data()[0], GL_STATIC_DRAW);
		vao->VertexAttribute(texture_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (tang.size() > 0)
	{
		vao->BufferData(tangent_location, GL_ARRAY_BUFFER, tang.size()*sizeof(float), &tang.data()[0], GL_STATIC_DRAW);
		vao->VertexAttribute(tangent_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}


	vao->indices = indices;

	vao->Unbind();
}

void MeshSection::Render()
{
	vao->DrawArrays(GL_TRIANGLES, 0, vert.size() / 3);
}

Scene::Components::MeshAsset::MeshAsset()
{
}

Scene::Components::MeshAsset::~MeshAsset()
{
}

void Scene::Components::MeshAsset::LoadAsset(std::string file)
{
	Log::WriteOnStream("Loading asset: " + file + "\n", Log::log_stream);

	Importer importer;
	const aiScene *scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::WriteOnStream("Could not load file " + file + "\n", Log::error_stream);
		return;
	}

	for (int i = 0; i < scene->mNumMeshes; i++) {
		processMesh(scene->mMeshes[i], scene);
	}
}

void Scene::Components::MeshAsset::processMesh(aiMesh * ai_mesh, const aiScene * scene)
{
	std::shared_ptr<MeshSection> section(new MeshSection);

	for (int i = 0; i < ai_mesh->mNumVertices; i++) {
		vec3 m_vert(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
		vec3 m_norm(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
		vec3 m_tang = vec3(0.0); // (mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		vec2 m_tex_coord = ai_mesh->HasTextureCoords(0) ? vec2(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y) : vec2(0.0, 0.0);

		section->vert.push_back(m_vert.x);
		section->vert.push_back(m_vert.y);
		section->vert.push_back(m_vert.z);

		section->norm.push_back(m_norm.x);
		section->norm.push_back(m_norm.y);
		section->norm.push_back(m_norm.z);

		section->tang.push_back(m_tang.x);
		section->tang.push_back(m_tang.y);
		section->tang.push_back(m_tang.z);

		section->tex_coord.push_back(m_tex_coord.x);
		section->tex_coord.push_back(-m_tex_coord.y);
	}

	for (int i = 0; i < ai_mesh->mNumFaces; i++) {
		aiFace &face = ai_mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++) {
			section->indices.push_back(face.mIndices[j]);
		}
	}

	aiString texture_diffuse;
	scene->mMaterials[ai_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture_diffuse, NULL, NULL, NULL, NULL, NULL);
	section->material.texture_diffuse.LoadFromFile("data/models/" + std::string(texture_diffuse.data));

	const aiMaterial *material = scene->mMaterials[ai_mesh->mMaterialIndex];
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emissive;
	aiColor4D reflective;

	int shininess;

	float refraction = 0;

	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive);
	aiGetMaterialInteger(material, AI_MATKEY_SHININESS, &shininess);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_REFLECTIVE, &reflective);
	aiGetMaterialFloat(material, AI_MATKEY_REFRACTI, &refraction);


	if (emissive.r != 0.0 || emissive.g != 0.0 || emissive.b != 0.0) {
		ambient = emissive;
		section->material.emissive = true;
	}

	int shadingModel;
	material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
	SDL_Log("Shading model: %d\n", shadingModel);

	section->material.ambient = glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a);
	section->material.diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	section->material.specular = glm::vec4(specular.r, specular.g, specular.b, specular.a);
	section->material.shininess = shininess;
	
	if (shadingModel == 2 || reflective.r != 0.0 || reflective.g != 0.0 || reflective.b != 0.0) {
		SDL_Log("Found reflective material..\n");
		section->material.reflection = true;
	}

	section->material.refraction = (refraction != 0 ? true : false);

	SDL_Log("Section %d has %d vertices, %d normals, %d texture coords, %d tangents, %d indices, %d bones\n", 
			sections.size(), 
			section->vert.size() / 3,
			section->norm.size() / 3,
			section->tex_coord.size() / 2,
			section->tang.size() / 3,
			section->indices.size(),
			ai_mesh->mNumBones);

	section->ConstructVAO();
	sections.push_back(section);
}
