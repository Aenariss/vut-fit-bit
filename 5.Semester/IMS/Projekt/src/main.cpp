/*
 * @file: main.cpp
 * @author: Vojtech Bubela
 * @login: xbubel08
 * @brief: implementation of AR model for IMS project
 */

/* Minor edits by:
 * Vojtech Fiala (xfiala61)
*/

#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>

// minimal value of autocorrelation found out in experiment 1
#define MIN_INFL 0.194
// number of electromobiles sold prior to 2021
#define CURR_ELECTRO 7109 

using namespace std;

/**
 * @name: calculate_prediction
 * @param vect: deque with values of time series (sales of electromobiles in
 * our case)
 * @param minInfluence: minimal level of influence that autocorrelation must
 * have to be used in the prediction
 * @brief: predict how many electromobiles will be sold in the next month
 * @description: this function uses AR model - autoregression - to predict the
 * next value of time series recieved in param vect. The influence of previous
 * months on the next month is predicted using autocorrelation. Autocorrelation
 * returns a value in interval <-1;1>, the bigger the value, the bigger the
 * impact. prediction is calculated by adding important values.
 * @retval: predicted value
 */
double calculate_prediction(deque<double> vect, double minInfulence);

/**
 * @name: experiment_1
 * @param vect: deque with values of time series (sales of electromobiles in
 * our case)
 * @brief: try to predict last 12 months of electric cars sales, based on sales
 * made previous to last 12 months
 * @description: in experiment one, we will determine what is the best level of
 * influence of autocorrelation that still should be valid. For example, if
 * there is a value 0.8 as a result of autocorrelation offsetet by 1 that means,
 * that a previous month had a significant influence on predicted value, if
 * its 0.2 that influence is much lower. This will be done by predicting last 12
 * months of sales and the shifting the minimal value of autocorrelation coeficient
 * until best result are achieved.
 */
void experiment_1(deque<double> vect);

/**
 * @name: experiment_2
 * @param vect: deque with values of time series (sales of electromobiles in
 * our case)
 * @brief: predict electric car sales for every year until 2030
 * @description: using the level of influence determined in experiment 1
 *               we will predict next 9 years, month by month
 */
void experiment_2(deque<double> vect);

/**
 * @name: experiment_3
 * @param vect: deque with values of time series (sales of electromobiles in
 * @brief: predict electric car sales for every year until 2030 when taking grant
 *         from the goverment into account
 */
void experiment_3(deque<double> vect);

int main(int argc, char **argv) {
  deque<double> vect;

  ifstream fin;
  fin.open("./src/input_data.txt", ifstream::in);

  // load data set from file
  while (true) {
    double x;
    fin >> x;

    if (fin.eof()) break;

    vect.push_front(x);
  }

  experiment_1(vect);
  experiment_2(vect);
  experiment_3(vect);

  return 0;
}

void experiment_1(deque<double> vect) {
  double minInfluence = 0.01;
  double prediction = 0;
  int year_pred = 0;
  double realitPerc = 0;

  deque<string> months = {"Listopad", "Říjen",  "Září",   "Srpen",
                          "Červenec", "Červen", "Květen", "Duben",
                          "Březen",   "Únor",   "Leden",  "Prosinec"};
  deque<double> smallerVect = vect;

  cout << "################ EXPERIMENT 1 ################ \n";

  for (int i = 0; i <= 11; i++) {
    smallerVect.pop_front();
  }

  // iterate until maximum value of autocorrelation coeficient
  while (minInfluence <= 1) {
    while (smallerVect.size() != (vect.size())) {
      prediction = calculate_prediction(smallerVect, minInfluence);

      prediction = floor(prediction);

      year_pred += prediction;
      smallerVect.push_front(prediction);
    }


    // percentual value of predictet sale
    // 2377 is number of cars actually sold (edited to not have unpredicted spike)
    realitPerc = (year_pred / 2377.0) * 100;

    // if the difference between predicted and real value is not very big print it out
    if( realitPerc >= 50 && realitPerc <= 150) {
    cout << "Hranice koeficientu autokorelace " << minInfluence << " predikce "
         << year_pred << "   odpovídá realitě z " << realitPerc << " %\n";

    }

    for (int i = 0; i <= 11; i++) {
      smallerVect.pop_front();
    }

    year_pred = 0;
    minInfluence += 0.001;
  }

  minInfluence = MIN_INFL;
  int toggle = 0;

  // calculate prediction from december 2020 to november 2021
  while (smallerVect.size() < (vect.size())) {
    prediction = calculate_prediction(smallerVect, minInfluence);

    prediction = floor(prediction);

    if (toggle == 0) {
      toggle = 1;
      cout << "Predikce: " << prediction << "    reálně: 198 ("
           << vect[vect.size() - smallerVect.size() - 1] << ")  "
           << months[vect.size() - smallerVect.size() - 1] << "\n";

    } else {
      cout << "Predikce: " << prediction
           << "    reálně: " << vect[vect.size() - smallerVect.size() - 1]
           << "         " << months[vect.size() - smallerVect.size() - 1]
           << "\n";
    }
    year_pred += prediction;
    smallerVect.push_front(prediction);
  }

  cout << "----------------------------------------------\n";

  cout << "Součet:   " << year_pred << "    "
       << "       2377 (3314)\n";
  cout << "##############################################\n\n";

  return;
}

