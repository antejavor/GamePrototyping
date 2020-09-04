#pragma once

class Component
{
public:
	//Update order defines, opreder priority
	Component(class Actor* owner, int updateOrder = 100); 
	virtual ~Component();

	virtual void Update(float deltaTime);
	int GetUpdateOrder() const { return mUpdateOrder; }
	
protected:

	class Actor* mOwner;
	int mUpdateOrder;

};

