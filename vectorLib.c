#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vectorLib.h"

struct Vector *vector_create(char *name, int len)
{
    int a_len; //actual length to be used

    printf("Creating Vector.\n");
    printf("\t NAME: %s, LENGTH: %d\n",name,len);
    //guard for SIZE of vector
    if(len > MAX_SAMPLES)
    {   
        //if desired len > MAX_SAMPLES, 
        //creates vector with MAX_SAMPLES
        printf("WARNING @vector_create: LENGHT of Vector > MAX SAMPLES.\n ");
        printf("Creating Vector with MAX_SAMPLES = %d,\n",MAX_SAMPLES);
        a_len = MAX_SAMPLES;
    }
    else
    if(len < 0)
    {
        //if negative, creates with length = 1
        printf("WARNING @vector_create: LENGHT of Vector cannot be negative.\n ");
        printf("Creating Vector with only one sample.\n");
        a_len = 1;

    }
    else
    {
        a_len = len;
    }

    //allocates memory
    struct Vector *vector = malloc(sizeof(struct Vector));
    //checks if there is memory 
    assert(vector != NULL);

    //copies the name into the char pointer of the struct
    vector->name = strdup(name);
    //assigns lenght of vector
    vector->len = a_len;
    //allocates memory filled with zeros 
    vector->array = calloc(a_len,sizeof(float));

    return vector;
}

void vector_destroy(struct Vector *vector)
{
    //check if pointer is ok
    assert(vector != NULL);

    //free the memory
    free(vector->name);
    free(vector->array);
    free(vector);
}

//prints name and lenght of vector 
void vector_print_info(struct Vector *vector)
{
    printf("Info of vector\n");
    printf("Name: %s\n", vector->name);
    printf("Length: %d\n", vector->len);
}

//print samples at terminal
void vector_print_samples(struct Vector *vector,int numPoints)
{
    int len; //length variable for printing
    
    //initial message
    printf("**********************\n");
    printf("Printing vector points\n");
    printf("**********************\n");

    //check if it's too much data to be printed
    if(vector->len > MAX_PRINTABLE_LEN)
    {
        printf("WARNING @vector_print_samples: Too much data!\n");
        printf("Printing only the first %d points.\n",MAX_PRINTABLE_LEN);
        len = MAX_PRINTABLE_LEN; //assigns 50 for max printable data
    }
    else
    {   
        //else, assigns original asked points to print
        len = numPoints;
    }
    
    //loop for printing
    int i;
    printf("Samples of vector: \n");
    for(i = 0 ; i < len; i++)
    {
        //prints the sample index + the float content
        printf("%s[%d] : %.3f\n",vector->name,i,*(vector->array+i));
    }

}

//get the sample from a *vector at index "index"
float vector_get_sample(struct Vector *vector, int index)
{
    //check routine: if index out of bounds, gets the latest
    if(index > vector->len)
    {   
        int last_i;
        last_i = vector->len-1;
        printf("ERROR @vector_get_sample: index out of bounds.\n ");
        printf("Getting the last sample index,\n");
        return *(vector->array+last_i);
    }
    else
    if(index < 0) //if negative, gets index zero
    {
        printf("ERROR @vector_get_sample: index out of bounds (negative).\n ");
        printf("Getting the first sample index.\n");
        return *(vector->array);
    }
    else //if within limits, gets the right one
    {
        int actual_i;
        actual_i = index-1;
        return *(vector->array+actual_i);
    }

}

//changes a sample at a defined position of a vector
//carefull must be taken, this function changes data information
int vector_change_sample(struct Vector *vector, int index, float value)
{
    //check routine: if index out of bounds, gets the closest
    if(index > vector->len)
    {   
        printf("ERROR @vector_change_sample: index out of bounds.\n ");
        return -1;
    }
    else
    if(index < 0)
    {
        printf("ERROR @vector_change_sample: index out of bounds (negative).\n ");
        return -1;
    }
    else
    {
        int actual_i;
        actual_i = index-1;
        *(vector->array+actual_i) = value;
    }
    return 0;
}

