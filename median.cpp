#include "median.h"

#include <cmath>
#include <limits>

#include "image.h"

int get_median_color()
{
	DATA32 *ptr = imlib_image_get_data_for_reading_only();
	int dw = imlib_image_get_width();
	int dh = imlib_image_get_height();

	auto get_dist=[=] (unsigned char *compare) -> double
	{
		double dist=0;

		for (int y = 0; y<dh; ++y)
		{
			for (int x = 0; x<dw; ++x)
			{
				unsigned char *pix = (unsigned char *)&ptr[y*dw+x];
				double db = pix[0];
				double dg = pix[1];
				double dr = pix[2];

				db-=compare[0];
				dg-=compare[1];
				dr-=compare[2];

				dist+=sqrt(dr*dr+dg*dg+db*db);
			}
		}

		return dist;
	};

	double best_dist=std::numeric_limits<double>::max();
	DATA32 best_col=0;

	for (int y = 0; y<dh; ++y)
	{
		for (int x = 0; x<dw; ++x)
		{
			DATA32 col = ptr[y*dw+x];
			unsigned char *pix = (unsigned char *)&col;
			double dist = get_dist(pix);

			if (dist<best_dist)
			{
				best_dist=dist;
				best_col=col;
			}
		}
	}

	return best_col;
}
