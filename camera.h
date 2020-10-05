#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class camera {
public:

	camera() {
		const double aspect_ratio = 16.0 / 9.0; // screen_width/screen_height
		// camera settings£¬origin must be the center of the screen
		const double focal_length = 1.0;
		origin = point3(0, 0, 0);
		// viewport settings
		const double viewport_height = 2.0;
		const double viewport_width = aspect_ratio * viewport_height;
		horizontal = vec3(viewport_width, 0, 0);
		vertical = vec3(0, viewport_height, 0);
		lower_left_corner = point3(origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length));
	};

	// emit a ray, uv starts from lower_left_corner
	ray get_ray(double u, double v) {
		return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};


#endif // !CAMERA_H
