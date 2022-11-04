# Don't mind the missing modules here, the virtual env takes care of that
import csv
import re
import csnake as c

types = dict()

with open('types.csv', newline='') as types_csv:
    for line in csv.DictReader(types_csv, delimiter=';', quotechar='"'):
        types[line['Name']] = line

variables = list()

with open('variables.csv', newline='') as variables_csv:
    for line in csv.DictReader(variables_csv, delimiter=';', quotechar='"'):
        variables.append(line)

# Generate getters and setters

for variable in variables:
    variable['Getter'] = c.Function(
        name="get_{Name}".format(**variable),
        return_type=variable['Type']
    )
    variable['Getter'].add_code(
        "return application.{Name};".format(**variable)
    )

    variable['Setter'] = c.Function(
        name="set_{Name}".format(**variable),
        return_type="void",
        arguments=[
            c.Variable("value", variable['Type'])
        ]
    )

    domain = re.match(r"\[(\d+),\s?(\d+)]", types[variable['Type']]['Domain'])
    if domain:

        # Don't check zero lower bound if type is unsigned (gcc generates warning -Wtype-limits otherwise)
        low_bound = f"value >= {domain.group(1)} &&" \
            if types[variable['Type']]['Declaration'][0] != "u" or domain.group(1) != "0" \
            else ""

        variable['Setter'].add_code((
            f"if ({low_bound} value <= {domain.group(2)}) {{",
            "application.{Name} = value;".format(**variable),
            "}"
        ))
    else:
        variable['Setter'].add_code((
            "application.{Name} = value;".format(**variable)
        ))

# Generate init function
init = c.Function("application_init")
for variable in variables:
    init.add_code("application.{Name} = {Default};".format(**variable))

# Generate header file

header_file = c.CodeWriter()
header_file.add_autogen_comment()

header_file.start_if_def("DATA_DICTIONARY_H", invert=True)
header_file.add_define("DATA_DICTIONARY_H")

header_file.include("<stdbool.h>")
header_file.include("<stdint.h>")

for typedef in types.values():

    if typedef['Genre'] == 'atom':
        header_file.add_lines(
            """
            /**
             * \\brief {Comment}
             */
             typedef {Declaration} {Name};
            """.format(**typedef)
        )
    elif typedef['Genre'] == 'enum':
        typedef_enum = c.Enum(
            name=typedef['Name'],
            typedef=True,
            prefix=typedef['Name'][:-1].upper()
        )
        typedef_enum.add_values(eval(typedef['Declaration']))
        header_file.add_lines(
            """
            /**
             * \\brief {Comment}
             */""".format(**typedef)
        )
        header_file.add_enum(typedef_enum)

header_file.add_lines(
    """
    /**
     * \\brief Initializer function for the application data.
     */"""
)
header_file.add_function_prototype(init)

for variable in variables:
    header_file.add_lines(
        """
        /**
         * \\brief Getter for application.{Name}.
         * \\return {Comment}
         */""".format(**variable)
    )
    header_file.add_function_prototype(variable['Getter'])

    header_file.add_lines(
        """
        /**
         * \\brief Setter for application.{Name}.
         * \\param[in] value {Comment}
         */""".format(**variable)
    )
    header_file.add_function_prototype(variable['Setter'])

header_file.end_if_def()
header_file.write_to_file("../data_dictionary.h")

# Generate source file

source_file = c.CodeWriter()
source_file.include('"data_dictionary.h"')

struct_application = c.Struct("application_t")

for variable in variables:
    struct_application.add_variable(c.Variable(variable['Name'], variable['Type']))

source_file.add_struct(struct_application)

source_file.add_variable_declaration(
    c.Variable("application", "struct application_t")
)

for variable in variables:
    source_file.add_function_definition(variable['Getter'])
    source_file.add_function_definition(variable['Setter'])

source_file.add_function_definition(init)

source_file.write_to_file("../data_dictionary.c")
