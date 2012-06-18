#ifndef __COMMAND__
#define __COMMAND__
#include <ypspur.h>

typedef enum
{
	RUN_FREE = 0,
	RUN_STOP,
	RUN_LINEFOLLOW,
	RUN_STOP_LINE,
	RUN_CIRCLEFOLLOW,
	RUN_SPIN,
	RUN_ORIENT,
	RUN_VEL,
	RUN_WHEEL_VEL,
	RUN_WHEEL_TORQUE,
	RUN_WHEEL_ANGLE
} SpurRunMode;

typedef struct _spur_user_params *SpurUserParamsPtr;
typedef struct _spur_user_params
{
	double dv;
	double dw;
	double v;
	double w;
	double vref;
	double wref;
	double wrref;
	double wlref;
	double x;
	double y;
	double torque_r;
	double torque_l;
	double wheel_accel_r;
	double wheel_accel_l;
	double wheel_vel_r;
	double wheel_vel_l;
	double wheel_angle_r;
	double wheel_angle_l;
	double theta;
	double radius;
	double tilt;
	double dir;
	double control_dt;
	int freeze;
	int before_freeze;
	SpurRunMode run_mode;
	SpurRunMode before_run_mode;
	pthread_mutex_t mutex;
} SpurUserParams;

/* 
 * typedef struct _spur_reference{ double x; double y; double theta; double v; double w; double radius; double accel; double ang_accel; double tilt; // 現在位置の傾き
 * double dir; // 傾きの高い方向（tiltが正なら） } SpurReference; */

void init_spur_command( void );
void command( void );
void message_return( int msq_id, long retpid, YPSpur_msg * res_msg );

void init_command_thread( pthread_t * thread );
SpurUserParamsPtr get_spur_user_param_ptr(  );

/* command_run.c */
void line_com( int cs, double *data, SpurUserParamsPtr spur );
void stop_line_com( int cs, double *data, SpurUserParamsPtr spur );
void circle_com( int cs, double *data, SpurUserParamsPtr spur );
void spin_com( int cs, double *data, SpurUserParamsPtr spur );
void orient_com( int cs, double *data, SpurUserParamsPtr spur );
void stop_com( double *data, SpurUserParamsPtr spur );
void free_com( double *data, SpurUserParamsPtr spur );
void vel_com( double *data, SpurUserParamsPtr spur );
void wheel_vel_com( double *data, SpurUserParamsPtr spur );
void wheel_angle_com( double *data, SpurUserParamsPtr spur );

/* command_set.c */
void set_pos_com( int cs, double *data, SpurUserParamsPtr spur );
void set_GL_on_GL_com( double *data, SpurUserParamsPtr spur );
void set_adjust_com( int cs, double *data, SpurUserParamsPtr spur );
void set_vel_com( double *data, SpurUserParamsPtr spur );
void set_ang_accel_com( double *data, SpurUserParamsPtr spur );;
void set_accel_com( double *data, SpurUserParamsPtr spur );
void set_ang_vel_com( double *data, SpurUserParamsPtr spur );
void set_tilt_com( int cs, double *data, SpurUserParamsPtr spur );
void set_torque_com( double *data, SpurUserParamsPtr spur );
void set_wheel_vel_com( double *data, SpurUserParamsPtr spur );
void set_wheel_accel_com( double *data, SpurUserParamsPtr spur );

/* command_get.c */
void get_pos_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur );
void get_vel_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur );
int near_pos_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur );
int near_ang_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur );
int over_line_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur );
void get_wheel_vel_com( double *data, double *resdata, SpurUserParamsPtr spur );
void get_wheel_ang_com( double *data, double *resdata, SpurUserParamsPtr spur );
void get_force_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur );
void get_wheel_torque_com( int cs, double *data, double *resdata, SpurUserParamsPtr spur );

/* command_param.c */
void param_set_com( int cs, double *data, SpurUserParamsPtr spur );
int param_get_com( int cs, double *resdata, SpurUserParamsPtr spur );
void param_state_com( int cs, double *data, SpurUserParamsPtr spur );

/* command_ad.c */
void get_ad_com( double *data, double *resdata );
void set_io_dir_com( double *data, double *resdata );
void set_io_data_com( double *data, double *resdata );

#endif
