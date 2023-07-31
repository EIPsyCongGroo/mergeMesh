//
// Created by DELL on 2023/7/22.
//

#include "mergeMeshes.h"

std::vector<Mesh> mergeMeshes::readMeshes(std::string file_path) {
    std::vector<Mesh> meshes;
    for (const auto& entry : std::filesystem::directory_iterator(file_path))
    {
        Mesh mesh;
        std::string mesh_path = entry.path().string();
        if (!OpenMesh::IO::read_mesh(mesh, mesh_path)) {
            std::cerr << "Error: Cannot read mesh from " << mesh_path << std::endl;
        }
        meshes.push_back(mesh);
    }
    std::cout<<"ready to merge "<<meshes.size()<<" meshes"<<"\n";
    return meshes;
}

Mesh mergeMeshes::mergMeshes(const std::vector<Mesh> &meshes) {
    Mesh mergedMesh;
    std::map < Mesh ::Point , Mesh ::VertexHandle> Vmap;
    for (const auto & mesh:meshes) {
        for (const auto& v: mesh.vertices()) {
            //auto it = Vmap.find(mesh.point(v));
            if(Vmap.empty()){
                Mesh ::VertexHandle new_v = mergedMesh.add_vertex(mesh.point(v));
                Vmap[mesh.point(v)] = new_v;
            }
            else{
                for (auto it:Vmap) {
                    if ((it.first - mesh.point(v)).length()< 1e-6)
                    {
                        Vmap[mesh.point(v)] = it.second;
                    }
                    else
                    {
                        Mesh ::VertexHandle new_v = mergedMesh.add_vertex(mesh.point(v));
                        Vmap[mesh.point(v)] = new_v;
                    }

                }
            }

            //std::cout<<mesh.point(v)<<"\t"<<Vmap[mesh.point(v)]<<"\n";
        }

    }
    for (const auto & mesh:meshes)
    {
        for (const auto & f: mesh.faces()) {
            Mesh::VertexHandle vhandle[3];
            std::vector<Mesh ::VertexHandle> face_vhandles;
            face_vhandles.clear();
            int i=0;
            for(const auto &fv:mesh.fv_ccw_range(f)){
                vhandle[i] = Vmap[mesh.point(fv)];
                face_vhandles.push_back(vhandle[i++]);
            }
            mergedMesh.add_face(face_vhandles);

        }
    }
    std::cout<<"merge complete!"<<"\n";
    return mergedMesh;
}

Mesh mergeMeshes::apply(std::string file_path) {
    std::vector<Mesh> meshes = readMeshes(file_path);
    Mesh mergedMesh = mergMeshes(meshes);
    return mergedMesh;
}
