double find_average(double * values_array, int len_array)
{
  int i;
  double sum = 0;
  double average;
  for (i = 0; i < len_array; i++)
  {
    sum += values_array[i];
  }
  average = sum / len_array;
  return average;
}

void insert_into_queue(double * queue_in, int len_queue, double new_value)
{
  int i;
  for (i = 0; i < (len_queue - 1); i++)
  {
    queue_in[i] = queue_in[i + 1];
  }
  queue_in[len_queue - 1] = new_value;
}
