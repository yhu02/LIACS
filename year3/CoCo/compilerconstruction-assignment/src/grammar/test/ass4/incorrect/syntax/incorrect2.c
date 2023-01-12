int main() {
	int var_a, var_b, var_c, var_d, var_e;
	var_a = var_b;
	var_b = var_b;
	var_c = var_b;
	var_e = var_c;
	var_a = +var_c)--(+var_a);
	print var_e;
}