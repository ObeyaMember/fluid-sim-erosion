#include "general_purpose.h"

void print_int_array(int* array, int array_size){
    for (int i = 0; i < array_size; i += 1){
        printf("array[%d]: %d\n", i, array[i]);
    }
}

void print_float_array(float* array, int array_size){
    for (int i = 0; i < array_size; i += 1){
        printf("array[%d]: %f\n", i, array[i]);
    }
}

void print_vec3_array(vec3* array, int array_size){
    for (int i = 0; i < array_size; i += 1){
        printf("vec3_arr[%d]: (%f, %f, %f)\n", i, array[i][0], array[i][1], array[i][2]);
    }
}

int get_max_int_array(int* array, int array_size){
    int max = INT_MIN;
    for (int i = 0; i < array_size; i += 1){
        if (max < array[i]){
            max = array[i];
        }
    }

    return max;
}

static void radix_sort_one_pass(int* input, int input_size, int exp){
    int output[input_size];
    int counters[10] = {0,0,0,0,0,0,0,0,0,0};
    // first count
    for (int i = 0; i < input_size; i += 1){
        int digit = (input[i] / exp) % 10;
        //printf("exp = %d, digit[%d] = %d\n", exp, i, digit);
        counters[digit] += 1;
    }
    
    //print_int_array(counters, 10);
    
    // prefix sum
    for (int i = 1; i < 10; i += 1){
        counters[i] += counters[i-1];
    }
    // order
    for (int i = input_size - 1; i >= 0; i -= 1){
        int digit = (input[i] / exp) % 10;
        output[counters[digit] - 1] = input[i];
        counters[digit] -= 1;
    }

    // set values in place
    for (int i = 0; i < input_size; i += 1){
        input[i] = output[i];
    }
}

static void radix_sort_map_one_pass(int* input, int* map_input, int input_size, int exp){
    int output[input_size];
    int counters[10] = {0,0,0,0,0,0,0,0,0,0};
    // first count
    for (int i = 0; i < input_size; i += 1){
        int digit = (map_input[i] / exp) % 10;
        //printf("exp = %d, digit[%d] = %d\n", exp, i, digit);
        counters[digit] += 1;
    }
    //printf("counters:\n");
    //print_int_array(counters, 10);
    //printf("-----------\n");
    //print_int_array(counters, 10);
    
    // prefix sum
    for (int i = 1; i < 10; i += 1){
        counters[i] += counters[i-1];
    }
    // order
    for (int i = input_size - 1; i >= 0; i -= 1){
        int digit = (map_input[input[i]] / exp) % 10;
        output[counters[digit] - 1] = input[i];
        counters[digit] -= 1;
    }

    // set values in place
    for (int i = 0; i < input_size; i += 1){
        input[i] = output[i];
    }
}

void radix_sort(int* input, int input_size){
    int max = get_max_int_array(input, input_size);

    int n_passes = 0;
    while (max > 0){
        max = max / 10;
        n_passes += 1;
    }
    //printf("%d\n", n_passes);
    int exp = 1;
    for (int i = 0; i < n_passes; i += 1){
        
        //printf("pass number %d\n", i + 1);
        radix_sort_one_pass(input, input_size, exp); 
        exp = exp * 10;
    }
}

void radix_sort_max_known(int* input, int input_size, int max){
    int n_passes = 0;
    while (max > 0){
        max = max / 10;
        n_passes += 1;
    }
    
    int exp = 1;
    for (int i = 0; i < n_passes; i += 1){
        exp = exp * 10;
        radix_sort_one_pass(input, input_size, exp); 
    }
}

void radix_sort_map(int* input, int* map_input, int input_size){
    int max = get_max_int_array(map_input, input_size);

    int n_passes = 0;
    while (max > 0){
        max = max / 10;
        n_passes += 1;
    }
    //printf("%d\n", n_passes);
    int exp = 1;
    for (int i = 0; i < n_passes; i += 1){
        
        //printf("pass number %d\n", i + 1);
        radix_sort_map_one_pass(input, map_input, input_size, exp); 
        exp = exp * 10;
    }
}

void radix_sort_map_max_known(int* input, int* map_input, int input_size, int max){
    int n_passes = 0;
    while (max > 0){
        max = max / 10;
        n_passes += 1;
    }
    
    int exp = 1;
    for (int i = 0; i < n_passes; i += 1){
        //printf("pass number %d\n", i + 1);
        radix_sort_map_one_pass(input, map_input, input_size, exp); 
        exp = exp * 10;
    }
}