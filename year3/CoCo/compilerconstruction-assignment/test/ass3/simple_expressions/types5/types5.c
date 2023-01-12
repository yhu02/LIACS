int main() {
	int8_t var_a;
	var_a = 124;
	var_a = var_a;
	var_a = var_a;
	{
		uint8_t var_b, var_c;
		var_c = 32;
		{
			int8_t var_f;
			uint8_t var_g;
			int var_h;
			unsigned var_d, var_e;
			var_d = 301106307;
			{
				uint8_t var_i;
				var_i = 161;
				var_i = var_i*var_c;
			}
			{
				int var_j;
				{
					int8_t var_k;
					var_d = var_d;
				}
			}
		}
	}
	print var_a;
}