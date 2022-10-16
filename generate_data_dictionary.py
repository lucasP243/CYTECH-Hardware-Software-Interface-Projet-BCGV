import os
import csv

types = dict()

with open('types.csv', newline='') as types_csv:
    for line in csv.DictReader(types_csv, delimiter=';', quotechar='"'):
        types[line['Name']] = line

variables = list()

with open('variables.csv', newline='') as variables_csv:
    for line in csv.DictReader(variables_csv, delimiter=';', quotechar='"'):
        variables.append(line)

with open('lib/data_dictionary.h', mode='w') as header_file:
    header_file.write(
        """/**
         * \\file data_dictionary.h
         * \\brief This file encloses all the types getters and setters
         *        that are global to the application.
         * \\details Generated by 'generate_data_dictionary.py'
         * \\author Lucas Pinard
         */
        #ifndef DATA_DICTIONARY_H
        #define DATA_DICTIONARY_H

        #include <stdint.h>
        #include <stdbool.h>

        """
    )

    for typedef in types.values():
        header_file.write(
            """
            /**
             * \\brief {Comment}
             */
            typedef {Declaration} {Name};

            """.format(**typedef)
        )

    for variable in variables:
        header_file.write(
            """
            /**
             * \\brief Getter for application.{Name}
             * \\return {Type}: {Comment}
             */
            {Type} get_{Name}();

            /**
             * \\brief Setter for application.{Name}
             * \\param {Type}: {Comment}
             */
            void set_{Name}({Type});

            """.format(**variable)
        )

    header_file.write(
        """
        /**
         * \\brief Initialization function for application variables
         */
         void init();
         """
    )

    header_file.write("#endif")

with open('lib/data_dictionary.c', mode='w') as source_file:
    source_file.write(
        """#include "data_dictionary.h"

        static struct {
        """
    )

    for variable in variables:
        source_file.write(
            """
            /**
             * \\brief {Comment}
             */
            {Type} {Name};

            """.format(**variable)
        )

    source_file.write(
        """
        } application;
        """
    )

    for variable in variables:

        setter = ''

        if types[variable['Type']]['Domain']:
            setter += 'if ('
            domain = types[variable['Type']]['Domain']
            if list(domain)[0] == '{':
                setter += ' || '.join([f'value == {x}' for x in domain[1:-1].split(',')])
            elif list(domain)[0] == '[':
                limits = domain[1:-1].split(',')
                if types[variable['Type']]['Declaration'][:4] != 'uint':
                    setter += f"value >= {limits[0]} && "
                setter += f"value <= {limits[1]}"
            setter += ') {'

        setter += 'application.{Name} = value;'.format(**variable)

        if types[variable['Type']]['Domain']:
            setter += '}'

        source_file.write(
            """
            {Type} get_{Name}() {{
                return application.{Name};
            }}

            void set_{Name}({Type} value) {{
                {setter}
            }}

            """.format(setter=setter, **variable)
        )

    source_file.write(
        """
        void init() {
        """
    )

    for variable in variables:
        source_file.write(
            """
            application.{Name} = {Default};
            """.format(**variable)
        )

    source_file.write(
        """
        }
        """
    )
