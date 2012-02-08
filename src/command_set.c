/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* ボディパラメータ */
#include <shvel-param.h>

/* SSM 用 */
// #include <ssm.h>
// #include <ssmtype/spur-odometry.h>

/* yp-spur用 */
#include <serial.h>
#include <param.h>
#include <control.h>
#include <command.h>
#include <odometry.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

void set_pos_com( int cs, double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;
	CSptr cs_src;
	CSptr cs_fs;

	x = data[0];
	y = data[1];
	theta = data[2];

	cs_src = get_cs_pointer( cs );
	cs_fs = get_cs_pointer( CS_FS );

	if( cs_src->parent != NULL )
	{
		CS_turn_base( &x, &y, &theta );
		CS_recursive_trans( cs_src->parent, cs_fs, &x, &y, &theta );
		set_cs( cs, x, y, theta );
	}
	else if( option( OPTION_ENABLE_SET_BS ) )
	{
		OdometryPtr odm;

		odm = get_odometry_ptr(  );
		odm->x = x;
		odm->y = y;
		odm->theta = theta;
		set_cs( CS_FS, x, y, theta );
	}
}

void set_GL_on_GL_com( double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;
	x = data[0];
	y = data[1];
	theta = data[2];

	cstrans_xy( CS_GL, CS_BS, &x, &y, &theta );
	set_cs( CS_GL, x, y, theta );
}

void set_adjust_com( int cs, double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;

	x = data[0];
	y = data[1];
	theta = data[2];

	cstrans_xy( cs, CS_GL, &x, &y, &theta );
	/* ロボット(FS)がGL上のx,y,thetaに見えるとするとき */
	/* FSからGLがどこに見えるか(GL->FS => FS->GL) */
	CS_turn_base( &x, &y, &theta );
	/* それはBS上のどこか */
	cstrans_xy( CS_FS, CS_BS, &x, &y, &theta );
	/* GLをセット */
	set_cs( CS_GL, x, y, theta );

	// 走行制御用座標系も修正
	x = data[0];
	y = data[1];
	theta = data[2];
	cstrans_xy( cs, CS_SP, &x, &y, &theta );
	/* ロボット(FS)がGL上のx,y,thetaに見えるとするとき */
	/* FSからGLがどこに見えるか(GL->FS => FS->GL) */
	CS_turn_base( &x, &y, &theta );
	/* それはBS上のどこか */
	cstrans_xy( CS_FS, CS_BS, &x, &y, &theta );
	/* SPをセット */
	set_cs( CS_SP, x, y, theta );
}

void set_vel_com( double *data, SpurUserParamsPtr spur )
{
	spur->v = data[0];
	if( spur->v > p( YP_PARAM_MAX_VEL ) )
		spur->v = p( YP_PARAM_MAX_VEL );
	if( spur->v < -p( YP_PARAM_MAX_VEL ) )
		spur->v = -p( YP_PARAM_MAX_VEL );
}

void set_torque_com( double *data, SpurUserParamsPtr spur )
{
	spur->torque_l = data[1];
	spur->torque_r = data[0];
	spur->run_mode = RUN_WHEEL_TORQUE;
}

void set_ang_vel_com( double *data, SpurUserParamsPtr spur )
{
	spur->w = data[0];
	if( spur->w > p( YP_PARAM_MAX_W ) )
		spur->w = p( YP_PARAM_MAX_W );
	if( spur->w < 0 )
		spur->w = 0;
}

void set_accel_com( double *data, SpurUserParamsPtr spur )
{
	spur->dv = data[0];
	if( spur->dv > p( YP_PARAM_MAX_ACC_V ) )
		spur->dv = p( YP_PARAM_MAX_ACC_V );
	if( spur->dv < 0 )
		spur->dv = 0;
}

void set_ang_accel_com( double *data, SpurUserParamsPtr spur )
{
	spur->dw = data[0];
	if( spur->dw > p( YP_PARAM_MAX_ACC_W ) )
		spur->dw = p( YP_PARAM_MAX_ACC_W );
	if( spur->dw < 0 )
		spur->dw = 0;
}

void set_tilt_com( int cs, double *data, SpurUserParamsPtr spur )
{
	double x, y, theta;

	x = 0;
	y = 0;
	theta = data[0];

	cstrans_xy( CS_GL, cs, &x, &y, &theta );

	spur->dir = theta;
	spur->tilt = data[1];
}