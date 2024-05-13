// #include "UnitTests.h"
// #include "Interfaces.h"
// #include "CollisionHandler.h"
// #include <memory>
// #include <vector>

// namespace Ecosim
// {
//     class Test_Collidable : public Collidable
//     {
//     private:
//         Vector2<float> m_Pos;
//         int m_Radius = 2;

//     public:
//         Test_Collidable(Vector2<float> pos) : m_Pos(pos) {}

//         void handleCollision(std::shared_ptr<Collidable> &other) override {}

//         bool Collides(std::shared_ptr<Collidable> &other) override
//         {
//             return (m_Pos - other->getPosition()).Magnitude() < m_Radius + other->getRadius();
//         }
//         Vector2<float> getPosition() override { return m_Pos; }
//         int getRadius() override { return m_Radius; }
//         void OnEaten() override {}
//         float getEnergy() override { return 0; }
//         int GetId() override { return -1; }
//         CollidableType getType() override { return FOOD; }
//     };
// }

// void CollisionHandler_Insert()
// {
//     std::vector<std::shared_ptr<Ecosim::Collidable>> collidables;
//     for (int i = 0; i < 100; i++)
//     {
//         Ecosim::Test_Collidable c{Ecosim::Vector2<float>(1, 1)};
//         std::shared_ptr<Ecosim::Test_Collidable> ptr = std::make_shared<Ecosim::Test_Collidable>(c);
//         collidables.push_back(ptr);
//     }

//     Ecosim::CollisionHandler::SetCollidables(collidables);
//     Ecosim::CollisionHandler::Rebuild();

//     std::vector<std::shared_ptr<Ecosim::Collidable>> queryResults;
//     Ecosim::CollisionHandler::Query(Ecosim::Vector2<float>(0, 0), 10, queryResults);

//     FAIL_IF_FALSE(queryResults.size() == 100, "CollisionHandler_Insert");
// }

// int main()
// {
//     std::cout << "Running CollisionHandler unit tests" << std::endl;
//     TEST_FUNCTION(CollisionHandler_Insert);
//     return 0;
// }

int main()
{
    return 0;
}
