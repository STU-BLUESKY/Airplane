/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * 文件名  ：IMU.c
 * 描述    ：姿态解算         
 * 实验平台：Air Nano四轴飞行器
 * 库版本  ：ST3.5.0
 * 作者    ：Air Nano Team 
 * 淘宝    ：http://byd2.taobao.com
**********************************************************************************/
#include "IMU.h"
#include "math.h"
#include "MP6050.h"

struct _angle angle;
float GY;

/*	
	Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
	R:测量噪声，R增大，动态响应变慢，收敛稳定性变好	
*/
#define KALMAN_Q        0.02
#define KALMAN_R        8.0000

/**************************************
 * 函数名：KalmanFilter
 * 描述  ：得到当前姿态
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 *************************************/
static double KalmanFilter_x(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
   kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
   x_now=x_mid+kg*(ResrcData-x_mid);//估计出的最优值
                
   p_now=(1-kg)*p_mid;//最优值对应的covariance       
   p_last = p_now; //更新covariance值
   x_last = x_now; //更新系统状态值
   return x_now;                
 }

static double KalmanFilter_y(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
   kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
   x_now=x_mid+kg*(ResrcData-x_mid);//估计出的最优值
                
   p_now=(1-kg)*p_mid;//最优值对应的covariance       
   p_last = p_now; //更新covariance值
   x_last = x_now; //更新系统状态值
   return x_now;                
 }
static double KalmanFilter_z(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
   kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
   x_now=x_mid+kg*(ResrcData-x_mid);//估计出的最优值
                
   p_now=(1-kg)*p_mid;//最优值对应的covariance       
   p_last = p_now; //更新covariance值
   x_last = x_now; //更新系统状态值
   return x_now;                
 }


//////////QYQ QYQ QYQ


 //   快速求平方根倒数
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
 
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                      
	i  = 0x5f3759df - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration （第一次牛顿迭代）
	return y;
} 
//  求float型数据绝对值
float FL_ABS(float x)
{
   if(x < 0)  return -x;
	 else return x; 
}
/***********************************************
  * @brief  可变增益自适应参数
  * @param  None
  * @retval None
************************************************/
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
void Prepare_Data(void)
{
	MPU6050_Dataanl();
	
	sensor.acc.averag.x = KalmanFilter_x(sensor.acc.origin.x,KALMAN_Q,KALMAN_R);
	sensor.acc.averag.y = KalmanFilter_y(sensor.acc.origin.y,KALMAN_Q,KALMAN_R);
	sensor.acc.averag.z = KalmanFilter_z(sensor.acc.origin.z,KALMAN_Q,KALMAN_R);
}

#define Kp 2.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.001f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.00125f                   // half the sample period采样周期的一半


/**************************************
 * 函数名：Get_Attitude
 * 描述  ：得到当前姿态
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 *************************************/
void Get_Attitude(void)
{
	IMUupdate(sensor.gyro.radian.x,
						sensor.gyro.radian.y,
						sensor.gyro.radian.z,
						sensor.acc.averag.x,sensor.acc.averag.y,sensor.acc.averag.z);	//*0.0174转成弧度
}

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
  float norm;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;

  // 先把这些用得到的值算好
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
//  float q0q3 = q0*q3;//
  float q1q1 = q1*q1;
//  float q1q2 = q1*q2;//
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
	
	if(ax*ay*az==0)
 		return;
		
  norm = Q_rsqrt(ax*ax + ay*ay + az*az);       //acc数据归一化
  ax = ax *norm;
  ay = ay * norm;
  az = az * norm;


  // estimated direction of gravity and flux (v and w)              估计重力方向和流量/变迁
  vx = 2*(q1q3 - q0q2);												//四元素中xyz的表示
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                           					 //向量外积在相减得到差分就是误差
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + VariableParameter(ex) * ex * Ki;								  //对误差进行积分
  eyInt = eyInt + VariableParameter(ey) * ey * Ki;
  ezInt = ezInt + VariableParameter(ez) * ez * Ki;
// adjusted gyroscope measurements

  gx = gx + Kp *  VariableParameter(ex) * ex + exInt;	
	gy = gy + Kp *  VariableParameter(ey) * ey + eyInt;	
	gz = gz + Kp *  VariableParameter(ez) * ez + ezInt;	   							
	
  // integrate quaternion rate and normalise						   //四元素的微分方程
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

  angle.roll = atan2(2*q2q3 + 2*q0q1, -2*q1q1 - 2*q2q2 + 1) *RtA; // roll
	angle.pitch = asin(-2*q1q3 + 2*q0q2) * RtA; // pitch
	
}











