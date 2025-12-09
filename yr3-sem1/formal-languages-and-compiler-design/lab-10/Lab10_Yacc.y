%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#define MAX_RECORDS 100
#define MAX_FIELDS_PER_RECORD 20
#define MAX_FIELDS 20

typedef struct DatasetLiteral {
    int record_count;
    char *values[MAX_RECORDS][MAX_FIELDS_PER_RECORD];
    int value_counts[MAX_RECORDS]; // Track how many values each record has
} DatasetLiteral;

typedef struct {
    char* name;
    char* type;
} Field;

Field fields[20];
int field_count = 0;
int first_record = 1; // flag to know if we are parsing the first record
char* current_dataset_name = NULL; // track current dataset name for struct generation
char* selected_fields = NULL; // track which fields were selected
char* current_working_dataset = NULL; // track which dataset is currently being worked on

void yyerror(const char *s);
int yylex();
extern FILE *yyin;

void emit(const char *code) {
    printf("%s\n", code);
}

void emit_header() {
    printf("#include <stdio.h>\n");
    printf("#include <stdlib.h>\n");
    printf("#include <string.h>\n");
    printf("#include <stdarg.h>\n");
    printf("#include <ctype.h>\n\n");
    printf("int main() {\n\n");
}


void emit_footer() {
    printf("    return 0;\n");
    printf("}\n");
}

DatasetLiteral* append_record(DatasetLiteral *ds, char** record_values, int num_values);

char *current_record_values[MAX_FIELDS_PER_RECORD];
int current_record_value_count = 0;

void reset_current_record() {
    current_record_value_count = 0;
}

// Helper function to convert condition string to use array indexing
void convert_condition_to_array_access(char* condition, char* dataset_name, char* output) {
    char result[512];
    strcpy(result, condition);
    
    // Remove outer parentheses if present
    if (result[0] == '(' && result[strlen(result)-1] == ')') {
        result[strlen(result)-1] = '\0';
        memmove(result, result + 1, strlen(result));
    }
    
    // Replace field names with dataset_name[i].field_name
    // Process from right to left to avoid replacing already replaced strings
    for (int i = field_count - 1; i >= 0; i--) {
        char search[64], replace[128];
        sprintf(search, "%s", fields[i].name);
        sprintf(replace, "%s[i].%s", dataset_name, fields[i].name);
        
        // Find and replace all occurrences, but only if not already part of an array access
        char* pos = result;
        while ((pos = strstr(pos, search)) != NULL) {
            // Check if it's a whole word and not already part of array access
            int is_word_start = (pos == result || (!isalnum(pos[-1]) && pos[-1] != '_' && pos[-1] != ']'));
            int is_word_end = (!isalnum(pos[strlen(search)]) && pos[strlen(search)] != '_' && pos[strlen(search)] != '[');
            
            // Also check that it's not already part of "dataset[i].field" pattern
            int is_already_replaced = 0;
            if (pos > result && pos - result >= 3) {
                // Check if there's "[i]." before this position
                char* check_pos = pos - 4;
                if (check_pos >= result && strncmp(check_pos, "[i].", 4) == 0) {
                    is_already_replaced = 1;
                }
            }
            
            if (is_word_start && is_word_end && !is_already_replaced) {
                char temp[512];
                int pos_idx = pos - result;
                strncpy(temp, result, pos_idx);
                temp[pos_idx] = '\0';
                strcat(temp, replace);
                strcat(temp, pos + strlen(search));
                strcpy(result, temp);
                pos = result + pos_idx + strlen(replace);
            } else {
                pos += strlen(search);
            }
        }
    }
    
    strcpy(output, result);
}

// Helper function to get struct name from dataset name
void get_struct_name(char* dataset_name, char* struct_name) {
    strcpy(struct_name, dataset_name);
    if (struct_name[0] >= 'a' && struct_name[0] <= 'z') {
        struct_name[0] = struct_name[0] - 'a' + 'A';
    }
}


%}

%code requires {
    // Forward declaration for header file - full definition is in %{ %} section
    typedef struct DatasetLiteral DatasetLiteral;
}

%union {
    char* str;   // for identifiers and strings
    int num;     // for numbers
    DatasetLiteral *ds;
}

