/*
 * Kalman.c
 *
 *  Created on: May 14, 2024
 *      Author: HieuNM
 */

#include "kalman.h"
#include "stdint.h"
#include "math.h"

void SimpleKalmanFilter(KalmanFilter *kf, float mea_e, float est_e, float q) {
    kf->_err_measure = mea_e;
    kf->_err_estimate = est_e;
    kf->_q = q;
    kf->_kalman_gain = 0;
    kf->_current_estimate = 0;
    kf->_last_estimate = 0;
}

float updateEstimate(KalmanFilter *kf, float mea) {
    kf->_kalman_gain = kf->_err_estimate/(kf->_err_estimate + kf->_err_measure);
    kf->_current_estimate = kf->_last_estimate + kf->_kalman_gain * (mea - kf->_last_estimate);
    kf->_err_estimate =  (1.0 - kf->_kalman_gain)*kf->_err_estimate + fabs(kf->_last_estimate-kf->_current_estimate) * kf->_q;
    kf->_last_estimate=kf->_current_estimate;
    return kf->_current_estimate;
}

void setMeasurementError(KalmanFilter *kf, float mea_e) {
    kf->_err_measure=mea_e;
}

void setEstimateError(KalmanFilter *kf, float est_e) {
    kf->_err_estimate=est_e;
}

void setProcessNoise(KalmanFilter *kf, float q) {
    kf->_q=q;
}

float getKalmanGain(KalmanFilter *kf) {
    return kf->_kalman_gain;
}

float getEstimateError(KalmanFilter *kf) {
    return kf->_err_estimate;
}