//writes a .txt file with name "vector-name.txt"
//writes one sample per line
//can also save a file without extension.
//ctrl_flag = YES indicates a file saved
//following the name + .txt extension
//ctrl_flag = NO disregards the extensions and writes a file
//only with the name of the vector.
// example of output of text file:
// 0.000000
// 1.450000
// 2.454030 
int vector_write_txt(struct Vector *vector, ctrl_flag writeTxt)
{   
    //assigns pointer for the name of the file
    char *nameFile;

    //if control flag = YES, then writes with .txt extension
    if(writeTxt == YES)
    {  
        //creates a an array for the extension
        char *extension;
        extension = ".txt";
        //init memory size for allocation
        int size1, size2, sizet;
        //computes memory size of vector name
        size1 = strlen(vector->name);
        //computes memory size of extension
        size2 = strlen(extension);
        //sums all sizes +1 for the "\0" char in strcpy
        sizet = size1 + size2 + 1;

        nameFile = malloc(sizet * sizeof(char));
        //copies data and concatenates extension 
        strcpy(nameFile,vector->name);
        strcat(nameFile,extension);

    }
    else //if control flag = NO, writes plain file with no extension
    {
        nameFile = strdup(vector->name);
    }

    //opens file for saving
    FILE *fid = fopen(nameFile, "w");
    if(fid == NULL)
    {
        printf("ERROR @vector_write_txt: Could not create file named %s.\n", nameFile);
        return -1;
    }

    //loop for writing to file
    int i;
    for ( i = 0 ; i < vector->len ; i++)
    {
        fprintf(fid,"%f\n", *(vector->array+i));
    }
    
    free(nameFile);
    fclose(fid);
    return 0;
}

//reads and loads a vector from a file with a certain name
//reads each line of a file and assigns to an array of vector
int vector_read_txt(struct Vector *vector, char *nameFile)
{
    printf("Calling vector_read_txt to import data from file named: %s.\n", nameFile);
    FILE *fid = fopen(nameFile,"r");
    if(fid == NULL)
    {
        printf("ERROR @vector_read_txt: Could not read file named %s.\n", nameFile);
        return -1;
    }

    //loop for writing to vector
    int i;
    for (i=0 ; i < vector->len ; i++)
    {
        fscanf(fid,"%f\n",(vector->array+i));
    }
    printf("Import completed.\n");
    fclose(fid);
    return 0;
}


