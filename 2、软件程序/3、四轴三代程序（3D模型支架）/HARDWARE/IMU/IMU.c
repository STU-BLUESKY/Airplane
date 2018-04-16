#include "imu.h"


//ŷ���Ƕ���
AHRS_EulerAngleTypeDef EulerAngle;	

float Rot_matrix[9] = {1.0f,0.0f,0.0f, 
											 0.0f,1.0f,0.0f, 
											 0.0f,0.0f,1.0f };		//��λ����
float DCMgb[3][3];	//�������Ҿ��� ����������ת������������

/*---------------------------------------------
  �ɱ���������Ӧ����
---------------------------------------------*/
float VariableParameter(float error)
{
	float  result = 0;
	
	if(error < 0)
	{
	   error = -error;
	}
  if(error >0.6f)
	{
	   error = 0.6f;
	}
	result = 1 - 1.667 * error;
	if(result < 0)
	{
	   result = 0;
	}
	return result;
}
//  ��float�����ݾ���ֵ
float FL_ABS(float x)
{
   if(x < 0)  return -x;
	 else return x; 
}

// Carmackƽ���������㷨
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
 
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                      
	i  = 0x5f375a86 - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration ����һ��ţ�ٵ�����
	return y;
} 


// ��Ԫ������
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
float exInt = 0, eyInt = 0, ezInt = 0;  
float q0q0, q0q1, q0q2, q0q3;
float q1q1, q1q2, q1q3;
float q2q2, q2q3;
float q3q3;

/*------------------------------------------
 ��������: ��̬�ںϽ���
 ��������: ��
 �������Ϊ���ٶ�ԭʼ���ݣ�������ԭʼ���ݣ������ǻ���������
------------------------------------------*/
#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f                       // integral gain governs rate of convergence of gyroscope biases
#define halfT 	0.005f                  // �������ڵ�һ��(s)

void Angle_Calculate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
	u8 i=0;
  float norm;
	float hx, hy, hz, bx, bz;	//***
  float wx, wy, wz;					//***
  float vx, vy, vz;
  float ex, ey, ez;
	
		// �Ȱ���Щ�õõ���ֵ���
  q0q0 = q0*q0;
  q0q1 = q0*q1;
  q0q2 = q0*q2;
  q0q3 = q0*q3;
  q1q1 = q1*q1;
  q1q2 = q1*q2;
  q1q3 = q1*q3;
  q2q2 = q2*q2;
  q2q3 = q2*q3;
  q3q3 = q3*q3;
	
	if(ax*ay*az==0)	return;		
	if(mx*my*mz == 0)	return;
        
	
  norm = Q_rsqrt(ax*ax + ay*ay + az*az);       //acc���ݹ�һ��
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;

	norm = Q_rsqrt(mx*mx + my*my + mz*mz);       	//mag���ݹ�һ��
  mx = mx * norm;
  my = my * norm;
  mz = mz * norm;

	//***
	hx = 2 * mx * (0.5 - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);  
	hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5 - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);  
	hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5 - q1q1 -q2q2);          
	bx = sqrt((hx*hx) + (hy*hy));  
	bz = hz;

  // estimated direction of gravity and flux (v and w)   �����������������/��Ǩ
  vx = 2*(q1q3 - q0q2);								//��Ԫ����xyz�ı�ʾ
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);  
  wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);  
  wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2); 

	//����Ǽ��ٶȺ����������ݵĲ��
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) + (my*wz - mz*wy);                         	//�������������õ���־������
  ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

  exInt = exInt + Ki * VariableParameter(ex) * ex;								  //�������л���
  eyInt = eyInt + Ki * VariableParameter(ey) * ey;
  ezInt = ezInt + Ki * VariableParameter(ez) * ez;
	
	// adjusted gyroscope measurements
  gx = gx + Kp * VariableParameter(ex) * ex + exInt;	
	gy = gy + Kp * VariableParameter(ey) * ey + eyInt;	
	gz = gz + Kp * VariableParameter(ez) * ez + ezInt;	   							
	
  // integrate quaternion rate and normalise						   //��Ԫ�ص�΢�ַ���
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // normalise quaternion
  norm = Q_rsqrt(q0q0 + q1q1 + q2q2 + q3q3);
  q0 = q0 * norm;
  q1 = q1 * norm;
  q2 = q2 * norm;
  q3 = q3 * norm;
			
	EulerAngle.Pitch = asin(-2*q1q3 + 2*q0q2) * RtA; // pitch		-90~90
	EulerAngle.Roll = atan2(2*q2q3 + 2*q0q1, -2*q1q1 - 2*q2q2 + 1) *RtA; // roll	-180~180
	EulerAngle.Yaw = atan2(2*q1q2 + 2*q0q3, -2*q2q2 - 2*q3q3 +1) *RtA;	// yaw -180~180
	
	//���㷽�����Ҿ���  ���ڶ��߶�Ч��������ת��
	Rot_matrix[0] = q0q0 + q1q1 - q2q2 - q3q3;// 11
	Rot_matrix[1] = 2.f * (q1q2 + q0q3);	// 12
	Rot_matrix[2] = 2.f * (q1q3 - q0q2);	// 13
	Rot_matrix[3] = 2.f * (q1q2 - q0q3);	// 21
	Rot_matrix[4] = q0q0 - q1q1 + q2q2 - q3q3;// 22
	Rot_matrix[5] = 2.f * (q2q3 + q0q1);	// 23
	Rot_matrix[6] = 2.f * (q1q3 + q0q2);	// 31
	Rot_matrix[7] = 2.f * (q2q3 - q0q1);	// 32
	Rot_matrix[8] = q0q0 - q1q1 - q2q2 + q3q3;// 33
	
	for(i=0; i<9; i++)
	{
			*(&(DCMgb[0][0]) + i)=Rot_matrix[i];
	}
		
}

