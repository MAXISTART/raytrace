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
#include "dielectric.h"

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


hittable_list random_scene() {
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}


int main() {

	ofstream outfile;
	outfile.open("C:\\workArea\\image.ppm", ios::out);


	// Image

	const auto aspect_ratio = 3.0 / 2.0;
	const int image_width = 1200;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 500;
	const int max_depth = 50;
	const int max_color_value = 255;
	// World

	auto world = random_scene();

	// Camera

	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);



	// PPM 图像格式，打开预览
	/*
		P3
		image_width image_height
		max_color_value
		color_1 color_2 ......
		color_3 color_4 ......(color is vec3)
	*/
	outfile << "P3" << endl << image_width << " " << image_height << endl << max_color_value << endl;
	// 线程
	vector<thread> ts;
	// 每个线程完成的结果缓存
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
			// 写的时候锁起来，同时要有顺序的写
			mtx.lock();
			file_storage[image_height - 1- j] = cs;
			count++;
			cout << "当前进度为： " << double(count) / image_height << endl;
			mtx.unlock();
		});


		// thread 没有拷贝构造，需要左值转右值
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
	// 主线程等待所有子线程完毕后进行汇总
	for (auto& t : ts) t.join();
	// 汇总整个vector中的颜色
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








