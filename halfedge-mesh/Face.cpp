#include "Face.h"
#include "HalfEdge.h"
#include "Vertex.h"
#define EPSILON 1e-6

bool Face::isBoundary() const
{
    return he->onBoundary;
}

double Face::area() const
{
    if (isBoundary()) {
        return 0;
    }
    
    return 0.5 * normal.norm();
}

void Face::computeFeatures()
{
    const Eigen::Vector3d& a(he->vertex->position);
    const Eigen::Vector3d& b(he->next->vertex->position);
    const Eigen::Vector3d& c(he->next->next->vertex->position);
    
    centroid = (a + b + c) / 3.0;
    
    Eigen::Vector3d v1 = a - b;
    Eigen::Vector3d v2 = c - b;
    
    normal = (v1.cross(v2)).normalized();
}