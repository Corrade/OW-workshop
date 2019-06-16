#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void output(FILE *);

int main(void) {
    FILE *f = fopen("./deck_gen.output", "w");
    output(f);
    
    return EXIT_SUCCESS;
}

// Create effect at gM[?][3], visible to gM[?][4], radius = gK[3], varying colour based on gM[?][2]
// Create in-world text at gM[?][3], visible to gM[?][4], varying value based on gM[?][1]
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
            
            int i = 4*(value - 2) + suit + 1;
            
            fprintf(
                out,
                "Create Effect(\n"
                "    Value In Array(Value In Array(Global Variable(M), %d)), 4),\n"
                "    Sphere, %s,\n"
                "    Value In Array(Value In Array(Global Variable(M), %d)), 3),\n"
                "    Value In Array(Global Variable(K), 3),\n"
                "    Visible To Position and Radius\n"
                ");\n"
                "Create In-World Text(\n"
                "    Value In Array(Value In Array(Global Variable(M), %d)), 4),\n"
                "    %s,\n"
                "    Value In Array(Value In Array(Global Variable(M), %d)), 3),\n"
                "    1, Do Not Clip, Visible To Position and String\n"
                ");\n\n",
                i, effect_col, i,
                i, text_value, i
            );
        }
    }
}