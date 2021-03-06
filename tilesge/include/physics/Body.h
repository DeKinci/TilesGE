//
// Created by DeKinci on 5/3/2020.
//

#ifndef TILESGE_BODY_H
#define TILESGE_BODY_H

#include <glm/gtx/norm.hpp>

#include "graphics/Model.h"
#include "graphics/GraphicsObject.h"
#include "AABB.h"

#define EPSILON_SQ 0.0000001f
#define INF_MASS 0

class Body {
public:
    Body(GraphicsObject *go, const glm::vec3 &position, float mass);
    explicit Body(const Body *body);
    ~Body();

    void merge(const Body* body);

    void setPosition(const glm::vec3 &position);

    void setVelocity(const glm::vec3 &velocity);

    void setAcceleration(const glm::vec3 &acceleration);

    void addPosition(const glm::vec3 &delta);

    void addVelocity(const glm::vec3 &delta);

    void addAcceleration(const glm::vec3 &delta);

    const glm::vec3 &getPosition() const;

    const glm::vec3 &getVelocity() const;

    const glm::vec3 &getAcceleration() const;

    AABB *getAabb() const;

    void show(Camera &camera);

    bool isMoving() const;

    void setPermAcceleration(const glm::vec3 &permAcceleration);

    const glm::vec3 &getPermAcceleration() const;

    float restitution = 1;
    float mass = 1;
    float invMass;

    const glm::mat4 &getTransormation() const;

private:
    GraphicsObject* go;
    AABB *aabb = nullptr;
    glm::vec3 position{glm::vec3(0, 0, 0)};
    glm::vec3 velocity{glm::vec3(0, 0, 0)};
    glm::vec3 acceleration{glm::vec3(0, 0, 0)};
    glm::vec3 permAcceleration{glm::vec3(0, 0, 0)};

    glm::mat4 transormation;

    bool moving = false;

    void updateMovingFlag();

    void computeAABB();
};


#endif //TILESGE_BODY_H