void experiment_2(deque<double> vect) {
  deque<double> future_predict;
  double min_infl = MIN_INFL;
  future_predict = vect;
  double next_pred = 0;
  double year_pred = 0;
  int year_count = 2021;
  double summ = 0;

  // add December of 2021
  next_pred = calculate_prediction(future_predict, min_infl);
  next_pred = floor(next_pred);
  future_predict.push_front(next_pred);

  for (unsigned int i = 0; i <= 11; i++) {
    year_pred += future_predict[i];
  }

  cout << "################ EXPERIMENT 2 ################\n";

  // calculate preddiction for next 9 years month by month
  for (unsigned int i = 0; i < 108; i++) {
    if (i % 12 == 0) {
      summ += year_pred;
      cout << year_count << "   " << year_pred << "   " << summ+CURR_ELECTRO << "\n";
      year_pred = 0;
      year_count++;
    }

    next_pred = calculate_prediction(future_predict, min_infl);
    next_pred = floor(next_pred);
    future_predict.push_front(next_pred);

    year_pred += next_pred;
  }

  summ += year_pred;

  cout << year_count << "   " << year_pred << "   " << summ+CURR_ELECTRO << "\n";
  cout << "Celkem elektro aut: " << summ+CURR_ELECTRO << "\n";
  cout << "##############################################\n\n";

  return;
}

void experiment_3(deque<double> vect) {
  deque<double> future_predict;
  double min_infl = MIN_INFL;
  future_predict = vect;
  double next_pred = 0;
  double year_pred = 0;
  int year_count = 2021;
  double summ = 0;

  // add December of 2021
  next_pred = calculate_prediction(future_predict, min_infl);
  next_pred = floor(next_pred);
  future_predict.push_front(next_pred);

  for (unsigned int i = 0; i <= 11; i++) {
    year_pred += future_predict[i];
  }

  cout << "################ EXPERIMENT 3 ################\n";

  for (unsigned int i = 0; i < 108; i++) {
    if (i % 12 == 0) {
      summ += year_pred;
      cout << year_count << "   " << year_pred << "   " << summ+CURR_ELECTRO << "\n";
      year_pred = 0;
      year_count++;
    }

    next_pred = calculate_prediction(future_predict, min_infl);

    // mltiplay first year predicted by coeficient based on grant given by goverment
    // in other countries
    if (i <= 11) {
      next_pred = next_pred * 1.021875;
    }

    next_pred = floor(next_pred);
    future_predict.push_front(next_pred);

    year_pred += next_pred;
  }


  summ += year_pred;
  cout << year_count << "   " << year_pred << "   " << summ+CURR_ELECTRO << "\n";
  cout << "Celkem elektro aut: " << summ+CURR_ELECTRO << "\n";
  cout << "##############################################\n\n";

  return;
}

double calculate_prediction(deque<double> vect, double minInfluence) {
  long double mean = 0;

  // calculate mean
  for (unsigned int i = 0; i != vect.size(); i++) {
    mean += vect[i];
  }

  mean = mean / vect.size();

  deque<double> temp;

  for (unsigned int i = 0; i != vect.size(); i++) {
    temp.push_back(vect[i] - mean);
  }

  // get square root of deviations
  double sqr_dev = 0;
  for (unsigned int i = 0; i != vect.size(); i++) {
    sqr_dev += temp[i] * temp[i];
  }

  // calculate all autocorrelation coeficients
  deque<double> autoCorr;
  for (unsigned int i = 1; i != vect.size(); i++) {
    double result = 0;
    int count = 0;
    for (unsigned int j = i; j != temp.size(); j++) {
      result += temp[count] * temp[j];
      count++;
    }
    autoCorr.push_back(result);
  }

  double goodCorSum = 0;
  deque<int> index;

  // compare autocorrelation coeficient and minimal autocorrelation coeficient
  for (unsigned int i = 0; i != vect.size(); i++) {
    autoCorr[i] = autoCorr[i] / sqr_dev;
    if (abs(autoCorr[i]) > minInfluence) {
      goodCorSum += autoCorr[i];
      index.push_back(i);
    }
  }

  double prediction = 0;

  if (index.empty()) return prediction;

  // calculate prediction from impactful values multiplied by their importance
  for (unsigned int i = 0; i < index.size(); i++) {
    prediction += vect[index[i]] * (((autoCorr[index[i]]) / goodCorSum));
  }

  return prediction;
}
