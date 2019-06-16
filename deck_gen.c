#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void output(FILE *);

int main(void) {
    FILE *f = fopen("./deck_gen.output", "w");
    output(f);
    
    return EXIT_SUCCESS;
}

// gM is sorted in this manner
void output(FILE *out) {
    for (int value = 2; value <= 14; value++) {
        for (int suit = 0; suit <= 3; suit++) {
            char effect_col[20];
            switch (suit) {
                case 0:
                    strcpy(effect_col, "Red");
                    break;
                case 1:
                    strcpy(effect_col, "Blue");
                    break;
                case 2:
                    strcpy(effect_col, "Yellow");
                    break;
                default:
                    strcpy(effect_col, "Green");
                    break;
            }
            
            char text_value[100];
            if (value <= 10) {
                sprintf(text_value, "%d", value);
            } else if (value == 11) {
                strcpy(text_value, "Hero Icon String(Hero(Junkrat))");
            } else if (value == 12) {
                strcpy(text_value, "Hero Icon String(Hero(Mercy))");
            } else if (value == 13) {
                strcpy(text_value, "Hero Icon String(Hero(Reinhardt))");
            } else {
                strcpy(text_value, "Hero Icon String(Hero(Tracer))");
            }
            
            int index_in_gM = 4*(value - 2) + suit;
            
            fprintf(
                out,
                "Create Effect(\n"
                "    Value In Array(Value In Array(Global Variable(M), %d), 3),\n"
                "    Sphere, %s,\n"
                "    Value In Array(Value In Array(Global Variable(M), %d), 2),\n"
                "    Value In Array(Global Variable(K), 2),\n"
                "    Visible To Position and Radius\n"
                ");\n"
                "Create In-World Text(\n"
                "    Value In Array(Value In Array(Global Variable(M), %d), 3),\n"
                "    %s,\n"
                "    Value In Array(Value In Array(Global Variable(M), %d), 2),\n"
                "    1, Do Not Clip, Visible To Position and String\n"
                ");\n\n",
                index_in_gM, effect_col, index_in_gM,
                index_in_gM, text_value, index_in_gM
            );
        }
    }
}