//
// Created by sebdis on 02/05/20.
//
#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#ifndef PROGETTO_LABSO_PROCESS_TEXTFILE_H
#define PROGETTO_LABSO_PROCESS_TEXTFILE_H



#define MAX_CHARACTER 95


///
/// \param c: character
/// \return the index of the array from a char value
int char_to_index(const char c){
    return c - 32;
}

///
/// \param filename: name of the file to print
void print_filetext_to_current_terminal(const char* filename){
    FILE* read_file;
    read_file = fopen(filename, "r");
    if (read_file == NULL){
        perror(("An error has occurred during the opening of the file \'%s\'", filename));
        exit(1);
    }
    int c;
    c = fgetc(read_file);
    while (c != EOF){
        printf("%c",c);
        c = fgetc(read_file);
    }
    fclose(read_file);
}

///
/// \param index: index of the array
/// \return the corresponding character of the index
int index_to_char(const int index){
    return index + 32;
}

///
/// \param data: array with the percentages of each character
/// \param filename: name of the file to analyze
/// \return 0 if there is an error during the opening of the file, 1 if successful
int analyze_file(float * data, const char* filename){
    FILE* file;
    file = fopen(filename, "r");
    if (file == NULL){
        return 0;
    }
    int c = getc(file);
    int total_letter = 1;
    while (c != EOF){
        total_letter++;
        data[char_to_index(c)]++;
        c = getc(file);
    }
    fclose(file);
    int i;
    for (i = 0; i < MAX_CHARACTER; i++){
        data[i] = 100 * data[i] / ((float)total_letter);
    }
    return 1;
}

///
/// \param data: array with the data of the analyzed file
/// \param report_name : name of the report file
/// \param report_path  : where to save the report
/// \return 1 if successful , 0 if an error occur during the opening of the file stream
int generate_report(const float* data, char* report_name, char* report_path){
    chdir(report_path);
    FILE* write_file;
    write_file = fopen(report_name, "w");
    if (write_file == NULL){
        return  0;
    }
    int i;
    for (i = 0; i < MAX_CHARACTER; i++){
        fprintf(write_file, "character %c : %0.2f\n" , index_to_char(i), data[i]);
    }
    fclose(write_file);
    return 1;
} //create a file .txt with the report (return 1 if successfull, 0 if not)

///
/// \param number
/// \return "-" + "number" -> "-${number}"
char* get_name_ext(const int number){
    char* ret = (char*) malloc(sizeof(char) *  2);
    ret[0] = '-';
    ret[1] = number + '0';
    //ret[2] = '\0';
    return ret;
}

///
/// \param src_filename: Example("ciao.txt") the original name of the file
/// \param number_of_part: The number of part for which the file will be divided
/// \return  a matrix with the names of the new subfiles of the original file
char** create_subfiles_names(const char* src_filename, int number_of_part){
    char** ret = (char**) malloc(sizeof(char**) * (number_of_part + 2));
    int i,j;
    for (i = 0; i < number_of_part; i++){
        ret[i] = (char*) malloc(sizeof(char) * (strlen(src_filename) + 2));
    }
    int len_name=strlen(src_filename)- 4;
    const char* ext = ".txt";
    for (i = 0; i < number_of_part; i++){
        for (j = 0; j < len_name; j++){
            ret[i][j] = src_filename[j];
        }
        strcat(ret[i], get_name_ext(i));
        strcat(ret[i], ext);
    }
    //free(&ext);
    return ret;
}

///
/// \param or_filename: the original name of the file
/// \param or_file_path : path of the file that is to be divided
/// \param save_path : path of the save location for the subfiles
/// \param number_of_part : number of subfiles necessary
/// \return
char** create_subfiles(const char* or_filename, const char* or_file_path, const char* save_path, int number_of_part){
    //chdir(save_path);
    //printf("%d", *number_of_part - '0');
    printf("Malloc started\n");
    printf("number of part to create %d\n", number_of_part);
    char* cd_command = (char *) malloc(sizeof(char) * 100);
    char* buffer2 = (char *) malloc(sizeof(char) * 100);
    char* mv_command = (char *) malloc(sizeof(char) * 100);

    char** list_filedivision= (char **) malloc(sizeof(char**) * number_of_part );
    int i;
    for (i = 0; i < number_of_part; i++){
        list_filedivision[i] = (char *) malloc(sizeof(char) * 4);
    }

    strcpy(cd_command, "cd ");

    printf("Malloc completed\n");

    char* cwd = getcwd(NULL, 0);
    strcpy(buffer2, cwd);
    strcat(cd_command, buffer2);

    char* space = " ";

    strcpy(buffer2, "split -n");
    char* n = (char*) malloc(sizeof(char));
    *n = number_of_part + '0';

    strcat(buffer2, n);
    strcat(buffer2, space);
    strcat(buffer2, or_filename);





    system(cd_command);
    system(buffer2);

    printf("Subfiles of file %s created\n", or_filename);

    char** new_subfiles_names = create_subfiles_names(or_filename, number_of_part);
    system("ls x* > temp_list.txt");


    FILE* copyfile;
    copyfile = fopen("temp_list.txt", "r");
    if (copyfile == NULL){
        perror("An error as occurred during the creation of the file stream for the temp_list with the names of the subfiles");
        exit(1);
    }


    char c;
    c = getc(copyfile);
    int current_index_x = 0;
    int current_index_y = 0;
    while(c != EOF){
       // printf("x = %d, y = %d, c = %c\n", current_index_x, current_index_y, c);

        if (c == '\n'){
            list_filedivision[current_index_x][current_index_y] = '\0';
            current_index_x++;
            current_index_y = 0;
        } else {
            list_filedivision[current_index_x][current_index_y] = c;
            current_index_y++;
        }
        c= getc(copyfile);
    }
    fclose(copyfile);

    printf("List_file_division created\n");

    strcpy(mv_command, save_path);
    printf("%s\n", mv_command);
    for (i = 0; i < number_of_part; i++){
        strcat(mv_command, space);
        strcat(mv_command, new_subfiles_names[i]);
        printf("%s\n", mv_command);
        rename(list_filedivision[i], mv_command);
        strcpy(mv_command, save_path);
    }



    for (i = 0; i < number_of_part ; i++){
        free(list_filedivision[i]);
        printf("%d freed !\n", i);
    }

    free(list_filedivision);
    free(buffer2);
    free(cwd);
    free(cd_command);

    return new_subfiles_names;
}

#endif //PROGETTO_LABSO_PROCESS_TEXTFILE_H
