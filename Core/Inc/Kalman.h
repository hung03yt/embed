/*
 * Kalman.h
 *
 *  Created on: May 14, 2024
 *      Author: HieuNM
 */

#ifndef USER_UTILITIES_KALMAN_KALMAN_H_
#define USER_UTILITIES_KALMAN_KALMAN_H_
typedef struct {
    float _err_measure;   // Sai số đo lường
    float _err_estimate;  // Sai số ước lượng
    float _q;             // Nhiễu quá trình
    float _kalman_gain;   // Hệ số Kalman
    float _current_estimate; // Ước lượng hiện tại
    float _last_estimate;    // Ước lượng trước đó
} KalmanFilter;

void SimpleKalmanFilter(KalmanFilter *kf, float mea_e, float est_e, float q);

float updateEstimate(KalmanFilter *kf, float mea);

void setMeasurementError(KalmanFilter *kf, float mea_e);

void setEstimateError(KalmanFilter *kf, float est_e);

void setProcessNoise(KalmanFilter *kf, float q);

float getKalmanGain(KalmanFilter *kf);

float getEstimateError(KalmanFilter *kf);

#endif /* USER_UTILITIES_KALMAN_KALMAN_H_ */