//assigns step signal to structure vector
//creates a step function with amplitude 1
//beginning at init_index
//returns 0 if success
//returns -1 if fails
int vector_create_step(struct Vector *vector,int init_index, float amplitude)
{
    //guard to check init_index range
    if(init_index > vector->len)
    {
        printf("ERROR @vector_create_step: Step Signal init_index > vector length. Aborting\n");
        return -1;
    }
    else
    if(init_index < 0)
    {
        printf("ERROR @vector_create_step: init_index cannot be negative. Aborting.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for( i = 0 ; i < vector->len ; i++)
    {
        if( i >= init_index)
        {
            *(vector->array+i) = amplitude;
        }
        else
        {
            *(vector->array+i) = 0;
        }   
    }
    return 0;
}

//creates a signal in the form of y[n] = slope*n
//beginning at init_index
//returns 0 if success
//returns -1 if fails
int vector_create_ramp(struct Vector *vector,int init_index,float slope)
{
     //guard to check init_index range
    if(init_index > vector->len)
    {
        printf("ERROR @vector_create_ramp: Step Signal init_index > vector length. Aborting\n");
        return -1;
    }
    else
    if(init_index < 0)
    {
        printf("ERROR @vector_create_ramp: init_index cannot be negative. Aborting.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for( i = 0 ; i < vector->len ; i++)
    {
        if( i >= init_index)
        {
            *(vector->array+i) = slope*(i-init_index);
        }
        else
        {
            *(vector->array+i) = 0;
        }   
    }
    return 0;
}

//creates a signal in the form of y[n] = cos(2*pi*n/N)
//beginning at init_index
//returns 0 if success
//returns -1 if fails
int vector_create_cos(struct Vector *vector,int init_index, int period)
{

    //guard to check init_index range
    if(init_index > vector->len)
    {
        printf("ERROR @vector_create_cos: Step Signal init_index > vector length. Aborting\n");
        return -1;
    }
    else
    if(init_index < 0)
    {
        printf("ERROR @vector_create_cos: init_index cannot be negative. Aborting.\n");
        return -1;
    }

    if(period < 0)
    {
        printf("ERROR @vector_create_cos: Frequency cannot be negative. Aborting.\n");
        return -1;
    }
    else
    if(period > vector->len)
    {
        printf("ERROR @vector_create_cos: Frequency cannot be greater than vector lenght. Aborting\n");
    }

    float p = (float)period;
    //loop for numerical processing
    int i;
    for( i = 0 ; i < vector->len ; i++)
    {
        if( i >= init_index)
        {
            *(vector->array+i) = cos(2*PI*(i-init_index)/p);
        }
        else
        {
            *(vector->array+i) = 0;
        }   
    }
    return 0;
}

//creates a signal in the form of y[n] = sin(2*pi*n/N)
//beginning at init_index
//returns 0 if success
//returns -1 if fails
int vector_create_sin(struct Vector *vector,int init_index, int period)
{

    //guard to check init_index range
    if(init_index > vector->len)
    {
        printf("ERROR: Step Signal init_index > vector length. Aborting\n");
        return -1;
    }
    else
    if(init_index < 0)
    {
        printf("ERROR: init_index cannot be negative. Aborting.\n");
        return -1;
    }

    //guard to check period value
    if(period < 0)
    {
        printf("ERROR @vector_create_sin: Frequency cannot be negative. Aborting.\n");
        return -1;
    }
    else
    if(period > vector->len)
    {
        printf("ERROR @vector_create_sin: Frequency cannot be greater than vector lenght. Aborting\n");
    }

    float p = (float)period;
    //loop for numerical processing
    int i;
    for( i = 0 ; i < vector->len ; i++)
    {
        if( i >= init_index)
        {
            *(vector->array+i) = sin(2*PI*(i-init_index)/p);
        }
        else
        {
            *(vector->array+i) = 0;
        }   
    }
    return 0;
}

//creates a function in the form of y[n] = e^(expoent*n)
//beginning at init_index
//returns 0 if success
//returns -1 if fails
int vector_create_exp(struct Vector *vector, int init_index, float expoent)
{
    //guard to check init_index range
    if(init_index > vector->len)
    {
        printf("ERROR @vector_create_exp: Step Signal init_index > vector length. Aborting\n");
        return -1;
    }
    else
    if(init_index < 0)
    {
        printf("ERROR @vector_create_exp: init_index cannot be negative. Aborting.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for( i = 0 ; i < vector->len ; i++)
    {
        if( i >= init_index)
        {
            *(vector->array+i) = exp(expoent*(i-init_index));
        }
        else
        {
            *(vector->array+i) = 0;
        }   
    }
    return 0;

}

//copies the array of one vector into another
//mantains the name of the dest vector, but changes array
//vectors must be the same lenght size;
int vector_copy_array(struct Vector *src,struct Vector *dest)
{
    //guard to check if they have the same length
    if(src->len != dest->len)
    {
        printf("ERROR @vector_copy_array: Vectors have different lengths.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for( i = 0 ; i < src->len ; i++)
    {
        *(dest->array+i) = *(src->array+i);
    }

    return 0;
}

//sums two vectors in the form of result[n] = src1[n] + src2[n]
int vector_add(struct Vector *src1,struct Vector *src2,struct Vector *res)
{
    //guards
    //guard for length 
    if(src1->len != src2->len)
    {
        printf("ERROR @vector_add: Source vectors have different lengths.\n");
        return -1;
    }
    else 
    if(src1->len != res->len)
    {
        printf("ERROR @vector_add: Result vector has different length w.r.t to source vectors.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for (i = 0 ; i < src1->len ; i++)
    {
        *(res->array+i) = *(src1->array+i) + *(src2->array+i);
    }
    return 0;

}

//substracts two vectors in the form of result[n] = src1[n] - src2[n]
int vector_sub(struct Vector *src1,struct Vector *src2,struct Vector *res)
{
    //guards
    //guard for length 
    if(src1->len != src2->len)
    {
        printf("ERROR @vector_add: Source vectors have different lengths.\n");
        return -1;
    }
    else 
    if(src1->len != res->len)
    {
        printf("ERROR @vector_add: Result vector has different length w.r.t to source vectors.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for (i = 0 ; i < src1->len ; i++)
    {
        *(res->array+i) = *(src1->array+i) - *(src2->array+i);
    }
    return 0;

}

//divides two vectors in the form result[n] = src1[n]/src2[n]
int vector_div(struct Vector *src1,struct Vector *src2,struct Vector *res)
{
    //guards
    //guard for length 
    if(src1->len != src2->len)
    {
        printf("ERROR @vector_add: Source vectors have different lengths.\n");
        return -1;
    }
    else 
    if(src1->len != res->len)
    {
        printf("ERROR @vector_add: Result vector has different length w.r.t to source vectors.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for (i = 0 ; i < src1->len ; i++)
    {
        *(res->array+i) = *(src1->array+i) / *(src2->array+i);
    }
    return 0;

}

//multiples two vectors in the form of result[n] = src1[n]*src2[n]
int vector_mult(struct Vector *src1,struct Vector *src2,struct Vector *res)
{
    //guards
    //guard for length 
    if(src1->len != src2->len)
    {
        printf("ERROR @vector_add: Source vectors have different lengths.\n");
        return -1;
    }
    else 
    if(src1->len != res->len)
    {
        printf("ERROR @vector_add: Result vector has different length w.r.t to source vectors.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for (i = 0 ; i < src1->len ; i++)
    {
        *(res->array+i) = *(src1->array+i) * *(src2->array+i);
    }
    return 0;

}

//creates a vector in the form of dest[n] = src[n].^(power)
int vector_pow(struct Vector *src,struct Vector *dest,float power)
{
    //guards
    if(src->len != dest->len)
    {
        printf("ERROR @vector_add: Result vector has different length w.r.t to source vectors.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    for (i = 0 ; i < dest->len ; i++)
    {
        *(dest->array+i) = pow(*(src->array+i),power);
    }
    return 0;

}

//computes the inner product of two real vectors v1 and v2
int vector_inner_product(struct Vector *src1, struct Vector *src2, float *result)
{
    //guards
    //guard for length 
    if(src1->len != src2->len)
    {
        printf("ERROR @vector_inner_product: Source vectors have different lengths.\n");
        return -1;
    }

    //loop for numerical processing
    int i;
    float y;
    y = 0.0;
    for (i = 0 ; i < src1->len ; i++)
    {
        y = y + *(src1->array+i) * *(src2->array+i);
    }
    //external float pointer receives the result of y
    *result = y;
    return 0;
}

//computes the sum of all samples of a vector
int vector_sum(struct Vector *vector, float *result)
{
     //loop for numerical processing
    int i;
    float y;
    y = 0.0;
    for (i = 0 ; i < vector->len ; i++)
    {
        y = y + *(vector->array + i);
    }
    //external float pointer receives the result of y
    *result = y;
    return 0;
}

//computes the energy of the signal
int vector_energy(struct Vector *vector, float *result)
{
     //loop for numerical processing
    int i;
    float y;
    y = 0.0;
    for (i = 0 ; i < vector->len ; i++)
    {
        y = y + *(vector->array + i) * *(vector->array + i);
    }
    //external float pointer receives the result of y
   *result = y;
   return 0;
}

//multiplies signal by constant
int vector_mult_const(struct Vector *vector, float gain)
{
    int i;

    for(i = 0 ; i < vector->len ; i++)
    {
        *(vector->array + i) = *(vector->array + i) * gain;
    }

    return 0;
}