%token <str> IDENTIFIER STRING NUMBER

%token DATASET INPUT FILTER WHERE SELECT COUNT GROUP BY SORT ASC DESC OUTPUT
%token IF END FOR IN FROM TO AND OR LEN AVG MAX MIN
%token ASSIGN EQ NEQ LE GE LT GT
%token LBRACKET RBRACKET LBRACE RBRACE COLON COMMA DOT LPAREN RPAREN

%type <str> program statement dataset_init input_stmt filter_stmt select_stmt
%type <str> count_stmt group_stmt sort_stmt output_stmt if_stmt for_stmt
%type <str> field_or_id optional_to condition expr function
%type <str> record field_assignments field_list field value
%type <str> comp_op logical_op
%type <num> order

%type <ds> dataset_literal record_list

%start program

%%

program:
      statement
    | statement program
    ;

statement:
      dataset_init
    | input_stmt
    | filter_stmt
    | select_stmt
    | count_stmt
    | group_stmt
    | sort_stmt
    | output_stmt
    | if_stmt
    | for_stmt
    ;

dataset_init:
    DATASET IDENTIFIER ASSIGN dataset_literal
    {
        DatasetLiteral *ds = $4;
        current_dataset_name = $2;
        
        // Generate struct definition
        char struct_name[256];
        sprintf(struct_name, "%s", $2);
        // Capitalize first letter for struct name
        if (struct_name[0] >= 'a' && struct_name[0] <= 'z') {
            struct_name[0] = struct_name[0] - 'a' + 'A';
        }
        
        printf("struct %s {\n", struct_name);
        for (int j = 0; j < field_count; j++) {
            printf("    %s %s;\n", fields[j].type, fields[j].name);
        }
        printf("};\n\n");
        
        // Generate array initialization
        printf("struct %s %s[] = {\n", struct_name, $2);
        
        // Print each record with proper C struct initialization syntax
        for (int i = 0; i < ds->record_count; i++) {
            printf("    {");
            int num_values = ds->value_counts[i];
            for (int j = 0; j < field_count && j < num_values; j++) {
                // Get the value - it should be stored at index j
                char* val = ds->values[i][j];
                if (val != NULL) {
                    printf(".%s = %s", fields[j].name, val);
                    if (j < field_count - 1 && j < num_values - 1) {
                        printf(", ");
                    }
                }
            }
            printf("}");
            if (i < ds->record_count - 1) {
                printf(",");
            }
            printf("\n");
        }
        printf("};\n");
        
        // Generate array size variable
        printf("int %s_count = %d;\n", $2, ds->record_count);
        
        // Initialize work pointer to original array (for unfiltered datasets)
        printf("struct %s* %s_work = %s;\n", struct_name, $2, $2);
        printf("int %s_work_count = %s_count;\n\n", $2, $2);
        
        // Update current working dataset
        if (current_working_dataset) free(current_working_dataset);
        current_working_dataset = strdup($2);
        
        // Clean up allocated memory
        for (int i = 0; i < ds->record_count; i++) {
            for (int j = 0; j < field_count; j++) {
                free(ds->values[i][j]);
            }
        }
        free(ds);
        
        // Reset state (but keep field_count for use in filter/sort/select/output)
        first_record = 0;
        // Don't reset field_count - we need it for filter/sort/select/output operations 
    }
;



input_stmt:
      INPUT IDENTIFIER
    {
        char buf[256];
        sprintf(buf, "/* Read input into %s */", $2);
        emit(buf);
    }
    | INPUT IDENTIFIER FROM STRING
    {
        char buf[256];
        sprintf(buf, "/* Read input %s from file %s */", $2, $4);
        emit(buf);
    }
    ;

