#ifndef _VECTORLIBH
#define _VECTORLIBH

//defines a guard for memory allocations of arrays
#define MAX_SAMPLES 100000
//defines a guard for maximum printable information of array
#define MAX_PRINTABLE_LEN 50
#define PI 3.14159265359
#define EXP 2.7182818284
 
//boolean control flag for some functions
typedef enum{
    YES,
    NO
} ctrl_flag;

//struct of vector of real samples
struct Vector
{
    float *array;
    char *name;
    int len;
};

/*
//#modifications (under construction!)
//struct for 2-dimensional vector 
struct Complex_Vector
{
    struct Vector v_real;
    struct Vector v_imag;
    int len;
    char *name;
};
*/

//data handling functions for struct Vector
struct Vector *vector_create(char *name, int len);
int vector_copy_array(struct Vector *src,struct Vector *dest);
void vector_destroy(struct Vector *vector);
void vector_print_info(struct Vector *vector);
void vector_print_samples(struct Vector *vector,int numPoints);;
float vector_get_sample(struct Vector *vector, int index);
int vector_change_sample(struct Vector *vector, int index, float value);
int vector_write_txt(struct Vector *vector, ctrl_flag write_txt);
int vector_read_txt(struct Vector *vector, char *nameFile);

//signal creating functions;
int vector_create_step(struct Vector *vector,int init_index, float amplitude);
int vector_create_ramp(struct Vector *vector,int init_index,float slope);
int vector_create_cos(struct Vector *vector,int init_index, int period);
int vector_create_sin(struct Vector *vector,int init_index, int period);
int vector_create_exp(struct Vector *vector, int init_index, float expoent);

//signal operations
int vector_add(struct Vector *src1,struct Vector *src2,struct Vector *res);
int vector_sub(struct Vector *src1,struct Vector *src2,struct Vector *res);
int vector_mult(struct Vector *src1,struct Vector *src2,struct Vector *res);
int vector_div(struct Vector *src1,struct Vector *src2,struct Vector *res);
int vector_pow(struct Vector *src,struct Vector *dest,float power);

//linear algebra operations
//#modifications -- need to test these functions!
int vector_inner_product(struct Vector *src1, struct Vector *src2, float *result);
int vector_sum(struct Vector *vector, float *result);
int vector_energy(struct Vector *vector, float *result);
int vector_mult_const(struct Vector *vector, float gain);
//int vector_conv(struct Vector *src1, struct Vector *src2, struct Vector *result);
#endif