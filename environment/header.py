import sys

with open("dota/7_28c/npc_heroes.txt", "r") as fin:
    with open("../src/dota/7_28c/npc_heroes.h", "w") as fout:
        fout.write("const char *npc_heroes7_28c[] =\n");
        fout.write("{\n");
        for line in fin.readlines():
            fout.write("\""+line.replace("\"", "\\\"").replace("\n", "")+"\",\n");
        fout.write("};\n");
        