filter_stmt:
      FILTER IDENTIFIER WHERE condition
    {
        char struct_name[256];
        get_struct_name($2, struct_name);
        
        // Convert condition to use array indexing
        char cond_buf[512];
        convert_condition_to_array_access($4, $2, cond_buf);
        
        // Generate filtered array
        printf("// Filter %s where %s\n", $2, $4);
        printf("struct %s %s_filtered[%d];\n", struct_name, $2, MAX_RECORDS);
        printf("int %s_filtered_count = 0;\n", $2);
        printf("for (int i = 0; i < %s_count; i++) {\n", $2);
        printf("    if (%s) {\n", cond_buf);
        printf("        %s_filtered[%s_filtered_count++] = %s[i];\n", $2, $2, $2);
        printf("    }\n");
        printf("}\n");
        printf("// Update working dataset\n");
        printf("%s_work = %s_filtered;\n", $2, $2);
        printf("%s_work_count = %s_filtered_count;\n\n", $2, $2);
        
        // Update current working dataset
        if (current_working_dataset) free(current_working_dataset);
        current_working_dataset = strdup($2);
        
        free($4);
    }
    ;

select_stmt:
      SELECT IDENTIFIER field_list
    {
        // Store selected fields for later use in output
        if (selected_fields) free(selected_fields);
        selected_fields = strdup($3);
        printf("// Select fields from %s: %s\n\n", $2, $3);
        
        // Update current working dataset
        if (current_working_dataset) free(current_working_dataset);
        current_working_dataset = strdup($2);
        
        free($3);
    }
    ;

count_stmt:
      COUNT IDENTIFIER
    {
        char buf[256];
        sprintf(buf, "/* Count records in %s */", $2);
        emit(buf);
    }
    ;

group_stmt:
      GROUP IDENTIFIER BY field
    {
        char buf[256];
        sprintf(buf, "/* Group %s by %s */", $2, $4);
        emit(buf);
    }
    ;

sort_stmt:
      SORT IDENTIFIER order
    {
        char struct_name[256];
        get_struct_name($2, struct_name);
        const char* order_str = ($3 == ASC) ? "ASC" : "DESC";
        
        // Determine which array to use (filtered or original)
        printf("// Sort %s %s\n", $2, order_str);
        printf("// Simple bubble sort\n");
        printf("for (int i = 0; i < %s_work_count - 1; i++) {\n", $2);
        printf("    for (int j = 0; j < %s_work_count - i - 1; j++) {\n", $2);
        
        // Sort by first field (simple approach)
        if (field_count > 0) {
            char* first_field = fields[0].name;
            char* field_type = fields[0].type;
            
            if (strcmp(field_type, "int") == 0) {
                if ($3 == DESC) {
                    printf("        if (%s_work[j].%s < %s_work[j+1].%s) {\n", $2, first_field, $2, first_field);
                } else {
                    printf("        if (%s_work[j].%s > %s_work[j+1].%s) {\n", $2, first_field, $2, first_field);
                }
            } else {
                if ($3 == DESC) {
                    printf("        if (strcmp(%s_work[j].%s, %s_work[j+1].%s) < 0) {\n", $2, first_field, $2, first_field);
                } else {
                    printf("        if (strcmp(%s_work[j].%s, %s_work[j+1].%s) > 0) {\n", $2, first_field, $2, first_field);
                }
            }
            printf("            struct %s temp = %s_work[j];\n", struct_name, $2);
            printf("            %s_work[j] = %s_work[j+1];\n", $2, $2);
            printf("            %s_work[j+1] = temp;\n", $2);
            printf("        }\n");
        }
        printf("    }\n");
        printf("}\n\n");
        
        // Update current working dataset
        if (current_working_dataset) free(current_working_dataset);
        current_working_dataset = strdup($2);
    }
    ;

