int main() {
	int8_t var_c;
	uint8_t var_b;
	int var_a;
	var_c = 44;
	var_a = 1118260120;
	var_c = var_c--(var_c*var_c);
	{
		uint8_t var_f;
		int var_d, var_g;
		unsigned var_e, var_h;
		var_f = 133;
		{
			uint8_t var_i;
			var_i = var_f;
		}
	}
	print var_a;
}