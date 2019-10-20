#include "player.hpp"

using ast::Player;

namespace
{
    glm::mat4 computeOrientation(const glm::mat4& identity, const float& rotationDegrees, const glm::vec3& up)
    {
        return glm::rotate(identity, glm::radians(rotationDegrees), up);
    }

    glm::vec3 computeForwardDirection(const glm::mat4& orientation)
    {
        return glm::normalize(orientation * glm::vec4(0, 0, 1, 0));
    }
} // namespace

struct Player::Internal
{
    const glm::mat4 identity;
    const glm::vec3 up;
    const float moveSpeed{5.0f};
    const float turnSpeed{120.0f};

    float rotationDegrees;
    glm::vec3 position;
    glm::mat4 orientation;
    glm::vec3 forwardDirection;

    Internal(const glm::vec3& position)
        : identity(glm::mat4(1)),
          up(glm::vec3{0.0f, 1.0f, 0.0f}),
          rotationDegrees(0.0f),
          position(position),
          orientation(::computeOrientation(identity, rotationDegrees, up)),
          forwardDirection(::computeForwardDirection(orientation)) {}

    void moveForward(const float& delta)
    {
        position -= forwardDirection * (moveSpeed * delta);
    }

    void moveBackward(const float& delta)
    {
        position += forwardDirection * (moveSpeed * delta);
    }

    void turnLeft(const float& delta)
    {
        rotate(turnSpeed * delta);
    }

    void turnRight(const float& delta)
    {
        rotate(-turnSpeed * delta);
    }

    void moveUp(const float& delta)
    {
        position.y += (moveSpeed * delta);
    }

    void moveDown(const float& delta)
    {
        position.y -= (moveSpeed * delta);
    }

    void rotate(const float& amount)
    {
        rotationDegrees += amount;

        if (rotationDegrees > 360.0f)
        {
            rotationDegrees -= 360.0f;
        }
        else if (rotationDegrees < 0.0f)
        {
            rotationDegrees += 360.0f;
        }

        orientation = ::computeOrientation(identity, rotationDegrees, up);
        forwardDirection = ::computeForwardDirection(orientation);
    }
};

Player::Player(const glm::vec3& position) : internal(ast::make_internal_ptr<Internal>(position)) {}

void Player::moveForward(const float& delta)
{
    internal->moveForward(delta);
}

void Player::moveBackward(const float& delta)
{
    internal->moveBackward(delta);
}

void Player::turnLeft(const float& delta)
{
    internal->turnLeft(delta);
}

void Player::turnRight(const float& delta)
{
    internal->turnRight(delta);
}

glm::vec3 Player::getPosition() const
{
    return internal->position;
}

glm::vec3 Player::getDirection() const
{
    return internal->forwardDirection;
}

void Player::moveUp(const float& delta)
{
    internal->moveUp(delta);
}

void Player::moveDown(const float& delta)
{
    internal->moveDown(delta);
}
