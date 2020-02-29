// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

if (prx + 8 >= _en_x && prx <= _en_x + 8) {
	if (_en_my == 0 || 
		(_en_my > 0 && pry > _en_y) ||
		(_en_my < 0 && pry < _en_y)
	) {
		_t = (pry < _en_y) ? 0 : 4;
	}
}
