/******************************************************************************
 *	Copyright (C) 2020	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define ALX_NO_PREFIX
#include <libalx/extra/cv/core.h>
#include <libalx/extra/cv/highgui.h>
#include <libalx/extra/cv/videoio.h>


/******************************************************************************
 ******* enum *****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* struct / union *******************************************************
 ******************************************************************************/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static
int	init	(img_s **restrict img, cam_s **restrict cam);
static
void	deinit	(img_s *restrict img, cam_s *restrict cam);
static
int	proc	(void);


/******************************************************************************
 ******* static variables *****************************************************
 ******************************************************************************/
static	const char *const cam_url	= "http://192.168.0.10/video.mjpg";


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	main	(void)
{
	if (proc())
		return	-1;

	return	0;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static
int	init	(img_s **restrict img, cam_s **restrict cam)
{

	if (alx_cv_alloc_img(img))
		return	-1;
	if (alx_cv_init_img(*img, 1, 1))
		goto out_free_1;
	if (alx_cv_alloc_cam(cam))
		goto out_deinit_1;
	alx_cv_init_cam(*cam, cam_url, 0, 0);
	return	0;

out_deinit_1:
	alx_cv_deinit_img(img);
out_free_1:
	alx_cv_free_img(img);
	return	-1;
}

static
void	deinit	(img_s *restrict img, cam_s *restrict cam)
{

	alx_cv_deinit_cam(cam);
	alx_cv_free_cam(cam);
	alx_cv_deinit_img(img);
	alx_cv_free_img(img);
}

static
int	proc	(void)
{
	img_s		*img;
	cam_s		*cam;
	int		status;

	status	= -1;
	if (init(&img, &cam))
		return	-1;


	clock_t	time_0;
	clock_t	time_1;
	double		time_tot;

	time_0 = clock();
	if (alx_cv_cam_read(img, cam))
		goto err;
	time_1 = clock();

	time_tot = ((double) time_1 - time_0) / CLOCKS_PER_SEC;
	printf("Total time:	%5.3lf s;\n", time_tot);

	alx_cv_imwrite(img, "cam_pic.png");

	status	= 0;
err:
	deinit(img, cam);
	return	status;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
