#include <iostream>
#include <fstream>
#include "color.h"
#include "ray.h"
#include "sphere.h"
using namespace std;



double hit_sphere(const vec3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = r.dir.length_squared();
	auto half_b = dot(r.dir, oc);
	auto c = oc.length_squared() - radius * radius;
	auto delta = half_b * half_b - a * c;
	if (delta < 0) return -1;
	else return (-half_b - sqrt(delta)) / a;
}

color ray_color(const ray& r) {	
	sphere circle(point3(0, 0, -1), 0.5);
	hit_record rec;
	bool is_hit = circle.hit(r, 0, 99999.0, rec);
	if (is_hit) {
		// return normal vector
		// normal in [-1, 1]^3, transform to color space [0,1]^3
		return 0.5 * (rec.normal + vec3(1, 1, 1));
	}

	// paint background
	auto dir = unit_vector(r.dir);
	//  -1 <= dir.y <= 1, so first limit y to [0,1]
	auto t = (dir.y() + 1.0) / 2.0;
	return (1 - t)* color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}




int main() {

	ofstream outfile;
	outfile.open("C:\\workArea\\image.ppm", ios::out);


	// PPM 图像格式，打开预览
	/*
		P3	
		image_width image_height
		max_color_value
		color_1 color_2 ......
		color_3 color_4 ......(color is vec3)
	*/
	const double aspect_ratio = 16.0 / 9.0; // screen_width/screen_height
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int max_color_val = 255;


	// camera settings，origin must be the center of the screen
	const double focal_length = 1.0;
	const vec3 origin(0,0,0);
	
	// viewport settings
	const double viewport_height = 2.0;
	const double viewport_width = aspect_ratio * viewport_height;
	const vec3 horizontal(viewport_width, 0, 0);
	const vec3 vertical(0, viewport_height, 0);
	const vec3 lower_left_corner(origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length));


	outfile << "P3" << endl << image_width << " " << image_height << endl << max_color_val << endl;
	
	for (int j = image_height - 1; j >= 0; --j) {
		cout << "\rScanlines : " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto u = double(i) / (image_width - 1);
			auto v = double(j) / (image_height - 1);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
			write_color(outfile, ray_color(r));
		}
	}


	cout << "\nDone.\n" << std::flush;
	outfile.close();

	ray r1(point3(0, 0, 0), vec3(0,0,0));
	r1.origin();
}








