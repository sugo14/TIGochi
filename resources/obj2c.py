# fragile awful abysmal evil script that should not have been created

OBJ_NAME = "porygon"
X_OFFSET = 0
Y_OFFSET = -0.375
Z_OFFSET = 1.0

OBJ_FILE = OBJ_NAME + ".obj"
OUTPUT_FILE = OBJ_NAME + "_c.txt"

vertexes = []
faces = []

with open(OBJ_FILE, 'r') as f:
    lines = f.readlines()
    print(lines)
    for line in lines:
        if line == "":
            continue
        line = line.strip()
        line_split = line.split(" ")
        print(line_split)
        if line_split[0] == 'v':
            vertexes.append([
                float(line_split[1].strip()) + X_OFFSET,
                float(line_split[2].strip()) + Y_OFFSET,
                float(line_split[3].strip()) + Z_OFFSET
            ])
        elif line_split[0] == 'f':
            face = []
            for i in range(1, 4):
                things = line_split[i].split("/")
                face.append(int(things[0].strip()) - 1)
            faces.append(face)

with open(OUTPUT_FILE, 'w') as f:
    f.write("#pragma once\n\n")
    f.write("#define VERT_CNT " + str(len(vertexes)) + "\n")
    f.write("#define TRI_CNT " + str(len(faces)) + "\n\n")
    f.write("#include \"mesh.h\"\n\n")
    f.write("struct Mesh mesh = {\n")
    f.write("\t{\n")
    for vertex in vertexes:
        f.write("\t\t{")
        f.write(f"{vertex[0]}, {vertex[1]}, {vertex[2]}" + "},\n")
    f.write("\t},\n")
    f.write("\t{\n")
    for face in faces:
        if len(face) < 3:
            continue
        f.write("\t\t{")
        f.write(f"{face[0]}, {face[1]}, {face[2]}" + "},\n")
    f.write("\t}\n")
    f.write("};\n")
