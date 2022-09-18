const int nbPins=4;
int myPins[nbPins];
int values[nbPins];


double mean=0;
double variance=0;

const int windowsSize=100;
int window[nbPins][windowsSize];
int w_cursor = 0;
int w_sum[nbPins];
double w_mean[nbPins];
double w_variance[nbPins];
int current_size = 1;

void setup() {
  for (byte i = 0; i < nbPins; i++){
      myPins[i] = i;
      values[i] = 0;
      w_sum[i] = 0;
      w_mean[i] = 0.0;
      w_variance[i] = 0.0;
      
      for (byte j = 0; j < windowsSize; j++){
        window[i][j]=0;
      }
  }
  Serial.begin(9600);
}

void loop() {
  
  //read the values
  mean = 0;
  for (byte i = 0; i < nbPins; i++){
    values[i] = analogRead(myPins[i]);
    
    w_sum[i]-= window[i][w_cursor];
    window[i][w_cursor] = values[i];
    w_sum[i]+= window[i][w_cursor];
    
    Serial.print(values[i]);
    Serial.print(" ");
    mean+=values[i];
  }
  Serial.println(';');
  if(nbPins>0)
    mean=mean/(1.0*nbPins);  
  Serial.print("mean : ");
  Serial.println(mean);
  
  //compute variance
  variance = 0;
  for (byte i = 0; i < nbPins; i++){
    variance += pow(values[i] - mean, 2);
  }
  if(nbPins>0)
    variance=variance/(1.0*nbPins);
    
  Serial.print("variance : ");
  Serial.println(variance);

  for (byte i = 0; i < nbPins; i++){
    //w_mean
    w_mean[i] = (1.0*w_sum[i])/(1.0*current_size);
    //w_variance
    w_variance[i] = 0;
    for (byte j = 0; j < windowsSize; j++){
      w_variance[i] += pow(window[i][j] - w_mean[i], 2);
    }
    w_variance[i]/=(1.0*current_size);
  }


  //print w_mean and w_variance
  Serial.print("w_mean : [");
  for (byte i = 0; i < nbPins; i++){
    Serial.print(w_mean[i]);
    Serial.print(" ");
  }
  Serial.println("]");

  Serial.print("w_variance : [");
  for (byte i = 0; i < nbPins; i++){
    Serial.print(w_variance[i]);
    Serial.print(" ");
  }
  Serial.println("]");


  //Q5 detect when a sensor produces erroneous readings

  double var_treshold=300.0;
  if(variance>var_treshold){
    // if varience goes above treshold
    // or we compare sensor against each other
    
    int current_defect_pin = 0;
    int max_dist = 0;
    //compute the distance
    for (byte i = 0; i < nbPins; i++){
      int current_dist=0;
      for (byte j = 0; j < nbPins; j++){
        current_dist += abs(values[i]-values[j]);
      }
      if(current_dist>max_dist){
        max_dist=current_dist;
        current_defect_pin = i;
      }
    }

    Serial.print("Detected failed pin: ");
    Serial.println(current_defect_pin);
  }
  
  w_cursor++;
  w_cursor%=windowsSize;
  current_size = min(current_size+1, windowsSize);
}
