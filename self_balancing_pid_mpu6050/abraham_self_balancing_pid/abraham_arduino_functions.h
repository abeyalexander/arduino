double find_average(double values_array[])
{
  int i;
  double sum = 0;
  double average;
  int array_len = sizeof(values_array)/sizeof(double);
  Serial.println(sizeof(values_array));
  for (i=0; i<array_len; i++)
  {
    sum += values_array[i];
  }
  average = sum / array_len;
  return average;
}

void insert_into_queue(double queue_in[], double new_value)
{
	int i;
	int array_len = sizeof(queue_in)/sizeof(double);
	for (i=0; i<(array_len-1); i++)
	{
		queue_in[i] = queue_in[i+1];
	}
	queue_in[array_len] = new_value;
}
