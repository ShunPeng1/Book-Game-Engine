#include "Engine/Renders/Mesh.h"

Mesh::Mesh() : mIsInitialized(false), mVAO(0), mVBO(0), mEBO(0), mDrawMode(GL_TRIANGLES) 
{

}

Mesh::Mesh(QString path, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->path = path;
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
	this->mDrawMode = GL_TRIANGLES;
}

Mesh::Mesh(QString path, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, GLenum drawMode)
{
	this->path = path;
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->mDrawMode = drawMode;
}

void Mesh::tryInit()
{
	if (!mIsInitialized)
	{
		init();
		mIsInitialized = true;
	}
}

void Mesh::init()
{
	initializeOpenGLFunctions();

	if (vertices.size() > 0)
		setupMesh();
}

void Mesh::setupMesh() {

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	// vertex color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);
}

void Mesh::write(QJsonObject& json) const {
    json[SERIALIZE_MESH_PATH] = path;

    QJsonArray verticesArray;
    for (const auto& vertex : vertices) {
        QJsonObject vertexObject;
        vertex.write(vertexObject); // Assuming Vertex class has a write method
        verticesArray.append(vertexObject);
    }
    json[SERIALIZE_MESH_VERTICES] = verticesArray;

    QJsonArray indicesArray;
    for (const auto& index : indices) {
        indicesArray.append(static_cast<int>(index));
    }
    json[SERIALIZE_MESH_INDICES] = indicesArray;

    QJsonArray texturesArray;
    for (const auto& texture : textures) {
        QJsonObject textureObject;
        //texture.write(textureObject); // Assuming Texture class has a write method
        //texturesArray.append(textureObject);
    }
    json[SERIALIZE_MESH_TEXTURES] = texturesArray;

    json[SERIALIZE_MESH_DRAW_MODE] = static_cast<int>(mDrawMode);
}

void Mesh::read(const QJsonObject& json) {
    path = json[SERIALIZE_MESH_PATH].toString();

    vertices.clear();
    QJsonArray verticesArray = json[SERIALIZE_MESH_VERTICES].toArray();
    for (int i = 0; i < verticesArray.size(); ++i) {
        QJsonObject vertexObject = verticesArray[i].toObject();
        Vertex vertex;
        vertex.read(vertexObject); // Assuming Vertex class has a read method
        vertices.push_back(vertex);
    }

    indices.clear();
    QJsonArray indicesArray = json[SERIALIZE_MESH_INDICES].toArray();
    for (int i = 0; i < indicesArray.size(); ++i) {
        indices.push_back(static_cast<unsigned int>(indicesArray[i].toInt()));
    }

    textures.clear();
    QJsonArray texturesArray = json[SERIALIZE_MESH_TEXTURES].toArray();
    for (int i = 0; i < texturesArray.size(); ++i) {
        QJsonObject textureObject = texturesArray[i].toObject();
        //Texture texture;
        //texture.read(textureObject); // Assuming Texture class has a read method
        //textures.push_back(texture);
    }

    mDrawMode = static_cast<GLenum>(json[SERIALIZE_MESH_DRAW_MODE].toInt());
}

void Mesh::draw(ShaderProgram& shader) {
	/*unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setUniformInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}*/

	glBindVertexArray(mVAO);
	glDrawElements(mDrawMode, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

