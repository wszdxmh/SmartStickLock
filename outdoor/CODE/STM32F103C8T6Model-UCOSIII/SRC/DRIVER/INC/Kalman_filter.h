
#ifndef __KALMAN_FILTER_H_
#define __KALMAN_FILTER_H_

#define Pi 	3.1415926f
//��������������
#define dt           0.012f
#define R_angle      0.5f
#define Q_angle      0.001f
#define Q_gyro       0.003f//Խ��Խ�ͺ�

//�����ⲿ����
extern short aacx, aacy, aacz;		//���ٶȴ�����ԭʼ����
extern short gyrox, gyroy, gyroz;	//������ԭʼ����

extern float  Acc_angle,Gry_vivi;
extern float Angle,Gyro_x;  //С���˲�����б�Ƕ�/���ٶ�
extern float Angle_x_temp;  //�ɼ��ٶȼ����x��б�Ƕ�
extern float Angle_y_temp;  //�ɼ��ٶȼ����y��б�Ƕ�
extern float Angle_z_temp;
extern float Angle_X_Final; //X������б�Ƕ�
extern float Angle_Y_Final; //Y������б�Ƕ�
extern float Angle_Z_Final; //Z������б�Ƕ�
extern float Gyro_x;		 //X�������������ݴ�
extern float Gyro_y;     //Y�������������ݴ�
extern float Gyro_z;		 //Z�������������ݴ�
extern float A_P,A_R,A2_P;

double KalmanFilter(const double ResrcData, double ProcessNiose_Q, double MeasureNoise_R);
void Angle_Calcu(void);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);
void Kalman_Filter_Z(float Accel, float Gyro);
void yijiehubu_P(float angle_m, float gyro_m);
void erjiehubu_P(float angle_m, float gyro_m);
void Erjielvbo(float angle_m,float gyro_m);

#endif