output_stmt:
      OUTPUT field_or_id optional_to
    {
        // Use current working dataset (from last select/filter/sort), or the provided identifier as fallback
        // Make sure we have a valid string
        char dataset_name_buf[256];
        if (current_working_dataset != NULL && strlen(current_working_dataset) > 0) {
            strncpy(dataset_name_buf, current_working_dataset, sizeof(dataset_name_buf) - 1);
            dataset_name_buf[sizeof(dataset_name_buf) - 1] = '\0';
        } else {
            strncpy(dataset_name_buf, $2, sizeof(dataset_name_buf) - 1);
            dataset_name_buf[sizeof(dataset_name_buf) - 1] = '\0';
        }
        const char* dataset_name = dataset_name_buf;
        
        // Output the dataset
        printf("// Output %s\n", $2);
        printf("for (int i = 0; i < %s_work_count; i++) {\n", dataset_name);
        
        // Print fields - if select was called, only print selected fields
        if (selected_fields != NULL) {
            // Parse selected_fields (comma-separated list) and print only those
            char* fields_str = strdup(selected_fields);
            char* token = strtok(fields_str, ",");
            int first = 1;
            while (token != NULL) {
                // Trim whitespace
                while (*token == ' ') token++;
                char* end = token + strlen(token) - 1;
                while (end > token && *end == ' ') *end-- = '\0';
                
                // Find the field index
                for (int j = 0; j < field_count; j++) {
                    if (strcmp(fields[j].name, token) == 0) {
                        if (!first) printf("    printf(\" \");\n");
                        if (strcmp(fields[j].type, "int") == 0) {
                            printf("    printf(\"%%d\", %s_work[i].%s);\n", dataset_name, fields[j].name);
                        } else {
                            printf("    printf(\"%%s\", %s_work[i].%s);\n", dataset_name, fields[j].name);
                        }
                        first = 0;
                        break;
                    }
                }
                token = strtok(NULL, ",");
            }
            free(fields_str);
        } else {
            // Print all fields
            for (int j = 0; j < field_count; j++) {
                if (strcmp(fields[j].type, "int") == 0) {
                    printf("    printf(\"%%d\", %s_work[i].%s);\n", dataset_name, fields[j].name);
                } else {
                    printf("    printf(\"%%s\", %s_work[i].%s);\n", dataset_name, fields[j].name);
                }
                if (j < field_count - 1) {
                    printf("    printf(\" \");\n");
                }
            }
        }
        printf("    printf(\"\\n\");\n");
        printf("}\n\n");
        
        free($2);
        if ($3 && strlen($3) > 0) {
            free($3);
        }
    }
    ;

field_or_id:
      IDENTIFIER { $$ = strdup($1); }
    | field      { $$ = strdup($1); }
    ;

optional_to:
      /* empty */ { $$ = strdup(""); }
    | TO STRING   { $$ = strdup($2); }
    ;

if_stmt:
      IF condition statements END
    {
        char buf[512];
        sprintf(buf, "if (%s) { /* ... */ }", $2);
        emit(buf);
    }
    ;

for_stmt:
      FOR IDENTIFIER IN IDENTIFIER statements END
    {
        char buf[512];
        sprintf(buf, "for (%s in %s) { /* ... */ }", $2, $4);
        emit(buf);
    }
    ;

statements:
      statement { /* just propagate */ }
    | statement statements { /* propagate multiple statements */ }
    ;

condition:
      expr comp_op expr
    {
        char buf[256];
        sprintf(buf, "(%s %s %s)", $1, $2, $3);
        $$ = strdup(buf);
    }
    | condition logical_op condition
    {
        char buf[256];
        sprintf(buf, "(%s %s %s)", $1, $2, $3);
        $$ = strdup(buf);
    }
    ;

expr:
      value      { $$ = strdup($1); }
    | field      { $$ = strdup($1); }
    | function LPAREN field RPAREN
      { 
        char buf[256];
        sprintf(buf, "%s(%s)", $1, $3);
        $$ = strdup(buf);
      }
    | function LPAREN IDENTIFIER RPAREN
      { 
        char buf[256];
        sprintf(buf, "%s(%s)", $1, $3);
        $$ = strdup(buf);
      }
    ;

function:
      LEN   { $$ = strdup("len"); }
    | AVG   { $$ = strdup("avg"); }
    | MAX   { $$ = strdup("max"); }
    | MIN   { $$ = strdup("min"); }
    | COUNT { $$ = strdup("count"); }
    ;

dataset_literal:
    LBRACKET record_list RBRACKET
    { $$ = $2; } // $2 is the DatasetLiteral*
;

