#include "Mesh.h"
#include "MeshIO.h"

Mesh::Mesh()
{
    
}

Mesh::Mesh(const Mesh& mesh)
{
    *this = mesh;
}

bool Mesh::read(const std::string& fileName)
{
    std::ifstream in(fileName.c_str());

    if (!in.is_open()) {
        std::cerr << "Error: Could not open file for reading" << std::endl;
        return false;
    }
    
    bool readSuccessful = false;
    if ((readSuccessful = MeshIO::read(in, *this))) {
        normalize();
        markFeatures(35 * M_PI / 180.0);
    }
    
    return readSuccessful;
}

bool Mesh::write(const std::string& fileName) const
{
    std::ofstream out(fileName.c_str());
    
    if (!out.is_open()) {
        std::cerr << "Error: Could not open file for writing" << std::endl;
        return false;
    }
    
    MeshIO::write(out, *this);
    
    return false;
}

void Mesh::normalize()
{
    // compute center of mass
    Eigen::Vector3d cm = Eigen::Vector3d::Zero();
    for (VertexCIter v = vertices.begin(); v != vertices.end(); v++) {
        cm += v->position;
    }
    cm /= (double)vertices.size();
    
    // translate to origin
    for (VertexIter v = vertices.begin(); v != vertices.end(); v++) {
        v->position -= cm;
    }
    
    // determine radius
    double rMax = 0;
    for (VertexCIter v = vertices.begin(); v != vertices.end(); v++) {
        rMax = std::max(rMax, v->position.norm());
    }
    
    // rescale to unit sphere
    for (VertexIter v = vertices.begin(); v != vertices.end(); v++) {
        v->position /= rMax;
    }
    
    for (FaceIter f = faces.begin(); f != faces.end(); f++) {
        f->computeFeatures();
    }
}

void Mesh::markFeatures(const double angle)
{
    for (EdgeIter e = edges.begin(); e != edges.end(); e++) {
        
        if (!e->he->onBoundary && !e->he->flip->onBoundary) {
            // compute dihedral angle
            Eigen::Vector3d n1 = e->he->next->next->vertex->normal();
            Eigen::Vector3d n2 = e->he->flip->next->next->vertex->normal();
            double d = n1.dot(n2);
            if (d < -1.0) d = -1.0;
            else if (d >  1.0) d = 1.0;
            
            if (std::abs(acos(d)) > angle) {
                e->feature = true;
            }
        }
    }
}
