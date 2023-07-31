#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "mergeMeshes.h"
typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

bool read_mesh(Mesh& mesh, const std::string& input_filename) {
    if (!OpenMesh::IO::read_mesh(mesh, input_filename)) {
        std::cerr << "Error: Cannot read mesh from " << input_filename << std::endl;
        return false;
    }
    return true;
}

Mesh merge_duplicate_vertices(Mesh& mesh) {
    Mesh new_mesh;
    std::map < Mesh ::Point , Mesh ::VertexHandle> Vmap;

    std::cout << "before vertices: " << mesh.n_vertices()<<"\n";
    std::cout << "before faces: " << mesh.n_faces()<<"\n";
    for (const auto& v: mesh.vertices()) {
        auto it = Vmap.find(mesh.point(v));
        if (it != Vmap.end())
        {
            Vmap[mesh.point(v)] = it->second;
        }
        else
        {
            Mesh ::VertexHandle new_v = new_mesh.add_vertex(mesh.point(v));
            Vmap[mesh.point(v)] = new_v;
        }
        //std::cout<<mesh.point(v)<<"\t"<<Vmap[mesh.point(v)]<<"\n";
    }

    for (const auto & f: mesh.faces()) {
        Mesh::VertexHandle vhandle[3];
        std::vector<Mesh ::VertexHandle> face_vhandles;
        face_vhandles.clear();
        int i=0;
        for(const auto &fv:mesh.fv_ccw_range(f)){
            vhandle[i] = Vmap[mesh.point(fv)];
            face_vhandles.push_back(vhandle[i++]);
        }
        new_mesh.add_face(face_vhandles);
    }

    std::cout << "after vertices: " << new_mesh.n_vertices() << "\n";
    std::cout << "after faces: " << new_mesh.n_faces()<<"\n";
    return new_mesh;
}

bool write_mesh(const Mesh& mesh, const std::string& output_filename) {
    if (!OpenMesh::IO::write_mesh(mesh, output_filename)) {
        std::cerr << "Error: Cannot write mesh to " << output_filename << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
 
    Mesh mesh, new_mesh;
    //std::string input_file = "../models/oriented1.off";
    std::string input_file = "../models/mergeMeshes";
    if(argc == 2)
         input_file = argv[1];
    int option;
    option = input_file.rfind(".obj");
//    std::cout<<"which option do you choose?"<<"\n";
//    std::cout<<"1. merge vertices"<<"\n"<<"2. merge meshes"<<"\n";
//    std::cin>>option;
    if (option != -1)
    {
         if (!read_mesh(mesh, input_file)) {
             return 1;
         }

         new_mesh = merge_duplicate_vertices(mesh);
    }
    else
    {
        mergeMeshes merge;
        new_mesh = merge.apply(input_file);
    }


    if (!write_mesh(new_mesh, input_file + "-out.obj")) {
        return 1;
    }

    return 0;
}
