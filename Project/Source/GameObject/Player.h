#pragma once
#include "GameObject.h"
class Player :
    public GameObject
{
public:
    Player();
    virtual ~Player();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:

};

