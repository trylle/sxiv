#include "median.h"

#include <array>
#include <cmath>
#include <limits>
#include <iostream>

#include "image.h"

typedef std::array<double, 3> pixel;

template<class func_type>
void for_each_pixel(func_type ft)
{
	DATA32 *ptr = imlib_image_get_data_for_reading_only();
	int dw = imlib_image_get_width();
	int dh = imlib_image_get_height();

	for (int y = 0; y<dh; ++y)
	{
		for (int x = 0; x<dw; ++x)
		{
			unsigned char *pix = (unsigned char *)&ptr[y*dw+x];
			double db = pix[0];
			double dg = pix[1];
			double dr = pix[2];

			ft({ dr, dg, db });
		}
	}
}

int get_median_color()
{
	pixel y={128, 128, 128};

	auto dist=[] (pixel left, pixel right)
	{
		double d=0;

		for (int i=0; i<3; ++i)
		{
			double delta=left[i]-right[i];

			d+=delta*delta;
		}

		return sqrt(d);
	};

	for (int i=0; i<32; ++i)
	{
		double sum_inv_dist=0;
		pixel y_n;

		auto sum_inv_dist_func=[&] (pixel x)
		{
			sum_inv_dist+=1/dist(x, y);
		};

		auto weighted_sample_func=[&] (pixel x)
		{
			double w=1/dist(x, y);

			for (int i=0; i<3; ++i)
			{
				y_n[i]+=x[i]*w;
			}
		};

		for_each_pixel(sum_inv_dist_func);
		for_each_pixel(weighted_sample_func);

		for (int i=0; i<3; ++i)
		{
			y_n[i]/=sum_inv_dist;
		}

		std::cout << sum_inv_dist << std::endl;

		y=y_n;

		std::cout << y[0] << ", " << y[1] << ", "<< y[2] << std::endl;
	}

	DATA32 best_col;
	unsigned char *best_colc=(unsigned char *)&best_col;

	best_colc[0]=y[2];
	best_colc[1]=y[1];
	best_colc[2]=y[0];

	return best_col;
}