record_list:
    record 
    { 
        // Base case: Create the first DatasetLiteral struct
        DatasetLiteral *ds = (DatasetLiteral*)malloc(sizeof(DatasetLiteral));
        ds->record_count = 0;
        
        // Append the record data - initialize all slots first
        for (int j = 0; j < MAX_FIELDS_PER_RECORD; j++) {
            ds->values[ds->record_count][j] = NULL;
        }
        // Then store the actual values
        ds->value_counts[ds->record_count] = current_record_value_count;
        for (int j = 0; j < current_record_value_count; j++) {
            ds->values[ds->record_count][j] = current_record_values[j];
        }
        ds->record_count++;
        
        // Reset temporary storage
        reset_current_record();
        
        $$ = ds; 
    }
    | record_list COMMA record
    {
        // Recursive case: Append the new record to the existing struct ($1)
        DatasetLiteral *ds = $1;
        int i = ds->record_count; // The index for the new record
        
        if (i < MAX_RECORDS) {
            // Initialize all slots first
            for (int j = 0; j < MAX_FIELDS_PER_RECORD; j++) {
                ds->values[i][j] = NULL;
            }
            // Store the count and then the values
            ds->value_counts[i] = current_record_value_count;
            for (int j = 0; j < current_record_value_count; j++) {
                ds->values[i][j] = current_record_values[j];
            }
            ds->record_count++;
        }
        
        // Reset temporary storage
        reset_current_record();
        
        $$ = ds;
    }
;

record:
    LBRACE field_assignments RBRACE
    {
        // The values are now stored in `current_record_values` and `current_record_value_count`
        // We can't easily return them as a string, so we'll use a placeholder.
        $$ = strdup("RECORD_VALUES_READY"); 
    }
;

field_assignments:
    field COLON value
    {
        // Add field definition only for the first record and only if not already added
        if (first_record) {
            // Check if field already exists
            int exists = 0;
            for (int k = 0; k < field_count; k++) {
                if (strcmp(fields[k].name, $1) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (!exists && field_count < MAX_FIELDS) {
                fields[field_count].name = strdup($1);
                // Simple type check: if value starts with quote, it's a string, otherwise int
                fields[field_count].type = ($3[0] == '"') ? "char*" : "int";
                field_count++;
            }
        }
        
        // Store the value string for the current record
        if (current_record_value_count < MAX_FIELDS_PER_RECORD) {
            current_record_values[current_record_value_count++] = strdup($3); // Store a copy
        }
        
        $$ = strdup("/* Field assignment done */"); // Return a dummy string
    }
    | field COLON value COMMA field_assignments
    {
        // Add field definition only for the first record and only if not already added
        if (first_record) {
            // Check if field already exists
            int exists = 0;
            for (int k = 0; k < field_count; k++) {
                if (strcmp(fields[k].name, $1) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (!exists && field_count < MAX_FIELDS) {
                fields[field_count].name = strdup($1);
                fields[field_count].type = ($3[0] == '"') ? "char*" : "int";
                field_count++;
            }
        }
        
        // Store the value string for the current record
        if (current_record_value_count < MAX_FIELDS_PER_RECORD) {
            current_record_values[current_record_value_count++] = strdup($3); // Store a copy
        }

        $$ = strdup("/* Field assignment done */"); // Return a dummy string
    }
    ;


field_list:
      field { $$ = strdup($1); }
    | field COMMA field_list
      {
          char buf[256];
          sprintf(buf, "%s, %s", $1, $3);
          $$ = strdup(buf);
      }
    ;

field:
      IDENTIFIER { $$ = strdup($1); }
    | IDENTIFIER DOT field
      {
          char buf[256];
          sprintf(buf, "%s.%s", $1, $3);
          $$ = strdup(buf);
      }
    ;

value:
      STRING { $$ = strdup($1); }
    | NUMBER { $$ = strdup($1); }  // NUMBER is already a string from lexer
    ;

comp_op:
      EQ { $$ = strdup("=="); }
    | NEQ { $$ = strdup("!="); }
    | LT { $$ = strdup("<"); }
    | LE { $$ = strdup("<="); }
    | GT { $$ = strdup(">"); }
    | GE { $$ = strdup(">="); }
    ;

logical_op:
      AND { $$ = strdup("&&"); }
    | OR  { $$ = strdup("||"); }
    ;

order:
      ASC  { $$ = ASC; }
    | DESC { $$ = DESC; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    // Set stdout to binary mode to avoid UTF-16 encoding on Windows
    #ifdef _WIN32
    freopen(NULL, "wb", stdout);
    #endif
    
    emit_header();  // emit #include and main() start

    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("Error opening file");
            return 1;
        }
        yyin = f;
    }

    yyparse();

    emit_footer();  // emit return 0 and closing brace
    return 0;
}
