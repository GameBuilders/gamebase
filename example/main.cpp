/*
 * This is a simple demo highlighting basic usage of the SFML and Box2D libraries.
 * Hopefully this code will help you get started if you're unfamiliar with either.
 * For more advanced usage, be sure to consult the respective libraries' documentation!
 */

#include <cstdlib>

// Include the SFML and Box2D headers
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

// Returns a pseudo-random number between 0 and 1
double randDouble()
{
    return rand() / (double)RAND_MAX;
}

// Returns a pseudo-random number in the given range
double randRange(double mn, double mx)
{
    return (randDouble() * (mx - mn)) + mn;
}

// Conversion factor between Box2D units and pixels. 1 Box2D unit = 50 pixels
#define FACTOR 50

// Converstion factor between radians and degrees. 1 radian = 180 / pi degrees
#define RADIAN_TO_DEGREE 57.2957795131

struct Shape
{
    float centerX, centerY;
    float rotation;
};

struct Box : public Shape
{
    float halfWidth, halfHeight;
};

struct Circle : public Shape
{
    float radius;
};

/**
 * Creates and renders shapes on behalf of other objects
 */
class ShapeRenderer
{
public:
    Box * addBox(float centerX, float centerY, float halfWidth, float halfHeight)
    {
        Box* b = new Box();
        b->centerX = centerX;
        b->centerY = centerY;
        b->halfWidth = halfWidth;
        b->halfHeight = halfHeight;
        m_boxList.push_back(b);
        return b;
    }

    Circle * addCircle(float centerX, float centerY, float radius)
    {
        Circle* c = new Circle();
        c->centerX = centerX;
        c->centerY = centerY;
        c->radius = radius;
        m_circleList.push_back(c);
        return c;
    }

    void renderShapes(sf::RenderWindow& window)
    {
        for (std::vector<Box*>::iterator it = m_boxList.begin(); it != m_boxList.end(); ++it) {
            sf::RectangleShape shape(sf::Vector2f((*it)->halfWidth * 2 * FACTOR, (*it)->halfHeight * 2 * FACTOR));
            shape.setOrigin((*it)->halfWidth * FACTOR, (*it)->halfHeight * FACTOR);
            shape.setPosition((*it)->centerX * FACTOR, (*it)->centerY * FACTOR);
            shape.setRotation((*it)->rotation * RADIAN_TO_DEGREE);
            shape.setFillColor(sf::Color(100, 250, 50));
            window.draw(shape);
        }

        for (std::vector<Circle*>::iterator it = m_circleList.begin(); it != m_circleList.end(); ++it) {
            sf::CircleShape shape((*it)->radius * FACTOR);
            shape.setOrigin((*it)->radius * FACTOR, (*it)->radius * FACTOR);
            shape.setPosition((*it)->centerX * FACTOR, (*it)->centerY * FACTOR);
            shape.setRotation((*it)->rotation * RADIAN_TO_DEGREE);
            shape.setFillColor(sf::Color(100, 250, 50));
            window.draw(shape);
        }
    }
private:
    std::vector<Box*> m_boxList;
    std::vector<Circle*> m_circleList;
};

/**
 * Abstract interface for a game state that gets updated over time
 */
class State
{
public:
    virtual ~State() = default;

    /**
     * Call to update the state the given amount of time
     * @param dt The delta time (i.e. the amount of time to update the state)
     */
    virtual void update(double dt) = 0;
};

struct FixtureUserData
{
    Shape * shape;
};

/**
 * A state that controls the game
 */
class GameState : public State
{
private:
    ShapeRenderer * m_renderer;

    b2World * m_physicsWorld;
    double m_accumTime;
public:
    GameState(ShapeRenderer * renderer) : m_renderer(renderer), m_accumTime(0.0)
    {
        // Construct a physical world with gravity
        m_physicsWorld = new b2World(b2Vec2(0, 10));

        // Add balls to demonstrate the physics are working
        spawnBalls();

        // Add a ground box to keep the circles from falling forever
        addStaticBox(0.0f, 16.0f, 10.0f, 10.1f);
        addStaticBox(0.0f, 0.1f, 00.0f, 10.0f);
        addStaticBox(6.0f, 6.1f, 5.0f, 10.0f);
        addStaticBox(10.0f, 10.1f, 5.0f, 10.0f);
        addStaticBox(15.9f, 16.0f, 0.0f, 10.0f);
    }

