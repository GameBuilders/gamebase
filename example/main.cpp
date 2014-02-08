/*
 * This is a simple demo highlighting basic usage of the SFML and Box2D libraries.
 * Hopefully this code will help you get started if you're unfamiliar with either.
 * For more advanced usage, be sure to consult the respective libraries' documentation!
 */

// Include the SFML and Box2D headers
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

struct Box
{
    float centerX, centerY;
    float halfWidth, halfHeight;
};

struct Circle
{
    float centerX, centerY;
    float radius;
};

/**
 * Creates and renders shapes on behalf of other objects
 */
class ShapeRenderer
{
public:
    Box * addBox()
    {
        // I need an implementation
        return NULL;
    }

    Circle * addCircle()
    {
        // I need an implementation
        return NULL;
    }

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

/**
 * A state that controls the game
 */
class GameState : public State
{
private:
    ShapeRenderer * m_renderer;

    b2World * m_physicsWorld;
    double m_accumTime;

    b2Body * m_testBody;
    Circle * m_testCircleRender;

    b2Body * m_groundBody;
    Box * m_groundBoxRender;
public:
    GameState(ShapeRenderer * renderer) : m_renderer(renderer), m_accumTime(0.0)
    {
        m_physicsWorld = new b2World(b2Vec2(0, -10));

        // Add a simple circle to demonstrate the physics are working
        b2CircleShape testCircleShape;
        testCircleShape.m_p.SetZero();
        testCircleShape.m_radius = 1.0f;

        b2BodyDef testBodyDef;
        testBodyDef.position.SetZero();
        testBodyDef.type = b2_dynamicBody;

        m_testBody = m_physicsWorld->CreateBody(&testBodyDef);
        m_testBody->CreateFixture(&testCircleShape, 1.0f);

        m_testCircleRender = m_renderer->addCircle();
        if (m_testCircleRender != NULL)
        {
            m_testCircleRender->radius = testCircleShape.m_radius;
        }

        // Add a ground box to keep the circle from falling forever
        b2PolygonShape groundShape;
        groundShape.SetAsBox(10.0f, 1.0f);

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -20.0f);
        groundBodyDef.type = b2_staticBody;

        m_groundBody = m_physicsWorld->CreateBody(&groundBodyDef);
        m_groundBody->CreateFixture(&groundShape, 0.0f);

        m_groundBoxRender = m_renderer->addBox();
        if (m_groundBoxRender != NULL)
        {
            m_groundBoxRender->centerX = 0.0f;
            m_groundBoxRender->centerY = 0.0f;
            m_groundBoxRender->halfWidth = 10.0f;
            m_groundBoxRender->halfHeight = 1.0f;
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

        // Show that the test circle is actually moving down
        b2Vec2 bodyPos = m_testBody->GetPosition();
        printf("%f\n", bodyPos.y);

        if (m_testCircleRender != NULL)
        {
            m_testCircleRender->centerX = bodyPos.x;
            m_testCircleRender->centerY = bodyPos.y;
        }
    }
};


// Function prototypes
void drawShapes(sf::RenderWindow& window);

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
        drawShapes(window);

        // Notify the window that we're ready to render
        window.display();
    }

    delete state;

    return 0;
}

// Draws some shapes!
void drawShapes(sf::RenderWindow& window) {
    // Draw a circle to the window
    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color(100, 250, 50));
    window.draw(shape);
}
