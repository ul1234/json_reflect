import ctypes

class Struct(ctypes.Structure):
    _fields_ = [("name", ctypes.c_char_p), ("fields", ctypes.c_char_p)]

def read_structs(filename):
    with open(filename, "r") as f:
        content = f.read()

    structs = []
    for line in content.split("\n"):
        if line.startswith("struct"):
            name = line.split(" ")[1]
            fields = []
            for field in line.split("{")[1].split("}")[0].split(";"):
                if field.strip() != "":
                    fields.append(field.strip())
            structs.append(Struct(name.encode(), ",".join(fields).encode()))

    return structs

if __name__ == "__main__":
    structs = read_structs("test.c")
    for struct in structs:
        print(struct.name.decode(), struct.fields.decode())
        