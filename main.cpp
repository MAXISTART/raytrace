#include <iostream>
#include <fstream>
#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "tools.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include <thread>
#include <mutex>
#include <map>

using namespace std;

std::mutex mtx;           // write_file lock

color ray_color(const ray& r, const hittable_list& world, int recruse_depth) {

	hit_record rec;
	bool is_hit = world.hit(r, 0.0001, INFINITY, rec);
	if (is_hit && recruse_depth > 0) {
		color attenuation;
		ray scattered;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, recruse_depth - 1);
		}
		else return color(0, 0, 0);
	}
	else {
		if (is_hit) {
			// draw color
			return color(0,0,0);
		}
		else {
			// paint background
			auto dir = unit_vector(r.dir);
			//  -1 <= dir.y <= 1, so first limit y to [0,1]
			auto t = (dir.y() + 1.0) / 2.0;
			return (1 - t)* color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
		}
	}
}




int main() {

	ofstream outfile;
	outfile.open("C:\\workArea\\image.ppm", ios::out);


	// PPM ͼ���ʽ����Ԥ��
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



	// make a scene
	hittable_list world;
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.25);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));



	// camera
	camera cam;


	// render
	const int samples_per_pixel = 200;
	const int max_depth = 50;

	outfile << "P3" << endl << image_width << " " << image_height << endl << max_color_val << endl;
	vector<thread> ts;
	vector<vector<color>> file_storage(image_height, vector<color>(image_width, color()));
	int count = 0;
	for (int j = image_height - 1; j >= 0; --j) {		
		thread t([&,j]() {
			vector<color> cs;
			for (int i = 0; i < image_width; ++i) {
				color c;
				for (int s = 0; s < samples_per_pixel; s++) {
					auto u = (i + random_double()) / (image_width - 1);
					auto v = (j + random_double()) / (image_height - 1);
					c += ray_color(cam.get_ray(u, v), world, max_depth);
				}
				cs.push_back(c);
			}	
			// д��ʱ����������ͬʱҪ��˳���д
			mtx.lock();
			file_storage[image_height - 1- j] = cs;
			count++;
			cout << "��ǰ����Ϊ�� " << double(count) / image_height << endl;
			mtx.unlock();
		});
		// thread û�п������죬��Ҫ��ֵת��ֵ
		ts.emplace_back(move(t));
		//for (int i = 0; i < image_width; ++i) {
		//	color c;
		//	for (int s = 0; s < samples_per_pixel; s++) {
		//		auto u = (i + random_double()) / (image_width - 1);
		//		auto v = (j + random_double()) / (image_height - 1);
		//		c += ray_color(cam.get_ray(u, v), world, max_depth);
		//	}
		//	write_color(outfile, c, samples_per_pixel);
		//}
	}

	for (auto& t : ts) t.join();
	// ��������vector�е���ɫ
	for_each(file_storage.begin(), file_storage.end(), [&](vector<color>& data) {
		for (color& c : data) {
			write_color(outfile, c, samples_per_pixel);
		}
	});

	cout << "\nDone.\n" << std::flush;
	outfile.close();

	ray r1(point3(0, 0, 0), vec3(0,0,0));
	r1.origin();
}








