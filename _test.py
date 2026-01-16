"""
Library Auto Documentation for clib+
"""

file = open("headers/clibp.h", "r")
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
"""
markdown = f"{md_header}\n* A Documentation for developers who know what they're doing!\n\n* This documentation MD was generated using ``_test.py``\n\n"
for lib in libs:
    markdown += f"# {lib}\n```c\n{libs[lib]}```\n"

md_file = open("quick_doc.md", "w")
md_file.write(markdown)
md_file.close()