#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <vector>
#include <memory>
#include "Vector2.h"

namespace Ecosim
{
    class Agent
    {
    private:
        Vector2<float> m_Pos;
        float m_Health;
        float m_Energy;

        int dnaLength;
        int numAttributes;
        std::vector<int> m_DNA;

    public:
        Agent();
        ~Agent();

        Vector2<float> getPos();
        float getHealth();
        float getEnergy();

        const std::vector<int> &getDNA() const { return m_DNA; }
    };

}

#endif /* AGENT_H */