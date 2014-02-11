/*
 * This is a simple demo highlighting basic usage of the SFML and Box2D libraries.
 * Hopefully this code will help you get started if you're unfamiliar with either.
 * For more advanced usage, be sure to consult the respective libraries' documentation!
 */

#include <cstdlib>
#include <sstream>

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

// Number of balls to play with
#define BALL_COUNT 100

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

struct Text : public Shape
{
    std::string text;
};

/**
 * Creates and renders shapes on behalf of other objects
 */
class ShapeRenderer
{
public:
    ShapeRenderer()
    {
        m_font.loadFromFile("example/DejaVuSans.ttf");
    }

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

    Text * addText(float centerX, float centerY)
    {
        Text* t = new Text();
        t->centerX = centerX;
        t->centerY = centerY;
        m_textList.push_back(t);
        return t;
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

        for (std::vector<Text*>::iterator it = m_textList.begin(); it != m_textList.end(); ++it) {
            sf::Text shape((*it)->text, m_font);
            sf::FloatRect bounds = shape.getGlobalBounds();
            shape.setPosition((*it)->centerX * FACTOR - bounds.width / 2,
                              (*it)->centerY * FACTOR - bounds.height / 2);
            shape.setRotation((*it)->rotation * RADIAN_TO_DEGREE);
            shape.setColor(sf::Color(100, 250, 50));
            window.draw(shape);
        }
    }
private:
    sf::Font m_font;
    std::vector<Box*> m_boxList;
    std::vector<Circle*> m_circleList;
    std::vector<Text*> m_textList;
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

struct BodyUserData
{
    bool isBall;
};

class BallQueryCallback : public b2QueryCallback
{
private:
    const b2Shape& m_shape;
    unsigned m_count;
public:
    BallQueryCallback(const b2Shape& shape) : m_shape(shape), m_count(0)
    {
    }

    unsigned getCount()
    {
        return m_count;
    }

    virtual bool ReportFixture(b2Fixture* fixture) override
    {
        b2Transform identity;
        identity.SetIdentity();
        b2Body * body = fixture->GetBody();
        BodyUserData * data = (BodyUserData*)body->GetUserData();
        if (data != NULL)
        {
            if (b2TestOverlap(fixture->GetShape(), 0, &m_shape, 0, body->GetTransform(), identity))
            {
                m_count++;
            }
        }
        return true;
    }
};

/**
 * A state that controls the game
 */
class GameState : public State
{
private:
    ShapeRenderer * m_renderer;
    Text * m_scoreText;

    b2World * m_physicsWorld;
    double m_accumTime;

    // Gameplay Data
    b2PolygonShape m_goalShape;
    b2Body * m_ropeAnchor;
    b2RevoluteJoint * m_clawJoint;
    b2RopeJoint * m_ropeJoint;
    bool m_openClaw = true;
    b2Vec2 m_clawDirection = b2Vec2(0, 0);
public:
    GameState(ShapeRenderer * renderer) : m_renderer(renderer), m_accumTime(0.0)
    {
        // Construct a physical world with gravity
        m_physicsWorld = new b2World(b2Vec2(0, 10));

        // Create text for showing the score
        m_scoreText = m_renderer->addText(8.0f, 8.0f);

        // Add balls to demonstrate the physics are working
        spawnBalls();

        // Add the claw
        addClaw();

        // Add a ground box to keep the circles from falling forever
        addBox(0.0f, 16.0f, 10.0f, 10.1f); // ground
        addBox(0.0f, 0.1f, 0.0f, 10.0f); // left post
        addBox(6.0f, 6.1f, 7.0f, 10.0f); // middle left post
        addSlope(6.0f, 10.1f,
                 7.0f, 5.0f); // middle slope
        addBox(10.0f, 10.1f, 5.0f, 10.0f); // middle right post
        addBox(15.9f, 16.0f, 0.0f, 10.0f); // right post

        m_goalShape.SetAsBox(3.0f, 2.5f, b2Vec2(13.0f, 7.5f), 0.0f);
    }

