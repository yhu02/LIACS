int main() {
	int var_a, var_b;
	var_b = var_a;
	{
		int var_c, var_d, var_e, var_f;
		var_c = var_a;
		var_d = var_e;
		var_e = var_b;
		var_a = +var_dvar_c%var_b%(var_b);
	}
	print var_a;
}