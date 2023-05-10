
import re, sys
from pprint import pprint

TYPE_NUMBER = 0
TYPE_BOOL = 1
TYPE_STRUCT = 2
TYPE_UNION = 3
TYPE_ENUM = 4
TYPE_UNKNOWN = 5
TYPE_NUMBER_ARRAY = 6
TYPE_NUMBER_2D_ARRAY = 7
TYPE_VAR_NUMBER_ARRAY = 8
TYPE_STRUCT_ARRAY = 9
TYPE_VAR_STRUCT_ARRAY = 10
TYPE_RESERVED = 11

# array type
TYPE_NOT_ARRAY = 0
TYPE_ARRAY_1D = 1
TYPE_ARRAY_2D = 2

type_json_format = {
    TYPE_NUMBER : "JSON_NUMBER",
    TYPE_BOOL : "JSON_BOOL",
    TYPE_STRUCT : "JSON_STRUCT",
    TYPE_UNION : "JSON_UNION",
    TYPE_NUMBER_ARRAY : "JSON_NUMBER_ARRAY",
    TYPE_NUMBER_2D_ARRAY : "JSON_NUMBER_2D_ARRAY",
    TYPE_VAR_NUMBER_ARRAY : "JSON_VAR_NUMBER_ARRAY",
    TYPE_STRUCT_ARRAY : "JSON_STRUCT_ARRAY",
    TYPE_VAR_STRUCT_ARRAY : "JSON_VAR_STRUCT_ARRAY",
    TYPE_RESERVED : "JSON_RESERVED",
    TYPE_UNKNOWN : "//无法识别, 请手动修改",
}

ARRAY_NUM_FLAG = "@ARRAY_NUM"