    void addStaticBox(float minX, float maxX, float minY, float maxY)
    {
        float width = maxX - minX;
        float height = maxY - minY;

        // 1. Make Shape
        b2PolygonShape boxShape;
        boxShape.SetAsBox(width / 2, height / 2);

        // 2. Make Body
        b2BodyDef boxBodyDef;
        boxBodyDef.position.Set(minX + width / 2, minY + height / 2);
        boxBodyDef.type = b2_staticBody;

        // 3. Attach Shape to Body with Fixture
        b2Body * boxBody = m_physicsWorld->CreateBody(&boxBodyDef);
        b2Fixture * boxFixture = boxBody->CreateFixture(&boxShape, 0.0f);
        boxFixture->SetUserData(new FixtureUserData
            {
                .shape =  m_renderer->addBox(0, 0, width / 2, height / 2)
            });
    }

    void spawnBalls()
    {
        float minX = 0.8f, maxX = minX + 4.0f;
        float minY = 0.2f, maxY = minY + 6.0f;
        float minRadius = 0.1f, maxRadius = 0.3f;
        for (int ballIndex = 0; ballIndex < 100; ballIndex++)
        {
            // 1. Make Shape
            b2BodyDef ballBodyDef;
            ballBodyDef.position.Set(randRange(minX, maxX), randRange(minY, maxY));
            ballBodyDef.type = b2_dynamicBody;

            // 2. Make Body
            b2CircleShape ballShape;
            ballShape.m_p.SetZero();
            ballShape.m_radius = randRange(minRadius, maxRadius);

            // 3. Attach Shape to Body with Fixture
            b2Body * ballBody = m_physicsWorld->CreateBody(&ballBodyDef);
            b2Fixture * ballFixture = ballBody->CreateFixture(&ballShape, 1.0f);
            ballFixture->SetRestitution(0.5f);
            ballFixture->SetFriction(0.5f);
            ballFixture->SetUserData(new FixtureUserData
                {
                    .shape = m_renderer->addCircle(0, 0, ballShape.m_radius)
                });

        }
    }

    virtual void update(double dt) override
    {
        // Perform physics simulation steps in constant chunks
        m_accumTime += dt;
        const float worldTimeStep = 1.0 / 60.0;
        while (m_accumTime > worldTimeStep)
        {
            m_physicsWorld->Step(worldTimeStep, 10, 10);
            m_accumTime -= (double)worldTimeStep;
        }

        // Update Fixture Data
        b2Body * bodyList = m_physicsWorld->GetBodyList();
        while (bodyList != NULL)
        {
            const b2Transform& tform = bodyList->GetTransform();
            b2Fixture * fixtureList = bodyList->GetFixtureList();
            while (fixtureList != NULL)
            {
                // If this fixture has data, update it
                FixtureUserData * fixtureData = (FixtureUserData *)fixtureList->GetUserData();
                if (fixtureData != NULL)
                {
                    fixtureData->shape->centerX = tform.p.x;
                    fixtureData->shape->centerY = tform.p.y;
                    fixtureData->shape->rotation = tform.q.GetAngle();
                }

                // Iterate the linked list
                fixtureList = fixtureList->GetNext();
            }

            // Iterate the linked list
            bodyList = bodyList->GetNext();
        }
    }
};

// Program entry point
int main() {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Crane Game!");

    ShapeRenderer renderer;

    // Start the game state
    State * state = new GameState(&renderer);

    sf::Event event;    // For polling events that SFML sends us
    sf::Clock clock;    // For keeping track of time

    while (window.isOpen()) {
        // Process any events sent by SFML since the last frame
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // SFML sent us a close event, so clean up
                window.close();
            }
        }

        // Update the current state with the amount of time since it was last updated
        state->update(clock.restart().asSeconds());

        // Clear the window before we start drawing to it
        window.clear();

        // Render some shapes
        renderer.renderShapes(window);

        // Notify the window that we're ready to render
        window.display();
    }

    delete state;

    return 0;
}
