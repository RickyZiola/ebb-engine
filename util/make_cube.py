import struct

# Define vertices (vec3 + vec2 + vec3)
vertices = [
    # Vertex 0
    b'v' + struct.pack("fff", -0.5, -0.5, 0) + struct.pack("ff", 0.0, 0.0) + struct.pack("fff", 0.0, 0.0, 0.0),
    # Vertex 1
    b'v' + struct.pack("fff", 0.5, -0.5, 0) + struct.pack("ff", 1.0, 0.0) + struct.pack("fff", 0.0, 0.0, 0.0),
    # Vertex 2
    b'v' + struct.pack("fff", 0.5, 0.5, 0) + struct.pack("ff", 1.0, 1.0) + struct.pack("fff", 0.0, 0.0, 0.0),
    # Vertex 3
    b'v' + struct.pack("fff", -0.5, 0.5, 0) + struct.pack("ff", 0.0, 1.0) + struct.pack("fff", 0.0, 0.0, 0.0),
    # Vertex 4
    b'v' + struct.pack("fff", -0.5, -0.5, 1) + struct.pack("ff", 0.0, 0.0) + struct.pack("fff", 0.0, 0.0, 0.0),
    # Vertex 5
    b'v' + struct.pack("fff", 0.5, -0.5, 1) + struct.pack("ff", 1.0, 0.0) + struct.pack("fff", 0.0, 0.0, 0.0),
    # Vertex 6
    b'v' + struct.pack("fff", 0.5, 0.5, 1) + struct.pack("ff", 1.0, 1.0) + struct.pack("fff", 0.0, 0.0, 0.0),
    # Vertex 7
    b'v' + struct.pack("fff", -0.5, 0.5, 1) + struct.pack("ff", 0.0, 1.0) + struct.pack("fff", 0.0, 0.0, 0.0),
]

# Define faces (indexes into the vertex array)
faces = [
    # Bottom face
    b'f' + struct.pack("III", 0, 1, 2) +  # Triangle 1
    b'f' + struct.pack("III", 0, 2, 3),   # Triangle 2

    # Front face
    b'f' + struct.pack("III", 0, 3, 7) +  # Triangle 1
    b'f' + struct.pack("III", 0, 7, 4),   # Triangle 2

    # Right face
    b'f' + struct.pack("III", 1, 0, 4) +  # Triangle 1
    b'f' + struct.pack("III", 1, 4, 5),   # Triangle 2

    # Back face
    b'f' + struct.pack("III", 2, 1, 5) +  # Triangle 1
    b'f' + struct.pack("III", 2, 5, 6),   # Triangle 2

    # Left face
    b'f' + struct.pack("III", 3, 2, 6) +  # Triangle 1
    b'f' + struct.pack("III", 3, 6, 7),   # Triangle 2

    # Top face
    b'f' + struct.pack("III", 7, 6, 5) +  # Triangle 1
    b'f' + struct.pack("III", 7, 5, 4)    # Triangle 2
]

# Write vertices and faces to a binary file
with open("cube.ebbm", "wb") as file:
    for vertex in vertices:
        file.write(vertex)
    for face in faces:
        file.write(face)
