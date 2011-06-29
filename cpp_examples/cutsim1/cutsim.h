#ifndef CUTSIM_H
#define CUTSIM
#include <QObject>

#include <string>
#include <iostream>
#include <cmath>
#include <vector>


#include <boost/bind.hpp>

#include <opencamlib/point.h>
#include <opencamlib/triangle.h>
#include <opencamlib/numeric.h>
#include <opencamlib/octree.h>
#include <opencamlib/octnode.h>

#include <opencamlib/volume.h>
#include <opencamlib/marching_cubes.h>

#include <opencamlib/gldata.h>

/// a Cutsim stores an Octree stock model, uses an iso-surface extraction
/// algorithm to generate surface triangles, and communicates with
/// the corresponding GLData surface which is used for rendering
class Cutsim : public QObject {
    Q_OBJECT
public:

    // std::cout << ocl::revision() << "\n";
    // std::cout << " Experimental C++ cutting simulation.\n";
    // Octree(root_scale, max_depth, cp)
    Cutsim () {
        ocl::Point octree_center(0,0,0);
        unsigned int max_depth = 7;
        tree = new ocl::Octree(10.0, max_depth, octree_center );
        std::cout << " tree before init: " << tree->str() << "\n";
        
        std::cout << "running init(3) on the octree. this splits the cube three times. \n";
        std::cout << "we go from 1 cube -> 8 -> 64 -> 512 cubes \n";
        tree->init(5u);
        tree->debug=true;
        std::cout << " tree after init: " << tree->str() << "\n";
        
        ocl::CubeVolume cube;
        cube.side = 4.0;
        cube.invert = false;
        cube.calcBB();
        
        tree->diff_negative( &cube);

        std::cout << " tree after stock-cut: " << tree->str() << "\n";
        mc = new ocl::MarchingCubes();
    } 
    void setGLData(ocl::GLData* gldata) {
        // this is the GLData that corresponds to the tree
        g = gldata;
        g->setTriangles(); // mc: triangles, dual_contour: quads
        g->setPosition(0,0,0); // position offset (?used)
        g->setUsageDynamicDraw();
    }
    void updateGL() {
        // traverse the octree and update the GLData correspondingly
        std::cout << " tree->updateGL() \n";
        std::cout << " tree before updateGL(): " << tree->str() << "\n";
        tree->updateGL();
    }
    /*
    void updateGL(ocl::Octnode* current) {
        // starting at current, update the isosurface
        if ( current->isLeaf() && current->surface() && !current->valid() ) { 
            // this is a leaf and a surface-node
            // std::vector<ocl::Triangle> node_tris = mc->mc_node(current);
            BOOST_FOREACH(ocl::Triangle t, mc->mc_node(current) ) {
                double r=1,gr=0,b=0;
                std::vector<unsigned int> polyIndexes;
                for (int m=0;m<3;++m) { // FOUR for quads
                    //unsigned int vertexId =  g->addVertex( t.p[m].x, t.p[m].y, t.p[m].z, r,gr,b,
                    //                        boost::bind(&ocl::Octnode::swapIndex, current, _1, _2)) ; // add vertex to GL
                    unsigned int vertexId =  g->addVertex( t.p[m].x, t.p[m].y, t.p[m].z, r,gr,b, current) ;
//                        boost::bind(&ocl::Octnode::swapIndex, current, _1, _2)) ; // add vertex to GL
                                            
                    g->setNormal( vertexId, t.n.x, t.n.y, t.n.z );
                    polyIndexes.push_back( vertexId );
                    current->addIndex( vertexId ); // associate node with vertex
                }
                g->addPolygon(polyIndexes); // add poly to GL
                current->setValid(); // isosurface is now valid for this node!
            }
        } else if ( current->isLeaf() && !current->surface() && !current->valid() ) { //leaf, but no surface
            // remove vertices, if any
            BOOST_FOREACH(unsigned int vId, current->vertexSet ) {
                g->removeVertex(vId);
            }
            // needed?? // current->clearIndex();
            current->setValid();
        }
        else {
            for (int m=0;m<8;++m) { // go deeper into tree, if !valid
                if ( current->hasChild(m) && !current->valid() ) {
                    updateGL(current->child[m]);
                }
            }
        }
    } */
    
    /*
    void surf() {
        // run mc on all nodes
        // std::vector<Triangle> mc_node(const Octnode* node);
        
        tree->updateGL();
        //tris = mc->mc_tree( tree ); // this gets ALL triangles from the tree and stores them here.
        //std::cout << " mc() got " << tris.size() << " triangles\n";
    }*/
    
    //std::vector<ocl::Triangle> getTris() {
    //    return tris;
    //}
    
public slots:
    void cut() { // demo slot of doing a cutting operation on the tree with a volume.
        std::cout << " cut! called \n";
        ocl::SphereOCTVolume s;
        s.radius = 1;
        s.center = ocl::Point(3,3,3);
        s.calcBB();
        tree->diff_negative( &s );
        updateGL();
    }
private:
    ocl::MarchingCubes* mc; // simplest isosurface-extraction algorithm
    std::vector<ocl::Triangle> tris; // do we need to store all tris here?? no!
    ocl::Octree* tree; // this is the stock model
    ocl::GLData* g; // this is the graphics object drawn on the screen
};

#endif
