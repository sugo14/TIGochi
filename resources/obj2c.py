# fragile awful abysmal evil script that should not have been created

vertexes = []
faces = []

OBJ_NAME = "cube"
OBJ_FILE = OBJ_NAME + ".obj"
OUTPUT_FILE = OBJ_NAME + "_c.txt"

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
            vertexes.append([line_split[1].strip(), line_split[2].strip(), line_split[3].strip()])
        elif line_split[0] == 'f':
            face = []
            for i in range(1, 4):
                things = line_split[i].split("/")
                face.append(int(things[0].strip()) - 1)
            faces.append(face)

with open(OUTPUT_FILE, 'w') as f:
    f.write("struct Mesh " + OBJ_NAME + " = {\n")
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
