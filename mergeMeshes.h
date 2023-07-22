//
// Created by DELL on 2023/7/22.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;
#ifndef MAIN_MERGEMESHES_H
#define MAIN_MERGEMESHES_H


class mergeMeshes {
public:
    Mesh apply(std::string file_path);
private:
    std::vector<Mesh> readMeshes(std::string file_path);

    Mesh mergMeshes(const std::vector<Mesh>& meshes);
};


#endif //MAIN_MERGEMESHES_H