    b2Body * addBox(float minX, float maxX, float minY, float maxY, b2BodyType type = b2_staticBody)
    {
        float width = maxX - minX;
        float height = maxY - minY;

        // 1. Make Shape
        b2PolygonShape boxShape;
        boxShape.SetAsBox(width / 2, height / 2);

        // 2. Make Body
        b2BodyDef boxBodyDef;
        boxBodyDef.position.Set(minX + width / 2, minY + height / 2);
        boxBodyDef.type = type;

        // 3. Attach Shape to Body with Fixture
        b2Body * boxBody = m_physicsWorld->CreateBody(&boxBodyDef);
        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = 50.0f;
        b2Fixture * boxFixture = boxBody->CreateFixture(&boxFixtureDef);
        boxFixture->SetUserData(new FixtureUserData
            {
                .shape =  m_renderer->addBox(0, 0, width / 2, height / 2)
            });
        return boxBody;
    }

    b2Body * addSlope(float x1, float x2, float y1, float y2, b2BodyType type = b2_staticBody)
    {
        float centerX = (x2 + x1) / 2;
        float centerY = (y2 + y1) / 2;
        float length = b2Vec2(x2 - x1, y2 - y1).Length();
        float thickness = 0.1f;
        b2Body * boxBody = addBox(centerX - length    / 2, centerX + length    / 2,
                                  centerY - thickness / 2, centerY + thickness / 2,
                                  type);
        float angle = atan2(y2 - y1, x2 - x1);
        boxBody->SetTransform(boxBody->GetPosition(), angle);
        return boxBody;
    }

    void addClaw()
    {
        float clawHeight = 2.0f;
        float clawWidth = 1.5f;
        float x = 2.0f, y = 2.0f;

        m_ropeAnchor = addBox(x - 0.05, x + 0.1f, 0.0f, 0.1f, b2_kinematicBody);

        // Create the claw bodies
        b2Body * leftUpperArm =
        addSlope(x, x - clawWidth  / 2,
                 y, y + clawHeight / 2,
                 b2_dynamicBody); // Left Upper Arm
        leftUpperArm->SetAngularDamping(0.0f);

        b2Body * rightUpperArm =
        addSlope(x, x + clawWidth  / 2,
                 y, y + clawHeight / 2,
                 b2_dynamicBody); // Right Upper Arm
        rightUpperArm->SetAngularDamping(0.0f);

        b2Body * leftLowerArm =
        addSlope(x - clawWidth  / 2, x,
                 y + clawHeight / 2, y + clawHeight,
                 b2_dynamicBody); // Left Lower Arm
        leftLowerArm->SetAngularDamping(0.0f);

        b2Body * rightLowerArm =
        addSlope(x + clawWidth  / 2, x,
                 y + clawHeight / 2, y + clawHeight,
                 b2_dynamicBody); // Right Lower Arm
        rightLowerArm->SetAngularDamping(0.0f);

        // Setup claw joints
        b2WeldJointDef leftElbow;
        leftElbow.Initialize(leftUpperArm, leftLowerArm, b2Vec2(x, y + clawHeight / 2));
        m_physicsWorld->CreateJoint(&leftElbow);

        b2WeldJointDef rightElbow;
        rightElbow.Initialize(rightUpperArm, rightLowerArm, b2Vec2(x + clawWidth, y + clawHeight / 2));
        m_physicsWorld->CreateJoint(&rightElbow);

        b2RevoluteJointDef clawJontDef;
        clawJontDef.Initialize(leftUpperArm, rightUpperArm, b2Vec2(x, y));
        clawJontDef.enableMotor = true;
        clawJontDef.maxMotorTorque = 500.0f;
        clawJontDef.enableLimit = true;
        clawJontDef.lowerAngle = -1.2;
        clawJontDef.upperAngle = 0;
        m_clawJoint = (b2RevoluteJoint *)m_physicsWorld->CreateJoint(&clawJontDef);

        b2RopeJointDef clawRopeDef;
        clawRopeDef.bodyA = m_ropeAnchor;
        clawRopeDef.bodyB = leftUpperArm;
        clawRopeDef.maxLength = 2.0f;
        clawRopeDef.localAnchorA.SetZero();
        clawRopeDef.localAnchorB = clawRopeDef.bodyB->GetLocalPoint(b2Vec2(x, y));
        m_ropeJoint = (b2RopeJoint *)m_physicsWorld->CreateJoint(&clawRopeDef);
    }

