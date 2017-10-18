#include "box.h"

gdBox::gdBox() {}
gdBox::gdBox(gdVec2 set_size, gdVec2 set_position, gdVec1 set_angle) {
	size(set_size.x, set_size.y);
	position(set_position.x, set_position.y);
	angle(set_angle);
}
gdBox::gdBox(gdVec4 set_rect, gdVec1 set_angle) {
	rect(set_rect.x, set_rect.y, set_rect.z, set_rect.w);
	angle(set_angle);
}