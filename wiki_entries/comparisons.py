vars = [
    "false",
    "empty_array",
    "zero",
    "null",
    "zero_vector",
    "null_entity",
    "nonempty_array_with_zero_head",
    "uninitialised_var",
    "true",
    "nonempty_array",
    "nonzero_number",
    "nonnull_entity",
    "nonzero_vector"
]

for var in vars:
    print(
        "Create HUD Text(All Players(All Teams),Null,Null,\
            Custom String(\
                \"{0}: [{{0}},{{1}},{{2}}]\",\
                Custom String(\"{{0}},{{1}},{{2}}\",\
                    Compare(Global Variable({0}), ==, Global Variable({1})),\
                    Compare(Global Variable({0}), ==, Global Variable({2})),\
                    Compare(Global Variable({0}), ==, Global Variable({3}))\
                ),\
                Custom String(\"{{0}},{{1}},{{2}}\",\
                    Compare(Global Variable({0}), ==, Global Variable({4})),\
                    Compare(Global Variable({0}), ==, Global Variable({5})),\
                    Compare(Global Variable({0}), ==, Global Variable({6}))\
                ),\
                Custom String(\"{{0}},{{1}},{{2}}\",\
                    Custom String(\"{{0}},{{1}},{{2}}\",\
                        Compare(Global Variable({0}), ==, Global Variable({7})),\
                        Compare(Global Variable({0}), ==, Global Variable({8})),\
                        Compare(Global Variable({0}), ==, Global Variable({9}))\
                    ),\
                    Custom String(\"{{0}},{{1}},{{2}}\",\
                        Compare(Global Variable({0}), ==, Global Variable({10})),\
                        Compare(Global Variable({0}), ==, Global Variable({11})),\
                        Compare(Global Variable({0}), ==, Global Variable({12}))\
                    ),\
                    Compare(Global Variable({0}), ==, Global Variable({13}))\
                )\
            ),\
            Left, 0, White, White, White, Visible To and String, Visible Always\
        );\n\n".format(var, vars[0], vars[1], vars[2], vars[3], vars[4], vars[5], vars[6], vars[7], vars[8], vars[9], vars[10], vars[11], vars[12])
    )