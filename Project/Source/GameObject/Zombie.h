#pragma once
#include "GameObject.h"

class Zombie : public GameObject
{
public:
	Zombie();
	virtual ~Zombie() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

private:

};