    void spawnBalls()
    {
        float minX = 0.8f, maxX = minX + 4.0f;
        float minY = 0.2f, maxY = minY + 6.0f;
        float minRadius = 0.1f, maxRadius = 0.3f;
        for (int ballIndex = 0; ballIndex < BALL_COUNT; ballIndex++)
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
            ballBody->SetUserData(new BodyUserData
                {
                    .isBall = true
                });
            b2Fixture * ballFixture = ballBody->CreateFixture(&ballShape, 1.0f);
            ballFixture->SetRestitution(0.5f);
            ballFixture->SetFriction(0.2f);
            ballFixture->SetUserData(new FixtureUserData
                {
                    .shape = m_renderer->addCircle(0, 0, ballShape.m_radius)
                });

        }
    }

    void toggleClaw()
    {
        m_openClaw = !m_openClaw;
    }

    void setClawDirection(const b2Vec2& clawDirection)
    {
        m_clawDirection = clawDirection;
    }

    unsigned getBallInGoalCount()
    {
        BallQueryCallback callback(m_goalShape);
        b2AABB shapeAABB;
        b2Transform identity;
        identity.SetIdentity();
        m_goalShape.ComputeAABB(&shapeAABB, identity, 0);
        m_physicsWorld->QueryAABB(&callback, shapeAABB);
        return callback.getCount();
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

        // Update Gameplayer Data

        m_clawJoint->SetMotorSpeed(m_openClaw ? -4.0f : 4.0f);

        b2Vec2 clawDelta = m_clawDirection;
        clawDelta *= dt;
        clawDelta *= 3;
        m_ropeJoint->SetMaxLength(m_ropeJoint->GetMaxLength() + clawDelta.y);
        // Ensure the rope is between specific lengths
        if (m_ropeJoint->GetMaxLength() < 0.1f)
        {
            m_ropeJoint->SetMaxLength(0.1f);
        }
        if (m_ropeJoint->GetMaxLength() > 10.0f)
        {
            m_ropeJoint->SetMaxLength(10.0f);
        }

        b2Vec2 achorPos = m_ropeAnchor->GetPosition();
        achorPos.x += clawDelta.x;
        m_ropeAnchor->SetTransform(achorPos, 0.0f);
        // Ensure the anchor does not go out of bounds
        if (achorPos.x < 0.1f)
        {
            m_ropeAnchor->SetTransform(b2Vec2(0.1f, achorPos.y), 0.0f);
        }
        if (achorPos.x > 15.9f)
        {
            m_ropeAnchor->SetTransform(b2Vec2(15.9f, achorPos.y), 0.0f);
        }

        std::stringstream scoreString;
        scoreString << getBallInGoalCount() << "/" << BALL_COUNT;
        m_scoreText->text = scoreString.str();
    }
};

// Program entry point
int main() {
    // Ensure the game is different every run
    srand(time(NULL));

    // Create the SFML window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Crane Game!", sf::Style::Default, settings);

    ShapeRenderer renderer;

    // Start the game state
    GameState * state = new GameState(&renderer);

    sf::Event event;    // For polling events that SFML sends us
    sf::Clock clock;    // For keeping track of time

    while (window.isOpen()) {
        // Process any events sent by SFML since the last frame
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // SFML sent us a close event, so clean up
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space)
                {
                    state->toggleClaw();
                }
            }
        }

        b2Vec2 clawDirection(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            clawDirection.y += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            clawDirection.y -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            clawDirection.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            clawDirection.x += 1.0f;
        }
        state->setClawDirection(clawDirection);

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
