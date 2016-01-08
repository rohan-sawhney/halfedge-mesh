#ifndef FACE_H
#define FACE_H

#include "Types.h"

class Face {
public:
    // one of the halfedges associated with this face
    HalfEdgeIter he;
    
    // returns normal to face
    Eigen::Vector3d normal;
    
    // returns centroid
    Eigen::Vector3d centroid;
    
    // checks if this face lies on boundary
    bool isBoundary() const;
    
    // returns face area
    double area() const;
    
    // compute normal and centroid
    void computeFeatures();
};

#endif