class JsonReflect:
    def __init__(self, filename):
        with open(filename, "r", encoding = "utf-8") as file:
            self.contents = file.read()

        self.all_types = {}
        self.all_type_names = {}
        for type in ['struct', 'union', 'enum']:
            self.all_types[type] = self.GetAllBraceTypes(type)
            self.all_type_names[type] = self.GetAllBraceTypeNames(type)
        #pprint(self.all_types['struct'])

        self.type_config = [
            ("u?int\d+_t", TYPE_NUMBER),
            ("bool", TYPE_BOOL),
        ]
        self.type_config = self.ExpandTypeConfig(self.type_config)
        #pprint(self.type_config)

    # types include braces: struct, union, enum
    def GetAllBraceTypes(self, type_name):
        regex = r"typedef\s+%s\s*\{([^}]*)\}\s*(\w+);" % type_name
        matches = re.findall(regex, self.contents, re.MULTILINE | re.DOTALL)
        # [(type_content, type_name), (...)]
        return matches

    def GetType(self, type_string, type_config = None):
        type_config = type_config or self.type_config
        for regex, type in type_config:
            if re.match(regex, type_string):
                return type
        return TYPE_UNKNOWN

    def GetAllBraceTypeNames(self, type_name):
        matches = self.GetAllBraceTypes(type_name)
        return [type_name for type_content, type_name in matches]

    def GetAllTypeDefs(self, type_config):
        regex = r"typedef\s+([\w\s\[\]]+)\s+([\w\d_]+)\s*;"
        matches = re.findall(regex, self.contents, re.MULTILINE | re.DOTALL)
        return [(typedef[1], self.GetType(typedef[0].strip(), type_config)) for typedef in matches]

    def ExpandTypeConfig(self, type_config):
        type_config += [(enum, TYPE_NUMBER) for enum in self.all_type_names['enum']]  # enum is actually a number
        type_config += [(union, TYPE_UNION) for union in self.all_type_names['union']]
        type_config += [(struct, TYPE_STRUCT) for struct in self.all_type_names['struct']]

        typedefs = self.GetAllTypeDefs(type_config)
        type_config += typedefs

        return type_config

    # for target struct
    def GetBraceTypeContent(self, type_name, type = 'struct'):
        all_types = self.all_types[type]
        if not type_name in [name for content, name in all_types]:
            return None
        content = [content for content, name in all_types if name == type_name]
        assert len(content) == 1, f"invalid content for {type_name}"
        return content[0]

    def GetStructNameList(self, type_name, type = 'struct'):
        fields = self.ExtractBraceTypeFields(type_name, type)
        struct_name_list = [type_name] if type == 'struct' else []
        for type_n in [TYPE_STRUCT, TYPE_UNION]:
            type = 'struct' if type_n == TYPE_STRUCT else 'union'
            type_names = [field_type for field_type, field_name, _ in fields if self.GetType(field_type) == type_n]

            for name in type_names:
                content = self.GetBraceTypeContent(name, type)
                if content:
                    struct_name_list += self.GetStructNameList(name, type)
        return struct_name_list

    def ExtractBraceTypeFields(self, type_name, type = 'struct'):
        content = self.GetBraceTypeContent(type_name, type)
        if not content: return []
        field_regex = r"(\w+)\s+([\w\[\]]+);(.*)"
        all_fields = re.findall(field_regex, content)
        # [(field_type, field_name, field_comment), (...)]
        #for field_type, field_name, field_comment in all_fields:
        #    print(f"Struct {type_name} has fields: {field_type}, {field_name}, {field_comment}")
        return all_fields

    def SortStructName(self, struct_names):
        return [name for name in self.all_type_names['struct'] if name in struct_names]

    def GetFieldNameAndArrayType(self, field_name, field_comment):
        array_type = TYPE_NOT_ARRAY
        var_array_num = ""
        if "[" in field_name and "]" in field_name:
            if field_name.count("[") == 1 and field_name.count("]") == 1:
                array_type = TYPE_ARRAY_1D
            elif field_name.count("[") == 2 and field_name.count("]") == 2:
                array_type = TYPE_ARRAY_2D
            else:
                print(f"Cannot check {field_name} is array or not, not support!")
            field_name = field_name.split("[")[0]
            match = re.search(r"\[%s\s*:\s*([^\]]+)\]" % ARRAY_NUM_FLAG, field_comment)
            if match:
                var_array_num = match.group(1)
                #print(var_array_num)
        return (field_name, array_type, var_array_num)

    def UpdateFieldType(self, field_type, array_type, field_name, var_array_num):
        if field_type == TYPE_NUMBER and field_name in ['rsv', 'rsvd', 'reserved', 'reserve']:
            field_type = TYPE_RESERVED
        elif field_type == TYPE_NUMBER and array_type == TYPE_ARRAY_1D:
            field_type = TYPE_VAR_NUMBER_ARRAY if var_array_num else TYPE_NUMBER_ARRAY
        elif field_type == TYPE_NUMBER and array_type == TYPE_ARRAY_2D:
            field_type = TYPE_NUMBER_2D_ARRAY
        elif field_type == TYPE_STRUCT and array_type == TYPE_ARRAY_1D:
            field_type = TYPE_VAR_STRUCT_ARRAY if var_array_num else TYPE_STRUCT_ARRAY
        elif field_type == TYPE_STRUCT and array_type == TYPE_ARRAY_2D:
            field_type = TYPE_STRUCT_ARRAY
        return field_type

    def FieldString(self, struct_name, field_type, field_type_name, field_name, var_array_num):
        if field_type in type_json_format:
            json_format = type_json_format[field_type]
            if json_format in ['JSON_STRUCT', 'JSON_STRUCT_ARRAY']:
                reflect_name = self.StructReflectName(field_type_name)
                field_string = f"{json_format}({struct_name}, {field_name}, {reflect_name}),"
            elif json_format == 'JSON_VAR_STRUCT_ARRAY':
                reflect_name = self.StructReflectName(field_type_name)
                field_string = f"{json_format}({struct_name}, {field_name}, {var_array_num}, {reflect_name}),"
            elif json_format == 'JSON_UNION':
                field_string = f"{json_format}({struct_name}, {field_name}, [union的指示变量名],  (const JsonReflect *)[union的reflect]),      // 请手动修改"
            elif json_format == 'JSON_VAR_NUMBER_ARRAY':
                field_string = f"{json_format}({struct_name}, {field_name}, {var_array_num}),"
            else:
                field_string = f"{json_format}({struct_name}, {field_name}),"
        else:
            print(f"Invalid field type: {struct_name} {field_type} {field_name}")
            field_string = f"//{field_type} 无法识别, 请手动调整"
        return field_string

    def StructReflectName(self, struct_name):
        var_name = struct_name[0].lower() + struct_name[1:]
        var_name = f"{var_name}Reflect"
        return var_name

    def GenStructReflect(self, struct_name):
        all_fields = self.ExtractBraceTypeFields(struct_name)
        var_name = self.StructReflectName(struct_name)

        content = f"static const JsonReflect {var_name}[] = {{\n"
        for field_type, field_name, field_comment in all_fields:
            type = self.GetType(field_type)
            field_name, array_type, var_array_num = self.GetFieldNameAndArrayType(field_name, field_comment)
            type = self.UpdateFieldType(type, array_type, field_name, var_array_num)
            json_string = self.FieldString(struct_name, type, field_type, field_name, var_array_num)
            content += f"    {json_string}\n"
            if type == TYPE_RESERVED: content += "\n"
            #if type == TYPE_STRUCT_ARRAY:       # add a var struct option to select
            #    json_string = self.FieldString(struct_name, TYPE_VAR_STRUCT_ARRAY, field_type, field_name)
            #    content += f"    //{json_string}, 挑选使用固定的array还是var array\n"
            #if type == TYPE_NUMBER_ARRAY:       # add a var struct option to select
            #    json_string = self.FieldString(struct_name, TYPE_VAR_NUMBER_ARRAY, field_type, field_name)
            #    content += f"    //{json_string}, 挑选使用固定的array还是var array\n"
        content += "    JSON_END()\n};\n\n"
        #print(content)
        return (struct_name, var_name, content)

    def AnalyseStruct(self, struct_name):
        struct_names = self.GetStructNameList(struct_name, 'struct')
        struct_names = self.SortStructName(struct_names)
        #pprint(struct_names)

        contents = []
        for name in struct_names:
            contents.append(self.GenStructReflect(name))
        return contents

    def WriteToFile(self, contents, filename):
        with open(filename, "w", encoding = "utf-8") as file:
            for struct_name, var_name, content in contents:
                file.write(content)
        print(f"Write file {filename} successfully!")

def print_usage():
    print("Usage: python json_reflect {input_file} {struct_name} {output_file}")
    print("Example: python json_reflect pucch_msg.h PucParam pucch_para.h")

if __name__ == '__main__':
    # Usage: python json_reflect pucch_msg.h PucParam pucch_para.h
    if len(sys.argv) < 4:
        print_usage()
    else:
        input_file, struct_name, output_file = sys.argv[1], sys.argv[2], sys.argv[3]
        reflect = JsonReflect(input_file)
        contents = reflect.AnalyseStruct(struct_name)
        reflect.WriteToFile(contents, output_file)
