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
			DATA32 pix = ptr[y*dw+x];
			double dr = (pix & 0xff0000) >> 16;
			double dg = (pix & 0xff00) >> 8;
			double db = pix & 0xff;

			ft({ dr, dg, db });
		}
	}
}

DATA32 get_median_color()
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

	auto dist_limited=[=] (pixel left, pixel right)
	{
		return std::max<double>(dist(left, right), 1e-1);
	};

	const int iterations=32;

	for (int i=0; i<iterations; ++i)
	{
		double sum_inv_dist=0;
		pixel y_n={ 0, 0, 0 };

		auto sum_inv_dist_func=[&] (pixel x)
		{
			sum_inv_dist+=1/dist_limited(x, y);
		};

		auto weighted_sample_func=[&] (pixel x)
		{
			double w=1/dist_limited(x, y);

			for (int i=0; i<3; ++i)
				y_n[i]+=x[i]*w;
		};

		for_each_pixel(sum_inv_dist_func);
		for_each_pixel(weighted_sample_func);

		for (int i=0; i<3; ++i)
			y_n[i]/=sum_inv_dist;

		y=y_n;
	}

	// since we only have an estimate, pick the closest actually used color to the estimate
	auto best_dist=std::numeric_limits<double>::max();
	auto best_cold=y;
	auto closest=[&] (pixel x)
	{
		auto d=dist(x, y);

		if (d<best_dist)
		{
			best_dist=d;
			best_cold=x;
		}
	};

	for_each_pixel(closest);

	y=best_cold;

	DATA32 best_col=0;

	best_col=(std::uint8_t(y[0]) << 16)+(std::uint8_t(y[1]) << 8)+std::uint8_t(y[2]);

	return best_col;
}

