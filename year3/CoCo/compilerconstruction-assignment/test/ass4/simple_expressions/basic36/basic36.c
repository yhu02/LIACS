int main() {
	int var_a, var_b, var_c, var_d;
	var_b = 712695597;
	var_c = 49615101;
	var_d = 1400146549;
	var_a = var_c%(var_d%((var_c)%var_b));
	var_b = var_d%var_b;
	var_c = (var_c)*var_d;
	print var_c;
}