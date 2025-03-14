#ifndef CONTAINER_H
#define CONTAINER_H


#include "Engine/Scenes/Node.h"
#include "Engine/Components/Transform.h"

class Container : public Node  
{  
public:  
	Container();  
	virtual ~Container() noexcept;

	virtual void setParent(Node* parent) override;

public: // Interfaces
	virtual void write(QJsonObject& json) const override;
	virtual void read(const QJsonObject& json) override;
	virtual void* accept(INodeVisitor* visitor) override;

public:  
	std::shared_ptr<Transform> transform; // Use shared_ptr
};


#endif // !CONTAINER_H
