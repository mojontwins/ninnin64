// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

if (pry + 8 >= _en_y && pry <= _en_y + 8) {
	if (_en_mx == 0 || 
		(_en_mx > 0 && prx > _en_x) ||
		(_en_mx < 0 && prx < _en_x)
	) {		
		_t = (prx < _en_x) ? 6 : 2;
	}
}
