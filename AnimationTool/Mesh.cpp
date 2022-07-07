//----------------------------------------------------------------------------//
//                                                                            //
// ozz-animation is hosted at http://github.com/guillaumeblanc/ozz-animation  //
// and distributed under the MIT License (MIT).                               //
//                                                                            //
// Copyright (c) Guillaume Blanc                                              //
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included in //
// all copies or substantial portions of the Software.                        //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //
// DEALINGS IN THE SOFTWARE.                                                  //
//                                                                            //
//----------------------------------------------------------------------------//
#include "pch.h"

#include "mesh.h"

#include "ozz/base/containers/vector_archive.h"
#include "ozz/base/memory/allocator.h"

#include "ozz/base/io/archive.h"

#include "ozz/base/maths/math_archive.h"
#include "ozz/base/maths/simd_math_archive.h"

namespace ozz {
namespace io {

void Extern<ozz::Mesh::Part>::Save(OArchive& _archive,
                                      const ozz::Mesh::Part* _parts,
                                      size_t _count) {
  for (size_t i = 0; i < _count; ++i) {
    const ozz::Mesh::Part& part = _parts[i];
    _archive << part.positions;
    _archive << part.normals;
    _archive << part.tangents;
    _archive << part.uvs;
    _archive << part.colors;
    _archive << part.joint_indices;
    _archive << part.joint_weights;
  }
}

void Extern<ozz::Mesh::Part>::Load(IArchive& _archive,
    ozz::Mesh::Part* _parts, size_t _count,
                                      uint32_t _version) {
  (void)_version;
  for (size_t i = 0; i < _count; ++i) {
    ozz::Mesh::Part& part = _parts[i];
    _archive >> part.positions;
    _archive >> part.normals;
    _archive >> part.tangents;
    _archive >> part.uvs;
    _archive >> part.colors;
    _archive >> part.joint_indices;
    _archive >> part.joint_weights;
  }
}

void Extern<ozz::Mesh>::Save(OArchive& _archive, const ozz::Mesh* _meshes,
                                size_t _count) {
  for (size_t i = 0; i < _count; ++i) {
    const ozz::Mesh mesh = _meshes[i];
    _archive << mesh.parts;
    _archive << mesh.triangle_indices;
    _archive << mesh.joint_remaps;
    _archive << mesh.inverse_bind_poses;
  }
}

void Extern<ozz::Mesh>::Load(IArchive& _archive, ozz::Mesh* _meshes,
                                size_t _count, uint32_t _version) {
  (void)_version;
  for (size_t i = 0; i < _count; ++i) {
    ozz::Mesh mesh = _meshes[i];
    _archive >> _meshes[i].parts;
    _archive >> _meshes[i].triangle_indices;
    _archive >> _meshes[i].joint_remaps;
    _archive >> _meshes[i].inverse_bind_poses;

  }
}
}  // namespace io
}  // namespace ozz
