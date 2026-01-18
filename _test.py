import sys
"""
Library Auto Documentation for clib+
"""

if len(sys.argv) < 3:
    print(f"[ x ] Error, Invalid arguments provided!\nUsage: {sys.argv[0]} <input> <output>")
    exit(0)

file = open(sys.argv[1], "r")
data = file.read()
lines = data.split("\n")
file.close()

libs = {}
current = ""
capture = False
comment = False
for line in lines:
        if line.startswith("#ifdef _CLIBP"):
            current = line.split(" ")[1]
            capture = True
            libs[current] = ""
            continue

        if capture and "/*" in line:
            if "*/" in line:
                continue
            comment = True
            continue

        if comment and "*/" in line:
            comment = False
            continue

        if comment or line.startswith("//") or line.startswith("/*"):
            continue

        if capture and line.startswith("#endif"):
            capture = False

        if capture:
            libs[current] += f"{line}\n"

md_header = """
<div align="center">
    <h1>clib+ Libraries</h1>

[Start-up](/README.md)
| [Introduction](/doc/intro.md)
| ● [Quick Doc](/quick_doc.md)
| [Libraries](/doc/libraries.md)
| [Todo](/doc/todo.md)
</div>

<table align="center">
<td align=center>
<p>Built-in Types<p>
int
| char
| string
| array
| map
| ptr
| fn
</td>
<td align=center>
<p>Bit-based Types<p>
i8
| i16
| i32
| i64
| u8
| u16
| u32
| u64
</td>
</table>

"""
markdown = f"{md_header}\n* A Documentation for developers who know what they're doing!\n\n* This documentation MD was generated using ``_test.py``\n\n"
for lib in libs:
    markdown += f"# {lib}\n```c\n{libs[lib]}```\n"

md_file = open(sys.argv[2], "w")
md_file.write(markdown)
md_file.close()