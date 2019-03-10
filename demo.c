#include "stdio.h"
#include "stdlib.h"
#include "mlp.h"

#define DATA_LOG_ENABLED 0
#define EPOCH_LIMIT_MODE 0

#define NUM_PATTERNS 4
#define NUM_EPOCHES 100000
#define LEARNING_RATE 0.01
#define TARGET_ERROR 0.1
#define REPORT_INTERVAL 10000

int main(){
  double pattern_set[NUM_PATTERNS * NUM_NEURONES_INPUT] = {
    SOFT_HIGH, SOFT_LOW, SOFT_LOW, SOFT_LOW,
    SOFT_LOW, SOFT_HIGH, SOFT_LOW, SOFT_LOW,
    SOFT_LOW, SOFT_LOW, SOFT_HIGH, SOFT_LOW,
    SOFT_LOW, SOFT_LOW, SOFT_LOW, SOFT_HIGH,
  };
  int i, j;
  unsigned long int i_epoch = 0;
  double mlp_err;

#if DATA_LOG_ENABLED
  FILE* logfile_ptr;
  logfile_ptr = fopen("error_log.csv", "w");
  if(logfile_ptr == NULL){
    printf("I/O Error: Cannot open log file.");
  }
#endif

  /* Initialise all weight matrices in neural network */
  MLP_Weights_Init();

  mlp_err = MLP_ErrorAvg(NUM_PATTERNS, pattern_set, pattern_set);
  printf("Initial avg error is %lf\n", mlp_err);
#if DATA_LOG_ENABLED
  fprintf(logfile_ptr, "0,%lf\n", mlp_err);
#endif

  /* Start training */
  for(;;){
    MLP_Train(NUM_PATTERNS, LEARNING_RATE, pattern_set, pattern_set);
    i_epoch += 1;
    mlp_err = MLP_ErrorAvg(NUM_PATTERNS, pattern_set, pattern_set);
    if(!(i_epoch % REPORT_INTERVAL)){
      printf("Avg error is %lf\tafter %ld epoches\n", mlp_err, i_epoch);
#if DATA_LOG_ENABLED
      fprintf(logfile_ptr, "%ld,%lf\n", i_epoch, mlp_err);
#endif
    }
#if EPOCH_LIMIT_MODE
    if(mlp_err <= TARGET_ERROR || i_epoch == NUM_EPOCHES){
      break;
    }
#else
    if(mlp_err <= TARGET_ERROR){
      break;
    }
#endif
  }

  /* Test the neural network with patterns */
  for(j = 0; j < NUM_PATTERNS; j++){
    for(i = 0; i < NUM_NEURONES_INPUT; i++){
      neurone_input[i] = pattern_set[NUM_NEURONES_INPUT * j + i];
    }
    MLP_Evaluate();
    MLP_Dump();
  }

#if DATA_LOG_ENABLED
  fclose(logfile_ptr);
#endif

  return 0;
}
